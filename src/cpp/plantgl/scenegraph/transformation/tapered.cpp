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




#include "tapered.h"
#include <plantgl/scenegraph/geometry/primitive.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/math/util_math.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


const real_t Tapered::DEFAULT_BASE_RADIUS(.5);
const real_t Tapered::DEFAULT_TOP_RADIUS(.5);


/* ----------------------------------------------------------------------- */

Taper::Taper( const real_t& baseRadius,
              const real_t& topRadius ) :
  Deformation(),
  __baseRadius(baseRadius),
  __deltaRadius(baseRadius - topRadius) {
  GEOM_ASSERT(isValid());
}

Taper::~Taper( ) {
}

const real_t& Taper::getBaseRadius( ) const {
  return __baseRadius;
}

real_t Taper::getTopRadius( ) const {
  return __baseRadius - __deltaRadius;
}

bool Taper::isValid( ) const {
  return ((__baseRadius >= 0) && (getTopRadius() >= 0));
}


Point3ArrayPtr Taper::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));

  if (points->size()) {

    pair<Point3Array::const_iterator,Point3Array::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point3Array::iterator _ti = _tPoints->begin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point3Array::iterator _i = points->begin();
           _i != points->end();
           _i++) {

        real_t _dZ = _i->z() - _zMinMax.first->z();
        real_t _factor = __baseRadius - __deltaRadius * (_dZ / _deltaZ);

        *_ti = Vector3(_i->x() * _factor, _i->y() * _factor, _i->z());

        _ti++;
      }

    else
      return points;
  };

  return _tPoints;
}


Point4ArrayPtr Taper::transform( const Point4ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point4ArrayPtr _tPoints(new Point4Array(points->size()));

  if (points->size()) {

    pair<Point4Array::const_iterator,Point4Array::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point4Array::iterator _ti = _tPoints->begin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point4Array::iterator _i = points->begin();
           _i != points->end();
           _i++) {

        real_t _dZ = _i->z() - _zMinMax.first->z();
        real_t _factor = __baseRadius - __deltaRadius * (_dZ / _deltaZ);

        *_ti = Vector4(_i->x() * _factor, _i->y() * _factor, _i->z(), _i->w());

        _ti++;
      }

    else
      return points;
  };

  return _tPoints;
}



Point3MatrixPtr Taper::transform( const Point3MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->size()));

  if (points->size()) {

    pair<Point3Matrix::const_iterator,Point3Matrix::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point3Matrix::iterator _ti = _tPoints->begin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point3Matrix::iterator _i = points->begin();
           _i != points->end();
           _i++) {

        real_t _dZ = _i->z() - _zMinMax.first->z();
        real_t _factor = __baseRadius - __deltaRadius * (_dZ / _deltaZ);

        *_ti = Vector3(_i->x() * _factor, _i->y() * _factor, _i->z());

        _ti++;
      }

    else
      return points;
  };

  return _tPoints;
}


Point4MatrixPtr Taper::transform( const Point4MatrixPtr& points ) const {
  GEOM_ASSERT(points);
  Point4MatrixPtr _tPoints(new Point4Matrix(points->size()));

  if (points->size()) {

    pair<Point4Matrix::const_iterator,Point4Matrix::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point4Matrix::iterator _ti = _tPoints->begin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point4Matrix::iterator _i = points->begin();
           _i != points->end();
           _i++) {

        real_t _dZ = _i->z() - _zMinMax.first->z();
        real_t _factor = __baseRadius - __deltaRadius * (_dZ / _deltaZ);

        *_ti = Vector4(_i->x() * _factor, _i->y() * _factor, _i->z(), _i->w());

        _ti++;
      }

    else
      return points;
  };

  return _tPoints;
}


/* ----------------------------------------------------------------------- */


Tapered::Builder::Builder( ) :
  Deformed::Builder(),
  BaseRadius(0),
  TopRadius(0) {
}


Tapered::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Tapered::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Tapered(BaseRadius ? *BaseRadius : DEFAULT_BASE_RADIUS,
                   TopRadius ? *TopRadius : DEFAULT_TOP_RADIUS,
                   *Primitive));
  return SceneObjectPtr();
}


void Tapered::Builder::destroy( ) {
  DefDestroy();
  if (BaseRadius) delete BaseRadius;
  if (TopRadius) delete TopRadius;
}


bool Tapered::Builder::isValid( ) const {
  if (! DefValid()) return false;

  if (BaseRadius && (*BaseRadius < 0)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Tapered","BaseRadius","Must be greater than 0.");
    return false;
  };

  if (TopRadius && (*TopRadius < 0)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Tapered","TopRadius","Must be greater than 0.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */

Tapered::Tapered( ) :
  Deformed(),
  __baseRadius(DEFAULT_BASE_RADIUS),
  __topRadius(DEFAULT_TOP_RADIUS) {
}

Tapered::Tapered( const real_t& baseRadius,
                  const real_t& topRadius,
                  const PrimitivePtr& primitive ) :
  Deformed(primitive),
  __baseRadius(baseRadius),
  __topRadius(topRadius) {
  GEOM_ASSERT(isValid());
}

Tapered::~Tapered( ) {
}

bool Tapered::isValid( ) const {
  Builder _builder;
  _builder.Primitive = const_cast<PrimitivePtr *>(&__primitive);
  _builder.BaseRadius = const_cast<real_t *>(&__baseRadius);
  _builder.TopRadius = const_cast<real_t *>(&__topRadius);
  return (_builder.isValid() && __primitive->isValid());
}

SceneObjectPtr Tapered::copy(DeepCopier& copier) const {
  Tapered * ptr = new Tapered(*this);
  copier.copy_object_attribute(ptr->getPrimitive());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

const real_t& Tapered::getBaseRadius( ) const {
  return __baseRadius;
}

real_t& Tapered::getBaseRadius( ){
  return __baseRadius;
}

Transformation3DPtr Tapered::getTransformation( ) const {
  return Transformation3DPtr(new Taper(__baseRadius,__topRadius));
}

const real_t&  Tapered::getTopRadius( ) const {
  return __topRadius;
}

real_t&  Tapered::getTopRadius( ) {
  return __topRadius;
}

bool Tapered::isBaseRadiusToDefault( ) const {
  return fabs(__baseRadius - DEFAULT_BASE_RADIUS) < GEOM_TOLERANCE;
}

bool Tapered::isTopRadiusToDefault( ) const {
  return fabs(__topRadius - DEFAULT_TOP_RADIUS) < GEOM_TOLERANCE;
}
