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




#include "beziercurve.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/math/util_polymath.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */


const uint_t BezierCurve::DEFAULT_STRIDE(30);


/* ----------------------------------------------------------------------- */


BezierCurve::Builder::Builder( ) :
    ParametricModel::Builder(),
    CtrlPointList(0),
    Degree(0),
    Stride(0),
    Width(0){
}


BezierCurve::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr BezierCurve::Builder::build( ) const {
    if (isValid()){
#ifdef GEOM_DEBUG
        if(!Degree)cout << "Degree value assign to " << ((*CtrlPointList)->size() - 1) << endl;
#endif
        return SceneObjectPtr(new BezierCurve(*CtrlPointList,
                                              (Stride ? *Stride : DEFAULT_STRIDE),
                                              (Width ? *Width : DEFAULT_WIDTH)));
    }
    return SceneObjectPtr();
}


void BezierCurve::Builder::destroy( ) {
  BCdestroy();
}

void BezierCurve::Builder::BCdestroy( ) {
  if (CtrlPointList) delete CtrlPointList;
  if (Degree) delete Degree;
  if (Stride) delete Stride;
  if (Width) delete Width;
}


bool BezierCurve::Builder::isValid( ) const {

  // CtrlPointList field
    if ( !CtrlPointList ) {
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Bezier Curve","CtrlPointList");
        return false;
    }
    uint_t _size = (*CtrlPointList)->size();
    if (_size < 3 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Bezier Curve","CtrlPointList","Must have more than 2 control points.");
        return false;
    }

    for(uint_t i=0; i < _size;i++)
        if(fabs((*CtrlPointList)->getAt(i).w()) < GEOM_TOLERANCE) {
            string _ith = number(i + 1);
            pglErrorEx
                (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Bezier Curve","CtrlPointList",_ith.c_str(),"Weight must not be null.");
            return false;
        }

    // Degree field
    uint_t _degree = (Degree ? *Degree : _size - 1);
    if (_degree < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Curve","Degree","Must be greater than 0.");
        return false;
    };

    // Stride field
    if ( Stride && *Stride < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Curve","Stride","Must be greater than 0.");
        return false;
    };

    // Fields dependency
    if (_size != (_degree + 1)) {
        string _mess =  "Must have " + number(_degree + 1) + " control points";
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Bezier Curve","CtrlPointList",_mess.c_str());
        return false;
    };

    return true;
}


/* ----------------------------------------------------------------------- */

BezierCurve::BezierCurve() :
    ParametricModel(), LineicModel(),
    __ctrlPointList(),
    __stride(DEFAULT_STRIDE){
}

BezierCurve::BezierCurve( const Point4ArrayPtr& ctrlPoints, uint_t stride, uchar_t width ) :
    ParametricModel(), LineicModel(width),
    __ctrlPointList(ctrlPoints),
    __stride(stride){
    GEOM_ASSERT(isValid());
}

BezierCurve::BezierCurve( const Point3ArrayPtr& ctrlPoints, uint_t stride, uchar_t width ) :
    ParametricModel(), LineicModel(width),
    __ctrlPointList(),
    __stride(stride){
    if (ctrlPoints) __ctrlPointList = Point4ArrayPtr(new Point4Array(*ctrlPoints,1));
    GEOM_ASSERT(isValid());
}

BezierCurve::~BezierCurve( ) {
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

const uint_t
BezierCurve::getStride( ) const {
  return __stride;
}

uint_t&
BezierCurve::getStride( ) {
  return __stride;
}

bool
BezierCurve::isStrideToDefault( ) const{
  return ( __stride == DEFAULT_STRIDE );
}

const uint_t
BezierCurve::getDegree( ) const {
  return __ctrlPointList->size() - 1;
}

/* ----------------------------------------------------------------------- */

Vector3 BezierCurve::getPointAt(real_t u) const{
  GEOM_ASSERT( u >= 0.0 && u <= 1.0 );

  uint_t _deg = getDegree();

  vector<Vector4> Q(__ctrlPointList->begin(),__ctrlPointList->end());

  for (vector<Vector4>::iterator it = Q.begin(); it != Q.end(); ++it){
      it->x() *= it->w(); it->y() *= it->w(); it->z() *= it->w();
  }

  real_t u1 = real_t(1.0) - u;
  for (uint_t k = 1; k <= _deg ; k++)
    for (uint_t i = 0; i <= ( _deg - k ) ; i++)
      Q[i] = (Q[i] * u1) + (Q[i+1] * u);

  if (fabs(Q[0].w()) < GEOM_TOLERANCE)
    return Vector3(Q[0].x(),Q[0].y(),Q[0].z());

  return Q[0].project();
}


bool BezierCurve::isValid( ) const {
  Builder _builder;
  _builder.CtrlPointList = const_cast<Point4ArrayPtr *>(&__ctrlPointList);
  _builder.Stride = const_cast<uint_t *>(&__stride);
  return _builder.isValid();
}

SceneObjectPtr
BezierCurve::copy(DeepCopier& copier) const
{
  BezierCurve * ptr = new BezierCurve(*this);
  copier.copy_attribute(ptr->getCtrlPointList());
  return SceneObjectPtr(ptr);
}

Vector3 BezierCurve::getPointAt2(real_t u) const{
    GEOM_ASSERT(!( (u < 0) || (u > 1) ));
    if(u==0)return __ctrlPointList->getAt(0).project();
    else if(u==1)return __ctrlPointList->getAt(__ctrlPointList->size()).project();
    vector<real_t> _allBernstein=all_bernstein(getDegree(),u);
    Vector4 C(0.0,0.0,0.0,0.0);
    for(uint_t i=0;i<=getDegree();i++){
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
      Vector4 res(__ctrlPointList->getAt(__ctrlPointList->size()-1)-__ctrlPointList->getAt(__ctrlPointList->size()-2));
      if(!res.w()) return Vector3(res.x(),res.y(),res.z());
      else return res.project();
    }

    Vector4 _tangent(0.0,0.0,0.0,0.0);
    uint_t _deg = getDegree();
    vector<real_t> derBernstein=all_bernstein(_deg-1,u);
    for(uint_t i=0;i<_deg;i++)
        _tangent+=(__ctrlPointList->getAt(i+1)-__ctrlPointList->getAt(i))*derBernstein[i];
    _tangent=_tangent*_deg;
    if(!_tangent.w())
        return Vector3(_tangent.x(),_tangent.y(),_tangent.z());
    else return _tangent.project();
}


Vector3 BezierCurve::getNormalAt(real_t u) const{
    GEOM_ASSERT(u>=0.0&&u<=1.0);
    Vector4 _normal(0.0,0.0,0.0,0.0);
    uint_t _deg = getDegree();
    vector<real_t> derBernstein=all_bernstein(_deg-2,u);
    for(uint_t i=0;i<(_deg-1);i++)
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
    CtrlPointList(),
    Degree(0),
    Stride(0){
}


BezierCurve2D::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr BezierCurve2D::Builder::build( ) const {
    if (isValid()){
        return SceneObjectPtr(new BezierCurve2D(*CtrlPointList,
                                                (Stride ? *Stride : BezierCurve::DEFAULT_STRIDE),
                                                (Width ? *Width : DEFAULT_WIDTH)));
    }
    return SceneObjectPtr();
}


void BezierCurve2D::Builder::destroy( ) {
  if (CtrlPointList) delete CtrlPointList;
  if (Degree) delete Degree;
  if (Stride) delete Stride;
  if (Width) delete Width;
}


bool BezierCurve2D::Builder::isValid( ) const {

  // CtrlPointList field
    if ( !CtrlPointList ) {
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"BezierCurve2D","CtrlPointList");
        return false;
    }
    uint_t _size = (*CtrlPointList)->size();
    if (_size < 3 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"BezierCurve2D","CtrlPointList","Must have more than 2 control points.");
        return false;
    }

    // Degree field
    uint_t _degree = (Degree ? *Degree : _size - 1);
    if (_degree < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"BezierCurve2D","Degree","Must be greater than 0.");
        return false;
    };

    // Stride field
    if ( Stride && *Stride < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"BezierCurve2D","Stride","Must be greater than 0.");
        return false;
    };

    // Fields dependency
    if (_size != (_degree + 1)) {
        string _mess = "Must have " + number(_degree + 1) + " control points";
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"BezierCurve2D","CtrlPointList",_mess.c_str());
        return false;
    };

    return true;
}


