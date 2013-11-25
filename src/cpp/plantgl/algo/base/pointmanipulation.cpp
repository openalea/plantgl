/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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


#include "pointmanipulation.h"
#include <plantgl/scenegraph/container/indexarray_iterator.h>
#include <stdio.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE







class ProgressStatus {
public:    
    ProgressStatus (uint32_t _nbsteps, char * _message =" %.2f processed.", float _percenttoprint = 1):
            nbsteps(_nbsteps), 
            percenttoprint(_percenttoprint), 
            message(_message),
            current(1),
            cpercent(-_percenttoprint) { }
    
     inline ProgressStatus & operator ++()  { increment(); return *this; }

protected:

    void increment() { 
        ++current; 
        if (current <= nbsteps){
            if (current == nbsteps){
                    std::string msg("\x0d");
                    msg += message;
                    msg += "\n";
                    printf(msg.c_str(),100.0);
            }
            else {
                real_t ncpercent = 100 * current / float(nbsteps);
                if(cpercent +  percenttoprint <= ncpercent ) {
                    std::string msg("\x0d");
                    msg += message;
                    printf(msg.c_str(),ncpercent);
                    cpercent = ncpercent;
                }
            }
        }
    }
    uint32_t current;
    uint32_t nbsteps;
    real_t cpercent;
    real_t percenttoprint;
    char * message;
};


#ifdef WITH_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
// #include <CGAL/Triangulation_3.h>

#include "cgalwrap.h"

#endif

#include <stack>
#include <iostream>
#include <cstdio>

IndexArrayPtr 
PGL::delaunay_point_connection(const Point3ArrayPtr points)
{
#ifdef WITH_CGAL

    typedef CGAL::Exact_predicates_inexact_constructions_kernel         TK;
    typedef CGAL::Triangulation_vertex_base_with_info_3<uint32_t, TK>   TVb;
    typedef CGAL::Triangulation_data_structure_3<TVb>                    Tds;

    typedef CGAL::Delaunay_triangulation_3<TK, Tds>                      Triangulation;
    // typedef CGAL::Triangulation_3<K,Tds>      Triangulation;


    typedef Triangulation::Cell_handle    TCell_handle;
    typedef Triangulation::Vertex_handle  TVertex_handle;
    typedef Triangulation::Locate_type    TLocate_type;
    typedef Triangulation::Point          TPoint;
    typedef Triangulation::Segment        TSegment;

    Triangulation triangulation;
    uint32_t pointCount = 0;
    for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it)
        triangulation.insert(toPoint3<TPoint>(*it))->info() = pointCount++;


    IndexArrayPtr result(new IndexArray(points->size(),Index()));
    for(Triangulation::Finite_edges_iterator it = triangulation.finite_edges_begin();
        it != triangulation.finite_edges_end(); ++it){
            uint32_t source = it->first->vertex(it->second)->info();
            uint32_t target = it->first->vertex(it->third)->info();
            result->getAt(source).push_back(target);
            result->getAt(target).push_back(source);
    } 
#else
    #ifdef _MSC_VER
    #pragma message("function 'delaunay_point_connection' disabled. CGAL needed.")
    #else
    #warning "function 'delaunay_point_connection' disabled. CGAL needed"
    #endif

    IndexArrayPtr result;
#endif
    return result;
}


struct PointSorter {
    PointSorter(Point3ArrayPtr _points, uint32_t _ref) : points(_points), refpoint(_points->getAt(_ref)) {}
    Point3ArrayPtr points;
    const Vector3& refpoint;

    bool operator()(uint32_t i, uint32_t j){
        return norm(refpoint-points->getAt(i)) <  norm(refpoint-points->getAt(j));  
    }
};

 IndexArrayPtr 
PGL::k_closest_points_from_delaunay(const Point3ArrayPtr points, size_t k)
{
    IndexArrayPtr res = delaunay_point_connection(points);
    IndexArrayPtr filteredres(new IndexArray(res->size()));
    IndexArray::iterator ittarget = filteredres->begin();
    uint32_t pointid = 0;
    for (IndexArray::iterator it = res->begin(); it != res->end(); ++it, ++ittarget){
        if (it->size() <= k) *ittarget = *it;
        else {
            std::sort(it->begin(), it->end(),PointSorter(points,pointid));
            *ittarget = Index(it->begin(),it->begin()+k);
        }
        ++pointid;
    }
    return filteredres;
}


#include <plantgl/algo/grid/kdtree.h>

IndexArrayPtr 
PGL::k_closest_points_from_ann(const Point3ArrayPtr points, size_t k, bool symmetric)
{
#ifdef WITH_ANN
    ANNKDTree3 kdtree(points);
    IndexArrayPtr result = kdtree.k_nearest_neighbors(k);
    if(symmetric) result = symmetrize_connections(result);
    return result;
#else
    #ifdef _MSC_VER
    #pragma message("function 'k_closest_points_from_ann' disabled. ANN needed.")
    #else
    #warning "function 'k_closest_points_from_ann' disabled. ANN needed"
    #endif

    return IndexArrayPtr();
#endif
}

#ifdef WITH_CGAL
#include <CGAL/Cartesian.h>
#endif
/*
//definition of the property map
struct My_point_property_map{
  typedef CPoint value_type;
  typedef const value_type& reference;
  typedef const CPoint_and_int& key_type;
  typedef boost::readable_property_map_tag category;
};

//get function for the property map
My_point_property_map::reference 
get(My_point_property_map,My_point_property_map::key_type p)
{return boost::get<0>(p);}

#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Search_traits_3.h>
#include <CGAL/Search_traits_adapter.h>

typedef CGAL::Search_traits_3<CK>                                                   Traits_base;
typedef CGAL::Search_traits_adapter<CPoint_and_int,My_point_property_map,Traits_base>    Traits;


typedef CGAL::Orthogonal_k_neighbor_search<Traits>                      K_neighbor_search;
typedef K_neighbor_search::Tree                                         Tree;
typedef K_neighbor_search::Distance                                     Distance;


// typedef CGAL::Search_traits_3<K> TreeTraits;
// typedef CGAL::Orthogonal_k_neighbor_search<TreeTraits> Neighbor_search;
// typedef Neighbor_search::Tree Tree;

#endif

IndexArrayPtr 
PGL::k_closest_points_from_cgal(const Point3ArrayPtr points, size_t k)
{
    size_t nbPoints = points->size();
    IndexArrayPtr result(new IndexArray(nbPoints,Index(0)));

    std::list<CPoint_and_int> pointdata;
    size_t pid = 0;
	for (Point3Array::const_iterator it = points->begin(); it != points->end() ; ++it, ++pid)
		pointdata.push_back(CPoint_and_int(toPoint<CPoint>(*it),pid));

   Tree tree(pointdata.begin(), pointdata.end());

   std::list<CPoint_and_int>::const_iterator piter = pointdata.begin();
   for(pid = 0; pid < nbPoints && piter != pointdata.end(); ++pid, ++piter){
       K_neighbor_search search(tree, piter->get<0>(), k+1);
       Index& cindex = result->getAt(pid);
       for(K_neighbor_search::iterator it = search.begin(); it != search.end(); ++it){
           size_t ngpid = it->first.get<1>();
           if (ngpid != pid) cindex.push_back(ngpid);
        }
   }


    return result;
}
*/

IndexArrayPtr 
PGL::symmetrize_connections(const IndexArrayPtr adjacencies)
{
    IndexArrayPtr newadjacencies(new IndexArray(*adjacencies));
    size_t pid = 0;
    for(IndexArray::const_iterator itpc = newadjacencies->begin(); itpc != newadjacencies->end(); ++itpc, ++pid)
    {
        for(Index::const_iterator itc = itpc->begin(); itc != itpc->end(); ++itc)
        {
            Index& target = newadjacencies->getAt(*itc);
            if(find(target.begin(),target.end(),pid) == target.end())
                target.push_back(pid);
        }
    }

    return newadjacencies;
}

#include "dijkstra.h"
#include <plantgl/tool/util_hashset.h>
#include <plantgl/tool/util_hashmap.h>
#include <list>

struct OneDistance {
        inline real_t operator()(uint32_t a, uint32_t b) const { return 1; }
        OneDistance() {}
};

ALGO_API IndexArrayPtr 
PGL::connect_all_connex_components(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, bool verbose)
{
#ifdef WITH_ANN
    // ids of points not accessible from the root connex component
    pgl_hash_set_uint32 nonconnected;

    size_t nbtotalpoints = points->size();

    // connected points. they are not inserted in the same order than points
    Point3ArrayPtr refpoints(new Point3Array());
    // nb of connected points. store to not recompute each time
    uint32_t nbrefpoints = 0;
    // map of point id from 'refpoints' to 'points' structure
    pgl_hash_map<uint32_t,uint32_t> pidmap;

    // connections to add to connect all connex components
    std::list<std::pair<uint32_t,uint32_t> > addedconnections;
    // root to consider for next connex component
    uint32_t next_root = 0;

    OneDistance distcomputer;
    while (true){

        // find all points accessible from next_root
        std::pair<Uint32Array1Ptr,RealArrayPtr> root_connections = dijkstra_shortest_paths(adjacencies,next_root,distcomputer);
        Uint32Array1Ptr parents = root_connections.first;

        // update set of refpoints (point connected) and non connnected.
        if(next_root == 0) { // if it is the first round, add nonconnected
            uint32_t pid = 0;
            for (Uint32Array1::const_iterator itParent = parents->begin(); itParent != parents->end(); ++itParent, ++pid)
            {
                if (*itParent == UINT32_MAX) {
                    nonconnected.insert(pid);
                }
                else {
                    refpoints->push_back(points->getAt(pid));
                    pidmap[nbrefpoints] = pid;
                    ++nbrefpoints;
                }
            }
        }
        else
        {   // iter on nonconnected and remove a point from this set if it has a parent.
            std::list<uint32_t> toerase;
            for(pgl_hash_set_uint32::const_iterator itncpid = nonconnected.begin();
                itncpid != nonconnected.end(); ++itncpid)
            {
                if(parents->getAt(*itncpid) != UINT32_MAX)
                {
                    uint32_t pid = *itncpid;
                    refpoints->push_back(points->getAt(pid));
                    pidmap[nbrefpoints] = pid;
                    ++nbrefpoints;
                    toerase.push_back(pid);
                }
            }
            for(std::list<uint32_t>::const_iterator iterase = toerase.begin(); iterase != toerase.end(); ++iterase)
                nonconnected.erase(*iterase);
        }
        if (verbose) printf("\x0dNb points processed %i (%.2f%%) [left : %i (%.2f%%)].\n",nbrefpoints,100 * nbrefpoints /float(nbtotalpoints), nonconnected.size(), 100 * nonconnected.size()/float(nbtotalpoints),next_root);
        // if no more non connected point, break
        if (nonconnected.empty()) break;

        // create kdtree from connected points
        ANNKDTree3 kdtree(refpoints);
        real_t dist = REAL_MAX;
        std::pair<uint32_t,uint32_t> connection;
        bool allempty = true;

        // find shortest connections between a non connected point and a connected one
        for(pgl_hash_set_uint32::const_iterator itncpid = nonconnected.begin(); itncpid != nonconnected.end(); ++itncpid){
            Index cp = kdtree.k_closest_points(points->getAt(*itncpid),1,dist);
            if (!cp.empty()){
                uint32_t refpoint = pidmap[cp[0]];
                real_t newdist = norm(points->getAt(refpoint)-points->getAt(*itncpid));
                if (newdist < dist ){
                    connection = std::pair<uint32_t,uint32_t>(refpoint,*itncpid);
                    dist = newdist;
                }
                allempty = false;
            }
        }
        assert(!allempty);
        // add the found connections into addedconnections and consider non connected point as next_root
        addedconnections.push_back(connection);
        next_root = connection.second;               
    }

    // copy adjacencies and update it with addedconnections
    IndexArrayPtr newadjacencies(new IndexArray(*adjacencies));
    for(std::list<std::pair<uint32_t,uint32_t> >::const_iterator itc = addedconnections.begin();
            itc != addedconnections.end(); ++itc)
    {
            newadjacencies->getAt(itc->first).push_back(itc->second);
            newadjacencies->getAt(itc->second).push_back(itc->first);
    }

    return newadjacencies;
#else
    #ifdef _MSC_VER
    #pragma message("function 'connect_all_connex_components' disabled. ANN needed.")
    #else
    #warning "function 'connect_all_connex_components' disabled. ANN needed"
    #endif

    return adjacencies;
#endif
}



struct PointDistance {
        const Point3ArrayPtr points;
        real_t operator()(uint32_t a, uint32_t b) const { return norm(points->getAt(a)-points->getAt(b)); }
        PointDistance(const Point3ArrayPtr _points) : points(_points) {}
};

struct PowerPointDistance {
        const Point3ArrayPtr points;
        real_t power;
        real_t operator()(uint32_t a, uint32_t b) const { return pow(norm(points->getAt(a)-points->getAt(b)),power) ; }
        PowerPointDistance(const Point3ArrayPtr _points, real_t _power) : points(_points), power(_power) {}
};

Index
PGL::r_neighborhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const real_t radius)
{
    GEOM_ASSERT(points->size() == adjacencies->size());

    struct PointDistance pdevaluator(points);

    NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,pid,pdevaluator,radius);
    Index result;
    for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
          result.push_back(itn->id);
    return result;
}

IndexArrayPtr 
PGL::r_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const RealArrayPtr radii)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    struct PointDistance pdevaluator(points);

    uint32_t current = 0;
    IndexArrayPtr result(new IndexArray(nbPoints));
    for(RealArray::const_iterator itradii = radii->begin(); itradii != radii->end(); ++itradii) {
        NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,*itradii);
        Index lres;
        for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
            lres.push_back(itn->id);
        result->setAt(current,lres);
        current++;

    }
    return result;
}

