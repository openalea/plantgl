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

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

const size_t SpaceColonization::NOID(UINT32_MAX);

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
    min_nb_pt(3),
    skeletonnodes(initialskeletonnodes), 
    skeletonparents(initialskeletonparents),
    active_nodes(_active_nodes)
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
    min_nb_pt(3),
    skeletonnodes(), 
    skeletonparents(),
    active_nodes()
{
    add_node(root);
}

SpaceColonization::~SpaceColonization()
{
}

size_t SpaceColonization::add_node(const Vector3& position, size_t parent, bool active) {
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

    if (active)  _activate_node(pid);   

    // remove closest attractors
    attractor_grid->disable_points(attractor_grid->query_ball_point(position,kill_radius));

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
    result.push_back(v0);
    for (int i = 1; i < nb; ++i)
        result.push_back(Matrix3::axisRotation(dir, i * 2*GEOM_PI/nb) *v0);
    return result;
}

void SpaceColonization::generate_buds(size_t pid) {
    Vector3 pos = node_position(pid);
    Vector3 dir = node_direction(pid);
    std::vector<Vector3> lateral_dirs = lateral_directions(dir, GEOM_PI/2, 4);
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
    if (neighbour_attractor_indices.size() >= min_nb_pt) {
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

void SpaceColonization::generate_all_buds() {
    budlist.clear();
    attractormap.clear();
    for(Index::const_iterator it = active_nodes.begin(); it != active_nodes.end(); ++it){
        node_buds_preprocess(*it);
        generate_buds(*it);
        node_buds_postprocess(*it);
    }
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

void SpaceColonization::growth() 
{
    for(BudList::const_iterator itbud = budlist.begin(); itbud != budlist.end(); ++itbud)
    {
        if (itbud->attractors->size() > min_nb_pt){
            Vector3 position = node_position(itbud->pid);
            Index nbg_att(itbud->attractors->begin(),itbud->attractors->end());

            // compute new position
            Vector3 mean_dir = pointset_mean_direction(position,attractors, nbg_att);
            Vector3 new_position = position + mean_dir * nodelength;
            new_position = attractors->getAt(findClosestFromSubset(new_position,attractors,nbg_att).first);
    
            // create new node
            add_node(new_position,itbud->pid);
        }
    }
    budlist.clear();
    attractormap.clear();
}

void SpaceColonization::step()
{
    generate_all_buds();
    growth();
}
void SpaceColonization::run()
{
    while(!active_nodes.empty())
        step();
}

IndexArrayPtr SpaceColonization::get_children() const { uint32_t root; IndexArrayPtr ch = determine_children(skeletonparents,root); return ch; }
