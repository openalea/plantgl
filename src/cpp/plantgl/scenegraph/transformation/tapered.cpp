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



#include "tapered.h"
#include <plantgl/scenegraph/geometry/primitive.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/math/util_math.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

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
  Point3ArrayPtr _tPoints(new Point3Array(points->getSize()));

  if (points->getSize()) {

    pair<Point3Array::const_iterator,Point3Array::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point3Array::iterator _ti = _tPoints->getBegin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point3Array::iterator _i = points->getBegin();
           _i != points->getEnd();
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
  Point4ArrayPtr _tPoints(new Point4Array(points->getSize()));

  if (points->getSize()) {

    pair<Point4Array::const_iterator,Point4Array::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point4Array::iterator _ti = _tPoints->getBegin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point4Array::iterator _i = points->getBegin();
           _i != points->getEnd();
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
  Point3MatrixPtr _tPoints(new Point3Matrix(points->getSize()));

  if (points->getSize()) {

    pair<Point3Matrix::const_iterator,Point3Matrix::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point3Matrix::iterator _ti = _tPoints->getBegin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point3Matrix::iterator _i = points->getBegin();
           _i != points->getEnd();
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
  Point4MatrixPtr _tPoints(new Point4Matrix(points->getSize()));

  if (points->getSize()) {

    pair<Point4Matrix::const_iterator,Point4Matrix::const_iterator> _zMinMax;
    _zMinMax = points->getZMinAndMax();

    real_t _deltaZ = _zMinMax.second->z() - _zMinMax.first->z();

    Point4Matrix::iterator _ti = _tPoints->getBegin();

    if (_deltaZ > GEOM_EPSILON)

      for (Point4Matrix::iterator _i = points->getBegin();
           _i != points->getEnd();
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
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Tapered","BaseRadius","Must be greater than 0.");
    return false;
  };

  if (TopRadius && (*TopRadius < 0)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Tapered","TopRadius","Must be greater than 0.");
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

bool Tapered::apply( Action& action ) {
  return action.process(this);
}

bool Tapered::isValid( ) const {
  Builder _builder;
  _builder.Primitive = const_cast<PrimitivePtr *>(&__primitive);
  _builder.BaseRadius = const_cast<real_t *>(&__baseRadius);
  _builder.TopRadius = const_cast<real_t *>(&__topRadius);
  return (_builder.isValid() && __primitive->isValid());
}

SceneObjectPtr Tapered::copy() const {
  Tapered * ptr = new Tapered(*this);
  if(__primitive)ptr->getPrimitive().cast(__primitive->copy());
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