IndexArrayPtr 
PGL::r_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, bool verbose)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    struct PointDistance pdevaluator(points);
    
    IndexArrayPtr result(new IndexArray(nbPoints));
    ProgressStatus st(nbPoints,"R-neighborhood computed for %.2f%% of points.");
    for(uint32_t current = 0; current < nbPoints; ++current) {
        NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,radius);
        Index lres;
        for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
            lres.push_back(itn->id);
        result->setAt(current,lres);
        if (verbose) ++st;
    }
    return result;
}

struct PointAnisotropicDistance {
        const Point3ArrayPtr points;
	    Vector3 direction;
	    real_t alpha, beta;
        real_t operator()(uint32_t a, uint32_t b) const { return radialAnisotropicNorm(points->getAt(a)-points->getAt(b),direction,alpha,beta); }
        PointAnisotropicDistance(const Point3ArrayPtr _points, const Vector3& _direction, real_t _alpha, real_t _beta) 
		: points(_points), direction(_direction), alpha(_alpha), beta(_beta) {}
};


Index
PGL::r_anisotropic_neighborhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const real_t radius, 
					 const Vector3& direction, 
					 const real_t alpha, const real_t beta)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());

    struct PointAnisotropicDistance pdevaluator(points,direction,alpha,beta);

    NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,pid,pdevaluator,radius);
    Index result;
    for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
          result.push_back(itn->id);
    return result;
}

IndexArrayPtr 
PGL::r_anisotropic_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const RealArrayPtr radii, 
			         const Point3ArrayPtr directions,
				 const real_t alpha, const real_t beta)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    GEOM_ASSERT(nbPoints == directions->size());

    uint32_t current = 0;
    IndexArrayPtr result(new IndexArray(nbPoints));

    for(RealArray::const_iterator itradii = radii->begin(); itradii != radii->end(); ++itradii) {
	struct PointAnisotropicDistance pdevaluator(points,directions->getAt(current),alpha,beta);
        NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,*itradii);
        Index lres;
        for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
            lres.push_back(itn->id);
        result->setAt(current,lres);
        ++current;
    }
    return result;
}


IndexArrayPtr 
PGL::r_anisotropic_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, 
			                      const Point3ArrayPtr directions,
				                  const real_t alpha, const real_t beta)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    GEOM_ASSERT(nbPoints == directions->size());

    
    IndexArrayPtr result(new IndexArray(nbPoints));
    for(uint32_t current = 0; current < nbPoints; ++current) {
	    struct PointAnisotropicDistance pdevaluator(points,directions->getAt(current),alpha,beta);
        NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,radius);
        Index lres;
        for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
            lres.push_back(itn->id);
        result->setAt(current,lres);
    }
    return result;
}


real_t
PGL::density_from_r_neighborhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies, 
                               const real_t radius)
{
    Index r_neighborhood_value = r_neighborhood(pid, points, adjacencies, radius);
    return r_neighborhood_value.size()/ (radius * radius);
}


TOOLS(RealArrayPtr)
PGL::densities_from_r_neighborhood(const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies, 
                               const real_t radius)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    RealArrayPtr result(new RealArray(nbPoints));
    uint32_t current = 0;
    for(RealArray::iterator itres = result->begin(); itres != result->end(); ++itres){
        *itres = density_from_r_neighborhood(current, points,adjacencies,radius);
        ++current;
    }
    return result;
}

Index PGL::get_k_closest_from_n(const Index& adjacencies, const uint32_t k, uint32_t pid, const Point3ArrayPtr points)
{
        uint32_t nbnbg = adjacencies.size();
        if (nbnbg <= k) return adjacencies;
        RealArrayPtr distances(new RealArray(nbnbg));
        Vector3 self = points->getAt(pid);
        for(size_t pnid = 0; pnid < nbnbg; ++pnid)
                distances->setAt(pnid,norm(self-points->getAt(pnid)));
        Index sorted = get_sorted_element_order(distances);
        return Index(sorted.begin(),sorted.begin()+k);
}

Index PGL::k_neighborhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k)
{
    GEOM_ASSERT(points->size() == adjacencies->size());

    Index result;
    uint32_t nbnbg = adjacencies->getAt(pid).size();
    if (k <= nbnbg) {
        result = get_k_closest_from_n(adjacencies->getAt(pid),k,pid,points);
    }
    else {
        struct PointDistance pdevaluator(points);

        NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,pid,pdevaluator,REAL_MAX,k);
        for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
              result.push_back(itn->id);
    }
    return result;
}

IndexArrayPtr
PGL::k_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    struct PointDistance pdevaluator(points);
    
    IndexArrayPtr result(new IndexArray(nbPoints));
    for(uint32_t current = 0; current < nbPoints; ++current) {
        Index lres;
        const Index& candidates = adjacencies->getAt(current);
        /*if (k <= candidates.size()) {
            lres = get_k_closest_from_n(candidates,k,current,points);
        }
        else {*/
            NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,REAL_MAX,k);
            for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
                lres.push_back(itn->id);
        // }
        result->setAt(current,lres);
    }
    return result;
}


real_t
PGL::pointset_max_distance(  const Vector3& origin,
                                 const Point3ArrayPtr points, 
			                     const Index& group)
{
    real_t max_distance = 0;
    for(Index::const_iterator it = group.begin(); it != group.end(); ++it)
        max_distance = std::max(max_distance,norm(origin-points->getAt(*it)));
    return max_distance;
}

real_t
PGL::pointset_max_distance(  uint32_t pid,
                                 const Point3ArrayPtr points, 
			                     const Index& group)
{
    return pointset_max_distance(points->getAt(pid), points, group);
}

ALGO_API real_t
PGL::pointset_mean_distance(  const Vector3& origin,
                const Point3ArrayPtr points, 
			    const Index& group)
{
    if (group.empty()) return 0;
    real_t sum_distance = 0;
    for(Index::const_iterator it = group.begin(); it != group.end(); ++it)
        sum_distance += norm(origin-points->getAt(*it));
    return sum_distance / group.size();
}


ALGO_API real_t
PGL::pointset_mean_radial_distance(  const Vector3& origin,
                                     const Vector3& direction,
                                     const Point3ArrayPtr points, 
			                         const Index& group)
{
    if (group.empty()) return 0;
    real_t sum_distance = 0;
    for(Index::const_iterator it = group.begin(); it != group.end(); ++it)
        sum_distance += radialAnisotropicNorm(origin-points->getAt(*it), direction, 0, 1);
    return sum_distance / group.size();
}



real_t
PGL::density_from_k_neighborhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k)
{
    Index adjacency = (k == 0 ? adjacencies->getAt(pid) : k_neighborhood(pid, points, adjacencies, k ));
    real_t radius = pointset_max_distance(pid, points, adjacency);
    return adjacency.size() / (radius * radius);
}


TOOLS(RealArrayPtr)
PGL::densities_from_k_neighborhood(const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    RealArrayPtr result(new RealArray(nbPoints));
    ProgressStatus  st(nbPoints, "density computed for %.2f%% of points.");
    uint32_t current = 0;
    for(RealArray::iterator itres = result->begin(); itres != result->end(); ++itres, ++current, ++st){
        *itres = density_from_k_neighborhood(current, points,adjacencies,k);
    }
    return result;
}

#ifdef WITH_CGAL
#include <CGAL/linear_least_squares_fitting_3.h>
#endif

Vector3 PGL::pointset_orientation(const Point3ArrayPtr points, const Index& group )
{
#ifdef WITH_CGAL
    typedef CGAL::Cartesian<real_t>   CK;
    typedef CK::Point_3               CPoint;
    typedef CK::Line_3                CLine;

	std::list<CPoint> pointdata;
	// for (Point3Array::const_iterator it = points->begin(); it != points->end() ; ++it)
	for (Index::const_iterator it = group.begin(); it != group.end() ; ++it)
		pointdata.push_back(toPoint3<CPoint>(points->getAt(*it)));

	CLine line;
	linear_least_squares_fitting_3(pointdata.begin(), pointdata.end(), line, CGAL::Dimension_tag<0>());

	return toVector3(line.to_vector());
#else
    #ifdef _MSC_VER
    #pragma message("function 'pointset_orientation' disabled. CGAL needed.")
    #else
    #warning "function 'pointset_orientation' disabled. CGAL needed"
    #endif

    return Vector3(0,0,0);
#endif
}

ALGO_API TOOLS(Vector3) 
PGL::triangleset_orientation(const Point3ArrayPtr points, const Index3ArrayPtr triangles)
{
#ifdef WITH_CGAL
    typedef CGAL::Cartesian<real_t>   CK;
    typedef CK::Point_3               CPoint;
    typedef CK::Line_3                CLine;
	typedef CK::Triangle_3			  CTriangle;

	std::list<CTriangle> cgaltriangles;
	for (Index3Array::const_iterator it = triangles->begin(); it != triangles->end() ; ++it)
		cgaltriangles.push_back(
				CTriangle(toPoint3<CPoint>(points->getAt(it->getAt(0))),
						  toPoint3<CPoint>(points->getAt(it->getAt(1))),
				          toPoint3<CPoint>(points->getAt(it->getAt(2)))
					));	


	CLine line;
	linear_least_squares_fitting_3(cgaltriangles.begin(), cgaltriangles.end(), line, CGAL::Dimension_tag<0>());

	return toVector3(line.to_vector());
#else
    #ifdef _MSC_VER
    #pragma message("function 'pointset_orientation' disabled. CGAL needed.")
    #else
    #warning "function 'pointset_orientation' disabled. CGAL needed"
    #endif

    return Vector3(0,0,0);
#endif
}


Point3ArrayPtr PGL::pointsets_orientations(const Point3ArrayPtr points, const IndexArrayPtr groups)
{
	size_t nbPoints = points->size();
	Point3ArrayPtr result(new Point3Array(nbPoints));

    ProgressStatus st(nbPoints, "orientations computed for %.2f%% of points.");
    IndexArray::const_iterator itNbrhd = groups->begin();
    for(Point3Array::iterator itRes = result->begin(); itRes != result->end(); ++itRes, ++itNbrhd, ++st)
        *itRes = pointset_orientation(points,*itNbrhd);

	return result;
}

#ifdef WITH_CGAL
#include <CGAL/Cartesian.h>
#include <CGAL/Monge_via_jet_fitting.h>
#endif


CurvatureInfo
PGL::principal_curvatures(const Point3ArrayPtr points, uint32_t pid, const Index& group, size_t fitting_degree , size_t monge_degree)
{
      CurvatureInfo result;
#ifdef WITH_CGAL
#ifdef WITH_LAPACK

        typedef CGAL::Cartesian<real_t>  Data_Kernel;
        typedef Data_Kernel::Point_3     DPoint;
        typedef CGAL::Monge_via_jet_fitting<Data_Kernel> My_Monge_via_jet_fitting;
        typedef My_Monge_via_jet_fitting::Monge_form     My_Monge_form;

      std::vector<DPoint> in_points;
      in_points.push_back(toPoint3<DPoint>(points->getAt(pid)));

      for(Index::const_iterator itNg = group.begin(); itNg != group.end(); ++itNg)
          if (*itNg != pid) in_points.push_back(toPoint3<DPoint>(points->getAt(*itNg)));

      My_Monge_form monge_form;
      My_Monge_via_jet_fitting monge_fit;
      monge_form = monge_fit(in_points.begin(), in_points.end(), fitting_degree, monge_degree);

      result.origin = toVector3(monge_form.origin());
      result.maximal_principal_direction = toVector3(monge_form.maximal_principal_direction());
      result.maximal_curvature = monge_form.principal_curvatures(0);
      result.minimal_principal_direction = toVector3(monge_form.minimal_principal_direction());
      result.minimal_curvature = monge_form.principal_curvatures(1);
      result.normal = toVector3(monge_form.normal_direction());

#else
    #ifdef _MSC_VER
    #pragma message("function 'principal_curvatures' disabled. LAPACK needed.")
    #else
    #warning "function 'principal_curvatures' disabled. LAPACK needed"
    #endif
#endif

#else
    #ifdef _MSC_VER
    #pragma message("function 'principal_curvatures' disabled. CGAL needed.")
    #else
    #warning "function 'principal_curvatures' disabled. CGAL needed"
    #endif
#endif
      return result;

}

std::vector<CurvatureInfo>
PGL::principal_curvatures(const Point3ArrayPtr points, const IndexArrayPtr groups, size_t fitting_degree , size_t monge_degree)
{
    std::vector<CurvatureInfo> result;
    uint32_t i = 0;
    for(IndexArray::const_iterator it = groups->begin(); it != groups->end(); ++it, ++i)
        result.push_back(principal_curvatures(points,i,*it,fitting_degree,monge_degree));
    return result;
}

std::vector<CurvatureInfo>
PGL::principal_curvatures(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, size_t fitting_degree , size_t monge_degree)
{
    std::vector<CurvatureInfo> result;
    uint32_t nbPoints = points->size();

    for(uint32_t i = 0; i < nbPoints; ++i){
        Index ng = r_neighborhood(i,points, adjacencies, radius);
        result.push_back(principal_curvatures(points,i,ng,fitting_degree,monge_degree));
    }
    return result;

}

