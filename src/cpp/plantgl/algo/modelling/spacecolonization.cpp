/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2009 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */

#include "spacecolonization.h"
#include "../base/pointmanipulation.h"
#include <plantgl/scenegraph/container/indexarray_iterator.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

const size_t SpaceColonization::NOID(UINT32_MAX);

/* ----------------------------------------------------------------------- */

SpaceColonization::SpaceColonization(const Point3ArrayPtr _attractors,
                                      real_t _nodelength,
                                      real_t _kill_radius,
                                      real_t _perception_radius,
                                      const Point3ArrayPtr    initialskeletonnodes,
                                      const Uint32Array1Ptr  initialskeletonparents,
                                      const Index& _active_nodes,
                                      size_t spacetilingratio):
    attractors(_attractors),
    attractor_grid(new Point3Grid(_attractors,spacetilingratio)),
    nodelength(_nodelength),
    kill_radius(_kill_radius),
    perception_radius(_perception_radius),
    coneangle(GEOM_PI/2),
    insertion_angle(GEOM_PI/2),
    nb_buds_per_whorl(4),
    min_nb_pt_per_bud(3),
    skeletonnodes(initialskeletonnodes),
    skeletonparents(initialskeletonparents),
    active_nodes(_active_nodes),
    nbIteration(0)
{
   if(!is_null_ptr(skeletonnodes) && active_nodes.size() == 0)
        active_nodes = range<Index>(0,skeletonnodes->size(),1);
}

SpaceColonization::SpaceColonization(const Point3ArrayPtr _attractors,
                                      real_t _nodelength,
                                      real_t _kill_radius,
                                      real_t _perception_radius,
                                      const Vector3&    root,
                                      size_t spacetilingratio):
    attractors(_attractors),
    attractor_grid(new Point3Grid(_attractors,spacetilingratio)),
    nodelength(_nodelength),
    kill_radius(_kill_radius),
    perception_radius(_perception_radius),
    coneangle(GEOM_PI/2),
    insertion_angle(GEOM_PI/2),
    nb_buds_per_whorl(4),
    min_nb_pt_per_bud(3),
    skeletonnodes(),
    skeletonparents(),
    nodeattractors(),
    active_nodes(),
    nbIteration(0)
{
    add_node(root);

    // remove closest attractors
    attractor_grid->disable_points(attractor_grid->query_ball_point(root,kill_radius));
 }

SpaceColonization::~SpaceColonization()
{
}

/* ----------------------------------------------------------------------- */

size_t SpaceColonization::add_node(const Vector3& position, size_t parent, const Index& attractors, bool active) {
    size_t pid = 0;
    if(is_null_ptr(skeletonnodes))
        skeletonnodes = Point3ArrayPtr(new Point3Array(1, position));
    else {
        pid = skeletonnodes->size();
        skeletonnodes->push_back(position);
    }

    if (is_null_ptr(skeletonparents))
        skeletonparents = Uint32Array1Ptr(new Uint32Array1(pid+1,NOID));
    else {
        for(size_t i = skeletonparents->size(); i < pid+1; ++i)
            skeletonparents->push_back(NOID);
    }
    if (parent != NOID)
        skeletonparents->setAt(pid,parent);
    else skeletonparents->setAt(pid,pid);


    if (is_null_ptr(nodeattractors))
        nodeattractors = IndexArrayPtr(new IndexArray(pid+1));
    else {
        for(size_t i = nodeattractors->size(); i < pid+1; ++i)
            nodeattractors->push_back(Index());
    }
    nodeattractors->setAt(pid,attractors);




    if (active)  _activate_node(pid);


    return pid;

}


void SpaceColonization::activate_leaves()
{
    uint32_t root;
    IndexArrayPtr children = determine_children(skeletonparents,root);

    Index previous_active_nodes = active_nodes;
    active_nodes = Index();

    // look to the node in post order and for each node compute its weight as sum of children weight + 1.
    uint32_t pid = 0;
    for(IndexArray::const_iterator pchildren = children->begin(); pchildren != children->end(); ++pchildren, ++pid)
    {
        if(pchildren->empty()){ // it is a leaf
            Index::iterator it = find(previous_active_nodes.begin(),previous_active_nodes.end(), pid);
            if (it == previous_active_nodes.end()) active_nodes.push_back(pid);
        }

    }

    active_nodes.insert(active_nodes.begin(), previous_active_nodes.begin(), previous_active_nodes.end());

}


