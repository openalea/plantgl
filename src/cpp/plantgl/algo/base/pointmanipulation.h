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

#ifndef __pointmanipulation_h__
#define __pointmanipulation_h__

#include "../algo_config.h"
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/algo/grid/regularpointgrid.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/function/function.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/tool/util_array.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/math/util_vector.h>
#include <memory>
#include <vector>

PGL_BEGIN_NAMESPACE


template <class PointListType>
RCPtr<PointListType> contract_point(RCPtr<PointListType> points, real_t radius)
{
	typedef typename PointListType::element_type VectorType;
	typedef PointRefGrid<PointListType> LocalPointGrid;
	typedef typename LocalPointGrid::PointIndexList PointIndexList;

	LocalPointGrid grid(radius,points);
	
	RCPtr<PointListType> result(new PointListType(points->size()));
	typename PointListType::iterator _itresult = result->begin();
	for(typename PointListType::const_iterator _itsource = points->begin();
		_itsource != points->end(); ++_itsource, ++_itresult)
	{
	  PointIndexList pointindices = grid.query_ball_point(*_itsource,radius);
	  VectorType center;
	  if (pointindices.size() > 0){
	   for(typename PointIndexList::const_iterator itptindex = pointindices.begin(); itptindex != pointindices.end(); ++itptindex)
	   { center += points->getAt(*itptindex); }
	   center /= pointindices.size();
	   *_itresult = center;
	  }
	  else *_itresult = *_itsource;

	}

	return result;
}


// to control display of progress
typedef void (* progressstatusfunction)(const char *, float);

ALGO_API void register_progressstatus_func(progressstatusfunction func);

ALGO_API void unregister_progressstatus_func();

//ALGO_API void group_info(const int &group_id);
//ALGO_API void group_size();
//ALGO_API void color_group(const int &group_id, const Color4 &color);
//ALGO_API ScenePtr clean_ground(const std::string &filename, const int &coeff_angle);
ALGO_API Index select_not_ground(const Point3ArrayPtr point, const IndexArrayPtr kclosest);
ALGO_API Index select_wire(const Point3ArrayPtr point, const IndexArrayPtr kclosest);

// typedef std::vector<std::vector<uint32_t> > AdjacencyMap;

/// K-Neighborhood computation
ALGO_API IndexArrayPtr 
delaunay_point_connection(const Point3ArrayPtr points);

ALGO_API Index3ArrayPtr 
delaunay_triangulation(const Point3ArrayPtr points);

ALGO_API IndexArrayPtr 
k_closest_points_from_delaunay(const Point3ArrayPtr points, size_t k);

ALGO_API IndexArrayPtr 
k_closest_points_from_ann(const Point3ArrayPtr points, size_t k, bool symmetric = false);

// ALGO_API IndexArrayPtr 
// k_closest_points_from_cgal(const Point3ArrayPtr points, size_t k);

ALGO_API IndexArrayPtr 
symmetrize_connections(const IndexArrayPtr adjacencies);

/// Reconnect all connex components of an adjacency graph
ALGO_API IndexArrayPtr 
connect_all_connex_components(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, bool verbose = false);

/// R-Neighborhood computation
ALGO_API Index 
r_neighborhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const real_t radius);

ALGO_API IndexArrayPtr 
r_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const TOOLS(RealArrayPtr) radii);

ALGO_API IndexArrayPtr 
r_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, bool verbose = false);

ALGO_API IndexArrayPtr 
r_neighborhoods_mt(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, bool verbose = false);

ALGO_API Index 
r_anisotropic_neighborhood(uint32_t pid, const Point3ArrayPtr points, 
					 const IndexArrayPtr adjacencies, 
					 const real_t radius, 
					 const TOOLS(Vector3)& direction, 
					 const real_t alpha, const real_t beta);

ALGO_API IndexArrayPtr 
r_anisotropic_neighborhoods(const Point3ArrayPtr points, 
			    const IndexArrayPtr adjacencies, 
			    const TOOLS(RealArrayPtr) radii, 
			    const Point3ArrayPtr directions, 
                const real_t alpha, 
	            const real_t beta);

