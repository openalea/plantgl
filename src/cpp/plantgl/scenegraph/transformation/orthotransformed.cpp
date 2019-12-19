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




#include "orthotransformed.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

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
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));
  Point3Array::iterator _ti = _tPoints->begin();
  for (Point3Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point4ArrayPtr OrthonormalBasis3D::transform( const Point4ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point4ArrayPtr _tPoints(new Point4Array(points->size()));
  Point4Array::iterator _ti = _tPoints->begin();
  for (Point4Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector4(__matrix * Vector3(_i->x(),_i->y(),_i->z()),_i->w());
  return _tPoints;
}


Point3MatrixPtr OrthonormalBasis3D::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->size()));
  Point3Matrix::iterator _ti = _tPoints->begin();
  for (Point3Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point4MatrixPtr OrthonormalBasis3D::transform( const Point4MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point4MatrixPtr _tPoints(new Point4Matrix(points->size()));
  Point4Matrix::iterator _ti = _tPoints->begin();
  for (Point4Matrix::const_iterator _i = points->begin();
       _i != points->end();
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
  Point2ArrayPtr _tPoints(new Point2Array(points->size()));
  Point2Array::iterator _ti = _tPoints->begin();
  for (Point2Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point3ArrayPtr OrthonormalBasis2D::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));
  Point3Array::iterator _ti = _tPoints->begin();
  for (Point3Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector3(__matrix * Vector2(_i->x(),_i->y()),_i->z());
  return _tPoints;
}


Point2MatrixPtr OrthonormalBasis2D::transform( const Point2MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point2MatrixPtr _tPoints(new Point2Matrix(points->size()));
  Point2Matrix::iterator _ti = _tPoints->begin();
  for (Point2Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


Point3MatrixPtr OrthonormalBasis2D::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->size()));
  Point3Matrix::iterator _ti = _tPoints->begin();
  for (Point3Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector3(__matrix * Vector2(_i->x(),_i->y()),_i->z());
  return _tPoints;
}


/* ----------------------------------------------------------------------- */