/// compute a whorl of 'nb' buds at branching angles.
std::vector<Vector3>
SpaceColonization::lateral_directions(const Vector3& dir, real_t angle, int nb){
    std::vector<Vector3> result;
    Vector3 rotdir = direction(dir.anOrthogonalVector());
    Matrix3 rotmat = Matrix3::axisRotation(rotdir, angle);
    Vector3 v0 = rotmat * dir;
    if (nb > 0) result.push_back(v0);
    for (int i = 1; i < nb; ++i)
        result.push_back(Matrix3::axisRotation(dir, i * 2*GEOM_PI/nb) *v0);
    return result;
}

void SpaceColonization::generate_buds(size_t pid) {
    Vector3 pos = node_position(pid);
    Vector3 dir = node_direction(pid);
    std::vector<Vector3> lateral_dirs = lateral_directions(dir, insertion_angle, nb_buds_per_whorl);
    lateral_dirs.push_back(dir);

    for(std::vector<Vector3>::const_iterator itldir = lateral_dirs.begin(); itldir != lateral_dirs.end(); ++itldir)
    {
        // generate a bud
        try_to_set_bud(pid, *itldir);
    }
}

bool SpaceColonization::try_to_set_bud(size_t pid, const Vector3& direction)
{
    // find nearest attractor points in cone of perception of given radius and angle
    AttractorList neighbour_attractor_indices = attractor_grid->query_points_in_cone(node_position(pid), direction, perception_radius, coneangle);
    if (neighbour_attractor_indices.size() >= min_nb_pt_per_bud) {
        // generate a bud
        add_bud(pid, direction, neighbour_attractor_indices);
        return true;
    }
    else return false;
}


void SpaceColonization::add_bud(size_t pid, const Vector3& direction, const AttractorList& attractors)
{
    Uint32ArrayPtr attlist(new Uint32Array1(attractors.begin(),attractors.end()));
    budlist.push_back(Bud(pid,direction,attlist));
    register_attractors(node_position(pid), attlist);
}

void SpaceColonization::add_bud(size_t pid, const AttractorList& attractors, real_t level)
{
    Uint32ArrayPtr attlist(new Uint32Array1(attractors.begin(),attractors.end()));
    budlist.push_back(Bud(pid,attlist,level));
    register_attractors(node_position(pid), attlist);
}

void SpaceColonization::add_latent_bud(size_t pid, const AttractorList& attractors, real_t level, uint32_t latency)
{
    if (latency == 0) add_bud(pid, attractors, level);
    Uint32ArrayPtr attlist(new Uint32Array1(attractors.begin(),attractors.end()));
    latentbudlist.push_back(std::pair<Bud,uint32_t>(Bud(pid,attlist,level),latency));
}

void SpaceColonization::generate_all_buds() {
    budlist.clear();
    attractormap.clear();
    LatentBudList previouslatentbudlist = latentbudlist;
    latentbudlist.clear();

    for(Index::const_iterator it = active_nodes.begin(); it != active_nodes.end(); ++it){
        node_buds_preprocess(*it);
        generate_buds(*it);
        node_buds_postprocess(*it);
    }

    if (!previouslatentbudlist.empty()) { // latent bud list is process to remove one delay step for all. If delay is zero then bud are treated as normal bud.
        for (LatentBudList::iterator it = previouslatentbudlist.begin(); it != previouslatentbudlist.end(); ++it){
            if (it->second <= 1) {
                it->first.attractors = Uint32ArrayPtr(new Uint32Array1(attractor_grid->filter_disabled(*(it->first.attractors))));

                budlist.push_back(it->first);
                register_attractors(node_position(it->first.pid), it->first.attractors);
            }
            else latentbudlist.push_back(std::pair<Bud,uint32_t>(it->first,it->second-1));
        }
    }

    attractormap.clear();
    active_nodes.clear();
}