Vector3 PGL::pointset_normal(const Point3ArrayPtr points, const Index& group )
{
#ifdef WITH_CGAL
    typedef CGAL::Cartesian<real_t>   CK;
    typedef CK::Point_3               CPoint;
    typedef CK::Plane_3                CPlane;

	std::list<CPoint> pointdata;
	// for (Point3Array::const_iterator it = points->begin(); it != points->end() ; ++it)
	for (Index::const_iterator it = group.begin(); it != group.end() ; ++it)
		pointdata.push_back(toPoint3<CPoint>(points->getAt(*it)));

	CPlane plane;
	linear_least_squares_fitting_3(pointdata.begin(), pointdata.end(), plane, CGAL::Dimension_tag<0>());

	return dir2Vector3(plane.orthogonal_direction());
#else
    #ifdef _MSC_VER
    #pragma message("function 'pointset_normal' disabled. CGAL needed.")
    #else
    #warning "function 'pointset_normal' disabled. CGAL needed"
    #endif

    return Vector3(0,0,0);
#endif
}


Point3ArrayPtr
PGL::pointsets_normals(const Point3ArrayPtr points, const IndexArrayPtr groups)
{
    Point3ArrayPtr result(new Point3Array(points->size()));
    uint32_t i = 0;
    for(IndexArray::const_iterator it = groups->begin(); it != groups->end(); ++it, ++i){
        result->setAt(i, pointset_normal(points,*it));
    }
    return result;
}


struct PointNormalDistance {
        const Point3ArrayPtr normals;

        real_t operator()(uint32_t a, uint32_t b) const { 
            real_t v = 1.0 - std::abs(normals->getAt(a) * normals->getAt(b)); 
            if (v < 0) return 0;
            return v;
        }

        PointNormalDistance(const Point3ArrayPtr _normals) : normals(_normals) {}
};

Point3ArrayPtr 
PGL::pointsets_orient_normals(const Point3ArrayPtr normals, const Point3ArrayPtr points, const IndexArrayPtr adjacencies)
{

    /// find first point
    Point3Array::const_iterator topz = points->getZMax();
    size_t source = std::distance<Point3Array::const_iterator>(points->begin(),topz);
    Vector3& firstnormal = normals->getAt(source);
    if (firstnormal.z() < 0) firstnormal = -firstnormal;
    
    return pointsets_orient_normals(normals, source, adjacencies);
}

Point3ArrayPtr 
PGL::pointsets_orient_normals(const Point3ArrayPtr initialnormals, uint32_t source, const IndexArrayPtr adjacencies)
{
    const Point3ArrayPtr normals(new Point3Array(*initialnormals));

    /// compute shortest pathes using normal difference
    struct PointNormalDistance pdevaluator( normals);
    std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)> treegraph = dijkstra_shortest_paths(adjacencies,source,pdevaluator);

    /// propagate normal orientation
    TOOLS(Uint32Array1Ptr) parents = treegraph.first;
    IndexArrayPtr children = determine_children(parents, source);

    IndexArrayPreOrderConstIterator piterator(children, source);

    for(++piterator; !piterator.atEnd(); ++piterator){
        if (dot(normals->getAt(parents->getAt(*piterator)),normals->getAt(*piterator)) < 0)
        {
            // if not oriented similarly, reorient
            normals->getAt(*piterator) = -normals->getAt(*piterator);
        }
    }

    return normals;



}


Index
PGL::point_section( uint32_t pid,
                    const Point3ArrayPtr points,
                    const IndexArrayPtr adjacencies, 
                    const Vector3& direction, 
                    real_t width)
{
    Vector3& pt = points->getAt(pid); 
    Vector3 dir = direction.normed();
    Index result;
    result.push_back(pid);

    std::set<uint32_t> considered;
    considered.insert(pid);
    std::deque<uint32_t> toprocess;
    toprocess.push_back(pid);
    while ( !toprocess.empty() )
    {
        uint32_t cid = toprocess.front();
        toprocess.pop_front();
        
        const Index& padjacency = adjacencies->getAt(cid);
        for(Index::const_iterator it = padjacency.begin(); it != padjacency.end(); ++it)
               if(considered.find(*it) == considered.end()){
                   considered.insert(*it);
                   if (radialAnisotropicNorm(points->getAt(*it)-pt,dir,1,0) < width) {
                       result.push_back(*it);
                       toprocess.push_back(*it);
                   }
               }
    }
    return result;
}

Index
PGL::point_section( uint32_t pid,
                    const Point3ArrayPtr points,
                    const IndexArrayPtr adjacencies, 
                    const Vector3& direction, 
                    real_t width,
                    real_t maxradius)
{
    Vector3& pt = points->getAt(pid); 
    Vector3 dir = direction.normed();
    Index result;
    result.push_back(pid);

    std::set<uint32_t> considered;
    considered.insert(pid);
    std::deque<uint32_t> toprocess;
    toprocess.push_back(pid);
    while ( !toprocess.empty() )
    {
        uint32_t cid = toprocess.front();
        toprocess.pop_front();
        
        const Index& padjacency = adjacencies->getAt(cid);
        for(Index::const_iterator it = padjacency.begin(); it != padjacency.end(); ++it)
               if(considered.find(*it) == considered.end()){
                   considered.insert(*it);
                   Vector3 lpoint = points->getAt(*it)-pt;
                   real_t a = dot(lpoint,dir); real_t b = norm(lpoint - a*dir);
                   if ((fabs(a) < width)&& (b < maxradius)){
                       result.push_back(*it);
                       toprocess.push_back(*it);
                   }
               }
    }
    return result;
}

ALGO_API IndexArrayPtr
PGL::points_sections( const Point3ArrayPtr points,
                          const IndexArrayPtr adjacencies, 
                          const Point3ArrayPtr directions, 
                          real_t width)
{
    uint32_t nbPoints = points->size();
    IndexArrayPtr result(new IndexArray(nbPoints));
    uint32_t i = 0;
    ProgressStatus st(nbPoints, "sections computed for %.2f%% of points.");
    Point3Array::const_iterator itd = directions->begin();

    for(IndexArray::iterator it = result->begin(); it != result->end(); ++it, ++i, ++itd, ++st){
        *it = point_section(i,points, adjacencies, *itd,  width);
    }
    return result;
}

#include <plantgl/algo/fitting/fit.h>

std::pair<Vector3,real_t>
PGL::pointset_circle( const Point3ArrayPtr points,
                      const Index&  group,
                      bool bounding)
{
    size_t gsize = group.size();
    if (gsize == 0) return std::pair<Vector3,real_t>(Vector3(0,0,0),0);
    else if (gsize == 1) return std::pair<Vector3,real_t>(points->getAt(group[0]),0);

    Vector3 center; 
    Plane3 plane;

    if(Fit::plane(points, center, plane, group)){
        Vector3 a = plane.base1(); Vector3 b = plane.base2();
        a.normalize(); b.normalize();

        if (bounding) {
            Point2ArrayPtr lpoints2(new Point2Array(group.size()));
            Point2Array::iterator  itp2 = lpoints2->begin();
            for(Index::const_iterator itg = group.begin(); itg != group.end(); ++itg, ++itp2){
                Vector3 p = points->getAt(*itg)-center;
                *itp2 = Vector2(dot(p,a),dot(p,b));
            }


            Vector2 center2;
            real_t radius;
            if(Fit::boundingCircle(lpoints2,center2,radius)){
                Vector3 center3 = center + a * center2.x() + b * center2.y();
                return std::pair<Vector3,real_t>(center3,radius);
            }
            else 
                return std::pair<Vector3,real_t>(center,pointset_mean_radial_distance(center, plane.getNormal(), points, group));
        }
        else return std::pair<Vector3,real_t>(center,pointset_mean_radial_distance(center, plane.getNormal(), points, group));
    }
    else {
        center = centroid_of_group(points,group); 
        return std::pair<Vector3,real_t>(center,pointset_mean_radial_distance(center, plane.getNormal(), points, group));
    }
}

std::pair<Vector3,real_t>
PGL::pointset_circle( const Point3ArrayPtr points,
                      const Index&  group,
                      const Vector3& direction,
                      bool bounding)
{
    size_t gsize = group.size();
    if (gsize == 0) return std::pair<Vector3,real_t>(Vector3(0,0,0),0);
    else if (gsize == 1) return std::pair<Vector3,real_t>(points->getAt(group[0]),0);

    Vector3 center = centroid_of_group(points,group); 

    if (bounding){
        Plane3 plane(direction,0);
        Vector3 a = plane.base1(); Vector3 b = plane.base2();
        a.normalize(); b.normalize();

        Point2ArrayPtr lpoints2(new Point2Array(group.size()));
        Point2Array::iterator  itp2 = lpoints2->begin();
        for(Index::const_iterator itg = group.begin(); itg != group.end(); ++itg, ++itp2){
                Vector3 p = points->getAt(*itg)-center;
                *itp2 = Vector2(dot(p,a),dot(p,b));
        }

        Vector2 center2;
        real_t radius;
        if(Fit::boundingCircle(lpoints2,center2,radius)){
            Vector3 center3 = center + a * center2.x() + b * center2.y();
            return std::pair<Vector3,real_t>(center3,radius);
        }
        else return std::pair<Vector3,real_t>(center,pointset_mean_radial_distance(center,direction,points, group));
    }
    else 
        return std::pair<Vector3,real_t>(center,pointset_mean_radial_distance(center, direction, points, group));
}

std::pair<Point3ArrayPtr,RealArrayPtr>
PGL::pointsets_circles( const Point3ArrayPtr points,
                        const IndexArrayPtr  groups,
                        const Point3ArrayPtr directions,
                        bool bounding)
{
    size_t nbpoints = points->size();
    Point3ArrayPtr respoints(new Point3Array(nbpoints));
    RealArrayPtr   resradius(new RealArray(nbpoints,0));
    Point3Array::iterator itrp = respoints->begin();
    RealArray::iterator itrr = resradius->begin();
    size_t pid = 0;

    ProgressStatus st(nbpoints, "circles computed for %.2f%% of points.");

    for (IndexArray::const_iterator itg = groups->begin(); itg != groups->end(); ++itg, ++itrp, ++itrr, ++st){
        std::pair<Vector3,real_t> lres ;
        if (directions) lres = pointset_circle(points,*itg, directions->getAt(pid++),bounding);
        else lres = pointset_circle(points,*itg,bounding);
        *itrp = lres.first; *itrr = lres.second;
    }
    return std::pair<Point3ArrayPtr,RealArrayPtr>(respoints, resradius);

}


std::pair<Point3ArrayPtr,RealArrayPtr>
PGL::pointsets_section_circles( const Point3ArrayPtr points,
                                const IndexArrayPtr  adjacencies,
                                const Point3ArrayPtr directions,
                                real_t width,
                                bool bounding)
{
    size_t nbpoints = points->size();
    Point3ArrayPtr respoints(new Point3Array(nbpoints));
    RealArrayPtr   resradius(new RealArray(nbpoints,0));
    Point3Array::iterator itrp = respoints->begin();
    RealArray::iterator itrr = resradius->begin();
    size_t pid = 0;

    ProgressStatus st(nbpoints, "section and circles computed for %.2f%% of points.");

    for (Point3Array::const_iterator itd = directions->begin(); itd != directions->end(); ++itd, ++itrp, ++itrr, ++pid, ++st){
        Index section = point_section(pid, points, adjacencies, *itd, width);
        // if(section.size() == 0) printf("Pb with %i\n",pid);
        std::pair<Vector3,real_t> lres ;
        lres = pointset_circle(points,section, *itd,bounding);
        *itrp = lres.first; *itrr = lres.second;
    }
    return std::pair<Point3ArrayPtr,RealArrayPtr>(respoints, resradius);

}

std::pair<Point3ArrayPtr,RealArrayPtr>
PGL::adaptive_section_circles(const Point3ArrayPtr points, 
                             const IndexArrayPtr adjacencies, 
                             const Point3ArrayPtr orientations,
                             const RealArrayPtr widths,
                             const RealArrayPtr maxradii)
{
 
    size_t nbpoints = points->size();
    Point3ArrayPtr respoints(new Point3Array(nbpoints));
    RealArrayPtr   resradius(new RealArray(nbpoints,0));
    Point3Array::iterator itrp = respoints->begin();
    RealArray::iterator itrr = resradius->begin();
    size_t pid = 0;

    ProgressStatus st(nbpoints, "Adaptive section contraction computed for %.2f%% of points.");

    for (Point3Array::const_iterator itd = orientations->begin(); itd != orientations->end(); ++itd, ++itrp, ++itrr, ++pid, ++st){
        Index section = point_section(pid, points, adjacencies, *itd,  widths->getAt(pid), maxradii->getAt(pid) );
        std::pair<Vector3,real_t> lres = pointset_circle(points,section, *itd);
        *itrp = lres.first; *itrr = lres.second;
    }
    return std::pair<Point3ArrayPtr,RealArrayPtr>(respoints, resradius);
}


struct AdaptiveRadiiIterator {
public:
    AdaptiveRadiiIterator(const RealArrayPtr _densities,
                 real_t _minradius, real_t _maxradius,
                 QuantisedFunctionPtr _densityradiusmap):
        densities(_densities), 
        minradius(_minradius), 
        maxradius(_maxradius), 
        densityradiusmap(_densityradiusmap),
        mindensity(*_densities->getMin()),
        maxdensity(*_densities->getMax()),
        deltaradius(_maxradius - _minradius),
        itdensity(_densities->begin()),
        cvalue(0)
    {
        deltadensity = maxdensity-mindensity;
        compute_current_value();
    }

     inline AdaptiveRadiiIterator & operator ++()  { 
         if(itdensity != densities->end()){
             ++itdensity; 
             if(itdensity != densities->end())
                 compute_current_value(); 
         }
         return *this; }

     real_t operator*() const { return cvalue; }


protected:
    void compute_current_value() {
        real_t normedDensity = (*itdensity-mindensity)/deltadensity;
        cvalue = minradius  + deltaradius * (densityradiusmap ? densityradiusmap->getValue(normedDensity) : normedDensity ) ; 
    }

