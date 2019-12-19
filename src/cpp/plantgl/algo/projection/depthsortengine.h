

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

    virtual void iprocess(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);
    virtual void iprocess(PolylinePtr polyline, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);
    virtual void iprocess(PointSetPtr pointset, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);

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