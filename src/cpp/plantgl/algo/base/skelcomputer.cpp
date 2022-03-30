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





#include "skelcomputer.h"
#include "discretizer.h"
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_SKELCOMPUTER_CHECK_CACHE(geom) \
  if (!geom->unique()) { \
    Cache<PolylinePtr>::Iterator _it = __cache.find(geom->getObjectId()); \
    if (! (_it == __cache.end())) { \
       __skeleton = _it->second; \
      return true; \
    }; \
  };


#define GEOM_SKELCOMPUTER_UPDATE_CACHE(geom) \
  if (!geom->unique()) \
     __cache.insert(geom->getObjectId(),__skeleton);


/* ----------------------------------------------------------------------- */


SkelComputer::SkelComputer( Discretizer& discretizer ) :
  Action(),
  __cache(),
  __skeleton(),
  __discretizer(discretizer) {
}

SkelComputer::~SkelComputer( ) {
}

void SkelComputer::clear( ) {
  __cache.clear();
//  __skeleton = PolylinePtr();
}

Discretizer&
SkelComputer::getDiscretizer( ) {
  return __discretizer;
}

PolylinePtr
SkelComputer::getSkeleton( ) const {
  return __skeleton;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  __skeleton = amapSymbol->getSkeleton();
  if(__skeleton) return true;
  else{
    __skeleton = PolylinePtr();
    return false;
  }
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);
  __skeleton = PolylinePtr(new Polyline(asymmetricHull->getBottom(),
                                        asymmetricHull->getTop()));
  return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);

  GEOM_SKELCOMPUTER_CHECK_CACHE(axisRotated);

  if(!(axisRotated->getGeometry()->apply(*this)))return false;
  if (__skeleton) {
      __skeleton = dynamic_pointer_cast<Polyline>(__skeleton->transform(axisRotated->getTransformation()));
  }
  else return false;

  GEOM_SKELCOMPUTER_UPDATE_CACHE(axisRotated);
  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( BezierCurve * bezierCurve ) {
    GEOM_ASSERT(bezierCurve);
    if(bezierCurve->apply(__discretizer))
      __skeleton = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
    else
    __skeleton = PolylinePtr(new Polyline(
        bezierCurve->getPointAt(0),
        bezierCurve->getPointAt(1)));

    return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( BezierPatch * bezierPatch ) {
    GEOM_ASSERT(bezierPatch);

    __skeleton = PolylinePtr(new Polyline(bezierPatch->getPointAt(0.0,0.0),
                              bezierPatch->getPointAt(1.0,1.0)));
    return true;
}


/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Box * box ) {
  GEOM_ASSERT(box);
  const real_t& _z = box->getSize().z();
  __skeleton = PolylinePtr(new Polyline(Vector3(0,0,-_z),Vector3(0,0,_z)));
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Cone * cone ) {
  GEOM_ASSERT(cone);
  __skeleton = PolylinePtr(new Polyline(Vector3::ORIGIN,
                                        Vector3(0,0,cone->getHeight())));
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);
  __skeleton = PolylinePtr
    (new Polyline(Vector3::ORIGIN,
                  Vector3(0,0,cylinder->getHeight())));
  return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);

  GEOM_SKELCOMPUTER_CHECK_CACHE(elevationGrid);

  GEOM_TRACE("... has to be implemented");

  GEOM_SKELCOMPUTER_UPDATE_CACHE(elevationGrid);
  return false;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);

  GEOM_SKELCOMPUTER_CHECK_CACHE(eulerRotated);

  if(!eulerRotated->getGeometry()->apply(*this))return false;
  if (__skeleton) {
    __skeleton = dynamic_pointer_cast<Polyline>(__skeleton->transform(eulerRotated->getTransformation()));
  }
  else return false;

  GEOM_SKELCOMPUTER_UPDATE_CACHE(eulerRotated);
  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);

  GEOM_SKELCOMPUTER_CHECK_CACHE(extrudedHull);

  if(!extrudedHull->getVertical()->apply(__discretizer))return false;

  pair<Point3Array::const_iterator,Point3Array::const_iterator>
    _minAndMax = __discretizer.getDiscretization()->getPointList()->getYMinAndMax();
  __skeleton = PolylinePtr(new Polyline(Vector3(_minAndMax.first->x(),
                                                0,
                                                _minAndMax.first->y()),
                                        Vector3(_minAndMax.second->x(),
                                                0,
                                                _minAndMax.second->y())));

  GEOM_SKELCOMPUTER_UPDATE_CACHE(extrudedHull);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process(Extrusion * extrusion ) {
  GEOM_ASSERT(extrusion);

  return (extrusion->getAxis()->apply(*this));

}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  __skeleton = faceSet->getSkeleton();
  if(__skeleton) return true;
  else return false;
}


