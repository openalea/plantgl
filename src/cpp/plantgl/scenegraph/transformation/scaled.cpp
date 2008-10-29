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



#include "scaled.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/*----------------------------------------------------------------------- */

Scaling::Scaling( const Vector3& factors ) :
  Matrix4Transformation(),
  __factors(factors) {
  GEOM_ASSERT(isValid());
}

Scaling::~Scaling( ) {
}

Matrix4 Scaling::getMatrix( ) const {
  return Matrix4( Matrix3::scaling(__factors) );
}

const Vector3&
Scaling::getFactors( ) const {
  return __factors;
}

bool Scaling::isValid( ) const {
  return
    (__factors.isValid()) &&
    (! (__factors == Vector3::ORIGIN));
}


Point3ArrayPtr Scaling::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->getSize()));
  Point3Array::iterator _ti = _tPoints->getBegin();
  for (Point3Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
       _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z() * __factors.z());
  return _tPoints;
}


Point4ArrayPtr Scaling::transform( const Point4ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point4ArrayPtr _tPoints(new Point4Array(points->getSize()));
  Point4Array::iterator _ti = _tPoints->getBegin();
  for (Point4Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector4(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z() * __factors.z(),
                     _i->w());
  return _tPoints;
}


Point3MatrixPtr Scaling::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->getSize()));
  Point3Matrix::iterator _ti = _tPoints->getBegin();
  for (Point3Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z() * __factors.z());
  return _tPoints;
}


Point4MatrixPtr Scaling::transform( const Point4MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point4MatrixPtr _tPoints(new Point4Matrix(points->getSize()));
  Point4Matrix::iterator _ti = _tPoints->getBegin();
  for (Point4Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector4(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z() * __factors.z(),
                     _i->w());
  return _tPoints;
}


/*----------------------------------------------------------------------- */


const Vector3 Scaled::DEFAULT_SCALE(1,1,1);


/* ----------------------------------------------------------------------- */


Scaled::Builder::Builder( ) :
  MatrixTransformed::Builder(),
  Scale(0) {
}


Scaled::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Scaled::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr(new Scaled(Scale ? *Scale : DEFAULT_SCALE,
                                                   *Geometry));
  return SceneObjectPtr();
}


void Scaled::Builder::destroy( ) {
  MTDestroy();
  if (Scale) delete Scale;
}


bool Scaled::Builder::isValid( ) const {
  if (! MTValid()) return false;

  if (Scale &&
      (*Scale == Vector3::ORIGIN)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Scaled","Scale","Couldn't scale with factors <0,0,0>.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */

Scaled::Scaled() :
    MatrixTransformed(),
    __scale(DEFAULT_SCALE) {
}

Scaled::Scaled( const Vector3& scale,
                const GeometryPtr& geometry ) :
  MatrixTransformed(geometry),
  __scale(scale) {
  GEOM_ASSERT(isValid());
}

Scaled::~Scaled( ) {
}

bool Scaled::apply( Action& action ) {
  return action.process(this);
}

bool Scaled::isValid( ) const {
  Builder _builder;
  _builder.Geometry = const_cast<GeometryPtr *>(&__geometry);
  _builder.Scale = const_cast<Vector3 *>(&__scale);
  return (_builder.isValid() && __geometry->isValid());
}

SceneObjectPtr Scaled::copy() const {
  Scaled * ptr = new Scaled(*this);
  if(__geometry)ptr->getGeometry() = dynamic_pointer_cast<Geometry>(__geometry->copy());
  return SceneObjectPtr(ptr);
}

const Vector3&
Scaled::getScale( ) const {
  return __scale;
}

Vector3&
Scaled::getScale( ) {
  return __scale;
}

Transformation3DPtr
Scaled::getTransformation( ) const {
  return Transformation3DPtr(new Scaling(__scale));
}

bool
Scaled::isScaleToDefault( ) const {
  return __scale == DEFAULT_SCALE;
}

/* ----------------------------------------------------------------------- */
Scaling2D::Scaling2D( const Vector2& factors ) :
  Matrix3Transformation(),
  __factors(factors) {
  GEOM_ASSERT(isValid());
}

Scaling2D::~Scaling2D( ) {
}

Matrix3 Scaling2D::getMatrix( ) const {
  return Matrix3::scaling(Vector3(__factors.x(),__factors.y(),1.));
}

const Vector2&
Scaling2D::getFactors( ) const {
  return __factors;
}

bool Scaling2D::isValid( ) const {
  return
    (__factors.isValid()) &&
    (! (__factors == Vector2::ORIGIN));
}


Point2ArrayPtr Scaling2D::transform( const Point2ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point2ArrayPtr _tPoints(new Point2Array(points->getSize()));
  Point2Array::iterator _ti = _tPoints->getBegin();
  for (Point2Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
       _i++)
    *_ti++ = Vector2(_i->x() * __factors.x(),
                     _i->y() * __factors.y());
  return _tPoints;
}


Point3ArrayPtr Scaling2D::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->getSize()));
  Point3Array::iterator _ti = _tPoints->getBegin();
  for (Point3Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z());
  return _tPoints;
}


Point2MatrixPtr Scaling2D::transform( const Point2MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point2MatrixPtr _tPoints(new Point2Matrix(points->getSize()));
  Point2Matrix::iterator _ti = _tPoints->getBegin();
  for (Point2Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector2(_i->x() * __factors.x(),
                     _i->y() * __factors.y());
  return _tPoints;
}


Point3MatrixPtr Scaling2D::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->getSize()));
  Point3Matrix::iterator _ti = _tPoints->getBegin();
  for (Point3Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z());
  return _tPoints;
}


/*----------------------------------------------------------------------- */
