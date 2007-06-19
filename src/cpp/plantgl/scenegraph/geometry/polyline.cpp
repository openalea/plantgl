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





#include "polyline.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Polyline::Builder::Builder( ) :
  ExplicitModel::Builder(){
}


Polyline::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Polyline::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr(new Polyline(*PointList,ColorList?*ColorList:NULL));
  return SceneObjectPtr();
}


void Polyline::Builder::destroy( ) {
  EMDestroy();
}


bool Polyline::Builder::isValid( ) const {
	if(!EMValid()) return false;

	if (ColorList && *ColorList) {
		uint32_t _colorListSize = (*ColorList)->getSize();
		if(_colorListSize != (*PointList)->getSize()){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Polyline","ColorList","Number of colors must be compatible to PointList size.");
			return false;
		}
	}
	return true;
}



/* ----------------------------------------------------------------------- */

Polyline::Polyline( ) :
  ExplicitModel(){
}

Polyline::Polyline( const Vector3& point1, const Vector3& point2 ) :
  ExplicitModel(Point3ArrayPtr(new Point3Array(2))){
  __pointList->setAt(0,point1);
  __pointList->setAt(1,point2);
  GEOM_ASSERT(isValid());
}


Polyline::Polyline( const Point3ArrayPtr& points, const Color4ArrayPtr& colors ) :
  ExplicitModel(points, colors){
  GEOM_ASSERT(isValid());
}

Polyline::~Polyline( ){
}

bool Polyline::apply( Action& action ) {
  return action.process(this);
}

const Vector3& 
Polyline::getPointListAt( uint32_t i ) const {
  GEOM_ASSERT(i < __pointList->getSize());
  return __pointList->getAt(i);
}

Vector3& 
Polyline::getPointListAt( uint32_t i ) {
  GEOM_ASSERT(i < __pointList->getSize());
  return __pointList->getAt(i);
}

const real_t 
Polyline::getFirstKnot() const{
  return 0;
}

const real_t 
Polyline::getLastKnot() const{
  return (real_t)(__pointList->getSize()-1);
}

const uint32_t
Polyline::getStride() const{
    return (__pointList->getSize()-1);
}

Vector3 Polyline::getPointAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    real_t u1 = (int)u;
    if(u1 == u)return __pointList->getAt((uint32_t)u1);
    else return ((__pointList->getAt((uint32_t)u1) * ((u1+1)-u)))+(__pointList->getAt((uint32_t)(u1+1)) * (u-u1));
}

Vector3 Polyline::getTangentAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    real_t u1 = (int)u;
    if(u <= 0) return (__pointList->getAt(1)-__pointList->getAt(0));
    else if(u >= (__pointList->getSize()-1)) return (__pointList->getAt(__pointList->getSize()-1)-__pointList->getAt(__pointList->getSize()-2));
    else if(u1 == u){
        Vector3 _a = (__pointList->getAt((uint32_t)u1)-__pointList->getAt((uint32_t)(u1-1)));
        Vector3 _b = (__pointList->getAt((uint32_t)u1+1)-__pointList->getAt((uint32_t)u1));
        _a.normalize();
        _b.normalize();
        return (_a+_b);
    }
    else return (__pointList->getAt((uint32_t)(u1+1))-(__pointList->getAt((uint32_t)u1)));
}

Vector3 Polyline::getNormalAt(real_t u) const{
    GEOM_ASSERT( (getFirstKnot() -u ) < GEOM_EPSILON &&  !((u - getLastKnot()) > GEOM_EPSILON));
    Vector3 t = getTangentAt(u);
    if(fabs(t.x()) < GEOM_EPSILON && fabs(t.y()) < GEOM_EPSILON  )return cross(t,Vector3(1,0,0));
    else return cross(t,Vector3(0,0,1));
}

bool Polyline::isValid( ) const {
  Builder _builder;
  _builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  return _builder.isValid();
}


ExplicitModelPtr
Polyline::transform( const Transformation3DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModelPtr(new Polyline(transformation->transform(__pointList),__colorList));
}

SceneObjectPtr 
Polyline::copy() const 
{
  Polyline * ptr = new Polyline(*this);
  if(__pointList)ptr->getPointList() = Point3ArrayPtr(new Point3Array(*__pointList));
  if(__colorList)ptr->getColorList() = Color4ArrayPtr(new Color4Array(*__colorList));
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */


Polyline2D::Builder::Builder( ) :
  Curve2D::Builder(),
  PointList(0) {
}


Polyline2D::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Polyline2D::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr(new Polyline2D(*PointList));
  return SceneObjectPtr();
}


void Polyline2D::Builder::destroy( ) {
  if (PointList) delete PointList;
}


bool Polyline2D::Builder::isValid( ) const {

  // PointList
  if (! PointList) {
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Polyline2D","PointList");
    return false;
  };
  if ((*PointList)->getSize() < 1) {
    genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Polyline2D","PointList","Number of points must be greater than 1.");
    return false;
  };

  return true;
}



/* ----------------------------------------------------------------------- */

Polyline2D::Polyline2D( ) :
  Curve2D(),
  __pointList(0) {
}

Polyline2D::Polyline2D( const Vector2& point1, const Vector2& point2 ) :
  Curve2D(),
  __pointList(new Point2Array(2)) {
  __pointList->setAt(0,point1);
  __pointList->setAt(1,point2);
  GEOM_ASSERT(isValid());
}


Polyline2D::Polyline2D( const Point2ArrayPtr& points ) :
  Curve2D(),
  __pointList(points) {
  GEOM_ASSERT(isValid());
}

Polyline2D::~Polyline2D( ) {
}

bool Polyline2D::apply( Action& action ) {
  return action.process(this);
}

bool Polyline2D::isValid( ) const {
  Builder _builder;
  _builder.PointList = const_cast<Point2ArrayPtr *>(&__pointList);
  return _builder.isValid();
}

SceneObjectPtr 
Polyline2D::copy() const 
{
  Polyline2D *  ptr = new Polyline2D(*this);
  if(__pointList)ptr->getPointList() = Point2ArrayPtr(new Point2Array(*__pointList));
  return SceneObjectPtr(ptr);
}

const Vector2& 
Polyline2D::getPointListAt( uint32_t i ) const {
  GEOM_ASSERT(i < __pointList->getSize());
  return __pointList->getAt(i);
}

Vector2&
Polyline2D::getPointListAt( uint32_t i ) {
  GEOM_ASSERT(i < __pointList->getSize());
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

uint32_t 
Polyline2D::getPointListSize( ) const {
  return __pointList->getSize();
}

const real_t 
Polyline2D::getFirstKnot() const{
  return 0;
}

const real_t 
Polyline2D::getLastKnot() const{
  return (real_t)(__pointList->getSize()-1);
}

const uint32_t 
Polyline2D::getStride() const{
  return __pointList->getSize();
}

Vector2 
Polyline2D::getPointAt(real_t u) const{
  real_t u1 = (int)u;
  if(u1 == u)return __pointList->getAt((uint32_t)u1);
  else return ((__pointList->getAt((uint32_t)u1) * (u-u1))+(__pointList->getAt((uint32_t)(u1+1)) * ((u1+1)-u)));
}

/*
ExplicitModel2DPtr
Polyline2D::transform( const Transformation2DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModel2DPtr(new Polyline2D(transformation->transform(__pointList)));
}
*/

/* ----------------------------------------------------------------------- */