ALGO_API IndexArrayPtr 
r_anisotropic_neighborhoods(const Point3ArrayPtr points, 
			    const IndexArrayPtr adjacencies, 
			    const real_t radius, 
			    const Point3ArrayPtr directions, 
                const real_t alpha, 
	            const real_t beta);

/// Extended K-Neighborhood computation
ALGO_API Index 
k_neighborhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k);

ALGO_API IndexArrayPtr
k_neighborhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k);


// Useful function

/// Find the k closest point from the set of adjacencies
ALGO_API Index get_k_closest_from_n(const Index& adjacencies, const uint32_t k, uint32_t pid, const Point3ArrayPtr points);


ALGO_API real_t
pointset_max_distance(  uint32_t pid,
                        const Point3ArrayPtr points, 
			            const Index& group);

ALGO_API real_t
pointset_max_distance( const TOOLS(Vector3)& origin,
                       const Point3ArrayPtr points, 
			           const Index& group);

ALGO_API real_t
pointset_min_distance(  uint32_t pid,
                        const Point3ArrayPtr points, 
			            const Index& group);

ALGO_API real_t
pointset_min_distance( const TOOLS(Vector3)& origin,
                       const Point3ArrayPtr points, 
			           const Index& group);

// ALGO_API
template<class IndexGroup>
real_t pointset_mean_distance(  const TOOLS(Vector3)& origin,
                         const Point3ArrayPtr points, 
                         const IndexGroup& group)
{
    if (group.empty()) return 0;
    real_t sum_distance = 0;
    for(typename IndexGroup::const_iterator it = group.begin(); it != group.end(); ++it)
        sum_distance += norm(origin-points->getAt(*it));
    return sum_distance / group.size();
}

template<class IndexGroupArray>
TOOLS(RealArrayPtr) pointset_mean_distances(  const Point3ArrayPtr origins,
                                 const Point3ArrayPtr points, 
                                 const RCPtr<IndexGroupArray> groups)
{
    typedef typename IndexGroupArray::element_type IndexGroup;
    TOOLS(RealArrayPtr) result(new TOOLS(RealArray)(groups->size()));
    TOOLS(RealArray)::iterator itres = result->begin();
    Point3Array::const_iterator itorigin = origins->begin();
    for(typename IndexGroupArray::const_iterator it = groups->begin(); it != groups->end(); ++it, ++itorigin, ++itres )
        *itres = pointset_mean_distance<IndexGroup>(*itorigin, points, *it);
    return result;
}

ALGO_API real_t
pointset_mean_radial_distance(  const TOOLS(Vector3)& origin,
                                const TOOLS(Vector3)& direction,
                                const Point3ArrayPtr points, 
                                const Index& group);

ALGO_API real_t
pointset_max_radial_distance(  const TOOLS(Vector3)& origin,
                                const TOOLS(Vector3)& direction,
                                const Point3ArrayPtr points, 
                                const Index& group);


ALGO_API TOOLS(Matrix3) pointset_covariance(const Point3ArrayPtr points,  const Index& group = Index());


ALGO_API Index 
get_sorted_element_order(const TOOLS(RealArrayPtr) distances);


/// Density computation
ALGO_API real_t
density_from_r_neighborhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies, 
                               const real_t radius);

ALGO_API TOOLS(RealArrayPtr)
densities_from_r_neighborhood(const Point3ArrayPtr points, 
                               const IndexArrayPtr adjacencies, 
                               const real_t radius);

ALGO_API TOOLS(RealArrayPtr)
densities_from_r_neighborhood(const IndexArrayPtr neighborhood, 
                              const real_t radius);


// if k == 0, then k is directly the nb of point given in adjacencies.
ALGO_API real_t
density_from_k_neighborhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k = 0);

ALGO_API TOOLS(RealArrayPtr)
densities_from_k_neighborhood(const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k = 0);



/// Orientation estimations
ALGO_API TOOLS(Vector3) 
pointset_orientation(const Point3ArrayPtr points, const Index& group);

