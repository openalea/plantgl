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




#include "scaled.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

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
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));
  Point3Array::iterator _ti = _tPoints->begin();
  for (Point3Array::const_iterator _i = points->begin();
       _i != points->end();
       _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z() * __factors.z());
  return _tPoints;
}


Point4ArrayPtr Scaling::transform( const Point4ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point4ArrayPtr _tPoints(new Point4Array(points->size()));
  Point4Array::iterator _ti = _tPoints->begin();
  for (Point4Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector4(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z() * __factors.z(),
                     _i->w());
  return _tPoints;
}


Point3MatrixPtr Scaling::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->size()));
  Point3Matrix::iterator _ti = _tPoints->begin();
  for (Point3Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z() * __factors.z());
  return _tPoints;
}


Point4MatrixPtr Scaling::transform( const Point4MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point4MatrixPtr _tPoints(new Point4Matrix(points->size()));
  Point4Matrix::iterator _ti = _tPoints->begin();
  for (Point4Matrix::const_iterator _i = points->begin();
       _i != points->end();
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
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Scaled","Scale","Couldn't scale with factors <0,0,0>.");
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

bool Scaled::isValid( ) const {
  Builder _builder;
  _builder.Geometry = const_cast<GeometryPtr *>(&__geometry);
  _builder.Scale = const_cast<Vector3 *>(&__scale);
  return (_builder.isValid() && __geometry->isValid());
}

SceneObjectPtr Scaled::copy(DeepCopier& copier) const {
  Scaled * ptr = new Scaled(*this);
  copier.copy_object_attribute(ptr->getGeometry());
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
  Point2ArrayPtr _tPoints(new Point2Array(points->size()));
  Point2Array::iterator _ti = _tPoints->begin();
  for (Point2Array::const_iterator _i = points->begin();
       _i != points->end();
       _i++)
    *_ti++ = Vector2(_i->x() * __factors.x(),
                     _i->y() * __factors.y());
  return _tPoints;
}


Point3ArrayPtr Scaling2D::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));
  Point3Array::iterator _ti = _tPoints->begin();
  for (Point3Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z());
  return _tPoints;
}


Point2MatrixPtr Scaling2D::transform( const Point2MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point2MatrixPtr _tPoints(new Point2Matrix(points->size()));
  Point2Matrix::iterator _ti = _tPoints->begin();
  for (Point2Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector2(_i->x() * __factors.x(),
                     _i->y() * __factors.y());
  return _tPoints;
}


Point3MatrixPtr Scaling2D::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->size()));
  Point3Matrix::iterator _ti = _tPoints->begin();
  for (Point3Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector3(_i->x() * __factors.x(),
                     _i->y() * __factors.y(),
                     _i->z());
  return _tPoints;
}


/*----------------------------------------------------------------------- */
