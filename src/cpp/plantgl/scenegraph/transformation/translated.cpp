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




#include "translated.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

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