ALGO_API Point3ArrayPtr 
pointsets_orientations(const Point3ArrayPtr points, const IndexArrayPtr groups);

ALGO_API TOOLS(Vector3) 
pointset_normal(const Point3ArrayPtr points, const Index& group);

ALGO_API Point3ArrayPtr 
pointsets_normals(const Point3ArrayPtr points, const IndexArrayPtr groups);


ALGO_API Point3ArrayPtr 
pointsets_orient_normals(const Point3ArrayPtr normals, const Point3ArrayPtr points, const IndexArrayPtr riemanian);


ALGO_API Point3ArrayPtr 
pointsets_orient_normals(const Point3ArrayPtr normals, uint32_t source, const IndexArrayPtr riemanian);

/// Orientation estimations
ALGO_API TOOLS(Vector3) 
triangleset_orientation(const Point3ArrayPtr points, const Index3ArrayPtr triangles);


struct CurvatureInfo {
    TOOLS(Vector3) origin;
    TOOLS(Vector3) maximal_principal_direction;
    real_t maximal_curvature;
    TOOLS(Vector3) minimal_principal_direction;
    real_t minimal_curvature;
    TOOLS(Vector3) normal;
};

ALGO_API CurvatureInfo
principal_curvatures(const Point3ArrayPtr points, uint32_t pid, const Index& group, size_t fitting_degree = 4, size_t monge_degree = 4);

ALGO_API std::vector<CurvatureInfo>
principal_curvatures(const Point3ArrayPtr points, const IndexArrayPtr groups, size_t fitting_degree = 4, size_t monge_degree = 4);

ALGO_API std::vector<CurvatureInfo>
principal_curvatures(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, size_t fitting_degree = 4, size_t monge_degree = 4);

// Compute the set of points that are at a distance < width from the plane at point pid in direction
ALGO_API Index
point_section( uint32_t pid,
               const Point3ArrayPtr points,
               const IndexArrayPtr adjacencies, 
               const TOOLS(Vector3)& direction, 
               real_t width);

ALGO_API Index
point_section( uint32_t pid,
               const Point3ArrayPtr points,
               const IndexArrayPtr adjacencies, 
               const TOOLS(Vector3)& direction, 
               real_t width,
               real_t maxradius);

ALGO_API IndexArrayPtr
points_sections( const Point3ArrayPtr points,
                 const IndexArrayPtr adjacencies, 
                 const Point3ArrayPtr directions, 
                 real_t width);

/// Compute a circle from a point set
ALGO_API std::pair<TOOLS(Vector3),real_t>
pointset_circle( const Point3ArrayPtr points,
                 const Index&  group,
                 bool bounding = false);

ALGO_API std::pair<TOOLS(Vector3),real_t>
pointset_circle( const Point3ArrayPtr points,
                 const Index&  group,
                 const TOOLS(Vector3)& direction,
                 bool bounding = false);

ALGO_API std::pair<Point3ArrayPtr,TOOLS(RealArrayPtr)>
pointsets_circles( const Point3ArrayPtr points,
                   const IndexArrayPtr  groups,
                   const Point3ArrayPtr directions = Point3ArrayPtr(0),
                   bool bounding = false);

ALGO_API std::pair<Point3ArrayPtr,TOOLS(RealArrayPtr)>
pointsets_section_circles( const Point3ArrayPtr points,
                           const IndexArrayPtr  adjacencies,
                           const Point3ArrayPtr directions,
                           real_t width,
                           bool bounding = false);


// Adaptive contraction
ALGO_API std::pair<Point3ArrayPtr,TOOLS(RealArrayPtr)>
adaptive_section_circles(const Point3ArrayPtr points, 
                         const IndexArrayPtr adjacencies,
                         const Point3ArrayPtr orientations,
                         const TOOLS(RealArrayPtr) widths, 
                         const TOOLS(RealArrayPtr) maxradii);

// Adaptive contraction
ALGO_API std::pair<Point3ArrayPtr,TOOLS(RealArrayPtr)>
adaptive_section_circles(const Point3ArrayPtr points, 
                         const IndexArrayPtr adjacencies,
                         const Point3ArrayPtr orientations,
                         const real_t width, 
                         const TOOLS(RealArrayPtr) maxradii);