    RealArrayPtr densities;
    RealArray::const_iterator itdensity;
    real_t minradius; real_t maxradius;
    QuantisedFunctionPtr densityradiusmap;
    real_t mindensity;
    real_t maxdensity;
    real_t deltadensity;
    real_t deltaradius;

    real_t cvalue;
};

RealArrayPtr
PGL::adaptive_radii( const RealArrayPtr density,
                 real_t minradius, real_t maxradius,
                 QuantisedFunctionPtr densityradiusmap)
{
    AdaptiveRadiiIterator itC(density,minradius,maxradius,densityradiusmap);
    RealArrayPtr radii(new RealArray(density->size()));

    for(RealArray::iterator itradius = radii->begin(); itradius != radii->end(); ++itC, ++itradius)
    {
        *itradius = *itC ; 
    }
    return radii;

}

Point3ArrayPtr
PGL::adaptive_contration(const Point3ArrayPtr points, 
                     const Point3ArrayPtr orientations,
                     const IndexArrayPtr adjacencies, 
                     const RealArrayPtr density,
                     real_t minradius, real_t maxradius,
                     QuantisedFunctionPtr densityradiusmap,
                     const real_t alpha, 
	                 const real_t beta)
{
 
    RealArrayPtr radii = adaptive_radii(density, minradius, maxradius, densityradiusmap);
    IndexArrayPtr neighborhoods = r_anisotropic_neighborhoods(points,  adjacencies, radii, orientations, alpha, beta);    
    return centroids_of_groups(points, neighborhoods);
}

std::pair<Point3ArrayPtr,RealArrayPtr>
PGL::adaptive_section_contration(const Point3ArrayPtr points, 
                     const Point3ArrayPtr orientations,
                     const IndexArrayPtr adjacencies, 
                     const RealArrayPtr density,
                     real_t minradius, real_t maxradius,
                     QuantisedFunctionPtr densityradiusmap,
                     const real_t alpha, 
	                 const real_t beta)
{
 
    AdaptiveRadiiIterator itRadiusCcontractor(density,minradius,maxradius,densityradiusmap);

    size_t nbpoints = points->size();
    Point3ArrayPtr respoints(new Point3Array(nbpoints));
    RealArrayPtr   resradius(new RealArray(nbpoints,0));
    Point3Array::iterator itrp = respoints->begin();
    RealArray::iterator itrr = resradius->begin();
    size_t pid = 0;

    ProgressStatus st(nbpoints, "Adaptive section contraction computed for %.2f%% of points.");

    for (Point3Array::const_iterator itd = orientations->begin(); itd != orientations->end(); ++itd, ++itrp, ++itrr, ++pid, ++st, ++itRadiusCcontractor){
        Index section = r_anisotropic_neighborhood(pid, points,  adjacencies, *itRadiusCcontractor, *itd, alpha, beta);            
        std::pair<Vector3,real_t> lres = pointset_circle(points,section, *itd);
        *itrp = lres.first; *itrr = lres.second;
    }
    return std::pair<Point3ArrayPtr,RealArrayPtr>(respoints, resradius);
}

std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)>
PGL::points_dijkstra_shortest_path(const Point3ArrayPtr points, 
			         const IndexArrayPtr adjacencies, 
                     uint32_t root,
                     real_t powerdist)
{
    if (powerdist == 1) {
        struct PointDistance pdevaluator(points);
        return dijkstra_shortest_paths(adjacencies,root,pdevaluator);
    }
    else {
        struct PowerPointDistance pdevaluator(points,powerdist);
        return dijkstra_shortest_paths(adjacencies,root,pdevaluator);
    }
}

struct DistanceCmp {
        const RealArrayPtr distances;
        real_t operator()(uint32_t a, uint32_t b) const { return distances->getAt(a) < distances->getAt(b); }
        DistanceCmp(const RealArrayPtr _distances) : distances(_distances){}
};



Index 
PGL::get_sorted_element_order(const TOOLS(RealArrayPtr) distances)
{
    Index result(range<Index>(distances->size(),0,1));
    std::sort(result.begin(),result.end(),DistanceCmp(distances));
    return result;
}

#include <plantgl/tool/util_hashset.h>

IndexArrayPtr 
PGL::quotient_points_from_adjacency_graph(  const real_t binsize,
                                const Point3ArrayPtr points, 
			                    const IndexArrayPtr adjacencies, 
			                    const TOOLS(RealArrayPtr) distance_to_root)
{
    uint32_t nbpoints = points->size();
    Index sortedpoints = get_sorted_element_order(distance_to_root);
    uint32_t nextlimit = 0;
    uint32_t currentlimit = nextlimit;
    real_t currentbinlimit = 0;
    real_t nextbinlimit = binsize;
    IndexArrayPtr groups(new IndexArray());

    while(nextlimit < nbpoints) {
        while(nextlimit < nbpoints && distance_to_root->getAt(sortedpoints[nextlimit]) < nextbinlimit) {
            ++nextlimit;
        }
        if (nextlimit == currentlimit) {
            if (nextlimit < nbpoints && distance_to_root->getAt(sortedpoints[nextlimit]) == REAL_MAX) break;
            else {
                nextbinlimit += binsize;
                continue;
            }
        }
        // printf("in [%i,%i]  (%i)\n",currentlimit,nextlimit,nbpoints);

        pgl_hash_set_uint32 pointmap;
        uint32_t nbgroupincluster = 0;
        for (Index::const_iterator itpoint = sortedpoints.begin()+currentlimit; itpoint != sortedpoints.begin()+nextlimit; ++itpoint)
            if(pointmap.find(*itpoint) == pointmap.end()){
                pointmap.insert(*itpoint);
                Index newgroup;
                newgroup.push_back(*itpoint);
                std::stack<Index> sn;
                sn.push(adjacencies->getAt(*itpoint));
                while(!sn.empty()){
                    Index curneighborhood = sn.top();
                    sn.pop();
                    for (Index::const_iterator itpoint2 = curneighborhood.begin(); itpoint2 != curneighborhood.end(); ++itpoint2){
                        real_t pdist = distance_to_root->getAt(*itpoint2);
                        if( currentbinlimit <= pdist && pdist < nextbinlimit && pointmap.find(*itpoint2) == pointmap.end()){
                            pointmap.insert(*itpoint2);
                            newgroup.push_back(*itpoint2);
                            sn.push(adjacencies->getAt(*itpoint2));
                        }
                    }
                }
                assert(newgroup.size() > 0);
                groups->push_back(newgroup);
                ++nbgroupincluster;
            }
        assert(nbgroupincluster > 0);
        currentlimit = nextlimit;
        currentbinlimit = nextbinlimit;
        nextbinlimit += binsize;
        if (currentlimit < nbpoints && distance_to_root->getAt(sortedpoints[currentlimit]) == REAL_MAX) break;
    }
    return groups;
}

Index PGL::points_in_range_from_root(const real_t initialdist, const real_t binsize, const TOOLS(RealArrayPtr) distances_to_root)
{
    Index group;
    real_t maxdist = initialdist + binsize;
    uint32_t pid = 0;
    for(RealArray::const_iterator it = distances_to_root->begin(); it != distances_to_root->end(); ++it, ++pid)
        if (initialdist <= *it && *it < maxdist)
            group.push_back(pid);
    return group;
}


std::pair<IndexArrayPtr,RealArrayPtr> 
PGL::next_quotient_points_from_adjacency_graph(const real_t initiallevel,
                                               const real_t binsize,
                                               const Index& currents,
			                                   const IndexArrayPtr adjacencies, 
			                                   const TOOLS(RealArrayPtr) distances_to_root)
{
    IndexArrayPtr groups(new IndexArray());
    RealArrayPtr  binlevels(new RealArray());

    pgl_hash_set_uint32 pointmap;
    for(Index::const_iterator itc = currents.begin(); itc != currents.end(); ++itc)
        pointmap.insert(*itc);

    for(Index::const_iterator itc = currents.begin(); itc != currents.end(); ++itc){
        // real_t cdist = initiallevel; // distances_to_root->getAt(*itc);
        // real_t cmaxdist = cdist+binsize;

        // check if it is also on the next limit
        const Index& nbgc = adjacencies->getAt(*itc);
        for(Index::const_iterator itfirstorder = nbgc.begin(); itfirstorder != nbgc.end(); ++itfirstorder){

            real_t cdisttoroot = distances_to_root->getAt(*itfirstorder);
            if (cdisttoroot > initiallevel){

                if(pointmap.find(*itfirstorder) == pointmap.end()){
                    // Compute bin limits
                    real_t deltadist = cdisttoroot - initiallevel;
                    int clevel = int(deltadist / binsize);
                    // printf("clevel %i %f %f %f\n",clevel,cdisttoroot,initiallevel,binsize);

                    /*if (clevel > 0) {
                        printf("gap %i\n", clevel);
                        Index sonbg = adjacencies->getAt(*itfirstorder);
                        bool should_skip = false;
                        for (Index::const_iterator itsonbg = sonbg.begin(); itsonbg != sonbg.end(); ++itsonbg){
                            int llevel = int((distances_to_root->getAt(*itsonbg) - cdist)/binsize);
                            printf("check %i for %i: %i %s\n", *itsonbg, *itfirstorder, llevel,(0 <= llevel && llevel < clevel?"True":"False"));
                            if (0 <= llevel && llevel < clevel){
                                printf("continue\n");
                                should_skip = true;
                                break;
                                
                            }
                        }

                        if (should_skip) continue;
                    }*/

                    if (clevel < 0) printf("Error in computing level: %f %f %i\n",cdisttoroot, initiallevel, clevel);
                    assert (clevel >= 0);


                    real_t mindist = initiallevel + clevel * binsize;
                    if ( clevel > 0 && fabs(cdisttoroot-mindist) < GEOM_EPSILON) { 
                        --clevel; 
                        mindist = initiallevel + clevel * binsize;
                        // printf("correct clevel %i %f %f %f\n",clevel,cdisttoroot,initiallevel,binsize);
                    }

                    real_t maxdist = initiallevel + (clevel + 1) * binsize;

                    pointmap.insert(*itfirstorder);

                    Index newgroup;
                    newgroup.push_back(*itfirstorder);

                    std::stack<Index> tolookat;
                    tolookat.push(adjacencies->getAt(*itfirstorder));

                    // Check in neighbor the ones which are on the same group
                    while (!tolookat.empty()){
                        Index sonbg = tolookat.top();
                        tolookat.pop();
                        for(Index::const_iterator itsonbg = sonbg.begin(); itsonbg != sonbg.end(); ++itsonbg){

                            real_t pdist = distances_to_root->getAt(*itsonbg);
                            if (mindist < pdist && pdist <= maxdist){

                                if(pointmap.find(*itsonbg) == pointmap.end()){
                                    pointmap.insert(*itsonbg);
                                    newgroup.push_back(*itsonbg);

                                    tolookat.push(adjacencies->getAt(*itsonbg));
                                }
                            }
                        }
                    }

                    groups->push_back(newgroup);
                    binlevels->push_back(maxdist);
                }
            }
        }
    }
    return std::pair<IndexArrayPtr,RealArrayPtr> (groups, binlevels);
}

IndexArrayPtr 
PGL::quotient_adjacency_graph(const IndexArrayPtr adjacencies, 
			                    const IndexArrayPtr groups)
{
    uint32_t nbpoints = adjacencies->size();
    uint32_t  * group = new uint32_t[nbpoints];
    for (uint32_t p = 0 ; p < nbpoints; ++p) group[p] = UINT32_MAX; // Default group is no group.
    uint32_t cgroup = 0;
    for(IndexArray::const_iterator itgs = groups->begin(); itgs != groups->end(); ++itgs, ++cgroup)
    {
        for(Index::const_iterator itg = itgs->begin(); itg != itgs->end(); ++itg){
            group[*itg] = cgroup;
        }
    }
    IndexArrayPtr macroadjacencies(new IndexArray(groups->size(),Index()));
    uint32_t cnode = 0;
    for(IndexArray::const_iterator itn = adjacencies->begin(); itn != adjacencies->end(); ++itn, ++cnode)
    {
        uint32_t cgroup = group[cnode];
        if (cgroup == UINT32_MAX) continue; // No group assigned to this point.
        Index& cmadjacency = macroadjacencies->getAt(cgroup);
        for(Index::const_iterator itadgroup = itn->begin(); itadgroup != itn->end(); ++itadgroup){
            uint32_t adjacentgroup = group[*itadgroup];
            assert (adjacentgroup != UINT32_MAX);
            if (cgroup != adjacentgroup){
                if(std::find(cmadjacency.begin(),cmadjacency.end(),adjacentgroup) == cmadjacency.end()){
                    cmadjacency.push_back(adjacentgroup);
                }
            }
            
        }
    }
    delete [] group;
    return macroadjacencies;
}

ALGO_API TOOLS(Vector3) 
PGL::centroid_of_group(const Point3ArrayPtr points, 
			           const Index& group)
{
        Vector3 gcentroid; real_t nbpoints = 0;
        for(Index::const_iterator itn = group.begin(); itn != group.end(); ++itn,++nbpoints){
            gcentroid += points->getAt(*itn);
        }
        return gcentroid/nbpoints;
}

Point3ArrayPtr 
PGL::centroids_of_groups(const Point3ArrayPtr points, 
			             const IndexArrayPtr groups)
{
    Point3ArrayPtr result(new Point3Array(groups->size()));
    uint32_t cgroup = 0;
    for(IndexArray::const_iterator itgs = groups->begin(); itgs != groups->end(); ++itgs, ++cgroup)
    {
        result->setAt(cgroup,centroid_of_group(points,*itgs));
    }
    return result;
}

