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





#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/math/util_polymath.h>
#include <plantgl/math/util_math.h>

#include "rayintersection.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

#define GEOM_DISCRETIZE( obj ) \
  GEOM_ASSERT( obj ); \
  if(!obj->apply(__discretizer)) \
    return false; \
  else return __discretizer.getDiscretization()->apply(*this); \

/* ----------------------------------------------------------------------- */


RayIntersection::RayIntersection(Discretizer& discretizer) :
  Action(),
  __discretizer(discretizer)
{
}

RayIntersection::~RayIntersection()
{
}

void
RayIntersection::setRay(const Ray& ray){
  __ray = ray;
}

Point3ArrayPtr
RayIntersection::getIntersection() {
  return __result;
}

/* ----------------------------------------------------------------------- */


bool
RayIntersection::beginProcess()
{
  return true;
}

bool
RayIntersection::endProcess()
{
  return true;
}


/* ----------------------------------------------------------------------- */



bool
RayIntersection::process( Inline * geominline )
{
  GEOM_ASSERT( geominline );
#ifdef __GNUC__
#pragma message "RayIntersection with Inline not yet implemented"
#endif
  return false;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Shape * geomShape )
{
  GEOM_ASSERT( geomShape );
  return geomShape->getGeometry()->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Material * material )
{
  GEOM_ASSERT( material );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( MonoSpectral * monoSpectral )
{
  GEOM_ASSERT( monoSpectral );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( MultiSpectral * multiSpectral )
{
  GEOM_ASSERT( multiSpectral );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( AmapSymbol * amapSymbol )
{
  GEOM_ASSERT( amapSymbol );
  __result = Point3ArrayPtr(new Point3Array());
  Vector3 intersect;
  Point3ArrayPtr points(amapSymbol->getPointList());

  for(IndexArray::iterator _it = amapSymbol->getIndexList()->begin();
      _it != amapSymbol->getIndexList()->end();
      _it++){
    if( _it->size() == 4 &&
        __ray.intersect( points->getAt(_it->getAt(0)),
                         points->getAt(_it->getAt(1)),
                         points->getAt(_it->getAt(2)),
                         points->getAt(_it->getAt(3)),
                         intersect) == 1){
      bool contained = false;
      for(Point3Array::const_iterator _i = __result->begin(); _i != __result->end() && !contained ; _i++){
        if( fabs(_i->x() - intersect.x()) < GEOM_EPSILON &&
            fabs(_i->y() - intersect.y()) < GEOM_EPSILON &&
            fabs(_i->z() - intersect.z()) < GEOM_EPSILON )contained = true;
      }
      if(!contained){
        __result->push_back(intersect);
      }
    }
    else if( _it->size() == 3 &&
             __ray.intersect( points->getAt(_it->getAt(0)),
                              points->getAt(_it->getAt(1)),
                              points->getAt(_it->getAt(2)),
                              intersect) == 1){
      bool contained = false;
      for(Point3Array::const_iterator _i = __result->begin(); _i != __result->end() && !contained ; _i++){
        if( fabs(_i->x() - intersect.x()) < GEOM_EPSILON &&
            fabs(_i->y() - intersect.y()) < GEOM_EPSILON &&
            fabs(_i->z() - intersect.z()) < GEOM_EPSILON )contained = true;
      }
      if(!contained){
        __result->push_back(intersect);
      }
    }
  }
  if(__result->empty())
    return false;
  else return true;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( AsymmetricHull * asymmetricHull )
{
  GEOM_DISCRETIZE( asymmetricHull );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( AxisRotated * axisRotated )
{
  GEOM_DISCRETIZE( axisRotated );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( BezierCurve * bezierCurve )
{
  GEOM_DISCRETIZE( bezierCurve );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( BezierPatch * bezierPatch )
{
  GEOM_DISCRETIZE( bezierPatch );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Box * box )
{
  GEOM_DISCRETIZE( box );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Cone * cone )
{
  GEOM_DISCRETIZE( cone );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Cylinder * cylinder )
{
  GEOM_DISCRETIZE( cylinder );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( ElevationGrid * elevationGrid )
{
  GEOM_DISCRETIZE( elevationGrid );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( EulerRotated * eulerRotated )
{
  GEOM_DISCRETIZE( eulerRotated );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( ExtrudedHull * extrudedHull )
{
  GEOM_DISCRETIZE( extrudedHull );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Extrusion * extrusion )
{
  GEOM_DISCRETIZE( extrusion );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( FaceSet * faceSet )
{
  GEOM_ASSERT( faceSet );
  __result = Point3ArrayPtr(new Point3Array());
  Vector3 intersect;
  Point3ArrayPtr points(faceSet->getPointList());

  for(IndexArray::iterator _it = faceSet->getIndexList()->begin();
      _it != faceSet->getIndexList()->end();
      _it++){
    if( _it->size() == 4 &&
        __ray.intersect( points->getAt(_it->getAt(0)),
                         points->getAt(_it->getAt(1)),
                         points->getAt(_it->getAt(2)),
                         points->getAt(_it->getAt(3)),
                         intersect) == 1){
      bool contained = false;
      for(Point3Array::const_iterator _i = __result->begin(); _i != __result->end() && !contained ; _i++){
        if( fabs(_i->x() - intersect.x()) < GEOM_EPSILON &&
            fabs(_i->y() - intersect.y()) < GEOM_EPSILON &&
            fabs(_i->z() - intersect.z()) < GEOM_EPSILON )contained = true;
      }
      if(!contained){
        __result->push_back(intersect);
      }
    }
    else if( _it->size() == 3 &&
             __ray.intersect( points->getAt(_it->getAt(0)),
                              points->getAt(_it->getAt(1)),
                              points->getAt(_it->getAt(2)),
                              intersect) == 1){
      bool contained = false;
      for(Point3Array::const_iterator _i = __result->begin(); _i != __result->end() && !contained ; _i++){
        if( fabs(_i->x() - intersect.x()) < GEOM_EPSILON &&
            fabs(_i->y() - intersect.y()) < GEOM_EPSILON &&
            fabs(_i->z() - intersect.z()) < GEOM_EPSILON )contained = true;
      }
      if(!contained){
        __result->push_back(intersect);
      }
    }
  }
  if(__result->empty())
    return false;
  else return true;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Frustum * frustum )
{
  GEOM_DISCRETIZE( frustum );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Group * group )
{
  GEOM_DISCRETIZE( group );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( IFS * ifs )
{
  GEOM_DISCRETIZE( ifs );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( NurbsCurve * nurbsCurve )
{
  GEOM_DISCRETIZE( nurbsCurve );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( NurbsPatch * nurbsPatch )
{
  GEOM_DISCRETIZE( nurbsPatch );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Oriented * oriented )
{
  GEOM_DISCRETIZE( oriented );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Paraboloid * paraboloid )
{
  GEOM_DISCRETIZE( paraboloid );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( PointSet * pointSet )
{
  GEOM_ASSERT( pointSet );
  __result = Point3ArrayPtr(new Point3Array());
  Point3ArrayPtr points(pointSet->getPointList());
  for(Point3Array::const_iterator _it = points->begin(); _it != points->end(); _it++)
    if(__ray.intersect(*_it))
      __result->push_back(*_it);
  return (!__result->empty());
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Polyline * polyline )
{
  GEOM_ASSERT( polyline );
  Vector3 intersect;
  __result = Point3ArrayPtr(new Point3Array());
  Point3ArrayPtr points(polyline->getPointList());
  for(Point3Array::const_iterator _it = points->begin(); _it != (points->end()-1); _it++)
    if(__ray.intersect(*_it,*(_it+1),intersect) == 1)
      __result->push_back(intersect);
  return (!__result->empty());
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Revolution * revolution )
{
  GEOM_DISCRETIZE( revolution );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( QuadSet * quadSet )
{
  GEOM_ASSERT( quadSet );
  __result = Point3ArrayPtr(new Point3Array());
  Vector3 intersect;
  Point3ArrayPtr points(quadSet->getPointList());

  for(Index4Array::iterator _it = quadSet->getIndexList()->begin();
      _it != quadSet->getIndexList()->end();
      _it++){
    if(__ray.intersect( points->getAt(_it->getAt(0)),
                        points->getAt(_it->getAt(1)),
                        points->getAt(_it->getAt(2)),
                        points->getAt(_it->getAt(3)),
                        intersect) == 1){
      bool contained = false;
      for(Point3Array::const_iterator _i = __result->begin(); _i != __result->end() && !contained ; _i++){
        if( fabs(_i->x() - intersect.x()) < GEOM_EPSILON &&
            fabs(_i->y() - intersect.y()) < GEOM_EPSILON &&
            fabs(_i->z() - intersect.z()) < GEOM_EPSILON )contained = true;
      }
      if(!contained){
        __result->push_back(intersect);
      }
    }
  }
  if(__result->empty())
    return false;
  else return true;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Scaled * scaled )
{
  GEOM_DISCRETIZE( scaled );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Sphere * sphere )
{
  GEOM_ASSERT( sphere );
  Vector3 i,j;
  int res = 0;
  if((res = __ray.intersect(Vector3::ORIGIN,sphere->getRadius(),i,j)) == 0) return false;
  __result = Point3ArrayPtr(new Point3Array(res));
  __result->setAt(0,i);
  if( res == 2 ) __result->setAt(1,j);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Swung * swung )
{
  GEOM_DISCRETIZE( swung );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Tapered * tapered )
{
  GEOM_DISCRETIZE( tapered );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Translated * translated )
{
  GEOM_DISCRETIZE( translated );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( TriangleSet * triangleSet )
{
  GEOM_ASSERT( triangleSet );
  __result = Point3ArrayPtr(new Point3Array());
  Vector3 intersect;
  Point3ArrayPtr points(triangleSet->getPointList());

  for(Index3Array::iterator _it = triangleSet->getIndexList()->begin();
      _it != triangleSet->getIndexList()->end();
      _it++){
    if(__ray.intersect( points->getAt(_it->getAt(0)),
                        points->getAt(_it->getAt(1)),
                        points->getAt(_it->getAt(2)),
                        intersect) == 1){
      bool contained = false;
      for(Point3Array::const_iterator _i = __result->begin(); _i != __result->end() && !contained ; _i++){
        if( fabs(_i->x() - intersect.x()) < GEOM_EPSILON &&
            fabs(_i->y() - intersect.y()) < GEOM_EPSILON &&
            fabs(_i->z() - intersect.z()) < GEOM_EPSILON )contained = true;
      }
      if(!contained){
        __result->push_back(intersect);
      }
    }
  }
  if(__result->empty())
    return false;
  else return true;
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( BezierCurve2D * bezierCurve )
{
  GEOM_DISCRETIZE( bezierCurve );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Disc * disc )
{
  GEOM_DISCRETIZE( disc );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( NurbsCurve2D * nurbsCurve )
{
  GEOM_DISCRETIZE( nurbsCurve );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( PointSet2D * pointSet )
{
  GEOM_DISCRETIZE( pointSet );
}


/* ----------------------------------------------------------------------- */


bool
RayIntersection::process( Polyline2D * polyline )
{
  GEOM_DISCRETIZE( polyline );
}


/* ----------------------------------------------------------------------- */

