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

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#ifdef WITH_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
// #include <CGAL/Triangulation_3.h>

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


template<class CGALPoint>
inline CGALPoint toPoint(const Vector3& v) { return CGALPoint(v.x(),v.y(),v.z()); }

template<class CGALPoint>
inline Vector3 toVector3(const CGALPoint& v) { return Vector3(v.x(),v.y(),v.z()); }

#endif

#include <stack>
#include <iostream>


IndexArrayPtr 
PGL::delaunay_point_connection(const Point3ArrayPtr points)
{
#ifdef WITH_CGAL
    Triangulation triangulation;
    uint32_t pointCount = 0;
    for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it)
        triangulation.insert(toPoint<TPoint>(*it))->info() = pointCount++;


    IndexArrayPtr result(new IndexArray(points->size(),Index()));
    for(Triangulation::Finite_edges_iterator it = triangulation.finite_edges_begin();
        it != triangulation.finite_edges_end(); ++it){
            uint32_t source = it->first->vertex(it->second)->info();
            uint32_t target = it->first->vertex(it->third)->info();
            result->getAt(source).push_back(target);
            result->getAt(target).push_back(source);
    } 
#else
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
PGL::k_closest_points_from_ann(const Point3ArrayPtr points, size_t k)
{
#ifdef WITH_ANN
    ANNKDTree3 kdtree(points);
    return kdtree.k_nearest_neighbors(k);
#else
    return IndexArrayPtr();
#endif
}

#ifdef WITH_CGAL
#include <CGAL/Cartesian.h>

typedef CGAL::Cartesian<double>   CK;
typedef CK::Point_3               CPoint;
typedef boost::tuple<CPoint,int>  CPoint_and_int;
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

#include "dijkstra.h"

struct PointDistance {
        const Point3ArrayPtr points;
        real_t operator()(uint32_t a, uint32_t b) const { return norm(points->getAt(a)-points->getAt(b)); }
        PointDistance(const Point3ArrayPtr _points) : points(_points) {}
};

Index
PGL::r_neighboorhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const real_t radius)
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
PGL::r_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const RealArrayPtr radii)
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
PGL::r_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    struct PointDistance pdevaluator(points);
    
    IndexArrayPtr result(new IndexArray(nbPoints));
    for(uint32_t current = 0; current < nbPoints; ++current) {
        NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,radius);
        Index lres;
        for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
            lres.push_back(itn->id);
        result->setAt(current,lres);
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
PGL::r_anisotropic_neighboorhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const real_t radius, 
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
PGL::r_anisotropic_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const RealArrayPtr radii, 
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
PGL::r_anisotropic_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, 
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
PGL::density_from_r_neighboorhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies, 
                               const real_t radius)
{
    Index r_neighboorhood_value = r_neighboorhood(pid, points, adjacencies, radius);
    return r_neighboorhood_value.size()/ (radius * radius);
}


TOOLS(RealArrayPtr)
PGL::densities_from_r_neighboorhood(const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies, 
                               const real_t radius)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    RealArrayPtr result(new RealArray(nbPoints));
    uint32_t current = 0;
    for(RealArray::iterator itres = result->begin(); itres != result->end(); ++itres){
        *itres = density_from_r_neighboorhood(current, points,adjacencies,radius);
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

Index PGL::k_neighboorhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k)
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
PGL::k_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    struct PointDistance pdevaluator(points);
    
    IndexArrayPtr result(new IndexArray(nbPoints));
    for(uint32_t current = 0; current < nbPoints; ++current) {
        Index lres;
        const Index& candidates = adjacencies->getAt(current);
        if (k <= candidates.size()) {
            lres = get_k_closest_from_n(candidates,k,current,points);
        }
        else {
            NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,REAL_MAX,k);
            for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
                lres.push_back(itn->id);
        }
        result->setAt(current,lres);
    }
    return result;
}

real_t
PGL::max_neighboorhood_distance(  uint32_t pid,
                                        const Point3ArrayPtr points, 
			                            const Index& adjacency)
{
    real_t max_distance = 0;
    for(Index::const_iterator itad = adjacency.begin(); itad != adjacency.end(); ++itad)
        max_distance = std::max(max_distance,norm(points->getAt(pid)-points->getAt(*itad)));
    return max_distance;
}

real_t
PGL::density_from_k_neighboorhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k)
{
    Index adjacency = (k == 0 ? adjacencies->getAt(pid) : k_neighboorhood(pid, points, adjacencies, k ));
    real_t radius = max_neighboorhood_distance(pid, points, adjacency);
    return adjacency.size() / (radius * radius);
}


TOOLS(RealArrayPtr)
PGL::densities_from_k_neighboorhood(const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k)
{
    uint32_t nbPoints = points->size();
    GEOM_ASSERT(nbPoints == adjacencies->size());
    GEOM_ASSERT(nbPoints == radii->size());
    RealArrayPtr result(new RealArray(nbPoints));
    uint32_t current = 0;
    for(RealArray::iterator itres = result->begin(); itres != result->end(); ++itres){
        *itres = density_from_k_neighboorhood(current, points,adjacencies,k);
        ++current;
    }
    return result;
}

#ifdef WITH_CGAL
#include <CGAL/linear_least_squares_fitting_3.h>

typedef CK::Line_3                CLine;
#endif

Vector3 PGL::pointset_orientation(const Point3ArrayPtr points, const Index& group )
{
#ifdef WITH_CGAL
	std::list<CPoint> pointdata;
	for (Point3Array::const_iterator it = points->begin(); it != points->end() ; ++it)
	for (Index::const_iterator it = group.begin(); it != group.end() ; ++it)
		pointdata.push_back(toPoint<CPoint>(points->getAt(*it)));

	CLine line;
	linear_least_squares_fitting_3(pointdata.begin(), pointdata.end(), line, CGAL::Dimension_tag<0>());

	return toVector3(line.to_vector());
#else
    return Vector3(0,0,0);
#endif
}

Point3ArrayPtr PGL::pointsets_orientations(const Point3ArrayPtr points, const IndexArrayPtr groups)
{
	size_t nbPoints = points->size();
	Point3ArrayPtr result(new Point3Array(nbPoints));

    IndexArray::const_iterator itNbrhd = groups->begin();
    for(Point3Array::iterator itRes = result->begin(); itRes != result->end(); ++itRes, ++itNbrhd)
        *itRes = pointset_orientation(points,*itNbrhd);

	return result;
}

std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)>
PGL::points_dijkstra_shortest_path(const Point3ArrayPtr points, 
			         const IndexArrayPtr adjacencies, 
                     uint32_t root)
{
    struct PointDistance pdevaluator(points);
    return dijkstra_shortest_paths(adjacencies,root,pdevaluator);
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

ALGO_API Point3ArrayPtr
PGL::skeleton_from_distance_to_root_clusters(const Point3ArrayPtr points, uint32_t root, real_t binsize, uint32_t k, TOOLS(Uint32Array1Ptr)& group_parents, IndexArrayPtr& group_components, bool verbose)
{
    if(verbose)std::cout << "Compute Remanian graph." << std::endl;
#ifdef WITH_ANN
    IndexArrayPtr remaniangraph =  k_closest_points_from_ann(points, k);
#else
    IndexArrayPtr remaniangraph =  k_closest_points_from_delaunay(points, k);
#endif
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
