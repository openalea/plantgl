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



#include "translated.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

Translation::Translation( const Vector3& v ) :
  Matrix4Transformation(),
  __vector(v) {
  GEOM_ASSERT(isValid());
}

Translation::~Translation( ) {
}

Matrix4 Translation::getMatrix( ) const {
  return Matrix4::translation(__vector);
}

const Vector3& Translation::getVector( ) const {
  return __vector;
}

bool Translation::isValid( ) const {
  return __vector.isValid();
}


Point3ArrayPtr Translation::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));
  Point3Array::iterator _ti = _tPoints->begin();
  for (Point3Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = *_i + __vector;
  return _tPoints;
}


Point4ArrayPtr Translation::transform( const Point4ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point4ArrayPtr _tPoints(new Point4Array(points->size()));
  Point4Array::iterator _ti = _tPoints->begin();
  for (Point4Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector4(_i->x() + __vector.x() * _i->w(),
                     _i->y() + __vector.y() * _i->w(),
                     _i->z() + __vector.z() * _i->w(),
                     _i->w());
  return _tPoints;
}


Point3MatrixPtr Translation::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->size()));
  Point3Matrix::iterator _ti = _tPoints->begin();
  for (Point3Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = *_i + __vector;
  return _tPoints;
}


Point4MatrixPtr Translation::transform( const Point4MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point4MatrixPtr _tPoints(new Point4Matrix(points->size()));
  Point4Matrix::iterator _ti = _tPoints->begin();
  for (Point4Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector4(_i->x() + __vector.x() * _i->w(),
                     _i->y() + __vector.y() * _i->w(),
                     _i->z() + __vector.z() * _i->w(),
                     _i->w());
  return _tPoints;
}


/* ----------------------------------------------------------------------- */


const Vector3 Translated::DEFAULT_TRANSLATION(Vector3::ORIGIN);


/* ----------------------------------------------------------------------- */


Translated::Builder::Builder( ) :
  MatrixTransformed::Builder(),
  Translation(0) {
}


Translated::Builder::~Builder( ) {
}


SceneObjectPtr Translated::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Translated(Translation ? *Translation : DEFAULT_TRANSLATION,
                      *Geometry));
  return SceneObjectPtr();
}


void Translated::Builder::destroy() {
  MTDestroy();
  if (Translation) delete Translation;
}


bool Translated::Builder::isValid( ) const {
  if(!MTValid())return false;

  if (Translation &&
      (! Translation->isValid())) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Translated","Translation","Must be a valid Translation");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


Translated::Translated() :
      MatrixTransformed(),
      __translation(DEFAULT_TRANSLATION){
  }

Translated::Translated( const Vector3& translation,
                        const GeometryPtr& geometry ) :
  MatrixTransformed(geometry),
  __translation(translation) {
  GEOM_ASSERT(isValid());
}

Translated::~Translated( ) {
#ifdef GEOM_DEBUG
    cerr <<"Translated " <<  __name << " destroyed" << endl;
#endif
}

bool Translated::isValid( ) const {
  Builder _builder;
  _builder.Geometry = const_cast<GeometryPtr *>(&__geometry);
  _builder.Translation = const_cast<Vector3 *>(&__translation);
  return (_builder.isValid() && __geometry->isValid());
}

SceneObjectPtr
Translated::copy(DeepCopier& copier) const {
  Translated * ptr = new Translated(*this);
  copier.copy_object_attribute(ptr->getGeometry());
  return SceneObjectPtr(ptr);
}

Transformation3DPtr
Translated::getTransformation( ) const {
  return Transformation3DPtr(new Translation(__translation));
}

const Vector3&
Translated::getTranslation( ) const {
  return __translation;
}

Vector3&
Translated::getTranslation( ) {
  return __translation;
}

bool
Translated::isTranslationToDefault( ) const {
  return __translation == DEFAULT_TRANSLATION;
}


/* ----------------------------------------------------------------------- */


Translation2D::Translation2D( const Vector2& v ) :
  Matrix3Transformation(),
  __vector(v) {
  GEOM_ASSERT(isValid());
}

Translation2D::~Translation2D(){
}

Matrix3
Translation2D::getMatrix( ) const {
  return Matrix3( 1,0,__vector.x(),
                  0,1,__vector.y(),
                  0,0,1);
}

const Vector2&
Translation2D::getVector( ) const {
  return __vector;
}

bool Translation2D::isValid( ) const {
  return __vector.isValid();
}

Point2ArrayPtr Translation2D::transform( const Point2ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point2ArrayPtr _tPoints(new Point2Array(points->size()));
  Point2Array::iterator _ti = _tPoints->begin();
  for (Point2Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = *_i + __vector;
  return _tPoints;
}


Point3ArrayPtr Translation2D::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));
  Point3Array::iterator _ti = _tPoints->begin();
  for (Point3Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector3(_i->x() + __vector.x() * _i->z(),
                     _i->y() + __vector.y() * _i->z(),
                     _i->z());
  return _tPoints;
}


Point2MatrixPtr Translation2D::transform( const Point2MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point2MatrixPtr _tPoints(new Point2Matrix(points->size()));
  Point2Matrix::iterator _ti = _tPoints->begin();
  for (Point2Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = *_i + __vector;
  return _tPoints;
}


Point3MatrixPtr Translation2D::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->size()));
  Point3Matrix::iterator _ti = _tPoints->begin();
  for (Point3Matrix::const_iterator _i = points->begin();
       _i != points->end();
         _i++)
    *_ti++ = Vector3(_i->x() + __vector.x() * _i->z(),
                     _i->y() + __vector.y() * _i->z(),
                     _i->z());
  return _tPoints;
}


/* ----------------------------------------------------------------------- */

