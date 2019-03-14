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
    \brief Some algorithms to compute overlay between  Geometric Model on points.
*/

/* ----------------------------------------------------------------------- */

#ifndef __actn_overlay_h__
#define __actn_overlay_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/scenegraph/geometry/polyline.h>
#include "../raycasting/ray.h"
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/**
   \class Overlay
   \brief An action which compute overlay between two a \e Geometry objects.
*/



class ALGO_API Overlay {
public:
	/// Compute the overlay between 2 closed planar polylines.
	static GeometryPtr process(const Polyline2DPtr&, const Polyline2DPtr&);

};


class ALGO_API CurveIntersection {
public:
	/// Compute intersection between polylines.
	static Point2ArrayPtr compute(const std::vector<Polyline2DPtr>& polylines);
	static bool check(const std::vector<Polyline2DPtr>& polylines);
};

ALGO_API real_t raySegmentDistance(const Ray& ray, 
						  const Vector3& segA,
						  const Vector3& segB);


ALGO_API IndexArrayPtr determine_faces_from_edges(const Point2ArrayPtr&, const std::vector<std::pair<uint32_t, uint32_t> >& edges);

ALGO_API IndexArrayPtr determine_faceedges_from_edges(const Point2ArrayPtr&, const std::vector<std::pair<uint32_t, uint32_t> >& edges);

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __actn_overlay_h__
#endif

