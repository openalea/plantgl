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





#include "beziercurve.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/math/util_polymath.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */


const uint32_t BezierCurve::DEFAULT_STRIDE(30);


/* ----------------------------------------------------------------------- */


BezierCurve::Builder::Builder( ) :
    ParametricModel::Builder(),
    CtrlPointList(0),
    Degree(0),
    Stride(0){
}


BezierCurve::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr BezierCurve::Builder::build( ) const {
    if (isValid()){
#ifdef GEOM_DEBUG
        if(!Degree)cout << "Degree value assign to " << ((*CtrlPointList)->getSize() - 1) << endl;
#endif
        return SceneObjectPtr(new BezierCurve(*CtrlPointList,(Stride ? *Stride : DEFAULT_STRIDE)));
    }
    return SceneObjectPtr();
}


void BezierCurve::Builder::destroy( ) {
  if (CtrlPointList) delete CtrlPointList;
  if (Degree) delete Degree;
  if (Stride) delete Stride;
}


bool BezierCurve::Builder::isValid( ) const {

  // CtrlPointList field
    if ( !CtrlPointList ) {
        genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Bezier Curve","CtrlPointList");
        return false;
    }
    uint32_t _size = (*CtrlPointList)->getSize();
    if (_size < 3 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Bezier Curve","CtrlPointList","Must have more than 2 control points.");
        return false;
    }

    for(uint32_t i=0; i < _size;i++)
        if(fabs((*CtrlPointList)->getAt(i).w()) < GEOM_TOLERANCE) {
            string _ith = number(i + 1);
            genMessage
                (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Bezier Curve","CtrlPointList",_ith.c_str(),"Weight must not be null.");
            return false;
        }

    // Degree field
    uint32_t _degree = (Degree ? *Degree : _size - 1);
    if (_degree < 1 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Curve","Degree","Must be greater than 0.");
        return false;
    };

    // Stride field
    if ( Stride && *Stride < 1 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Curve","Stride","Must be greater than 0.");
        return false;
    };

    // Fields dependency
    if (_size != (_degree + 1)) {
        string _mess =  "Must have " + number(_degree + 1) + " control points";
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Bezier Curve","CtrlPointList",_mess.c_str());
        return false;
    };

    return true;
}


/* ----------------------------------------------------------------------- */

BezierCurve::BezierCurve() :
    ParametricModel(),
    __ctrlPointList(0),
    __stride(DEFAULT_STRIDE){
}

BezierCurve::BezierCurve( const Point4ArrayPtr& ctrlPoints, uint32_t stride ) :
    ParametricModel(),
    __ctrlPointList(ctrlPoints),
    __stride(stride){
    GEOM_ASSERT(isValid());
}

BezierCurve::~BezierCurve( ) {
}

bool BezierCurve::apply( Action& action ) {
  return action.process(this);
}

/* ----------------------------------------------------------------------- */

Point4ArrayPtr
BezierCurve::getCtrlPoints( ) const {
  return __ctrlPointList;
}

const Point4ArrayPtr&
BezierCurve::getCtrlPointList( ) const {
  return __ctrlPointList;

}

Point4ArrayPtr&
BezierCurve::getCtrlPointList( ){
  return __ctrlPointList;
}

const real_t
BezierCurve::getFirstKnot() const {
  return 0;
}

const real_t
BezierCurve::getLastKnot() const {
  return 1;
}

const uint32_t
BezierCurve::getStride( ) const {
  return __stride;
}

uint32_t&
BezierCurve::getStride( ) {
  return __stride;
}

bool
BezierCurve::isStrideToDefault( ) const{
  return ( __stride == DEFAULT_STRIDE );
}

uint32_t
BezierCurve::getDegree( ) const {
  return __ctrlPointList->getSize() - 1;
}

/* ----------------------------------------------------------------------- */

Vector3 BezierCurve::getPointAt(real_t u) const{
  GEOM_ASSERT( u >= 0.0 && u <= 1.0 );

  uint32_t _deg = getDegree();

  vector<Vector4> Q(__ctrlPointList->getBegin(),__ctrlPointList->getEnd());

  real_t u1 = real_t(1.0) - u;
  for (uint32_t k = 1; k <= _deg ; k++)
    for (uint32_t i = 0; i <= ( _deg - k ) ; i++)
      Q[i] = (Q[i] * u1) + (Q[i+1] * u);

  if (fabs(Q[0].w()) < GEOM_TOLERANCE)
    return Vector3(Q[0].x(),Q[0].y(),Q[0].z());

  return Q[0].project();
}


bool BezierCurve::isValid( ) const {
  Builder _builder;
  _builder.CtrlPointList = const_cast<Point4ArrayPtr *>(&__ctrlPointList);
  _builder.Stride = const_cast<uint32_t *>(&__stride);
  return _builder.isValid();
}

SceneObjectPtr
BezierCurve::copy() const
{
  BezierCurve * ptr = new BezierCurve(*this);
  if(__ctrlPointList)ptr->getCtrlPointList() = Point4ArrayPtr(new Point4Array(*__ctrlPointList));
  return SceneObjectPtr(ptr);
}

Vector3 BezierCurve::getPointAt2(real_t u) const{
    GEOM_ASSERT(!( (u < 0) || (u > 1) ));
    if(u==0)return __ctrlPointList->getAt(0).project();
    else if(u==1)return __ctrlPointList->getAt(__ctrlPointList->getSize()).project();
    vector<real_t> _allBernstein=all_bernstein(getDegree(),u);
    Vector4 C(0.0,0.0,0.0,0.0);
    for(uint32_t i=0;i<=getDegree();i++){
        C = C + (__ctrlPointList->getAt(i) * _allBernstein[i]);
        cerr << " C = C + <" << __ctrlPointList->getAt(i).x() << "," << __ctrlPointList->getAt(i).y()
             << "," << __ctrlPointList->getAt(i).z() <<  "," << __ctrlPointList->getAt(i).w()  << "> * " << _allBernstein[i] << endl;
    }
    if (fabs(C.w()) < GEOM_TOLERANCE)
        return Vector3(C.x(),C.y(),C.z());
    else return C.project();
}

Vector3 BezierCurve::getTangentAt(real_t u) const{
    GEOM_ASSERT(u>=0.0&&u<=1.0);
    if(u < GEOM_EPSILON){
      Vector4 res(__ctrlPointList->getAt(1)-__ctrlPointList->getAt(0));
      res.normalize();
      if(!res.w()) return Vector3(res.x(),res.y(),res.z());
      else return res.project();
    }
    else if(u ==1){
      Vector4 res(__ctrlPointList->getAt(__ctrlPointList->getSize()-1)-__ctrlPointList->getAt(__ctrlPointList->getSize()-2));
      if(!res.w()) return Vector3(res.x(),res.y(),res.z());
      else return res.project();
    }

    Vector4 _tangent(0.0,0.0,0.0,0.0);
    uint32_t _deg = getDegree();
    vector<real_t> derBernstein=all_bernstein(_deg-1,u);
    for(uint32_t i=0;i<_deg;i++)
        _tangent+=(__ctrlPointList->getAt(i+1)-__ctrlPointList->getAt(i))*derBernstein[i];
    _tangent=_tangent*_deg;
    if(!_tangent.w())
        return Vector3(_tangent.x(),_tangent.y(),_tangent.z());
    else return _tangent.project();
}


Vector3 BezierCurve::getNormalAt(real_t u) const{
    GEOM_ASSERT(u>=0.0&&u<=1.0);
    Vector4 _normal(0.0,0.0,0.0,0.0);
    uint32_t _deg = getDegree();
    vector<real_t> derBernstein=all_bernstein(_deg-2,u);
    for(uint32_t i=0;i<(_deg-1);i++)
        _normal+=(__ctrlPointList->getAt(i+2)-(__ctrlPointList->getAt(i+1)*2)+__ctrlPointList->getAt(i))*derBernstein[i];
    _normal=_normal*(_deg*(_deg-1));
    Vector3 _tangent = getTangentAt(u);
    _normal.normalize();
    Vector3 _normal3;
    if(!_normal.w())
        _normal3 = Vector3(_normal.x(),_normal.y(),_normal.z());
    else _normal3 = _normal.project();
    return (cross(cross(_tangent,_normal3),_tangent))/pow(norm(_tangent),real_t(4));
}


/* ----------------------------------------------------------------------- */


BezierCurve2D::Builder::Builder( ) :
    Curve2D::Builder(),
    CtrlPointList(0),
    Degree(0),
    Stride(0){
}


BezierCurve2D::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr BezierCurve2D::Builder::build( ) const {
    if (isValid()){
        return SceneObjectPtr(new BezierCurve2D(*CtrlPointList,(Stride ? *Stride : BezierCurve::DEFAULT_STRIDE)));
    }
    return SceneObjectPtr();
}


void BezierCurve2D::Builder::destroy( ) {
  if (CtrlPointList) delete CtrlPointList;
  if (Degree) delete Degree;
  if (Stride) delete Stride;
}


bool BezierCurve2D::Builder::isValid( ) const {

  // CtrlPointList field
    if ( !CtrlPointList ) {
        genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"BezierCurve2D","CtrlPointList");
        return false;
    }
    uint32_t _size = (*CtrlPointList)->getSize();
    if (_size < 3 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"BezierCurve2D","CtrlPointList","Must have more than 2 control points.");
        return false;
    }

    // Degree field
    uint32_t _degree = (Degree ? *Degree : _size - 1);
    if (_degree < 1 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"BezierCurve2D","Degree","Must be greater than 0.");
        return false;
    };

    // Stride field
    if ( Stride && *Stride < 1 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"BezierCurve2D","Stride","Must be greater than 0.");
        return false;
    };

    // Fields dependency
    if (_size != (_degree + 1)) {
        string _mess = "Must have " + number(_degree + 1) + " control points";
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"BezierCurve2D","CtrlPointList",_mess.c_str());
        return false;
    };

    return true;
}