Point3ArrayPtr
PGL::skeleton_from_distance_to_root_clusters(const Point3ArrayPtr points, uint32_t root, real_t binsize, uint32_t k, 
                                             TOOLS(Uint32Array1Ptr)& group_parents, IndexArrayPtr& group_components, 
                                             bool connect_all_points, bool verbose)
{
    if(verbose)std::cout << "Compute Remanian graph." << std::endl;
#ifdef WITH_ANN
    IndexArrayPtr remaniangraph =  k_closest_points_from_ann(points, k, connect_all_points);
#else
    IndexArrayPtr remaniangraph =  k_closest_points_from_delaunay(points, k);
#endif
    if (connect_all_points){
        if(verbose)std::cout << "Connect all components of Riemanian graph." << std::endl;
        remaniangraph =  connect_all_connex_components(points, remaniangraph, verbose);
    }
    if(verbose)std::cout << "Compute distance to root." << std::endl;
    std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)> shortest_pathes = points_dijkstra_shortest_path(points, remaniangraph, root);
    Uint32Array1Ptr parents = shortest_pathes.first;
    RealArrayPtr distances_to_root = shortest_pathes.second;
    if(verbose)std::cout << "Compute cluster according to distance to root." << std::endl;
    group_components =  quotient_points_from_adjacency_graph(binsize, points, remaniangraph, distances_to_root);
    std::cout << "Nb of groups : " << group_components->size() << std::endl;
    if(verbose)std::cout << "Compute adjacency graph of groups." << std::endl;
    IndexArrayPtr group_adjacencies = quotient_adjacency_graph(remaniangraph, group_components);
    if(verbose)std::cout << "Compute centroid of groups." << std::endl;
    Point3ArrayPtr group_centroid =  centroids_of_groups(points,group_components);
    if(verbose)std::cout << "Compute spanning tree of groups." << std::endl;
    shortest_pathes = points_dijkstra_shortest_path(group_centroid, group_adjacencies, 0);
    group_parents = shortest_pathes.first;
    return group_centroid;
}

// Livny stuff

// compute parent-children relation from child-parent relation
IndexArrayPtr PGL::determine_children(const Uint32Array1Ptr parents, uint32_t& root)
{
    IndexArrayPtr result(new IndexArray(parents->size(),Index()));
    uint32_t pid = 0;
    for(Uint32Array1::const_iterator it = parents->begin(); it != parents->end(); ++it, ++pid){
         if (pid == *it) root = pid;
         else result->getAt(*it).push_back(pid);
    }
    return result;
}


RealArrayPtr PGL::carried_length(const Point3ArrayPtr points, const Uint32Array1Ptr parents)
{
    RealArrayPtr result(new RealArray(points->size()));
    uint32_t root;
    IndexArrayPtr children = determine_children(parents,root);


    IndexArrayPostOrderConstIterator piterator(children, root);

    // look to the node in post order and for each node compute its weight as sum of children weight + length of segment to children.
    for(;!piterator.atEnd(); ++piterator)
    {
        Index& nextgroup = children->getAt(*piterator);
        real_t sumw = 0;
        Vector3& refpoint = points->getAt(*piterator);
        for (Index::const_iterator itchildren = nextgroup.begin(); itchildren != nextgroup.end(); ++itchildren)
            sumw += result->getAt(*itchildren) + norm(refpoint-points->getAt(*itchildren));
        result->setAt(*piterator,sumw);
    }

    return result;
}

Uint32Array1Ptr PGL::subtrees_size(const TOOLS(Uint32Array1Ptr) parents)
{
    uint32_t root;
    IndexArrayPtr children = determine_children(parents,root);
    return subtrees_size(children,root);
}

Uint32Array1Ptr PGL::subtrees_size(const IndexArrayPtr children,  uint32_t root)
{
    
    Uint32Array1Ptr result(new Uint32Array1(children->size()));
    IndexArrayPostOrderConstIterator piterator(children, root);

    // look to the node in post order and for each node compute its weight as sum of children weight + 1.
    for(;!piterator.atEnd(); ++piterator)
    {
        Index& nextgroup = children->getAt(*piterator);
        uint32_t sumw = 1;
        for (Index::const_iterator itchildren = nextgroup.begin(); itchildren != nextgroup.end(); ++itchildren)
            sumw += result->getAt(*itchildren);
        result->setAt(*piterator,sumw);
    }

    return result;
}


Point3ArrayPtr PGL::optimize_orientations(const Point3ArrayPtr points, 
                                          const Uint32Array1Ptr parents, 
                                          const RealArrayPtr weights)
{
    uint32_t nbPoints = points->size();
    Point3ArrayPtr result( new Point3Array(nbPoints));
    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);

    // Compute first orientation as average direction to children weighted with 'weights'
    Vector3 firstorientation;
    real_t sumw = 0.0;
    for(Index::const_iterator itRChildren = children->getAt(root).begin(); 
        itRChildren != children->getAt(root).end(); ++itRChildren){
          real_t cv = weights->getAt(*itRChildren);
          firstorientation += cv * direction(points->getAt(*itRChildren)-points->getAt(root));
          sumw += cv;

    }
    result->setAt(root,firstorientation / sumw);

    // compute direction of all points as compromive between orientation of parent and direction(node-parent)
    IndexArrayPreOrderConstIterator piterator(children,root);
    for(++piterator; !piterator.atEnd(); ++piterator){
        uint32_t pid = *piterator;
        real_t cv = weights->getAt(pid);
        const Vector3& cpos = points->getAt(pid);
        uint32_t parent = parents->getAt(pid);
        real_t cvp_cv = (weights->getAt(parent) + cv) / 2;
        const Vector3& ppos = points->getAt(parent);
        Vector3 dp = direction(cpos-ppos);
        // assert (weights->getAt(parent) > cv);
        Vector3 ov = (cvp_cv* result->getAt(parent) + cv * dp) / (cvp_cv + cv);
        assert (ov.isValid());
        result->getAt(pid) = direction(ov);
    }
    return result;

}

Point3ArrayPtr PGL::optimize_positions(const Point3ArrayPtr points, 
                                           const Point3ArrayPtr orientations, 
                                           const TOOLS(Uint32Array1Ptr) parents, 
                                           const TOOLS(RealArrayPtr) weights)
{
    uint32_t nbPoints = points->size();
    Point3ArrayPtr result( new Point3Array(nbPoints));
    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);
    result->setAt(root,points->getAt(root));

    // compute position of all points as compromive between orientation and previous position
    IndexArrayPreOrderConstIterator piterator(children,root);
    for(++piterator; !piterator.atEnd(); ++piterator){
        uint32_t pid = *piterator;
        real_t cv = weights->getAt(pid);
        const Vector3& cpos = points->getAt(pid);

        uint32_t parent = parents->getAt(pid);
        real_t cvp_cv = (weights->getAt(parent) + cv)/2;
        const Vector3& ppos = points->getAt(parent);

        Vector3 relpos = cpos - ppos;
        real_t pdist = norm(relpos);

        const Vector3& corient = orientations->getAt(pid);
        const Vector3& porient = orientations->getAt(parent);

        // this position comes from the constraint of having edges coherent with orientation
        Vector3 relidealpos = result->getAt(parent) +  direction(corient+porient)*pdist;

        // this position comes from the constraint of having center of edges preserved
        Vector3 relt2pos = cpos+ppos-result->getAt(parent);

        // computes the new position from the compromise of the two previous positions
        Vector3 newpos = ( relt2pos* cv  + relidealpos * cvp_cv ) / (cv + cvp_cv);
        result->setAt(pid,newpos);
    }

    return result;
}


#include <plantgl/scenegraph/geometry/lineicmodel.h>

real_t 
PGL::average_radius(const Point3ArrayPtr points, 
                      const Point3ArrayPtr nodes,
                      const TOOLS(Uint32Array1Ptr) parents,
                      uint32_t maxclosestnode)
{
#ifdef WITH_ANN
    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);

    uint32_t nb_nodes = nodes->size();
    if (nb_nodes == 0) return REAL_MAX;
    if (maxclosestnode >= nb_nodes) maxclosestnode = nb_nodes;
    real_t sum_min_dist = 0;
    uint32_t nb_samples = 0;
    ANNKDTree3 tree(nodes);
    for (Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp)
    {
        real_t minpdist = REAL_MAX;
        Index nids = tree.k_closest_points(*itp,maxclosestnode);
        for(Index::const_iterator nit = nids.begin(); nit != nids.end(); ++nit){

            Vector3 point(*itp);
            real_t d = closestPointToSegment(point,nodes->getAt(*nit),nodes->getAt(parents->getAt(*nit)));
            if (d < minpdist) minpdist = d;

            for (Index::const_iterator nitc = children->getAt(*nit).begin(); nitc != children->getAt(*nit).end(); ++nitc){
                Vector3 mpoint(*itp);
                d = closestPointToSegment(mpoint,nodes->getAt(*nit),nodes->getAt(*nitc));
                if (d < minpdist) minpdist = d;
            }
        }
        if (minpdist != REAL_MAX) {
            sum_min_dist += minpdist;
            nb_samples += 1;
        }
    }
    return sum_min_dist / nb_samples;
#else
    #ifdef _MSC_VER
    #pragma message("function 'average_radius' disabled. ANN needed.")
    #else
    #warning "function 'average_radius' disabled. ANN needed"
    #endif

    return 0;
#endif
}

real_t PGL::average_distance_to_shape(const Point3ArrayPtr points, 
                                          const Point3ArrayPtr nodes,
                                          const TOOLS(Uint32Array1Ptr) parents,
                                          const TOOLS(RealArrayPtr) radii,
                                          uint32_t maxclosestnodes)
{
#ifdef WITH_ANN
    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);

    uint32_t nb_nodes = nodes->size();
    if (nb_nodes == 0) return REAL_MAX;
    if (maxclosestnodes >= nb_nodes) maxclosestnodes = nb_nodes;
    real_t sum_min_dist = 0;
    uint32_t nb_samples = 0;
    ANNKDTree3 tree(nodes);
    ProgressStatus st(points->size(),"distance to shape for %.2f%% of points.");

    for (Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp, ++st)
    {
        real_t minpdist = REAL_MAX;
        Index nids = tree.k_closest_points(*itp,maxclosestnodes);
        for(Index::const_iterator nit = nids.begin(); nit != nids.end(); ++nit){

            Vector3 point(*itp);
            real_t u;
            uint32_t parent = parents->getAt(*nit);
            real_t d = closestPointToSegment(point,nodes->getAt(*nit),nodes->getAt(parent),&u);
            real_t r = radii->getAt(parent) * u + radii->getAt(*nit) * (1 - u);
            d -= r;
            if (d < minpdist) minpdist = d;

            for (Index::const_iterator nitc = children->getAt(*nit).begin(); nitc != children->getAt(*nit).end(); ++nitc){
                Vector3 mpoint(*itp);
                d = closestPointToSegment(mpoint,nodes->getAt(*nit),nodes->getAt(*nitc),&u);
                real_t r = radii->getAt(*nitc) * u + radii->getAt(*nit) * (1 - u);
                d -= r;
                if (d < minpdist) minpdist = d;
            }
        }
        if (minpdist != REAL_MAX) {
            sum_min_dist += minpdist;
            nb_samples += 1;
        }
    }
    return sum_min_dist / nb_samples;
#else
    #ifdef _MSC_VER
    #pragma message("function 'average_distance_to_shape' disabled. ANN needed.")
    #else
    #warning "function 'average_distance_to_shape' disabled. ANN needed"
    #endif

    return 0;
#endif
}

TOOLS(RealArrayPtr) PGL::distance_to_shape(const Point3ArrayPtr points, 
                                          const Point3ArrayPtr nodes,
                                          const TOOLS(Uint32Array1Ptr) parents,
                                          const TOOLS(RealArrayPtr) radii,
                                          uint32_t maxclosestnodes)
{
#ifdef WITH_ANN
    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);

    uint32_t nb_nodes = nodes->size();
    if (nb_nodes == 0) return RealArrayPtr();
    if (maxclosestnodes >= nb_nodes) maxclosestnodes = nb_nodes;
    uint32_t nbPoints = points->size();
    RealArrayPtr result(new RealArray(nbPoints));
    uint32_t pid = 0;
    ANNKDTree3 tree(nodes);
    ProgressStatus st(nbPoints,"distance to shape for %.2f%% of points.");

    for (Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp, ++pid, ++st)
    {
        real_t minpdist = REAL_MAX;
        Index nids = tree.k_closest_points(*itp,maxclosestnodes);
        for(Index::const_iterator nit = nids.begin(); nit != nids.end(); ++nit){

            Vector3 point(*itp);
            real_t u;
            uint32_t parent = parents->getAt(*nit);
            real_t d = closestPointToSegment(point,nodes->getAt(*nit),nodes->getAt(parent),&u);
            real_t r = radii->getAt(parent) * u + radii->getAt(*nit) * (1 - u);
            d = fabs(d-r);
            if (d < minpdist) minpdist = d;

            for (Index::const_iterator nitc = children->getAt(*nit).begin(); nitc != children->getAt(*nit).end(); ++nitc){
                Vector3 mpoint(*itp);
                d = closestPointToSegment(mpoint,nodes->getAt(*nit),nodes->getAt(*nitc),&u);
                real_t r = radii->getAt(*nitc) * u + radii->getAt(*nit) * (1 - u);
                d = fabs(d-r);
                if (d < minpdist) minpdist = d;
            }
        }
        result->getAt(pid) = minpdist;
    }
    return result;
#else
    #ifdef _MSC_VER
    #pragma message("function 'distance_to_shape' disabled. ANN needed.")
    #else
    #warning "function 'distance_to_shape' disabled. ANN needed"
    #endif

    return 0;
#endif
}


inline bool distance_test ( real_t d, real_t distance, bool reversed){
    if (reversed) return d > distance;
    else return d < distance;
}

