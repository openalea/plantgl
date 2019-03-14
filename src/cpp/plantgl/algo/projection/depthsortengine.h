

/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2018 - Cirad/Inra/Inria
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://github.com/openalea/plantgl
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

/*! \file depthsortengine.h
    \brief Definition of Rendering Engine based on ZBuffer.
*/



#ifndef __DepthSortEngine_h__
#define __DepthSortEngine_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/scene/scene.h>
#include "projectionengine.h"
#include <list>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class DepthSortEngine
    \brief A Rendering Engine based on Painter's algorithm (depth sort).
*/

/* ----------------------------------------------------------------------- */

class ALGO_API DepthSortEngine : public ProjectionEngine {

public :

    DepthSortEngine();

    virtual ~DepthSortEngine();

    void processTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, uint32_t id);

    ScenePtr getResult(Color4::eColor4Format format = Color4::eARGB, bool cameraCoordinates = true) const;
    ScenePtr getProjectionResult(Color4::eColor4Format format = Color4::eARGB, bool cameraCoordinates = true) const;

    virtual void process(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id);
    virtual void process(PolylinePtr polyline, MaterialPtr material, uint32_t id);
    virtual void process(PointSetPtr pointset, MaterialPtr material, uint32_t id);

    struct PolygonInfo {
        Point3ArrayPtr points;
        Vector3 pmin, pmax;
        uint32_t id;
    };

    typedef std::list<PolygonInfo> PolygonInfoList;
    typedef std::list<PolygonInfo> PolygonInfoSet;
    typedef std::list<PolygonInfoSet::iterator> PolygonInfoIteratorList;

protected:

    void removePolygon(DepthSortEngine::PolygonInfoSet::iterator it);
    DepthSortEngine::PolygonInfoSet::iterator appendPolygon(const PolygonInfo& polygon);
    DepthSortEngine::PolygonInfoIteratorList appendPolygons(DepthSortEngine::PolygonInfoList::iterator begin, DepthSortEngine::PolygonInfoList::iterator end);
    DepthSortEngine::PolygonInfoIteratorList getIntersectingPolygons(const PolygonInfo& polygon);


    PolygonInfo _toPolygonInfo(const Point3ArrayPtr& points, uint32_t id) const;
    DepthSortEngine::PolygonInfoList _toPolygonInfo(const std::vector<Point3ArrayPtr>& polygons, uint32_t id) const;

    DepthSortEngine::PolygonInfoIteratorList::iterator _processTriangle( PolygonInfo polygon, 
                                                                         DepthSortEngine::PolygonInfoIteratorList& polygonstotest, 
                                                                         DepthSortEngine::PolygonInfoIteratorList::iterator begin);

    PolygonInfoList __polygonlist; 
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif