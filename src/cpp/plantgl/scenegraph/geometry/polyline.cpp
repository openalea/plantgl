/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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




#include "polyline.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Polyline::Builder::Builder( ) :
  ExplicitModel::Builder(),
  Width(0){
}


Polyline::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Polyline::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr(new Polyline(*PointList,ColorList?*ColorList:Color4ArrayPtr(),Width?*Width:DEFAULT_WIDTH));
  return SceneObjectPtr();
}


void Polyline::Builder::destroy( ) {
  EMDestroy();
  if (Width) delete Width;
}


bool Polyline::Builder::isValid( ) const {
	if(!EMValid()) return false;

	if (ColorList && *ColorList) {
		uint_t _colorListSize = (*ColorList)->size();
		if(_colorListSize != (*PointList)->size()){
			pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Polyline","ColorList","Number of colors must be compatible to PointList size.");
			return false;
		}
	}
	return true;
}



/* ----------------------------------------------------------------------- */

Polyline::Polyline( uchar_t width ) :
  ExplicitModel(),
  LineicModel(width){
}

Polyline::Polyline( const Vector3& point1, const Vector3& point2, uchar_t width ) :
  ExplicitModel(Point3ArrayPtr(new Point3Array(2))),
  LineicModel(width){
  __pointList->setAt(0,point1);
  __pointList->setAt(1,point2);
  GEOM_ASSERT(isValid());
}


Polyline::Polyline( const Point3ArrayPtr& points, uchar_t width ) :
  ExplicitModel(points),
  LineicModel(width) {
  GEOM_ASSERT(isValid());
}

Polyline::Polyline( const Point3ArrayPtr& points, const Color4ArrayPtr& colors, uchar_t width ) :
  ExplicitModel(points, colors),
  LineicModel(width) {
  GEOM_ASSERT(isValid());
}

Polyline::~Polyline( ){
}

const Vector3& 
Polyline::getPointListAt( uint_t i ) const {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}

Vector3& 
Polyline::getPointListAt( uint_t i ) {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}

const real_t 
Polyline::getFirstKnot() const{
  return 0;
}

const real_t 
Polyline::getLastKnot() const{
  return (real_t)(__pointList->size()-1);
}

const uint_t
Polyline::getStride() const{
    return (__pointList->size()-1);
}

Vector3 Polyline::getPointAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    real_t u1 = (int)u;
    if(u1 == u)return __pointList->getAt((uint_t)u1);
    else return ((__pointList->getAt((uint_t)u1) * ((u1+1)-u)))+(__pointList->getAt((uint_t)(u1+1)) * (u-u1));
}

Vector3 Polyline::getTangentAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    real_t u1 = (int)u;
    if(u <= 0) return (__pointList->getAt(1)-__pointList->getAt(0));
    else if(u >= (__pointList->size()-1)) return (__pointList->getAt(__pointList->size()-1)-__pointList->getAt(__pointList->size()-2));
    else if(u1 == u){
        Vector3 _a = (__pointList->getAt((uint_t)u1)-__pointList->getAt((uint_t)(u1-1)));
        Vector3 _b = (__pointList->getAt((uint_t)u1+1)-__pointList->getAt((uint_t)u1));
        real_t la = _a.normalize();
        real_t lb = _b.normalize();
        return (_a+_b)*(la+lb)/2;
    }
    else return (__pointList->getAt((uint_t)(u1+1))-(__pointList->getAt((uint_t)u1)));
}

Vector3 Polyline::getNormalAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    Vector3 t = getTangentAt(u);
    if(fabs(t.x()) < GEOM_EPSILON && fabs(t.y()) < GEOM_EPSILON  )return cross(t,Vector3(1,0,0));
    else return cross(t,Vector3(0,0,1));
}

bool Polyline::isValid( ) const {
  Builder _builder;
  if(__pointList)_builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  return _builder.isValid();
}


ExplicitModelPtr
Polyline::transform( const Transformation3DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModelPtr(new Polyline(transformation->transform(__pointList),__colorList));
}

SceneObjectPtr 
Polyline::copy(DeepCopier& copier) const 
{  
  Polyline * ptr = new Polyline(*this);
  copier.copy_attribute(ptr->getPointList());
  copier.copy_attribute(ptr->getColorList());
  // if(__pointList)ptr->getPointList() = Point3ArrayPtr(new Point3Array(*__pointList));
  // if(__colorList)ptr->getColorList() = Color4ArrayPtr(new Color4Array(*__colorList));
  return SceneObjectPtr(ptr);
}

std::pair<PolylinePtr,PolylinePtr> Polyline::split(real_t u) const {
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    int u_index = int(u);
    std::pair<PolylinePtr,PolylinePtr> result;
    result.first = PolylinePtr(new Polyline(Point3ArrayPtr(new Point3Array(__pointList->begin(),__pointList->begin()+u_index+1))));
    result.second = PolylinePtr(new Polyline(Point3ArrayPtr(new Point3Array(__pointList->begin()+u_index+1,__pointList->end()))));
    Vector3 mid_point = getPointAt(u);
    result.first->getPointList()->push_back(mid_point);
    result.second->getPointList()->insert(result.second->getPointList()->begin(),mid_point);
    return result;
}

/* ----------------------------------------------------------------------- */