ALGO_API Index PGL::points_at_distance_from_skeleton(const Point3ArrayPtr points, 
                                                const Point3ArrayPtr nodes,
                                                const TOOLS(Uint32Array1Ptr) parents,
                                                real_t distance,
                                                uint32_t maxclosestnodes)
{
#ifdef WITH_ANN
    Index result;
    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);

    uint32_t nb_nodes = nodes->size();
    if (nb_nodes == 0) return range<Index>(0,points->size(),1);

    if (maxclosestnodes >= nb_nodes) maxclosestnodes = nb_nodes;

    bool reversed = false;
    if (distance < 0) {
        distance = - distance;
        reversed = true;
    }

    
    ANNKDTree3 tree(nodes);

    size_t pid = 0;
    for (Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp, ++pid)
    {
        real_t minpdist = REAL_MAX;
        Index nids = tree.k_closest_points(*itp,maxclosestnodes);
        bool ok = false;
        for(Index::const_iterator nit = nids.begin(); !ok && nit != nids.end(); ++nit){

            Vector3 point(*itp);
            real_t d = closestPointToSegment(point,nodes->getAt(*nit),nodes->getAt(parents->getAt(*nit)));
            if (distance_test(d, distance,reversed)) { ok = true; result.push_back(pid); continue;}

            for (Index::const_iterator nitc = children->getAt(*nit).begin(); nitc != children->getAt(*nit).end(); ++nitc){
                Vector3 mpoint(*itp);
                d = closestPointToSegment(mpoint,nodes->getAt(*nit),nodes->getAt(*nitc));
                if (distance_test(d, distance,reversed)) { ok = true; result.push_back(pid); continue;}
            }
            if (ok) continue;
        }
    }
    return result;
#else
    #ifdef _MSC_VER
    #pragma message("function 'points_at_distance_from_skeleton' disabled. ANN needed.")
    #else
    #warning "function 'points_at_distance_from_skeleton' disabled. ANN needed"
    #endif

    return Index();
#endif

}

TOOLS(RealArrayPtr) 
PGL::estimate_radii(const Point3ArrayPtr nodes,
                    const TOOLS(Uint32Array1Ptr) parents, 
                    const TOOLS(RealArrayPtr) weights,
                    real_t averageradius,
                    real_t pipeexponent)
{
    uint32_t nbNodes = nodes->size();
    RealArrayPtr result( new RealArray(nbNodes,0));

    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);

    real_t cr = weights->getAt(root);
    real_t totalcu = cr;
    real_t totalcu_cr = cr * pow(cr, pipeexponent);

    // estimate radius of each nodes
    IndexArrayPreOrderConstIterator piterator(children,root);
    for(++piterator; !piterator.atEnd(); ++piterator){
        real_t cu = weights->getAt(*piterator);
        totalcu += cu;
        totalcu_cr += cu * pow(cu,pipeexponent);
    }

    real_t radiusroot =  averageradius * pow(cr, pipeexponent) * totalcu / totalcu_cr;
    // real_t radiusroot = pow(totalcu * averageradius / totalcu_cr,1.0/pipeexponent);

    uint32_t nid = 0;
    for(RealArray::iterator itres = result->begin(); itres != result->end(); ++itres, ++nid)
        *itres = radiusroot * pow(weights->getAt(nid) / cr, pipeexponent);

    return result;
}


Vector3 PGL::min_max_mean_edge_length(const Point3ArrayPtr points, const TOOLS(Uint32Array1Ptr) parents)
{
    real_t minl  = REAL_MAX;
    real_t maxl  = 0;
    real_t suml  = 0;
    uint32_t nbp = 0;
    uint32_t pid = 0;
    for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it, ++pid){
        uint32_t parent = parents->getAt(pid);
        if (parent != pid){
            real_t edgelength = norm(*it-points->getAt(parent));
            if (edgelength < minl) minl = edgelength;
            if (edgelength > maxl) maxl = edgelength;
            suml += edgelength;
            nbp += 1;
        }
    }
    return Vector3(minl, maxl,suml/nbp);
}

Vector3 PGL::min_max_mean_edge_length(const Point3ArrayPtr points, const IndexArrayPtr graph)
{
    real_t minl  = REAL_MAX;
    real_t maxl  = 0;
    real_t suml  = 0;
    uint32_t nbp = 0;
    uint32_t pid = 0;
    IndexArray::const_iterator itg = graph->begin();
    for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it, ++itg, ++pid){
        for (Index::const_iterator itnbg = itg->begin(); itnbg != itg->end(); ++itnbg){
            if (*itnbg != pid){
                real_t edgelength = norm(*it-points->getAt(*itnbg));
                if (edgelength < minl) minl = edgelength;
                if (edgelength > maxl) maxl = edgelength;
                suml += edgelength;
                nbp += 1;
            }
        }
    }
    return Vector3(minl, maxl,suml/nbp);
}

Index PGL::detect_short_nodes(const Point3ArrayPtr nodes,
                               const TOOLS(Uint32Array1Ptr) parents, 
                               real_t edgelengthfilter)
{
    Index toremove;
    uint32_t pid = 0;
    for (Point3Array::const_iterator it = nodes->begin(); it != nodes->end(); ++it, ++pid){
        uint32_t parent = parents->getAt(pid);
        if (parent != pid){
            real_t edgelength = norm(*it-nodes->getAt(parent));
            if (edgelength < edgelengthfilter) toremove.push_back(pid);
        }
    }
    return toremove;

 }

#include <plantgl/algo/base/intersection.h>
#include <plantgl/algo/base/curvemanipulation.h>
#include <plantgl/algo/base/tesselator.h>
#include <plantgl/algo/base/surfcomputer.h>
#include <plantgl/pgl_scenegraph.h>


std::pair<Point2ArrayPtr, IndexArrayPtr> my_polygon2ds_intersection(Point2ArrayPtr P, Point2ArrayPtr Q)
{
  IndexArrayPtr indexresult(new IndexArray());
  Point2ArrayPtr pointresult(new Point2Array());

  // Compute the intersection of P and Q.
  GeometryPtr result = Overlay::process(Polyline2DPtr(new Polyline2D(P)),Polyline2DPtr(new Polyline2D(Q)));
  GroupPtr group;
  if (!is_null_ptr(group = dynamic_pointer_cast<Group>(result))){
      for(GeometryArray::const_iterator  itph = group->getGeometryList()->begin(); itph != group->getGeometryList()->end(); ++itph){
          Polyline2DPtr resi = dynamic_pointer_cast<Polyline2D>(*itph);
          if (resi){
              Point2ArrayPtr lpointresult(resi->getPointList());
              lpointresult->erase(lpointresult->end()-1);
              IndexArrayPtr lindexresult = polygonization(lpointresult, eConvexTriangulation);
              size_t nbpoints = pointresult->size();
              if (nbpoints > 0) shift_all_indices(lindexresult, nbpoints);
              indexresult->insert(indexresult->end(),lindexresult->begin(),lindexresult->end());
              pointresult->insert(pointresult->end(),lpointresult->begin(),lpointresult->end());
          }
      }
      return std::pair<Point2ArrayPtr, IndexArrayPtr>(pointresult,indexresult);
  }
  else {
       Polyline2DPtr resi = dynamic_pointer_cast<Polyline2D>(result);
       if (resi){
            Point2ArrayPtr lpointresult(resi->getPointList());
            lpointresult->erase(lpointresult->end()-1);
            IndexArrayPtr lindexresult = polygonization(lpointresult, eConvexTriangulation);
            return std::pair<Point2ArrayPtr, IndexArrayPtr>(lpointresult,lindexresult);
        }
       else { return std::pair<Point2ArrayPtr, IndexArrayPtr>(); }
  }
}
/*
std::pair<Point2ArrayPtr, IndexArrayPtr> my_polygon2ds_intersection(Point2ArrayPtr P, Point2ArrayPtr Q)
{
    return polygon2ds_intersection(P, Q);
} */

bool PGL::node_continuity_test(const Vector3& node, real_t noderadius, 
                               const Vector3& parent, real_t parentradius,
                               const Vector3& child, real_t childradius,
                               real_t overlapfilter,
                               bool verbose, ScenePtr * visu)
{

    Vector3 np1 = node - parent; real_t l1 = np1.normalize();
    Vector3 np2 = child - node; real_t l2 = np2.normalize();
    Vector3 medium = (np1 + np2) / 2;

    Vector3 normalvec = cross(np1, np2);
    if (norm(normalvec) < GEOM_EPSILON) {
        real_t estimated_rad = parentradius + (childradius-parentradius)*l1/(l1+l2);
        if (((estimated_rad+(parentradius-estimated_rad)*overlapfilter) >= noderadius) &&
            (  noderadius >= (estimated_rad-(estimated_rad-childradius)*overlapfilter)  ))
            return true;
        else return false;
    }
    else {
        normalvec.normalize();
        Vector3 horizontalvec = cross(medium, normalvec);
        Vector3 horizontalvec1 = cross(np1, normalvec);
        horizontalvec1.normalize();
        Vector3 horizontalvec2 = cross(np2, normalvec);
        horizontalvec2.normalize();

        Vector2 hvec12D(dot(horizontalvec,horizontalvec1),dot(medium,horizontalvec1));
        hvec12D.normalize();
        Vector2 hvec22D(dot(horizontalvec,horizontalvec2),dot(medium,horizontalvec2));
        hvec22D.normalize();

        Vector2 pvec12D(dot(horizontalvec,np1),dot(medium,np1));
        pvec12D.normalize();
        Vector2 pvec22D(dot(horizontalvec,np2),dot(medium,np2));
        pvec22D.normalize();

        Point2ArrayPtr cone1(new Point2Array(childradius > GEOM_EPSILON? 4 : 3));
        uint32_t ipos = 0;
        cone1->setAt(ipos++,parentradius * hvec12D);
        if (childradius > GEOM_EPSILON){
            cone1->setAt(ipos++, 2 * l1 * pvec12D + childradius * hvec12D);
            cone1->setAt(ipos++, 2 * l1 * pvec12D - childradius * hvec12D);
        }
        else cone1->setAt(ipos++, 2 * l1 * pvec12D);
        cone1->setAt(ipos++, - parentradius * hvec12D);

        Point2ArrayPtr cone2(new Point2Array(childradius > GEOM_EPSILON? 4 : 3));
        ipos = 0;
        cone2->setAt(ipos++, l1 * pvec12D + noderadius * hvec22D);
        if (childradius > GEOM_EPSILON){
            cone2->setAt(ipos++, l1 * pvec12D + l2 * pvec22D + childradius * hvec22D);
            cone2->setAt(ipos++, l1 * pvec12D + l2 * pvec22D - childradius * hvec22D);
        }
        else cone2->setAt(ipos++, l1 * pvec12D + l2 * pvec22D);
        cone2->setAt(ipos++, l1 * pvec12D - noderadius * hvec22D);

        std::pair<Point2ArrayPtr, IndexArrayPtr> intersection = my_polygon2ds_intersection(cone1,cone2);

        real_t intersection_surface = 0;
        Index3ArrayPtr triangles = intersection.second->triangulate();
        for (Index3Array::const_iterator it = triangles->begin(); it != triangles->end(); ++it){
            intersection_surface += surface(intersection.first->getAt(it->getAt(0)),intersection.first->getAt(it->getAt(1)),intersection.first->getAt(it->getAt(2)));
        }

        real_t surface1 = l1 * (parentradius + childradius) * 2 ;
        real_t surface2 = l2 * (noderadius + childradius)  ;
    
        if(verbose) {
            printf("%f, %f, %f\n", intersection_surface, surface1, surface2);
            printf("%f, %f, %f\n", noderadius, childradius, l2);
            printf("esurface1 = %f vs %f\n", surface(cone1->getAt(0),cone1->getAt(1),cone1->getAt(2))+surface(cone1->getAt(0),cone1->getAt(2),cone1->getAt(3)), surface1);
            printf("esurface2 = %f vs %f\n", surface(cone2->getAt(0),cone2->getAt(1),cone2->getAt(2))+surface(cone2->getAt(0),cone2->getAt(2),cone2->getAt(3)), surface2);
            if(visu){
                *visu = ScenePtr(new Scene());
                (*visu)->add(ShapePtr(new Shape(GeometryPtr(new Polyline2D(cone1,3)),AppearancePtr(new Material(Color3(0,255,0))))));
                (*visu)->add(ShapePtr(new Shape(GeometryPtr(new Polyline2D(cone2,3)),AppearancePtr(new Material(Color3(0,0,255))))));
                (*visu)->add(ShapePtr(new Shape(GeometryPtr(new FaceSet(Point3ArrayPtr(new Point3Array(intersection.first,0)),intersection.second)),AppearancePtr(new Material(Color3(255,0,0))))));
           }
        }

        return ( (intersection_surface / surface1 > overlapfilter) || (intersection_surface / surface2 > overlapfilter));
    }
}