void SpaceColonization::register_attractors(const Vector3& pos, Uint32ArrayPtr attlist)
{
    size_t aid = 0;
    std::vector<size_t> todelete;
    for(Uint32Array1::const_iterator it = attlist->begin(); it != attlist->end(); ++it, ++aid)
    {
        real_t dist = norm(pos-attractors->getAt(*it));
        AttractorMap::iterator itmap = attractormap.find(*it);
        std::pair<Uint32ArrayPtr,real_t> info;
        if (itmap != attractormap.end()){
            // Competition
            if (dist > itmap->second.second) {
                info = itmap->second;

                // remove from this bud
                todelete.push_back(aid);
            }
            else {
                info = std::pair<Uint32ArrayPtr,real_t>(attlist,dist);

                // remove from previous bud
                Uint32ArrayPtr prevattlist = itmap->second.first;
                Uint32Array1::iterator itd = std::find(prevattlist->begin(), prevattlist->end(), *it);
                prevattlist->erase(itd);
            }
        }
        else {
            info = std::pair<Uint32ArrayPtr,real_t>(attlist,dist);
        }
        attractormap[*it] = info;
    }
    for(std::vector<size_t>::const_reverse_iterator itdel = todelete.rbegin(); itdel != todelete.rend(); ++itdel)
        attlist->erase(attlist->begin()+*itdel);
}

void SpaceColonization::process_bud(const Bud& bud)
{
        Vector3 position = node_position(bud.pid);
        Index nbg_att(bud.attractors->begin(),bud.attractors->end());

        // compute new position
        Vector3 mean_dir = pointset_mean_direction(position,attractors, nbg_att);
        Vector3 new_position = position + mean_dir * nodelength;
        new_position = attractors->getAt(findClosestFromSubset(new_position,attractors,nbg_att).first);

        // create new node
        add_node(new_position, bud.pid, nbg_att);

        // remove closest attractors
        attractor_grid->disable_points(attractor_grid->query_ball_point(position,kill_radius));

}

void SpaceColonization::growth()
{
    if (!budlist.empty()){
        size_t cparent = budlist[0].pid;
        node_child_preprocess(cparent);
        size_t nbactivenode = active_nodes.size();
        for(BudList::const_iterator itbud = budlist.begin(); itbud != budlist.end(); ++itbud)
        {
            if(cparent != itbud->pid){
                node_child_postprocess(cparent, Index(active_nodes.begin()+nbactivenode,active_nodes.end()));
                size_t nbactivenode = active_nodes.size();
                cparent = itbud->pid;
                node_child_preprocess(cparent);
            }
            if (itbud->attractors->size() > min_nb_pt_per_bud){
                process_bud(*itbud);
            }
        }
        node_child_postprocess(cparent, Index(active_nodes.begin()+nbactivenode,active_nodes.end()));
        budlist.clear();
    }
}

void SpaceColonization::step()
{
    StartEach();
    generate_all_buds();
    growth();
    EndEach();
    ++nbIteration;
}
void SpaceColonization::run()
{
    while (!atEnd()) step();
}

void SpaceColonization::iterate(size_t nbsteps)
{
    for(size_t i = 0; i < nbsteps && !atEnd(); ++i) step();
}


bool SpaceColonization::atEnd()
{
    return (active_nodes.empty() && latentbudlist.empty());
}

IndexArrayPtr SpaceColonization::get_children() const { uint32_t root; IndexArrayPtr ch = determine_children(skeletonparents,root); return ch; }

/* ----------------------------------------------------------------------- */


GraphColonization::GraphColonization(const Point3ArrayPtr attractors,
                                               real_t perception_radius,
                                               const IndexArrayPtr _graph,
                                               uint32_t _root,
                                               real_t _powerdistance,
                                               size_t spacetilingratio):
    SpaceColonization(attractors,
                      perception_radius, 0, perception_radius,
                      Point3ArrayPtr(),
                      Uint32ArrayPtr(),
                      Index(),
                      spacetilingratio ),
                      graph(_graph), root(_root), use_jonction_points(false), powerdistance(_powerdistance)

{
    min_nb_pt_per_bud = 0;
     init();
}


GraphColonization::~GraphColonization() { }

/* ----------------------------------------------------------------------- */