// adaptive contraction
ALGO_API TOOLS(RealArrayPtr)
adaptive_radii( const TOOLS(RealArrayPtr) density,
                 real_t minradius, real_t maxradius,
                 QuantisedFunctionPtr densityradiusmap = NULL);

// Adaptive contraction
ALGO_API Point3ArrayPtr
adaptive_contration(const Point3ArrayPtr points, 
                     const Point3ArrayPtr orientations,
                     const IndexArrayPtr adjacencies, 
                     const TOOLS(RealArrayPtr) densities,
                     real_t minradius, real_t maxradius,
                     QuantisedFunctionPtr densityradiusmap = NULL,
                     const real_t alpha = 1, 
	                 const real_t beta = 1);

// Adaptive contraction
ALGO_API std::pair<Point3ArrayPtr,TOOLS(RealArrayPtr)>
adaptive_section_contration(const Point3ArrayPtr points, 
                            const Point3ArrayPtr orientations,
                            const IndexArrayPtr adjacencies, 
                            const TOOLS(RealArrayPtr) densities,
                            real_t minradius, real_t maxradius,
                            QuantisedFunctionPtr densityradiusmap = NULL,
                            const real_t alpha = 1, 
	                        const real_t beta = 1);

/// Shortest path
ALGO_API std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)>
points_dijkstra_shortest_path(const Point3ArrayPtr points, 
			                  const IndexArrayPtr adjacencies, 
                              uint32_t root,
                              real_t powerdist = 1);


// Return groups of points
ALGO_API IndexArrayPtr 
quotient_points_from_adjacency_graph(const real_t binsize,
                        const Point3ArrayPtr points, 
			            const IndexArrayPtr adjacencies, 
			            const TOOLS(RealArrayPtr) distances_to_root);

// Return adjacencies between groups
ALGO_API IndexArrayPtr 
quotient_adjacency_graph(const IndexArrayPtr adjacencies, 
			                    const IndexArrayPtr groups);

ALGO_API TOOLS(Vector3) 
centroid_of_group(const Point3ArrayPtr points, 
			        const Index& group);

ALGO_API Point3ArrayPtr 
centroids_of_groups(const Point3ArrayPtr points, 
			        const IndexArrayPtr groups);


 
template<class IndexGroup>
TOOLS(Vector3)  centroid_of_group(const Point3ArrayPtr points, 
                                  const IndexGroup& group)
{
        TOOLS(Vector3) gcentroid; real_t nbpoints = 0;
        for(typename IndexGroup::const_iterator itn = group.begin(); itn != group.end(); ++itn,++nbpoints){
            gcentroid += points->getAt(*itn);
        }
        return gcentroid/nbpoints;
}


template<class IndexGroupArray>
Point3ArrayPtr  centroids_of_groups(const Point3ArrayPtr points, 
                                    const RCPtr<IndexGroupArray> groups)
{
    Point3ArrayPtr result(new Point3Array(groups->size()));
    uint32_t cgroup = 0;
    for(typename IndexGroupArray::const_iterator itgs = groups->begin(); itgs != groups->end(); ++itgs, ++cgroup)
    {
        result->setAt(cgroup,centroid_of_group(points,*itgs));
    }
    return result;
}

ALGO_API IndexArrayPtr cluster_points(const Point3ArrayPtr points, const Point3ArrayPtr clustercentroid);
ALGO_API TOOLS(Uint32Array1Ptr) points_clusters(const Point3ArrayPtr points, const Point3ArrayPtr clustercentroid);

// Xu 07 method for main branching system
ALGO_API Point3ArrayPtr
skeleton_from_distance_to_root_clusters(const Point3ArrayPtr points, uint32_t root, real_t binsize, uint32_t k,  
                                        TOOLS(Uint32Array1Ptr)& group_parents, IndexArrayPtr& group_components, 
                                        bool connect_all_points = false, bool verbose = false);

ALGO_API Index 
points_in_range_from_root(const real_t initialdist, const real_t binsize, const TOOLS(RealArrayPtr) distances_to_root);