bool PGL::node_intersection_test(const Vector3& root, real_t rootradius, 
                       const Vector3& p1, real_t radius1, 
                       const Vector3& p2, real_t radius2,
                       real_t overlapfilter,
                       bool verbose, ScenePtr * visu)
{
    if (verbose) printf("node_intersection_test\n");
    Point2ArrayPtr cone1(new Point2Array(radius1 > GEOM_EPSILON? 4 : 3));
    Point2ArrayPtr cone2(new Point2Array(radius2 > GEOM_EPSILON? 4 : 3));

    Vector3 np1 = p1 - root; real_t l1 = np1.normalize();
    Vector3 np2 = p2 - root; real_t l2 = np2.normalize();
    Vector3 medium = (np1 + np2) / 2;

    Vector3 normalvec = cross(np1, np2);
    if (norm(normalvec) < GEOM_EPSILON) {

        uint32_t ipos = 0;
        cone1->setAt(ipos++,Vector2(rootradius,0));
        if (radius1 > GEOM_EPSILON){
            cone1->setAt(ipos++, Vector2(radius1,l1));
            cone1->setAt(ipos++, Vector2(-radius1,l1));
        }
        else cone1->setAt(ipos++, Vector2(0,l1));
        cone1->setAt(ipos++,Vector2(-rootradius,0));

        ipos = 0;
        cone2->setAt(ipos++,Vector2(rootradius,0));
        if (radius2 > GEOM_EPSILON){
            cone2->setAt(ipos++, Vector2(radius2,l2));
            cone2->setAt(ipos++, Vector2(-radius2,l2));
        }
        else cone2->setAt(ipos++, Vector2(0,l2));
        cone2->setAt(ipos++,Vector2(-rootradius,0));
    }
    else {
        normalvec.normalize();
        Vector3 horizontalvec = cross(medium, normalvec);
        horizontalvec.normalize();
        Vector3 horizontalvec1 = cross(np1, normalvec);
        horizontalvec1.normalize();
        Vector3 horizontalvec2 = cross(np2, normalvec);
        horizontalvec2.normalize();

        Vector2 hvec12D(dot(horizontalvec,horizontalvec1),dot(medium,horizontalvec1));
        hvec12D.normalize();
        Vector2 hvec22D(dot(horizontalvec,horizontalvec2),dot(medium,horizontalvec2));
        hvec22D.normalize();

        Vector2 pvec12D(dot(horizontalvec,np1),dot(medium,np1));
        pvec12D.normalize();
        Vector2 pvec22D(dot(horizontalvec,np2),dot(medium,np2));
        pvec22D.normalize();

        uint32_t ipos = 0;
        cone1->setAt(ipos++,rootradius * hvec12D);
        if (radius1 > GEOM_EPSILON){
            cone1->setAt(ipos++, l1 * pvec12D + radius1 * hvec12D);
            cone1->setAt(ipos++, l1 * pvec12D - radius1 * hvec12D);
        }
        else cone1->setAt(ipos++, l1 * pvec12D);
        cone1->setAt(ipos++, - rootradius * hvec12D);

        ipos = 0;
        cone2->setAt(ipos++, rootradius * hvec22D);
        if (radius2 > GEOM_EPSILON){
            cone2->setAt(ipos++, l2 * pvec22D + radius2 * hvec22D);
            cone2->setAt(ipos++, l2 * pvec22D - radius2 * hvec22D);
        }
        else cone2->setAt(ipos++, l2 * pvec22D);
        cone2->setAt(ipos++, - rootradius * hvec22D);
    }

    if(verbose) {
        printf("a = [");
        for(Point2Array::const_iterator it = cone1->begin(); it != cone1->end(); ++it)
            printf("(%f,%f),", it->x(),it->y());
        printf("]\n");

        printf("b = [");
        for(Point2Array::const_iterator it = cone2->begin(); it != cone2->end(); ++it)
            printf("(%f,%f),", it->x(),it->y());
        printf("]\n");

        if (visu){
            *visu = ScenePtr(new Scene());
            (*visu)->add(ShapePtr(new Shape(GeometryPtr(new Polyline2D(cone1,3)),AppearancePtr(new Material(Color3(0,255,0))))));
            (*visu)->add(ShapePtr(new Shape(GeometryPtr(new Polyline2D(cone2,3)),AppearancePtr(new Material(Color3(0,0,255))))));
        }
    }

    std::pair<Point2ArrayPtr, IndexArrayPtr> intersection = my_polygon2ds_intersection(cone1,cone2);

    if(verbose) {
        printf("res = [");
        for(Point2Array::const_iterator it = intersection.first->begin(); it != intersection.first->end(); ++it)
            printf("(%f,%f),", it->x(),it->y());
        printf("]\n"); 
        if (visu)
            (*visu)->add(ShapePtr(new Shape(GeometryPtr(new FaceSet(Point3ArrayPtr(new Point3Array(intersection.first,0)),intersection.second)),AppearancePtr(new Material(Color3(255,0,0))))));
    }

    real_t intersection_surface = 0;
    Index3ArrayPtr triangles = intersection.second->triangulate();
    for (Index3Array::const_iterator it = triangles->begin(); it != triangles->end(); ++it){
        intersection_surface += surface(intersection.first->getAt(it->getAt(0)),intersection.first->getAt(it->getAt(1)),intersection.first->getAt(it->getAt(2)));
    }

    real_t surface1 = l1 * (rootradius + radius1)  ;
    real_t surface2 = l2 * (rootradius + radius2)  ;
    
    if(verbose) printf("%f, %f, %f\n", intersection_surface, surface1, surface2);

    return ( (intersection_surface / surface1 > overlapfilter) || (intersection_surface / surface2 > overlapfilter));

    // We check if mid central point of cylinder 2 is in cylinder 1
 /*   Vector3 midp1 = (p1-root) * overlapfilter;
    Vector3 rp2 = p2-root;
    real_t normp2 = norm(rp2);
    Vector3 drp2 = rp2/normp2;
    real_t corespu = dot(midp1,drp2);
    real_t corespt = corespu / normp2;
    Vector3 crp2 = drp2 * corespu;
    real_t radatcrp2 = rootradius + (radius2 -  rootradius) * corespt;
    return norm(midp1-crp2) < radatcrp2; */

}




IndexArrayPtr
PGL::detect_similar_nodes(const Point3ArrayPtr nodes,
                            const TOOLS(Uint32Array1Ptr) parents, 
                            const TOOLS(RealArrayPtr) radii, 
                            const TOOLS(RealArrayPtr) weights, 
                            real_t overlapfilter)
{
    typedef std::pair<uint32_t,uint32_t> Pair;
    IndexArrayPtr tomerge(new IndexArray());

    uint32_t root;
    IndexArrayPtr children = determine_children(parents, root);

    IndexArrayPreOrderConstIterator piterator(children,root);
    uint32_t nbNodes = nodes->size();
    std::set<uint32_t> toavoidtomergebycontinuity;
    ProgressStatus st(nbNodes, "filtering computed for %.2f%% of nodes.");

    for(; !piterator.atEnd(); ++piterator, ++st){

        const Index& pchildren = children->getAt(*piterator);
        const Vector3& rpos = nodes->getAt(*piterator);

        switch (pchildren.size()){
        case 0:
            break;
        case 1:
            if (*piterator != root && toavoidtomergebycontinuity.find(*piterator) == toavoidtomergebycontinuity.end()){
                if(node_continuity_test(rpos, radii->getAt(*piterator), 
                                        nodes->getAt(parents->getAt(*piterator)),radii->getAt(parents->getAt(*piterator)),
                                        nodes->getAt(pchildren[0]), radii->getAt(pchildren[0]),
                                        overlapfilter)) { 
                       size_t nbmerges = tomerge->size();
                       bool newmerge = true;
                       if (nbmerges != 0) {
                           Index& lastmerge = tomerge->getAt(nbmerges-1);
                           size_t nbnodes = lastmerge.size();
                           if (lastmerge[nbnodes-1] == *piterator) {
                               lastmerge.push_back(pchildren[0]);
                               newmerge = false;
                           }
                       }
                       if(newmerge){
                            Index m; m.push_back(*piterator); m.push_back(pchildren[0]);
                            tomerge->push_back(m);
                       }
                }
            }
            break;
        default:
            {
             Index fpchildren(pchildren);
             std::sort(fpchildren.begin(),fpchildren.end(),DistanceCmp(weights));

              std::set<uint32_t> processed;
              for (Index::const_reverse_iterator itch = fpchildren.rbegin(); itch != fpchildren.rend();  ++itch)
              {
                  if (processed.find(*itch) == processed.end()){

                      Index m; m.push_back(*itch); 
                      for (Index::const_reverse_iterator itch2 = itch+1; itch2 != fpchildren.rend();  ++itch2)
                      {
                              if (processed.find(*itch2) == processed.end() &&
                                  node_intersection_test(rpos, radii->getAt(*piterator),
                                  nodes->getAt(*itch), radii->getAt(*itch),
                                  nodes->getAt(*itch2), radii->getAt(*itch2),
                                  overlapfilter))
                              {
                                  m.push_back(*itch2);
                                  processed.insert(*itch2);
                              }
                      }
                      if (m.size() > 1) { 
                          tomerge->push_back(m);
                          for (Index::const_iterator itch3 = m.begin(); itch3 != m.end(); ++itch3)
                              if(children->getAt(*itch3).size() == 1) toavoidtomergebycontinuity.insert(*itch3);
                      }
                  }
              }
            }
            break;
        }
    }
    return tomerge;
}

void PGL::remove_nodes(const Index& toremove,
                       Point3ArrayPtr& nodes,
                       TOOLS(Uint32Array1Ptr)& parents, 
                       TOOLS(RealArrayPtr)& radii)
{

    Index sortedtoremove(toremove);
    std::sort(sortedtoremove.begin(),sortedtoremove.end());
    uint32_t nbNodes = nodes->size();
    uint32_t *  idmap = new uint32_t[nbNodes];
    uint32_t noid = UINT32_MAX;

    uint32_t pid = 0;
    uint32_t * itid = idmap;
    Index::const_iterator itdel = sortedtoremove.begin();

    for (uint32_t cid = 0;  cid < nbNodes; ++cid, ++itid){
        if (itdel != sortedtoremove.end() && cid == *itdel) { 
                *itid = noid; 
                while(itdel != sortedtoremove.end() && *itdel <= cid) ++itdel; 
        }
        else { 
            *itid = pid++; 
        }
    }
    uint32_t nbNewNodes = pid;

    Point3ArrayPtr newnodes(new Point3Array(*nodes));

    bool make_radii = is_valid_ptr(radii);
    RealArrayPtr newradii;
    if (make_radii)
        newradii = RealArrayPtr(new RealArray(*radii));

    uint32_t lastdelid = UINT32_MAX;
    for(Index::const_reverse_iterator itdel2 = sortedtoremove.rbegin(); itdel2 != sortedtoremove.rend(); ++itdel2){
        if (*itdel2 != lastdelid){
            newnodes->erase(newnodes->begin()+*itdel2);
            if (make_radii) newradii->erase(newradii->begin()+*itdel2);
            lastdelid = *itdel2;
        }
    }

    Uint32Array1Ptr newparents(new Uint32Array1(nbNewNodes));
    pid = 0;
    for(Uint32Array1::iterator itParent = newparents->begin(); itParent != newparents->end(); ++itParent)
    {
            while (idmap[pid] == noid) ++pid;
            uint32_t parent = parents->getAt(pid);
            while(idmap[parent] == noid) parent = parents->getAt(parent);
            *itParent = idmap[parent];
            ++pid;
    }

    delete [] idmap;

    nodes = newnodes;
    if (make_radii)
        radii = newradii;
    parents = newparents;

}

void PGL::merge_nodes(const IndexArrayPtr tomerge,
                       Point3ArrayPtr& nodes,
                       TOOLS(Uint32Array1Ptr)& parents, 
                       TOOLS(RealArrayPtr)& radii, 
                       TOOLS(RealArrayPtr) weight)
{
    uint32_t nbNodes = nodes->size();    
    size_t nbfinalnodes = nbNodes;
    
    typedef pgl_hash_map<uint32_t,uint32_t> MergeMapType;
    MergeMapType mergemap;

    for(IndexArray::const_iterator it = tomerge->begin(); it != tomerge->end(); ++it){
        nbfinalnodes -= it->size()-1;

        uint32_t fid = *it->begin(); // std::min_element(it->begin(),it->end());
        for(Index::const_iterator itidx = it->begin(); itidx != it->end(); ++itidx)
            mergemap[*itidx] = fid;
    }
    
    uint32_t *  idmap = new uint32_t[nbNodes];

    uint32_t pid = 0;
    uint32_t * itid = idmap;

    for (uint32_t cid = 0;  cid < nbNodes; ++cid, ++itid){
        MergeMapType::const_iterator itm = mergemap.find(cid);
        if (itm != mergemap.end() && itm->second != cid) { 
                *itid = idmap[itm->second]; 
        }
        else { 
            *itid = pid++; 
        }
    }

    Point3ArrayPtr newnodes(new Point3Array(nbfinalnodes));
    RealArrayPtr newradii(new RealArray(nbfinalnodes));

    for (uint32_t cid = 0;  cid < nbNodes; ++cid){
        if (mergemap.find(cid) == mergemap.end()){
            newnodes->getAt(idmap[cid]) = nodes->getAt(cid);
            newradii->getAt(idmap[cid]) = radii->getAt(cid);
        }
    }

    for(IndexArray::const_iterator it = tomerge->begin(); it != tomerge->end(); ++it){
        Vector3 mp(0,0,0);
        real_t mradius = 0;
        real_t sweights = 0;
        for(Index::const_iterator itidx = it->begin(); itidx != it->end(); ++itidx){
            real_t w = weight->getAt(*itidx);
            mp += nodes->getAt(*itidx) * w;
            mradius += radii->getAt(*itidx) * w;
            sweights += w;
        }
        size_t pid = idmap[*it->begin()];
        /*if (pid == 3607) {
            for(Index::const_iterator itidx = it->begin(); itidx != it->end(); ++itidx)
                printf("%i - %f %f %f - %f\n",*itidx,nodes->getAt(*itidx).x(),nodes->getAt(*itidx).y(), nodes->getAt(*itidx).z(), weight->getAt(*itidx));
            printf("M %i - %f %f %f - %f\n",pid,mp.x(),mp.y(), mp.z(), sweights);
        }*/
        if (sweights < GEOM_EPSILON) {
            sweights = 0;   
            for(Index::const_iterator itidx = it->begin(); itidx != it->end(); ++itidx){
                mp += nodes->getAt(*itidx);
                mradius += radii->getAt(*itidx);
                sweights += 1;
            }
        }
        newnodes->getAt(pid) = mp / sweights;
        newradii->getAt(pid) = mradius / sweights;
    }



    Uint32Array1Ptr newparents(new Uint32Array1(nbfinalnodes));
    pid = 0;
    for(Uint32Array1::iterator itParent = parents->begin(); itParent != parents->end(); ++itParent, ++pid)
    {
        // if (idmap[pid] == 1148) printf("pid = %i\n", pid);
        MergeMapType::const_iterator itm = mergemap.find(pid);
        if(itm == mergemap.end() || itm->second == pid)
        {
            // if (idmap[pid] == 1148) printf("merged (%i)\n", itm->second);
            uint32_t oparent = *itParent;
            // if (idmap[pid] == 1148) printf("parent = %i\n", oparent);
            MergeMapType::const_iterator itpm = mergemap.find(oparent);
            if (itpm != mergemap.end()) {
                oparent = itpm->second;
                // if (idmap[pid] == 1148) printf("merged parent = %i\n", oparent);
            }

            newparents->getAt(idmap[pid]) = idmap[oparent];
            // if (idmap[pid] == 1148) printf("new parent = %i\n", idmap[oparent]);
        }
        else {
            // if (idmap[pid] == 1148) printf("parent = %i\n", *itParent);
        }
    }

    delete [] idmap;

    nodes = newnodes;
    radii = newradii;
    parents = newparents;

}

