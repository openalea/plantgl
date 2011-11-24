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
#include <plantgl/tool/rcobject.h>
#include <plantgl/algo/grid/regularpointgrid.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/tool/util_array.h>

PGL_BEGIN_NAMESPACE


template <class PointListType>
RCPtr<PointListType> compress_point(RCPtr<PointListType> points, real_t radius)
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


// typedef std::vector<std::vector<uint32_t> > AdjacencyMap;

/// K-Neighborhood computation
ALGO_API IndexArrayPtr 
delaunay_point_connection(const Point3ArrayPtr points);

ALGO_API IndexArrayPtr 
k_closest_points_from_delaunay(const Point3ArrayPtr points, size_t k);

ALGO_API IndexArrayPtr 
k_closest_points_from_ann(const Point3ArrayPtr points, size_t k);

ALGO_API IndexArrayPtr 
k_closest_points_from_cgal(const Point3ArrayPtr points, size_t k);

/// R-Neighborhood computation
ALGO_API Index 
r_neighboorhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const real_t radius);

ALGO_API IndexArrayPtr 
r_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const TOOLS(RealArrayPtr) radii);

ALGO_API IndexArrayPtr 
r_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius);

ALGO_API Index 
r_anisotropic_neighboorhood(uint32_t pid, const Point3ArrayPtr points, 
					 const IndexArrayPtr adjacencies, 
					 const real_t radius, 
					 const TOOLS(Vector3)& direction, 
					 const real_t alpha, const real_t beta);

ALGO_API IndexArrayPtr 
r_anisotropic_neighboorhoods(const Point3ArrayPtr points, 
			    const IndexArrayPtr adjacencies, 
			    const TOOLS(RealArrayPtr) radii, 
			    const Point3ArrayPtr directions, 
                const real_t alpha, 
	            const real_t beta);

ALGO_API IndexArrayPtr 
r_anisotropic_neighboorhoods(const Point3ArrayPtr points, 
			    const IndexArrayPtr adjacencies, 
			    const real_t radius, 
			    const Point3ArrayPtr directions, 
                const real_t alpha, 
	            const real_t beta);

/// Extended K-Neighborhood computation
ALGO_API Index 
k_neighboorhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k);

ALGO_API IndexArrayPtr
k_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const uint32_t k);

// Useful function

/// Find the k closest point from the set of adjacencies
ALGO_API Index get_k_closest_from_n(const Index& adjacencies, const uint32_t k, uint32_t pid, const Point3ArrayPtr points);


ALGO_API real_t
max_neighboorhood_distance(  uint32_t pid,
                             const Point3ArrayPtr points, 
			                 const Index& adjacency);

ALGO_API Index 
get_sorted_element_order(const TOOLS(RealArrayPtr) distances);


/// Density computation
ALGO_API real_t
density_from_r_neighboorhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies, 
                               const real_t radius);

ALGO_API TOOLS(RealArrayPtr)
densities_from_r_neighboorhood(const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies, 
                               const real_t radius);


// if k == 0, then k is directly the nb of point given in adjacencies.
ALGO_API real_t
density_from_k_neighboorhood(  uint32_t pid,
                               const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k = 0);

ALGO_API TOOLS(RealArrayPtr)
densities_from_k_neighboorhood(const Point3ArrayPtr points, 
			                   const IndexArrayPtr adjacencies,
                               const uint32_t k = 0);


/// Orientation estimations
ALGO_API TOOLS(Vector3) 
pointset_orientation(const Point3ArrayPtr points, const Index& group);

ALGO_API Point3ArrayPtr 
pointsets_orientations(const Point3ArrayPtr points, const IndexArrayPtr groups);


/// Shortest path
ALGO_API std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)>
points_dijkstra_shortest_path(const Point3ArrayPtr points, 
			         const IndexArrayPtr adjacencies, 
                     uint32_t root);


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

// Xu 07 method for main branching system
ALGO_API Point3ArrayPtr
skeleton_from_distance_to_root_clusters(const Point3ArrayPtr points, uint32_t root, real_t binsize, uint32_t k,  TOOLS(Uint32Array1Ptr)& group_parents, IndexArrayPtr& group_components, bool verbose = false);


PGL_END_NAMESPACE

#endif

