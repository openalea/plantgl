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





#include "orthotransformed.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */
OrthonormalBasis3D::Builder::Builder( ){
}

OrthonormalBasis3D::Builder::~Builder( ){
}

/* ----------------------------------------------------------------------- */

OrthonormalBasis3D::OrthonormalBasis3D( const Matrix3& matrix ) :
  Matrix4Transformation(),
  __matrix(matrix) {
  GEOM_ASSERT(matrix.isOrthogonal());
  GEOM_ASSERT(isValid());
}

OrthonormalBasis3D::~OrthonormalBasis3D( ) {
}

Matrix3
OrthonormalBasis3D::getMatrix3( ) const {
  return __matrix;
}

Matrix4
OrthonormalBasis3D::getMatrix( ) const {
  return Matrix4(__matrix);
}

bool OrthonormalBasis3D::isValid( ) const {
  return __matrix.isOrthogonal();
}

Vector3 OrthonormalBasis3D::extractEulerAngles() const {
  return __matrix.eulerAnglesZYX();
}

Point3ArrayPtr OrthonormalBasis3D::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->getSize()));
  Point3Array::iterator _ti = _tPoints->getBegin();
  for (Point3Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point4ArrayPtr OrthonormalBasis3D::transform( const Point4ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point4ArrayPtr _tPoints(new Point4Array(points->getSize()));
  Point4Array::iterator _ti = _tPoints->getBegin();
  for (Point4Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector4(__matrix * Vector3(_i->x(),_i->y(),_i->z()),_i->w());
  return _tPoints;
}


Point3MatrixPtr OrthonormalBasis3D::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->getSize()));
  Point3Matrix::iterator _ti = _tPoints->getBegin();
  for (Point3Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point4MatrixPtr OrthonormalBasis3D::transform( const Point4MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point4MatrixPtr _tPoints(new Point4Matrix(points->getSize()));
  Point4Matrix::iterator _ti = _tPoints->getBegin();
  for (Point4Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector4(__matrix * Vector3(_i->x(),_i->y(),_i->z()),_i->w());
  return _tPoints;
}


/* ----------------------------------------------------------------------- */


OrthoTransformed::Builder::Builder( ) :
  MatrixTransformed::Builder() {
}


OrthoTransformed::Builder::~Builder( ) {
}


/* ----------------------------------------------------------------------- */
OrthoTransformed::OrthoTransformed() :
    MatrixTransformed() {
}

OrthoTransformed::OrthoTransformed( const GeometryPtr& geometry ) :
    MatrixTransformed(geometry) {
}

OrthoTransformed::~OrthoTransformed( ) {
}

/* ----------------------------------------------------------------------- */

OrthonormalBasis2D::OrthonormalBasis2D( const Matrix2& matrix ) :
  Matrix3Transformation(),
  __matrix(matrix) {
  GEOM_ASSERT(matrix.isOrthogonal());
  GEOM_ASSERT(isValid());
}

OrthonormalBasis2D::~OrthonormalBasis2D( ) {
}

Matrix3
OrthonormalBasis2D::getMatrix( ) const {
  return Matrix3(__matrix);
}

bool OrthonormalBasis2D::isValid( ) const {
  return __matrix.isOrthogonal();
}


Point2ArrayPtr OrthonormalBasis2D::transform( const Point2ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point2ArrayPtr _tPoints(new Point2Array(points->getSize()));
  Point2Array::iterator _ti = _tPoints->getBegin();
  for (Point2Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point3ArrayPtr OrthonormalBasis2D::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->getSize()));
  Point3Array::iterator _ti = _tPoints->getBegin();
  for (Point3Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector3(__matrix * Vector2(_i->x(),_i->y()),_i->z());
  return _tPoints;
}


Point2MatrixPtr OrthonormalBasis2D::transform( const Point2MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point2MatrixPtr _tPoints(new Point2Matrix(points->getSize()));
  Point2Matrix::iterator _ti = _tPoints->getBegin();
  for (Point2Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point3MatrixPtr OrthonormalBasis2D::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->getSize()));
  Point3Matrix::iterator _ti = _tPoints->getBegin();
  for (Point3Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = Vector3(__matrix * Vector2(_i->x(),_i->y()),_i->z());
  return _tPoints;
}


/* ----------------------------------------------------------------------- */