ALGO_API std::pair<IndexArrayPtr,TOOLS(RealArrayPtr)> 
next_quotient_points_from_adjacency_graph(const real_t initiallevel,
                                            const real_t binsize,
                                            const Index& currents,
			                                const IndexArrayPtr adjacencies, 
			                                const TOOLS(RealArrayPtr) distances_to_root);



// Livny method procedures
// compute parent-children relation from child-parent relation
ALGO_API IndexArrayPtr determine_children(const TOOLS(Uint32Array1Ptr) parents, uint32_t& root);

// compute a weight to each points as sum of length of carried segments
ALGO_API TOOLS(RealArrayPtr) carried_length(const Point3ArrayPtr points, const TOOLS(Uint32Array1Ptr) parents);

// compute a weight to each points as number of node in their 
ALGO_API TOOLS(Uint32Array1Ptr) subtrees_size(const TOOLS(Uint32Array1Ptr) parents);
ALGO_API TOOLS(Uint32Array1Ptr) subtrees_size(const IndexArrayPtr children,  uint32_t root);

// optimize orientation
ALGO_API Point3ArrayPtr optimize_orientations(const Point3ArrayPtr points, 
                                              const TOOLS(Uint32Array1Ptr) parents,
                                              const TOOLS(RealArrayPtr) weights);

// optimize orientation
ALGO_API Point3ArrayPtr optimize_positions(const Point3ArrayPtr points, 
                                           const Point3ArrayPtr orientations,
                                           const TOOLS(Uint32Array1Ptr) parents, 
                                           const TOOLS(RealArrayPtr) weights);

// estimate average radius around edges
ALGO_API real_t average_radius(const Point3ArrayPtr points, 
                               const Point3ArrayPtr nodes,
                               const TOOLS(Uint32Array1Ptr) parents,
                               uint32_t maxclosestnodes = 10);

ALGO_API TOOLS(RealArrayPtr) distance_to_shape(const Point3ArrayPtr points, 
                                          const Point3ArrayPtr nodes,
                                          const TOOLS(Uint32Array1Ptr) parents,
                                          const TOOLS(RealArrayPtr) radii,
                                          uint32_t maxclosestnodes = 10);

ALGO_API real_t average_distance_to_shape(const Point3ArrayPtr points, 
                                          const Point3ArrayPtr nodes,
                                          const TOOLS(Uint32Array1Ptr) parents,
                                          const TOOLS(RealArrayPtr) radii,
                                          uint32_t maxclosestnodes = 10);

ALGO_API Index points_at_distance_from_skeleton(const Point3ArrayPtr points, 
                                                const Point3ArrayPtr nodes,
                                                const TOOLS(Uint32Array1Ptr) parents,
                                                real_t distance,
                                                uint32_t maxclosestnodes = 10);

ALGO_API TOOLS(RealArrayPtr) estimate_radii_from_points(const Point3ArrayPtr points, 
                                                             const Point3ArrayPtr nodes,
                                                             const TOOLS(Uint32Array1Ptr) parents,
                                                             bool maxmethod = false,
                                                             uint32_t maxclosestnodes = 10);
// estimate radius for each node
ALGO_API TOOLS(RealArrayPtr) estimate_radii_from_pipemodel(const Point3ArrayPtr nodes,
                                            const TOOLS(Uint32Array1Ptr) parents, 
                                            const TOOLS(RealArrayPtr) weights,
                                            real_t averageradius,
                                            real_t pipeexponent = 2.5);

ALGO_API bool node_continuity_test(const TOOLS(Vector3)& node, real_t noderadius, 
                                   const TOOLS(Vector3)& parent, real_t parentradius,
                                   const TOOLS(Vector3)& child, real_t childradius,
                                   real_t overlapfilter = 0.5,
                                   bool verbose = false, ScenePtr * visu = NULL);

ALGO_API bool node_intersection_test(const TOOLS(Vector3)& root, real_t rootradius, 
                                     const TOOLS(Vector3)& p1,   real_t radius1, 
                                     const TOOLS(Vector3)& p2,   real_t radius2,
                                     real_t overlapfilter,
                                     bool verbose = false, ScenePtr * visu = NULL);