Polyline2DPtr Polyline2D::Circle(real_t radius, 
                                 uchar_t slices)
{
   Point2ArrayPtr pts = Point2ArrayPtr(new Point2Array(slices + 1));
   real_t angle_delta = GEOM_TWO_PI / slices;
   real_t angle = 0;
   pts->setAt(0,Vector2(radius,0));
   for (int i = 1; i < slices; ++i){
	 angle += angle_delta;
     pts->setAt(i,Vector2(radius*cos(angle),radius*sin(angle)));
   }
   pts->setAt(slices,Vector2(radius,0));
   return Polyline2DPtr(new Polyline2D(pts));
}

/* ----------------------------------------------------------------------- */

Polyline2D::Builder::Builder( ) :
  Curve2D::Builder(),
  PointList(0){
}


Polyline2D::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Polyline2D::Builder::build( ) const {
  if (isValid())
	  return SceneObjectPtr(new Polyline2D(*PointList,Width?*Width:DEFAULT_WIDTH));
  return SceneObjectPtr();
}


void Polyline2D::Builder::destroy( ) {
  cdestroy( );
  if (PointList) delete PointList;
}


bool Polyline2D::Builder::isValid( ) const {

  // PointList
  if (! PointList) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Polyline2D","PointList");
    return false;
  };
  if ((*PointList)->size() < 1) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Polyline2D","PointList","Number of points must be greater than 1.");
    return false;
  };

  return true;
}



/* ----------------------------------------------------------------------- */

Polyline2D::Polyline2D(uchar_t width) :
  Curve2D(width),
  __pointList() {
}

Polyline2D::Polyline2D( const Vector2& point1, const Vector2& point2, uchar_t width ) :
  Curve2D(width),
  __pointList(new Point2Array(2)) {
  __pointList->setAt(0,point1);
  __pointList->setAt(1,point2);
  GEOM_ASSERT(isValid());
}


Polyline2D::Polyline2D( const Point2ArrayPtr& points, uchar_t width ) :
  Curve2D(width),
  __pointList(points) {
  GEOM_ASSERT(isValid());
}

Polyline2D::~Polyline2D( ) {
}

bool Polyline2D::isValid( ) const {
  Builder _builder;
  if(__pointList)_builder.PointList = const_cast<Point2ArrayPtr *>(&__pointList);
  return _builder.isValid();
}

SceneObjectPtr 
Polyline2D::copy(DeepCopier& copier) const 
{
  Polyline2D *  ptr = new Polyline2D(*this);
  copier.copy_attribute(ptr->getPointList());
  return SceneObjectPtr(ptr);
}

const Vector2& 
Polyline2D::getPointListAt( uint_t i ) const {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}

Vector2&
Polyline2D::getPointListAt( uint_t i ) {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}

const Point2ArrayPtr& 
Polyline2D::getPointList( ) const {
  return __pointList;
}

Point2ArrayPtr& 
Polyline2D::getPointList( ){
  return __pointList;
}

uint_t 
Polyline2D::getPointListSize( ) const {
  return __pointList->size();
}

const real_t 
Polyline2D::getFirstKnot() const{
  return 0;
}

const real_t 
Polyline2D::getLastKnot() const{
  return (real_t)(__pointList->size()-1);
}

const uint_t 
Polyline2D::getStride() const{
    return (__pointList->size()-1);
}

Vector2 
Polyline2D::getPointAt(real_t u) const{
  GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
  real_t u1 = (int)u;
  if(u1 == u)return __pointList->getAt((uint_t)u1);
    else return ((__pointList->getAt((uint_t)u1) * ((u1+1)-u)))+(__pointList->getAt((uint_t)(u1+1)) * (u-u1));
}

Vector2 Polyline2D::getTangentAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    real_t u1 = (int)u;
    if(u <= 0) return (__pointList->getAt(1)-__pointList->getAt(0));
    else if(u >= (__pointList->size()-1)) return (__pointList->getAt(__pointList->size()-1)-__pointList->getAt(__pointList->size()-2));
    else if(u1 == u){
        Vector2 _a = (__pointList->getAt((uint_t)u1)-__pointList->getAt((uint_t)(u1-1)));
        Vector2 _b = (__pointList->getAt((uint_t)u1+1)-__pointList->getAt((uint_t)u1));
        real_t la = _a.normalize();
        real_t lb = _b.normalize();
        return (_a+_b)*(la+lb)/2;
    }
    else return (__pointList->getAt((uint_t)(u1+1))-(__pointList->getAt((uint_t)u1)));
}

Vector2 Polyline2D::getNormalAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    Vector2 t  = getTangentAt(u);
    return Vector2(-t.y(),t.x());
}

/*
ExplicitModel2DPtr
Polyline2D::transform( const Transformation2DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModel2DPtr(new Polyline2D(transformation->transform(__pointList)));
}
*/

std::pair<Polyline2DPtr,Polyline2DPtr> Polyline2D::split(real_t u) const {
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    int u_index = int(u);
    std::pair<Polyline2DPtr,Polyline2DPtr> result;
    result.first = Polyline2DPtr(new Polyline2D(Point2ArrayPtr(new Point2Array(__pointList->begin(),__pointList->begin()+u_index))));
    result.second = Polyline2DPtr(new Polyline2D(Point2ArrayPtr(new Point2Array(__pointList->begin()+u_index+1,__pointList->end()))));
    Vector2 mid_point = getPointAt(u);
    result.first->getPointList()->push_back(mid_point);
    result.second->getPointList()->insert(result.second->getPointList()->begin(),mid_point);
    return result;
}

/* ----------------------------------------------------------------------- */

