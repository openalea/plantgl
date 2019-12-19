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


/*! \file overlay.h
    \brief Some algorithms to compute overlay between  Geometric Model on points.
*/

/* ----------------------------------------------------------------------- */

#ifndef __actn_overlay_h__
#define __actn_overlay_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include "../raycasting/ray.h"
#include <plantgl/scenegraph/geometry/polyline.h>
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