// compute the minimum maximum and  mean edge length
ALGO_API TOOLS(Vector3) min_max_mean_edge_length(const Point3ArrayPtr points, const TOOLS(Uint32Array1Ptr) parents);
ALGO_API TOOLS(Vector3) min_max_mean_edge_length(const Point3ArrayPtr points, const IndexArrayPtr graph);

// determine nodes to filter
ALGO_API Index detect_short_nodes(const Point3ArrayPtr nodes,
                                  const TOOLS(Uint32Array1Ptr) parents, 
                                  real_t edgelengthfilter = 0.001);

ALGO_API void remove_nodes(const Index& toremove,
                           Point3ArrayPtr& nodes,
                           TOOLS(Uint32Array1Ptr)& parents, 
                           TOOLS(RealArrayPtr)& radii);

ALGO_API inline void remove_nodes(const Index& toremove,
                           Point3ArrayPtr& nodes,
                           TOOLS(Uint32Array1Ptr)& parents)
{
    TOOLS(RealArrayPtr) radii(0);
    remove_nodes(toremove, nodes, parents, radii);
}

// determine nodes to filter
ALGO_API IndexArrayPtr detect_similar_nodes(const Point3ArrayPtr nodes,
                                            const TOOLS(Uint32Array1Ptr) parents, 
                                            const TOOLS(RealArrayPtr) radii, 
                                            const TOOLS(RealArrayPtr) weights, 
                                            real_t overlapfilter = 0.5);

ALGO_API void merge_nodes(const IndexArrayPtr tomerge,
                          Point3ArrayPtr& nodes,
                          TOOLS(Uint32Array1Ptr)& parents, 
                          TOOLS(RealArrayPtr)& radii, 
                          TOOLS(RealArrayPtr) weights);


// determine mean direction of a set of points
ALGO_API TOOLS(Vector3) pointset_mean_direction(const TOOLS(Vector3)& origin, const Point3ArrayPtr points, const Index& group = Index());

// determine all directions of a set of points
ALGO_API Point3ArrayPtr pointset_directions(const TOOLS(Vector3)& origin, const Point3ArrayPtr points, const Index& group = Index());

// determine all directions of a set of points
ALGO_API Point2ArrayPtr pointset_angulardirections(const Point3ArrayPtr points, const TOOLS(Vector3)& origin = TOOLS(Vector3::ORIGIN), const Index& group = Index());

// find the closest point from a group
ALGO_API std::pair<uint32_t,real_t> findClosestFromSubset(const TOOLS(Vector3)& origin, const Point3ArrayPtr points, const Index& group = Index());

// compute the pair wise distance between orientation (in angular domain)
ALGO_API TOOLS(RealArray2Ptr) orientations_distances(const Point3ArrayPtr orientations, const Index& group = Index());

// compute the pair wise similarity between orientation (in angular domain) 
ALGO_API TOOLS(RealArray2Ptr) orientations_similarities(const Point3ArrayPtr orientations, const Index& group = Index());

// compute the points that make the junction of the two group
ALGO_API std::pair<Index,Index> cluster_junction_points(const IndexArrayPtr pointtoppology, const Index& group1, const Index& group2);


// from Tagliasacchi 2009
ALGO_API TOOLS(Vector3) section_normal(const Point3ArrayPtr pointnormals, const Index& section);
ALGO_API Point3ArrayPtr sections_normals(const Point3ArrayPtr pointnormals, const IndexArrayPtr& sections);

/*
    Compute the geometric median of a point sample.
    The geometric median coordinates will be expressed in the Spatial Image reference system (not in real world metrics).
    We use the Weiszfeld's algorithm (http://en.wikipedia.org/wiki/Geometric_median) 
*/
ALGO_API uint32_t approx_pointset_median(const Point3ArrayPtr points, uint32_t nbIterMax = 200);

// brute force approach
ALGO_API uint32_t pointset_median(const Point3ArrayPtr points);

PGL_END_NAMESPACE

#endif

