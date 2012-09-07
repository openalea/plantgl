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

/*! \file overlay.h
    \brief Some algorithms to compute intersection between polygons.
*/

/* ----------------------------------------------------------------------- */

#ifndef __actn_intersection_h__
#define __actn_intersection_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/geometry/plane.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


ALGO_API std::pair<Point2ArrayPtr, IndexArrayPtr> polygon2ds_intersection(Point2ArrayPtr polygon1, Point2ArrayPtr polygon2);

ALGO_API std::pair<Point2ArrayPtr, IndexArrayPtr> polygon2ds_intersection(Point2ArrayPtr points, Index polygon1, Index polygon2);



// Check if a plane and segment intersect. result contains intersection if true.
ALGO_API bool plane_segment_intersection(const Plane3& plane, 
                                         const TOOLS(Vector3)& p1, 
                                         const TOOLS(Vector3)& p2,
                                         TOOLS(Vector3)& result);

// Compute the actual intersection
TOOLS(Vector3) _plane_segment_intersection(const Plane3& plane, 
                                    real_t d1,
                                    const TOOLS(Vector3)& p1, 
                                    const TOOLS(Vector3)& p2);

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __intersection_h__
#endif