/* ----------------------------------------------------------------------- */

BezierCurve2D::BezierCurve2D( ) :
    Curve2D(),
    __ctrlPointList(),
    __stride(BezierCurve::DEFAULT_STRIDE){
    GEOM_ASSERT(isValid());
}

BezierCurve2D::BezierCurve2D( const Point3ArrayPtr& ctrlPoints, uint_t stride, uchar_t width ) :
    Curve2D(width),
    __ctrlPointList(ctrlPoints),
    __stride(stride){
    GEOM_ASSERT(isValid());
}

BezierCurve2D::BezierCurve2D( const Point2ArrayPtr& ctrlPoints, uint_t stride, uchar_t width  ) :
    Curve2D(width),
    __ctrlPointList(),
    __stride(stride){
    if (ctrlPoints) __ctrlPointList = Point3ArrayPtr(new Point3Array(*ctrlPoints,1));
    GEOM_ASSERT(isValid());
}

BezierCurve2D::~BezierCurve2D( ) {
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

const uint_t
BezierCurve2D::getStride( ) const {
  return __stride;
}

uint_t&
BezierCurve2D::getStride( ) {
  return __stride;
}

bool
BezierCurve2D::isStrideToDefault( ) const{
  return ( __stride == BezierCurve::DEFAULT_STRIDE );
}

const uint_t
BezierCurve2D::getDegree( ) const {
  return __ctrlPointList->size() - 1;
}

/* ----------------------------------------------------------------------- */

SceneObjectPtr
BezierCurve2D::copy(DeepCopier& copier) const
{
  BezierCurve2D * ptr = new BezierCurve2D(*this);
  copier.copy_attribute(ptr->getCtrlPointList());
  return SceneObjectPtr(ptr);
}

Vector2 BezierCurve2D::getPointAt(real_t u) const{
  GEOM_ASSERT( u >= 0.0 && u <= 1.0 );

  uint_t _deg = getDegree();

  vector<Vector3> Q(__ctrlPointList->begin(),__ctrlPointList->end());

  real_t u1 = real_t(1.0) - u;
  for (uint_t k = 1; k <= _deg ; k++)
    for (uint_t i = 0; i <= ( _deg - k ) ; i++)
      Q[i] = (Q[i] * u1) + (Q[i+1] * u);

  if (fabs(Q[0].z()) < GEOM_TOLERANCE)
    return Vector2(Q[0].x(),Q[0].y());

  return Q[0].project();
}


bool BezierCurve2D::isValid( ) const {
  Builder _builder;
  _builder.CtrlPointList = const_cast<Point3ArrayPtr *>(&__ctrlPointList);
  _builder.Stride = const_cast<uint_t *>(&__stride);
  return _builder.isValid();
}


Vector2 BezierCurve2D::getTangentAt(real_t u) const{
    GEOM_ASSERT(u>=0.0&&u<=1.0);
    if(u < GEOM_EPSILON){
      Vector3 res(__ctrlPointList->getAt(1)-__ctrlPointList->getAt(0));
      res.normalize();
      if(!res.z()) return Vector2(res.x(),res.y());
      else return res.project();
    }
    else if(u ==1){
      Vector3 res(__ctrlPointList->getAt(__ctrlPointList->size()-1)-__ctrlPointList->getAt(__ctrlPointList->size()-2));
      if(!res.z()) return Vector2(res.x(),res.y());
      else return res.project();
    }

    Vector3 _tangent(0.0,0.0,0.0);
    uint_t _deg = getDegree();
    vector<real_t> derBernstein=all_bernstein(_deg-1,u);
    for(uint_t i=0;i<_deg;i++)
        _tangent+=(__ctrlPointList->getAt(i+1)-__ctrlPointList->getAt(i))*derBernstein[i];
    _tangent=_tangent*_deg;
    if(!_tangent.z())
        return Vector2(_tangent.x(),_tangent.y());
    else return _tangent.project();
}


Vector2 BezierCurve2D::getNormalAt(real_t u) const{
    GEOM_ASSERT(u>=0.0&&u<=1.0);
    Vector3 _normal(0.0,0.0,0.0);
    uint_t _deg = getDegree();
    vector<real_t> derBernstein=all_bernstein(_deg-2,u);
    for(uint_t i=0;i<(_deg-1);i++)
        _normal+=(__ctrlPointList->getAt(i+2)-(__ctrlPointList->getAt(i+1)*2)+__ctrlPointList->getAt(i))*derBernstein[i];
    _normal=_normal*(_deg*(_deg-1));
    Vector2 _tangent = getTangentAt(u);
    _normal.normalize();
    Vector2 _normal2;
    if(!_normal.z())
        _normal2 = Vector2(_normal.x(),_normal.y());
    else _normal2 = _normal.project();

    Vector3 nml = cross(cross(Vector3(_tangent,0),Vector3(_normal2,0)),Vector3(_tangent,0))/pow(norm(_tangent),real_t(4));
    return Vector2(nml[0],nml[1]);
}

/* ----------------------------------------------------------------------- */