/* ----------------------------------------------------------------------- */

BezierCurve2D::BezierCurve2D( ) :
    Curve2D(),
    __ctrlPointList(0),
    __stride(BezierCurve::DEFAULT_STRIDE){
    GEOM_ASSERT(isValid());
}

BezierCurve2D::BezierCurve2D( const Point3ArrayPtr& ctrlPoints, uint32_t stride ) :
    Curve2D(),
    __ctrlPointList(ctrlPoints),
    __stride(stride){
    GEOM_ASSERT(isValid());
}

BezierCurve2D::~BezierCurve2D( ) {
}

bool BezierCurve2D::apply( Action& action ) {
  return action.process(this);
}

/* ----------------------------------------------------------------------- */

Point3ArrayPtr
BezierCurve2D::getCtrlPoints( ) const {
  return __ctrlPointList;
}

const Point3ArrayPtr&
BezierCurve2D::getCtrlPointList( ) const {
  return __ctrlPointList;
}

Point3ArrayPtr&
BezierCurve2D::getCtrlPointList( ) {
  return __ctrlPointList;
}

const real_t
BezierCurve2D::getFirstKnot() const {
  return 0;
}

const real_t
BezierCurve2D::getLastKnot() const {
  return 1;
}

const uint32_t
BezierCurve2D::getStride( ) const {
  return __stride;
}