Vector3 
PGL::pointset_mean_direction(const Vector3& origin, const Point3ArrayPtr points, const Index& group)
{
    Vector3 result(0,0,0);
    size_t nbpoints = group.size();
    if(nbpoints != 0) {
        for(Index::const_iterator itg = group.begin(); itg != group.end(); ++itg)
            result += direction(points->getAt(*itg)-origin);
        result /= nbpoints;
    }
    else {
        nbpoints = points->size();
        for(Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp)
            result += direction(*itp-origin);
        result /= nbpoints;
    }
    return result.normed();
}

// determine all directions of a set of points
Point3ArrayPtr 
PGL::pointset_directions(const TOOLS(Vector3)& origin, const Point3ArrayPtr points, const Index& group)
{
    Point3ArrayPtr result;
    size_t nbpoints = group.size();
    if(nbpoints != 0) {
        result = Point3ArrayPtr(new Point3Array(nbpoints));
        Point3Array::iterator it = result->begin();
        for(Index::const_iterator itg = group.begin(); itg != group.end(); ++itg)
            *it = direction(points->getAt(*itg)-origin);
    }
    else {
        result = Point3ArrayPtr(new Point3Array(points->size()));
        Point3Array::iterator it = result->begin();
        for(Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp)
            *it = direction(*itp-origin);
    }
    return result;
}

Point2ArrayPtr 
PGL::pointset_angulardirections(const Point3ArrayPtr points, const TOOLS(Vector3)& origin, const Index& group)
{
    Point2ArrayPtr result;
    size_t nbpoints = group.size();
    if(nbpoints != 0) {
        result = Point2ArrayPtr(new Point2Array(nbpoints));
        Point2Array::iterator it = result->begin();
        for(Index::const_iterator itg = group.begin(); itg != group.end(); ++itg, ++it){
            Vector3::Spherical s(direction(points->getAt(*itg)-origin));
            *it = Vector2(s.theta,s.phi);
        }
    }
    else {
        size_t nbpoints = points->size();
        result = Point2ArrayPtr(new Point2Array(nbpoints));
        Point2Array::iterator it = result->begin();
        for(Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp, ++it){
            Vector3::Spherical s(direction(*itp-origin));
            *it = Vector2(s.theta,s.phi);
        }
    }
    return result;
}


std::pair<uint32_t,real_t> 
PGL::findClosestFromSubset(const TOOLS(Vector3)& origin, const Point3ArrayPtr points, const Index& group)
{
   size_t nbpoints = group.size();
    if(nbpoints != 0) {
        Index::const_iterator itg = group.begin();
        real_t dist  = norm(origin-points->getAt(*itg));
        uint32_t pid = *itg;
        for(++itg; itg != group.end(); ++itg){
            real_t ldist  = norm(origin-points->getAt(*itg));
            if (ldist < dist){
                dist = ldist;
                pid = *itg;
            }
        }
        return std::pair<uint32_t,real_t>(pid,dist);
    }
    else {
        std::pair<Point3Array::const_iterator,real_t> res = findClosest(*points,origin);
        return std::pair<uint32_t,real_t>(std::distance<Point3Array::const_iterator>(points->begin(),res.first),res.second);
    }
 }


RealArray2Ptr PGL::orientations_distances(const Point3ArrayPtr orientations, const Index& group )
{
   RealArray2Ptr result;
   size_t nbpoints = group.size();
   if(nbpoints != 0) {
       result = RealArray2Ptr(new RealArray2(nbpoints,nbpoints,0));
       uint32_t i = 0, j = 0;
       for (Index::const_iterator it = group.begin(); it != group.end()-1; ++it, ++i){
            // result->setAt(i,i,0);
            j = i+1;
            for (Index::const_iterator it2 = it+1; it2 != group.end(); ++it2, ++j){
                real_t a = angle(orientations->getAt(*it),orientations->getAt(*it2));
                result->setAt(i,j,a); result->setAt(j,i,a);
            }
       }
   }
   else {
       size_t nbpoints = orientations->size();
       result = RealArray2Ptr(new RealArray2(nbpoints,nbpoints,0));
       for (uint32_t i = 0; i < nbpoints-1; ++i){
            // result->setAt(i,i,0);            
            for (uint32_t j = i+1; j < nbpoints;++j){
                real_t a = angle(orientations->getAt(i),orientations->getAt(j));
                result->setAt(i,j,a); result->setAt(j,i,a);
            }
       }
   }
   return result;
}

RealArray2Ptr PGL::orientations_similarities(const Point3ArrayPtr orientations, const Index& group )
{
   RealArray2Ptr result;
   size_t nbpoints = group.size();
   if(nbpoints != 0) {
       result = RealArray2Ptr(new RealArray2(nbpoints,nbpoints,1));
       uint32_t i = 0, j = 0;
       for (Index::const_iterator it = group.begin(); it != group.end()-1; ++it, ++i){
            // result->setAt(i,i,0);
            j = i+1;
            for (Index::const_iterator it2 = it+1; it2 != group.end(); ++it2, ++j){
                real_t a = 1 - (angle(orientations->getAt(*it),orientations->getAt(*it2)) / GEOM_PI);
                result->setAt(i,j,a); 
                result->setAt(j,i,a);
            }
       }
   }
   else {
       size_t nbpoints = orientations->size();
       result = RealArray2Ptr(new RealArray2(nbpoints,nbpoints,1));
       for (uint32_t i = 0; i < nbpoints-1; ++i){
            // result->setAt(i,i,0);            
            for (uint32_t j = i+1; j < nbpoints;++j){
                real_t a = 1 - (angle(orientations->getAt(i),orientations->getAt(j)) / GEOM_PI);
                result->setAt(i,j,a); result->setAt(j,i,a);
            }
       }
   }
   return result;
}

std::pair<Index,Index> PGL::cluster_junction_points(const IndexArrayPtr pointtoppology, const Index& _group1, const Index& _group2)
{
    bool sizetest = (_group1.size() >= _group2.size());
    const Index& group1(sizetest? _group1 : _group2 );
    const Index& group2(sizetest? _group2 : _group1 );

    Index jgroup1;

    pgl_hash_map<uint32_t,uint32_t> jgroup2;
    for(Index::const_iterator it = group2.begin(); it != group2.end(); ++it)
        jgroup2[*it] = 0;

    for(Index::const_iterator it = group1.begin(); it != group1.end(); ++it){
        const Index& inbg = pointtoppology->getAt(*it);
        for(Index::const_iterator itnbg = inbg.begin(); itnbg != inbg.end(); ++itnbg){
            pgl_hash_map<uint32_t,uint32_t>::iterator it2 = jgroup2.find(*itnbg);
            if (it2 != jgroup2.end()){
                jgroup1.push_back(*it);
                it2->second += 1;
            }
        }
    }

    Index fjgroup2;
    for(pgl_hash_map<uint32_t,uint32_t>::const_iterator it = jgroup2.begin(); it != jgroup2.end(); ++it)
        if(it->second > 0) fjgroup2.push_back(it->first);

    return std::pair<Index,Index>(jgroup1,fjgroup2);
}


real_t mean_over(const Point3ArrayPtr points, const Index& section, int i)
{
    real_t v = 0;
    for(Index::const_iterator it = section.begin(); it != section.end(); ++it)
        v += points->getAt(*it).getAt(i);
    return v / section.size();
}


real_t mean_over(const Point3ArrayPtr points, const Index& section, int i, int j)
{
    real_t v = 0;
    for(Index::const_iterator it = section.begin(); it != section.end(); ++it)
        v += points->getAt(*it).getAt(i) * points->getAt(*it).getAt(j);
    return v / section.size();
}

#ifdef WITH_CGAL
#include <CGAL/eigen.h>
#endif
TOOLS(Vector3) PGL::section_normal(const Point3ArrayPtr points, const Index& section)
{
#ifdef WITH_CGAL
    real_t mx  = mean_over(points,section,0);
    real_t mx2 = mean_over(points,section,0,0);
    real_t my  = mean_over(points,section,1);
    real_t my2 = mean_over(points,section,1,1);
    real_t mz  = mean_over(points,section,2);
    real_t mz2 = mean_over(points,section,2,2);
    real_t mxy = mean_over(points,section,0,1);
    real_t mxz = mean_over(points,section,0,2);
    real_t myz = mean_over(points,section,1,2);
    real_t mxyxy = 2*mxy-2*mx*my;
    real_t mxzxz = 2*mxz - 2*mx*mz;
    real_t myzyz = 2*myz - 2*my*mz;

/*    Matrix3 M(mx2-mx*mx , mxyxy     , mxzxz,
              mxyxy     , my2-my*my , myzyz,
              mxzxz     , myzyz     , mz2-mz*mz);*/

    real_t covariance[6];
    covariance[0] = mx2-mx*mx;
    covariance[1] = mxyxy;
    covariance[2] = my2-my*my;
    covariance[3] = mxzxz;
    covariance[4] = myzyz;
    covariance[5] = mz2-mz*mz;

    real_t eigen_values[3];
    real_t eigen_vectors[9];
    CGAL::internal::eigen_symmetric<real_t>(covariance,3,eigen_vectors,eigen_values);

    if (eigen_values[2] <  eigen_values[1] && eigen_values[2] <  eigen_values[0])
        return Vector3(eigen_vectors[6],eigen_vectors[7],eigen_vectors[8]);
    else if (eigen_values[1] <  eigen_values[0])
        return Vector3(eigen_vectors[3],eigen_vectors[4],eigen_vectors[5]);
    else
        return Vector3(eigen_vectors[0],eigen_vectors[1],eigen_vectors[2]);

#else
    return Vector3::ORIGIN;
#endif
}


uint32_t PGL::pointset_median(const Point3ArrayPtr points)
{
	real_t minsumdist = 0;
	Point3Array::const_iterator median = points->begin();
	for(Point3Array::const_iterator itp = points->begin()+1; itp != points->end(); ++itp) {
		minsumdist += norm(*itp-*median);
	}
	
	for(Point3Array::const_iterator itmedian = points->begin()+1; itmedian != points->end(); ++itmedian) {
		real_t sumdist = 0;
		for(Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp) {
			sumdist += norm(*itp-*itmedian);
		}
		if (sumdist < minsumdist) {
			median = itmedian; minsumdist = sumdist;
		}
	}
	return std::distance<Point3Array::const_iterator>(points->begin(),median);

}

uint32_t PGL::approx_pointset_median(const Point3ArrayPtr points, uint32_t nbIterMax)
{
	Vector3 center = points->getCenter();
	std::pair<Point3Array::const_iterator,real_t> closest = findClosest<Point3Array>(*points,center);
	Point3Array::const_iterator cpoint = closest.first;
	real_t dist = closest.second;
	if (dist <= GEOM_EPSILON) return std::distance<Point3Array::const_iterator>(points->begin(),cpoint);

	uint32_t iternb = 0;
	real_t lastdists[3];

	while (iternb < nbIterMax){
		Vector3 ncenter(0,0,0);
		real_t sumw = 0;
		real_t totdist = 0;
		real_t mindist = REAL_MAX;
		// compute mean point with weigth inversely proportional to distance to actual mean point.
		for(Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp) {
			real_t d = normSquared(*itp-center);
			// if (d <= GEOM_TOLERANCE) return std::distance<Point3Array::const_iterator>(points->begin(),itp);
			if (d < mindist) {
				mindist = d; cpoint = itp;
			}
			real_t ldist = sqrt(d);
			ncenter += *itp / ldist;
			sumw += 1/ldist;
			totdist += d;
		}
		if (mindist < GEOM_TOLERANCE)  return std::distance<Point3Array::const_iterator>(points->begin(),cpoint);

		real_t lastdist = (iternb <= 1?REAL_MAX:lastdists[(iternb-2) % 3]);
		if (fabs(lastdist - totdist) < GEOM_EPSILON) {
			// converged
			return std::distance<Point3Array::const_iterator>(points->begin(),cpoint);
		}
		else {
			// printf("%i %i %f\n",iternb, std::distance<Point3Array::const_iterator>(points->begin(),cpoint), totdist);
			lastdists[iternb % 3] = totdist;
			center = ncenter/sumw;
		}
		++iternb;
	}
	return std::distance<Point3Array::const_iterator>(points->begin(),cpoint);

}