void GraphColonization::generate_buds(size_t pid) {
    Vector3 pos = node_position(pid);
    Index components = node_components(pid);
    real_t plevel = node_level(pid);

    real_t level_inc =  perception_radius;
    std::pair<IndexArrayPtr,RealArrayPtr> groups = next_quotient_points_from_adjacency_graph(plevel,level_inc, components, graph, distances_from_root);
    size_t nbpotentialchild = groups.first->size();
/*    size_t testedpid = 45;
    if (pid == testedpid) {
        printf("nbpotentialchild of %i : %i\n", testedpid, nbpotentialchild);
    }*/

    for(size_t i = 0; i < nbpotentialchild; ++i) {
        const Index& nextcomponents = groups.first->getAt(i);

     /*   if (pid == testedpid)
            printf("New group of %i components at level %f (from %f)\n", nextcomponents.size(), groups.second->getAt(i), plevel); */

        AttractorList nnextcomponents = attractor_grid->filter_disabled(AttractorList(nextcomponents.begin(),nextcomponents.end()));
        if (nnextcomponents.size() >= min_nb_pt_per_bud) {
            real_t level = groups.second->getAt(i);
            int dist = round((level - plevel) / level_inc);
            // printf("dist %i %f %f %f\n", dist, level, plevel, perception_radius);
            /* if (pid == testedpid)
                printf("Accepted as group of %i components at delay %i\n", nnextcomponents.size(), dist); */

            if (dist <= 1) add_bud(pid, nnextcomponents, level);
            else add_latent_bud(pid, nnextcomponents, level, dist-1);
        }
     }
    if (nbpotentialchild == 0 && use_jonction_points) {
        Vector3 new_position = centroid_of_group(attractors, components);
        uint32_t npid = add_node(new_position, plevel, components, pid, false );
    }

}

void GraphColonization::process_bud(const Bud& bud)
{
        Vector3 position = node_position(bud.pid);
        Index components(bud.attractors->begin(),bud.attractors->end());

        bool no_junction = false;
        // compute new position
        // Vector3 mean_dir = pointset_mean_direction(position,attractors, nbg_att);
        // Vector3 new_position = position + mean_dir * nodelength;
        // new_position = attractors->getAt(findClosestFromSubset(new_position,attractors,nbg_att).first);

        Vector3 new_position;
        if (!use_jonction_points) new_position = centroid_of_group(attractors, components);
        else {

            Index parent_components = node_components(bud.pid);
            std::pair<Index,Index> junctions = cluster_junction_points(graph,components,parent_components);

            Index junction(junctions.first);
            junction.insert(junction.end(), junctions.second.begin(), junctions.second.end());

            // if junction is empty, it means that another group took over on the branch. To avoid missing a branch
            // we simply use centroid.
            if (junction.empty())  { new_position = centroid_of_group(attractors, components); no_junction = false; }
            else  new_position = centroid_of_group(attractors, junction);
        }

        if (! no_junction){
            // create new node
            uint32_t pid = add_node(new_position, bud.level, components, bud.pid );

            // remove closest attractors
            attractor_grid->disable_points(bud.attractors->begin(),bud.attractors->end());
        }



}

size_t GraphColonization::add_node(const Vector3& position,
                      real_t level,
                      const Index& components,
                      size_t parent, bool active)
{
    size_t pid = SpaceColonization::add_node(position, parent , active );

    if (is_null_ptr(nodelevels))
        nodelevels = RealArrayPtr(new RealArray(pid,0));
    else {
        for(size_t i = nodelevels->size(); i < pid; ++i)
            nodelevels->push_back(0);
    }

    nodelevels->push_back(level);

    if (is_null_ptr(nodecomponents))
        nodecomponents = IndexArrayPtr(new IndexArray(pid, Index()));
    else {
        for(size_t i = nodecomponents->size(); i < pid; ++i)
            nodecomponents->push_back(components);
    }

    nodecomponents->push_back(components);

    return pid;
}


void
GraphColonization::init()
{
    std::pair<Uint32ArrayPtr,RealArrayPtr> parents_and_distances_to_root = points_dijkstra_shortest_path(attractors, graph, root, powerdistance);
    distances_from_root = parents_and_distances_to_root.second;

    real_t flevel = perception_radius;
    Index firstgroup = points_in_range_from_root(0,flevel, distances_from_root);
    Vector3 firstposition = centroid_of_group(attractors, firstgroup);
    add_node(firstposition, flevel, firstgroup);

    // remove closest attractors
    attractor_grid->disable_points(firstgroup.begin(),firstgroup.end());
}


Index
GraphColonization::junction_components(size_t nid1, size_t nid2) const
{
    Index components1 = node_components(nid1);
    Index components2 = node_components(nid2);
    std::pair<Index,Index> junctions = cluster_junction_points(graph,components1,components2);

    Index junction(junctions.first);
    junction.insert(junction.end(), junctions.second.begin(), junctions.second.end());

    return junction;


}

Vector3 GraphColonization::junction_point(size_t nid1, size_t nid2) const
{
    return centroid_of_group(attractors, junction_components(nid1,nid2));
}
/* ----------------------------------------------------------------------- */
