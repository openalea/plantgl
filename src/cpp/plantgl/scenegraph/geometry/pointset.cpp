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



#include "pointset.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/transformation/transformed.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

const uchar_t PointSet::DEFAULT_WIDTH = 1;

/* ----------------------------------------------------------------------- */


PointSet::Builder::Builder( ) :
  ExplicitModel::Builder(),
  Width(0){
}


PointSet::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr PointSet::Builder::build( ) const {
  if (isValid())
	  return SceneObjectPtr(new PointSet(*PointList,ColorList?*ColorList:Color4ArrayPtr(),Width?*Width:DEFAULT_WIDTH));
  // Returns null as self is not valid.
  return SceneObjectPtr();
}


void PointSet::Builder::destroy( ) {
  EMDestroy();
  if (Width) delete Width;
}


bool PointSet::Builder::isValid( ) const {
	if(!EMValid()) return false;

	if (ColorList && *ColorList) {
		uint_t _colorListSize = (*ColorList)->size();
		if(_colorListSize != (*PointList)->size()){
			pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"PointSet","ColorList","Number of colors must be compatible to PointList size.");
			return false;
		}
	}
	return true;

}


/* ----------------------------------------------------------------------- */


PointSet::PointSet( ) :
  ExplicitModel(),__width(DEFAULT_WIDTH){
}

PointSet::PointSet( const Point3ArrayPtr& points, const Color4ArrayPtr& colors, uchar_t width ) :
  ExplicitModel(points,colors),__width(width){
  GEOM_ASSERT(isValid());
}

PointSet::~PointSet( ) {
}

bool PointSet::isValid( ) const {
  Builder _builder;
  if(__pointList)_builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  return _builder.isValid();
}

bool  PointSet::isACurve( ) const {
  return false;
}

bool  PointSet::isASurface( ) const {
  return false;
}

bool  PointSet::isAVolume( ) const {
  return false;
}


const Vector3& 
PointSet::getPointListAt( uint_t i ) const {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);   
}

Vector3& 
PointSet::getPointListAt( uint_t i ) {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);   
}


ExplicitModelPtr 
PointSet::transform( const Transformation3DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModelPtr(new PointSet(transformation->transform(__pointList),__colorList));
}

SceneObjectPtr 
PointSet::copy(DeepCopier& copier) const 
{
  PointSet * ptr = new PointSet(*this);
  copier.copy_attribute(ptr->getPointList());
  copier.copy_attribute(ptr->getColorList());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */



PointSet2D::Builder::Builder( ) :
  PlanarModel::Builder(),
  PointList(0),
  Width(0){
}


PointSet2D::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr PointSet2D::Builder::build( ) const {
  if (isValid())
	  return SceneObjectPtr(new PointSet2D(*PointList,Width?*Width:PointSet::DEFAULT_WIDTH));
  // Returns null as self is not valid.
  return SceneObjectPtr();
}


void PointSet2D::Builder::destroy( ) {
  if (PointList) delete PointList;
  if (Width) delete Width;
}


bool PointSet2D::Builder::isValid( ) const {

  // PointList
  if (! PointList) { 
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"PointSet2D","PointList");
    return false;
  };
  if ((*PointList)->size() < 1) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"PointSet2D","PointList","Number of points must be greater than 0.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


PointSet2D::PointSet2D( const Point2ArrayPtr& points, uchar_t width ) :
  PlanarModel(),
  __pointList(points), __width(width) {
  GEOM_ASSERT(isValid());
}

PointSet2D::PointSet2D(  ) :
  PlanarModel(),
  __pointList(),
  __width(PointSet::DEFAULT_WIDTH) {
}

PointSet2D::~PointSet2D( ) {
}

bool PointSet2D::isValid( ) const {
  Builder _builder;
  _builder.PointList = const_cast<Point2ArrayPtr *>(&__pointList);
  return _builder.isValid();
}

bool PointSet2D::isACurve( ) const {
  return false;
}

bool PointSet2D::isASurface( ) const {
  return false;
}

bool PointSet2D::isAVolume( ) const {
  return false;
}

SceneObjectPtr 
PointSet2D::copy(DeepCopier& copier) const 
{
  PointSet2D * ptr = new PointSet2D(*this);
  copier.copy_attribute(ptr->getPointList());
  return SceneObjectPtr(ptr);
}

const Vector2& 
PointSet2D::getPointListAt( uint_t i ) const {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);   
}

Vector2& 
PointSet2D::getPointListAt( uint_t i ) {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);   
}

const Point2ArrayPtr& 
PointSet2D::getPointList( ) const {
  return __pointList;
}

Point2ArrayPtr& 
PointSet2D::getPointList( ) {
  return __pointList;
}

uint_t 
PointSet2D::getPointListSize( ) const {
  return __pointList->size();
}

/*
ExplicitModel2DPtr 
PointSet2D::transform( const Transformation2DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModel2DPtr(new Point2DSet(transformation->transform(__pointList)));
}
*/

/* ----------------------------------------------------------------------- */