uint32_t&
BezierCurve2D::getStride( ) {
  return __stride;
}

bool
BezierCurve2D::isStrideToDefault( ) const{
  return ( __stride == BezierCurve::DEFAULT_STRIDE );
}

uint32_t
BezierCurve2D::getDegree( ) const {
  return __ctrlPointList->getSize() - 1;
}

/* ----------------------------------------------------------------------- */

SceneObjectPtr
BezierCurve2D::copy() const
{
  BezierCurve2D * ptr = new BezierCurve2D(*this);
  if(__ctrlPointList)ptr->getCtrlPointList() = Point3ArrayPtr(new Point3Array(*__ctrlPointList));
  return SceneObjectPtr(ptr);
}

Vector2 BezierCurve2D::getPointAt(real_t u) const{
  GEOM_ASSERT( u >= 0.0 && u <= 1.0 );

  uint32_t _deg = getDegree();

  vector<Vector3> Q(__ctrlPointList->getBegin(),__ctrlPointList->getEnd());

  real_t u1 = real_t(1.0) - u;
  for (uint32_t k = 1; k <= _deg ; k++)
    for (uint32_t i = 0; i <= ( _deg - k ) ; i++)
      Q[i] = (Q[i] * u1) + (Q[i+1] * u);

  if (fabs(Q[0].z()) < GEOM_TOLERANCE)
    return Vector2(Q[0].x(),Q[0].y());

  return Q[0].project();
}


bool BezierCurve2D::isValid( ) const {
  Builder _builder;
  _builder.CtrlPointList = const_cast<Point3ArrayPtr *>(&__ctrlPointList);
  _builder.Stride = const_cast<uint32_t *>(&__stride);
  return _builder.isValid();
}



/* ----------------------------------------------------------------------- */

