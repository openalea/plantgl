/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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




#include "skelcomputer.h"
#include "discretizer.h"
#include <pgl_geometry.h>
#include <pgl_transformation.h>
#include <scenegraph/container/pointarray.h>
#include <scenegraph/container/geometryarray2.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_SKELCOMPUTER_CHECK_CACHE(geom) \
  if (geom->isNamed()) { \
    Cache<PolylinePtr>::Iterator _it = __cache.find(geom->getId()); \
    if (! (_it == __cache.end())) { \
       __skeleton = _it->second; \
      return true; \
    }; \
  };


#define GEOM_SKELCOMPUTER_UPDATE_CACHE(geom) \
  if (geom->isNamed()) \
     __cache.insert(geom->getId(),__skeleton);


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
      __skeleton.cast(__skeleton->transform(axisRotated->getTransformation()));
  }
  else return false;

  GEOM_SKELCOMPUTER_UPDATE_CACHE(axisRotated);
  return true;
}


/* ----------------------------------------------------------------------- */


bool SkelComputer::process( BezierCurve * bezierCurve ) {
    GEOM_ASSERT(bezierCurve);
	if(bezierCurve->apply(__discretizer))
	  __skeleton = PolylinePtr::Cast(__discretizer.getDiscretization());
	else
    __skeleton = new Polyline(
        bezierCurve->getPointAt(0),
        bezierCurve->getPointAt(1));

    return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( BezierPatch * bezierPatch ) {
    GEOM_ASSERT(bezierPatch);

    __skeleton = new Polyline(bezierPatch->getPointAt(0.0,0.0),
                              bezierPatch->getPointAt(1.0,1.0));
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
    __skeleton.cast(__skeleton->transform(eulerRotated->getTransformation()));
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
	__skeleton = PolylinePtr::Cast(__discretizer.getDiscretization());
  else
	__skeleton = new Polyline(
	nurbsCurve->getPointAt(nurbsCurve->getFirstKnot()),
	nurbsCurve->getPointAt(nurbsCurve->getLastKnot()));
  
  return true;
}

/* ----------------------------------------------------------------------- */


bool SkelComputer::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  __skeleton = new Polyline
     (nurbsPatch->getPointAt(nurbsPatch->getFirstUKnot(),
                             nurbsPatch->getFirstVKnot()),
      nurbsPatch->getPointAt(nurbsPatch->getLastUKnot(),
                             nurbsPatch->getLastVKnot()));
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
  uint32_t size= pList->getSize();

  Vector2 ptStart, ptEnd;
  uint32_t i= 0;
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
    __skeleton.cast(__skeleton->transform(oriented->getTransformation()));
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
    __skeleton.cast(__skeleton->transform(scaled->getTransformation()));
  };

  GEOM_SKELCOMPUTER_UPDATE_CACHE(scaled);
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
    __skeleton.cast(__skeleton->transform(tapered->getTransformation()));
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
    __skeleton.cast(__skeleton->transform(translated->getTransformation()));
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
	  __skeleton = PolylinePtr::Cast(__discretizer.getDiscretization());
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
	  __skeleton = PolylinePtr::Cast(__discretizer.getDiscretization());
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