/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);
  __skeleton = PolylinePtr(new Polyline(Vector3::ORIGIN,
                                        Vector3(0,0,frustum->getHeight())));
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Group * group ) {
  GEOM_ASSERT(group);
  __skeleton = group->getSkeleton();
  if(__skeleton) return true;
  else return false;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  if(nurbsCurve->apply(__discretizer))
    __skeleton = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
  else
    __skeleton = PolylinePtr(new Polyline(
    nurbsCurve->getPointAt(nurbsCurve->getFirstKnot()),
    nurbsCurve->getPointAt(nurbsCurve->getLastKnot())));

  return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  __skeleton = PolylinePtr(new Polyline
     (nurbsPatch->getPointAt(nurbsPatch->getFirstUKnot(),
                             nurbsPatch->getFirstVKnot()),
      nurbsPatch->getPointAt(nurbsPatch->getLastUKnot(),
                             nurbsPatch->getLastVKnot())));
  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);

  GEOM_SKELCOMPUTER_CHECK_CACHE(revolution);

  if(!revolution->getProfile()->apply(__discretizer))return false;
  pair<Point3Array::const_iterator,Point3Array::const_iterator>
    _minMax = __discretizer.getDiscretization()->getPointList()->getYMinAndMax();

  __skeleton =
    PolylinePtr(new Polyline(Vector3(0,0,_minMax.first->y()),
                             Vector3(0,0,_minMax.second->y())));

  GEOM_SKELCOMPUTER_UPDATE_CACHE(revolution);
  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( Swung * swung )
{
  GEOM_ASSERT(swung);

  GEOM_SKELCOMPUTER_CHECK_CACHE(swung);

  const Curve2DArrayPtr& pList= swung->getProfileList();
  uint_t size= pList->size();

  Vector2 ptStart, ptEnd;
  uint_t i= 0;
  for( i= 0; i < size; i++ )
    {
    const Curve2DPtr& p= pList->getAt(i);

    const real_t uStart= p->getFirstKnot();
    const real_t uEnd= p->getLastKnot();

    ptStart+= p->getPointAt(uStart);
    ptEnd+= p->getPointAt(uEnd);
    }

  real_t zMin= ptStart.y() / real_t(size);
  real_t zMax= ptEnd.y() / real_t(size);

  __skeleton =
    PolylinePtr(new Polyline(Vector3(0,0,zMin),
                             Vector3(0,0,zMax)));

  GEOM_SKELCOMPUTER_UPDATE_CACHE(swung);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);

  GEOM_SKELCOMPUTER_CHECK_CACHE(oriented);

  if(!(oriented->getGeometry()->apply(*this)))return false;
  if (__skeleton) {
    __skeleton = dynamic_pointer_cast<Polyline>(__skeleton->transform(oriented->getTransformation()));
  }
  else return false;

  GEOM_SKELCOMPUTER_UPDATE_CACHE(oriented);
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);
  __skeleton = PolylinePtr
    (new Polyline(Vector3::ORIGIN,
                  Vector3(0,0,paraboloid->getHeight())));
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  __skeleton = PolylinePtr();
  return false;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  __skeleton = PolylinePtr(polyline);
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  __skeleton = quadSet->getSkeleton();
  if(__skeleton) return true;
  else return false;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);

  GEOM_SKELCOMPUTER_CHECK_CACHE(scaled);

  if(!(scaled->getGeometry()->apply(*this)))return false;
  if (__skeleton) {
    __skeleton = dynamic_pointer_cast<Polyline>(__skeleton->transform(scaled->getTransformation()));
  };

  GEOM_SKELCOMPUTER_UPDATE_CACHE(scaled);
  return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( ScreenProjected * scp ) {
  GEOM_ASSERT(scp);

  GEOM_SKELCOMPUTER_CHECK_CACHE(scp);

  if(!(scp->getGeometry()->apply(*this)))return false;

  GEOM_SKELCOMPUTER_UPDATE_CACHE(scp);
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);
  real_t _r = sphere->getRadius();
  __skeleton = PolylinePtr(new Polyline(Vector3(0,0,-_r),Vector3(0,0,_r)));
  return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);

  GEOM_SKELCOMPUTER_CHECK_CACHE(tapered);

  if(!(tapered->getPrimitive()->apply(*this)))return false;
  if (__skeleton) {
    __skeleton = dynamic_pointer_cast<Polyline>(__skeleton->transform(tapered->getTransformation()));
  }
  else return false;

  GEOM_SKELCOMPUTER_UPDATE_CACHE(tapered);
  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( Translated * translated ) {
  GEOM_ASSERT(translated);

  GEOM_SKELCOMPUTER_CHECK_CACHE(translated);

  if(!(translated->getGeometry()->apply(*this)))return false;
  if (__skeleton) {
    __skeleton = dynamic_pointer_cast<Polyline>(__skeleton->transform(translated->getTransformation()));
  }
  else return false;

  GEOM_SKELCOMPUTER_UPDATE_CACHE(translated);
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( TriangleSet * triangleSet )  {
  GEOM_ASSERT(triangleSet);
  __skeleton = triangleSet->getSkeleton();
  if(__skeleton) return true;
  else return false;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( IFS * ifs ) {
  GEOM_ASSERT(ifs);

  GEOM_SKELCOMPUTER_CHECK_CACHE(ifs);

  if(!(ifs->getGeometry()->apply(*this)))return false;

  if( __skeleton )
    {
    GEOM_SKELCOMPUTER_UPDATE_CACHE(ifs);
    }
  else
    return false;

  return true;
}

/* ----------------------------------------------------------------------- */



bool SkelComputer::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

    if(bezierCurve->apply(__discretizer))
      __skeleton = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
    else
    __skeleton = PolylinePtr(new Polyline(
      Vector3(bezierCurve->getPointAt(0),0),
      Vector3(bezierCurve->getPointAt(1),0)));

  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Disc * disc ) {
    GEOM_ASSERT(disc);
    __skeleton = PolylinePtr(new Polyline(Vector3::ORIGIN,Vector3::ORIGIN));
    return true;
  }

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

    if(nurbsCurve->apply(__discretizer))
      __skeleton = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
    else
      __skeleton = PolylinePtr(new Polyline(
      Vector3(nurbsCurve->getPointAt(nurbsCurve->getFirstKnot()),0),
      Vector3(nurbsCurve->getPointAt(nurbsCurve->getLastKnot()),0)));

  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( PointSet2D * pointSet ){
  GEOM_ASSERT(pointSet);
  __skeleton = PolylinePtr();
  return false;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Polyline2D * polyline ){
  GEOM_ASSERT(polyline);
  __skeleton = PolylinePtr(new Polyline(Point3ArrayPtr(new Point3Array(*(polyline->getPointList()),0))));
  return true;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Material * material ) {
  GEOM_ASSERT(material);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

bool SkelComputer::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

bool SkelComputer::process( Texture2D * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

bool SkelComputer::process( Texture2DTransformation * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

bool SkelComputer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

bool SkelComputer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

/* ----------------------------------------------------------------------- */

bool SkelComputer::process( Text * text ) {
  GEOM_ASSERT(text);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

bool SkelComputer::process( Font * font ) {
  GEOM_ASSERT(font);
  // nothing to do
  __skeleton = PolylinePtr();
  return false;
}

/* ----------------------------------------------------------------------- */
