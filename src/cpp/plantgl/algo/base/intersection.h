/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */


/*! \file intersection.h
    \brief Some algorithms to compute intersection between polygons.
*/

/* ----------------------------------------------------------------------- */

#ifndef __actn_intersection_h__
#define __actn_intersection_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/scenegraph/geometry/plane.h>
#include <vector>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


ALGO_API std::pair<Point2ArrayPtr, IndexArrayPtr> polygon2ds_intersection(Point2ArrayPtr polygon1, Point2ArrayPtr polygon2);

ALGO_API std::pair<Point2ArrayPtr, IndexArrayPtr> polygon2ds_intersection(Point2ArrayPtr points, Index polygon1, Index polygon2);



// Check if a plane and segment intersect. result contains intersection if true.
ALGO_API bool plane_segment_intersection(const Plane3& plane,
                                         const Vector3& p1,
                                         const Vector3& p2,
                                         Vector3& result);

// Compute the actual intersection
Vector3 _plane_segment_intersection(const Plane3& plane,
                                    real_t d1,
                                    const Vector3& p1,
                                    const Vector3& p2);


enum IntersectionType {
    NoIntersection = 0,
    Intersection   = 1,
    CoPlanar       = 2
} ;

// Check if a plane and segment intersect. result contains intersection if true.
ALGO_API IntersectionType triangle_triangle_intersection(const Vector3& t11, const Vector3& t12, const Vector3& t13,
                                                 const Vector3& t21, const Vector3& t22, const Vector3& t23,
                                                 Vector3& intersectionstart, Vector3& intersectionend);



std::pair<std::vector<std::pair<uint32_t,uint32_t> >,GeometryArrayPtr>
auto_intersection(Point3ArrayPtr points, Index3ArrayPtr triangles);

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __intersection_h__
#endif
