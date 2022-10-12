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



#include "extrusion.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>
#include <plantgl/scenegraph/transformation/orthotransformed.h>
#include "lineicmodel.h"
#include "curve.h"
#include "nurbscurve.h"
#include "profile.h"
#include "mesh.h"
#include <plantgl/tool/util_array.h>
#include <plantgl/scenegraph/container/pointarray.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

// #define DEFAULT_ORIENTATION_LIST ProfileTransformation::DEFAULT_ORIENTATION_LIST
// #define DEFAULT_SCALE_LIST ProfileTransformation::DEFAULT_SCALE_LIST

const RealArrayPtr Extrusion::DEFAULT_ORIENTATION_LIST(ProfileTransformation::DEFAULT_ORIENTATION_LIST);
const Point2ArrayPtr Extrusion::DEFAULT_SCALE_LIST(ProfileTransformation::DEFAULT_SCALE_LIST);
const bool Extrusion::DEFAULT_CCW(Mesh::DEFAULT_CCW);
const bool Extrusion::DEFAULT_SOLID(Mesh::DEFAULT_SOLID);
const Vector3 Extrusion::DEFAULT_INITIAL_NORMAL(Vector3::ORIGIN);

Extrusion::Builder::Builder() :
    ParametricModel::Builder(),
    Axis(0),
    CrossSection(0),
    Scale(0),
    Orientation(0),
    KnotList(0),
    Solid(0),
    CCW(0),
    InitialNormal(0){
}

Extrusion::Builder::~Builder(){
    // nothing to do
}

SceneObjectPtr Extrusion::Builder::build( ) const {

    if (isValid()){
    if((!Scale) && (!Orientation))
        return SceneObjectPtr(new Extrusion(*Axis,*CrossSection,
                        ( Solid ? *Solid : Mesh::DEFAULT_SOLID ),
                        ( CCW ? *CCW : Mesh::DEFAULT_CCW ),
                        ( InitialNormal ? *InitialNormal : DEFAULT_INITIAL_NORMAL )));

    ProfileTransformationPtr _profile;
    if(KnotList)_profile = ProfileTransformationPtr(new ProfileTransformation(( Scale ? *Scale : DEFAULT_SCALE_LIST),
                                          ( Orientation ? *Orientation : DEFAULT_ORIENTATION_LIST),*KnotList));
    else{
        _profile = ProfileTransformationPtr(new ProfileTransformation(( Scale ? *Scale : DEFAULT_SCALE_LIST),
                                      ( Orientation ? *Orientation : DEFAULT_ORIENTATION_LIST)));
    }
    return SceneObjectPtr(new Extrusion(*Axis,*CrossSection,_profile,
                        ( Solid ? *Solid : Mesh::DEFAULT_SOLID ),
                        ( CCW ? *CCW : Mesh::DEFAULT_CCW ),
                        ( InitialNormal ? *InitialNormal : DEFAULT_INITIAL_NORMAL )));
    }
    return SceneObjectPtr();
}

void Extrusion::Builder::destroy() {
    if (Axis) delete Axis;
    if (CrossSection) delete CrossSection;
    if (Scale) delete Scale;
    if (Orientation) delete Orientation;
    if (KnotList) delete KnotList;
    if (Solid) delete Solid;
    if (CCW) delete CCW;
    if (InitialNormal) delete InitialNormal;
}

bool Extrusion::Builder::isValid( ) const {
    // Axis field
    if(!Axis) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","Axis");
    return false;
    };

    if(!(*Axis)) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","*Axis");
    return false;
    };

    if(!(*Axis)->isValid()) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Axis","Must be a valid Geometry Object.");
    return false;
    };

    if(!CrossSection) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","CrossSection");
    return false;
    };

    if(!(*CrossSection)) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","*CrossSection");
    return false;
    };

    if(!(*CrossSection)->isValid()) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","CrossSection","Must be a valid Geometry Object.");
    return false;
    };

    if(Scale){
    if(!(*Scale)->isValid()) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Scale","Must be a valid Object.");
        return false;
    };
    if((*Scale)->size() == 0 ){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","Scale","Must have more values.");
        return false;
    }

    }

    if(Orientation){
    if((*Orientation)->size() == 0 ){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","Orientation","Must have more values.");
        return false;
    }
    }

    if( ((Scale)&&(*Scale)->size() !=1) &&
    ((Orientation)&&(*Orientation)->size() !=1) &&
    ((*Scale)->size()!=((*Orientation)->size())) ){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Orientation",
               "Must specifie Scale and Orientation with the same number of value.");
        return false;
    }

    if(KnotList){
    if(((!Scale)||(*Scale)->size() ==1)&&((!Orientation)||(*Orientation)->size() ==1)){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","KnotList",
               "Must specifie Scale or Orientation with more than one value.");
    }
    else if(Scale){
        if(((*Scale)->size() > 1)&&((*Scale)->size() != (*KnotList)->size())){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","KnotList",
               "Must have the same number of value than Scale .");
        return false;
        }
    }
    else {
        if(((*Orientation)->size() ==1)&&(((*Orientation)->size()) != (*KnotList)->size())){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","KnotList",
               "Must have the same number of value than Orientation .");
        return false;
        }
    }
    }

    return true;
}


/* ----------------------------------------------------------------------- */

Extrusion::Extrusion( ) :
    ParametricModel(),
    __axis(),
    __crossSection(),
    __profile(new ProfileTransformation()),
    __solid(Mesh::DEFAULT_SOLID),
    __ccw(Mesh::DEFAULT_CCW),
    __initialNormal(DEFAULT_INITIAL_NORMAL){
}

Extrusion::Extrusion(const LineicModelPtr& _axis,const Curve2DPtr& _crossSection,
             const ProfileTransformationPtr _profile, const bool _solid, const bool _ccw,
             const Vector3& initialNormal  ) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(_profile),
    __solid(_solid),
    __ccw(_ccw),
    __initialNormal(initialNormal){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,const Curve2DPtr& _crossSection,
             const bool _solid, const bool _ccw,
             const Vector3& initialNormal ) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation()),
    __solid(_solid),
    __ccw(_ccw),
    __initialNormal(initialNormal){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,
               const Curve2DPtr& _crossSection,
               const Point2ArrayPtr& _scale,
               const RealArrayPtr& _orientation,
               const RealArrayPtr& _knot,
               const bool _solid,
               const bool _ccw,
             const Vector3& initialNormal) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation(_scale,_orientation,_knot)),
    __solid(_solid),
    __ccw(_ccw),
    __initialNormal(initialNormal){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,
               const Curve2DPtr& _crossSection,
               const RealArrayPtr& _knot,
               const Point2ArrayPtr& _scale,
               const bool _solid,
               const bool _ccw,
             const Vector3& initialNormal) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation(_scale,DEFAULT_ORIENTATION_LIST,_knot)),
    __solid(_solid),
    __ccw(_ccw),
    __initialNormal(initialNormal){
    GEOM_ASSERT(isValid());
}


Extrusion::Extrusion(const LineicModelPtr& _axis,
               const Curve2DPtr& _crossSection,
               const RealArrayPtr& _orientation,
               const RealArrayPtr& _knot,
               const bool _solid,
               const bool _ccw,
             const Vector3& initialNormal) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation(DEFAULT_SCALE_LIST,_orientation,_knot)),
    __solid(_solid),
    __ccw(_ccw),
    __initialNormal(initialNormal){
    GEOM_ASSERT(isValid());
}


Extrusion::~Extrusion(){
}

/* ----------------------------------------------------------------------- */

const Curve2DPtr&
Extrusion::getCrossSection() const {
  return __crossSection;
}

Curve2DPtr&
Extrusion::getCrossSection() {
  return __crossSection;
}

const LineicModelPtr&
Extrusion::getAxis() const {
  return __axis;
}

LineicModelPtr&
Extrusion::getAxis() {
  return __axis;
}

const ProfileTransformationPtr&
Extrusion::getProfileTransformation() const {
  return __profile;
}

ProfileTransformationPtr&
Extrusion::getProfileTransformation(){
  return __profile;
}

bool
Extrusion::isACurve( ) const {
  return false;
}

bool
Extrusion::isASurface( ) const {
  return !__solid;
}

bool
Extrusion::isAVolume( ) const {
  return __solid;
}

bool
Extrusion::isSolidToDefault() const{
  return (__solid == Mesh::DEFAULT_SOLID);
}

const bool
Extrusion::getSolid() const {
  return __solid;
}

bool&
Extrusion::getSolid() {
  return __solid;
}

bool
Extrusion::isCCWToDefault() const {
  return (__ccw == Mesh::DEFAULT_CCW);
}

const bool
Extrusion::getCCW() const {
  return __ccw;
}

bool&
Extrusion::getCCW() {
  return __ccw;
}

/// Return the Scaling Factor List value.
const Point2ArrayPtr& Extrusion::getScale() const
{ return __profile->getScale(); }

/// Return the Scaling Factor List field.
Point2ArrayPtr& Extrusion::getScale()
{ return __profile->getScale(); }

/// Return the Orientation Factor List value.
const RealArrayPtr& Extrusion::getOrientation() const
{ return __profile->getOrientation(); }

/// Return the Orientation Factor List field.
RealArrayPtr& Extrusion::getOrientation()
{ return __profile->getOrientation(); }

/// Return the KnotList Factor List value.
const RealArrayPtr Extrusion::getKnotList() const
{ return __profile->getKnotList(); }

/// Return the KnotList Factor List field.
RealArrayPtr& Extrusion::getKnotList()
{ return __profile->getKnotList(); }

/* ----------------------------------------------------------------------- */


bool
Extrusion::isValid( ) const {
  Builder _builder;
  RealArrayPtr _knot;
  _builder.Axis = const_cast<LineicModelPtr *>(&__axis);
  _builder.CrossSection = const_cast<Curve2DPtr *>(&__crossSection);
  if(__profile){
    if(!isScaleToDefault())
      _builder.Scale = const_cast<Point2ArrayPtr *>(&(__profile->getScale()));
      if(!isOrientationToDefault())
    _builder.Orientation = const_cast<RealArrayPtr *>(&(__profile->getOrientation()));
      if(!__profile->isKnotListToDefault()){
    _knot = __profile->getKnotList();
    _builder.KnotList = const_cast<RealArrayPtr *>(&(_knot));
      }
  }
  return _builder.isValid();
}

bool
Extrusion::isScaleToDefault() const {
  if((__profile)&&(__profile->getScale()))return (__profile->getScale() == DEFAULT_SCALE_LIST);
  else return true;
}

bool
Extrusion::isOrientationToDefault() const {
  if((__profile)&&(__profile->getOrientation()))return (__profile->getOrientation() == DEFAULT_ORIENTATION_LIST);
  else return true;
}

bool
Extrusion::isKnotListToDefault() const {
  if(__profile)return (__profile->isKnotListToDefault());
  else return true;
}

SceneObjectPtr
Extrusion::copy(DeepCopier& copier) const {
  Extrusion * ptr = new Extrusion(*this);
  copier.copy_object_attribute(ptr->getAxis());
  copier.copy_object_attribute(ptr->getCrossSection());
  if(ptr->__profile) ptr->__profile = ptr->__profile->deepcopy(copier);
  if(__profile){
    Point2ArrayPtr scale((__profile->getScale()?new Point2Array(*(__profile->getScale())):0));
    RealArrayPtr orientation((__profile->getOrientation()?new RealArray(*(__profile->getOrientation())):0));
    RealArrayPtr knot((__profile->getKnotList()?new RealArray(*(__profile->getKnotList())):0));
    ptr->getProfileTransformation() = ProfileTransformationPtr(new ProfileTransformation(scale,orientation,knot));
  }
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

// Get the value of initial normal
Vector3 Extrusion::getInitialNormalValue() const {
    Vector3 _normal( __initialNormal );
    if( normSquared(_normal) > GEOM_EPSILON )
    { return _normal; }
    _normal = __axis->getNormalAt(__axis->getFirstKnot());
    if( normSquared(_normal) < GEOM_EPSILON )
    {
      // normal==0 : curve is locally like a line
      Vector3 tg( __axis->getTangentAt(__axis->getFirstKnot()) );
      Vector3 U=
        ( tg.x() < tg.y() ) ? ( ( tg.z() < tg.x() ) ? Vector3::OZ
                                                    : Vector3::OX )
                            : ( ( tg.z() < tg.y() ) ? Vector3::OZ
                                                    : Vector3::OY );

      _normal= cross(tg,U);
      }
    return _normal;
}

// Get the point value at u,v.
Vector3 Extrusion::getPointAt(real_t u, real_t v) const {
  return getPointAt(u,v,getFrameAt(u));
}

// Get the point value at u,v.
Vector3 Extrusion::getPointAt(real_t u, real_t v, const Matrix3& frame) const {
  Vector3 point = getCrossSection()->getPointAt(v);
  if(getProfileTransformation()){
    real_t u_trans = getProfileTransformation()->getUMin() + (getProfileTransformation()->getUMax()-getProfileTransformation()->getUMin())*(u - getAxis()->getFirstKnot())/(getAxis()->getLastKnot() - getAxis()->getFirstKnot());
    Matrix3TransformationPtr trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans));
    if(trans){
      Matrix3 _transf2D = trans->getMatrix();
      point = _transf2D * point;
    }
  }
  point = frame * point;
  point += getAxis()->getPointAt(u);
  return point;
}

Vector3 Extrusion::getUTangentAt(real_t u, real_t v) const {
  return getUTangentAt(u, v, getFrameAt(u));
}

Vector3 Extrusion::getUTangentAt(real_t u, real_t v, const Matrix3& frame) const {
  // A'(u) +(R'(u)S(u) + R(u)S'(u)) C(v)
  Vector3 avector = getAxis()->getTangentAt(u);

  real_t ds = 0.01;
  if ((u+ds - getAxis()->getLastKnot()) > -GEOM_EPSILON){
     u = getAxis()->getLastKnot()-ds-GEOM_EPSILON;
  }

  Vector3 cvector = Vector3(getCrossSection()->getPointAt(v),0);

  Matrix3 framedu = getNextFrameAt(u, frame, ds);
  Matrix3 dframeu = (framedu-frame)/ds;


  if(getProfileTransformation()){
    real_t u_trans = getProfileTransformation()->getUMin() + 
            (getProfileTransformation()->getUMax()-getProfileTransformation()->getUMin())*
            (u - getAxis()->getFirstKnot())/(getAxis()->getLastKnot() - getAxis()->getFirstKnot());
    Matrix3TransformationPtr trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans));
    if (trans){ 
      Matrix3 transf_u = trans->getMatrix();
      trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans+ds));
      Matrix3 transf_du = (trans->getMatrix() - transf_u)/ds;

      dframeu = dframeu * transf_u + frame * transf_du;

    }
  }
  return avector  + dframeu * cvector;
}

Vector3 Extrusion::getVTangentAt(real_t u, real_t v) const {
  return getVTangentAt(u,v,getFrameAt(u));
}

Vector3 Extrusion::getVTangentAt(real_t u, real_t v, const Matrix3& frame) const {
  // R(u) S(u) C'(v)
  Vector3 vector = getCrossSection()->getTangentAt(v);

  if(getProfileTransformation()){
    real_t u_trans = getProfileTransformation()->getUMin() + (getProfileTransformation()->getUMax()-getProfileTransformation()->getUMin())*(u - getAxis()->getFirstKnot())/(getAxis()->getLastKnot() - getAxis()->getFirstKnot());
    Matrix3TransformationPtr trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans));
    if (trans){ 
      Matrix3 _transf2D = trans->getMatrix();
      vector = _transf2D * vector;
    }
  }
  vector = frame * vector;
  return vector;
}

Vector3 Extrusion::getNormalAt(real_t u, real_t v) const{
    return getNormalAt(u,v,getFrameAt(u));
}

Vector3 Extrusion::getNormalAt(real_t u, real_t v, const Matrix3& frame) const{
    GEOM_ASSERT( u >= getFirstUKnot( ) && u <= getLastUKnot( ) && v>= getFirstVKnot( ) && v<=getLastVKnot( ));
    Vector3 _utangent = getUTangentAt(u,v,frame);
    _utangent.normalize();
    Vector3 _vtangent = getVTangentAt(u,v,frame);
    _vtangent.normalize();
    return cross(_utangent,_vtangent);
}

// Vector3 getDerivativeAt(real_t u, real_t v, int du, int dv) const;

  // Get the matrix frame value at u.
Matrix3 Extrusion::getFrameAt(real_t u) const {
    real_t _start = getAxis()->getFirstKnot();
    uint_t _size =  getAxis()->getStride();
    real_t _step =  (getAxis()->getLastKnot()-_start) / (real_t) _size;
    uint_t nbsteps = (uint_t)((u -_start)/ _step);

    Vector3 _oldBinormal;
    Vector3 _velocity;
    Vector3 _binormal;
    Vector3 _normal( getInitialNormalValue() );

    for (uint_t _i = 0; _i <= nbsteps; _i++) {
        _velocity = getAxis()->getTangentAt(_start);
        if(_i!=0) {
            _normal = cross(_oldBinormal,_velocity);
            real_t normnormal = norm(_normal);
            if (normnormal == 0){
              _normal = _velocity.anOrthogonalVector();
              _normal.normalize();
            }
            else _normal /= normnormal;
          }
        _velocity.normalize();
        _normal.normalize();
        _binormal = cross(_velocity,_normal);
        _binormal.normalize();
        _oldBinormal = _binormal;
    } 

    if(fabs(u-_start)>GEOM_EPSILON) {
        _velocity = getAxis()->getTangentAt(u);
        _normal = cross(_oldBinormal,_velocity);
        real_t normnormal = norm(_normal);
        if (normnormal == 0){
            _normal = _velocity.anOrthogonalVector();
            _normal.normalize();
        }
        else _normal /= normnormal;
      _velocity.normalize();
      _normal.normalize();
      _binormal = cross(_velocity,_normal);
      _binormal.normalize();
    }
    return Matrix3(_normal,_binormal,_velocity);    
}

// Get the next matrix frame value at u+du.
Matrix3 Extrusion::getNextFrameAt(real_t u, Matrix3 m, real_t du) const
{
    Vector3 _oldBinormal = m.getColumn(1);
    Vector3 _velocity = getAxis()->getTangentAt(u+du);
    Vector3 _normal = cross(_oldBinormal,_velocity);
    real_t normnormal = norm(_normal);
    if (normnormal == 0){
        _normal = _velocity.anOrthogonalVector();
        _normal.normalize();
    }
    else _normal /= normnormal;
    _velocity.normalize();
    _normal.normalize();
    Vector3 _binormal = cross(_velocity,_normal);
    _binormal.normalize();
    return Matrix3(_normal,_binormal,_velocity);    
}

NurbsCurvePtr get_nurbs_axis(LineicModelPtr axis){
  NurbsCurvePtr caxis = dynamic_pointer_cast<NurbsCurve>(axis);
  if (is_valid_ptr(caxis)) return caxis;
  BezierCurvePtr cbaxis = dynamic_pointer_cast<BezierCurve>(axis);
  if (is_valid_ptr(cbaxis)) return NurbsCurvePtr(new NurbsCurve(cbaxis->getCtrlPointList(), cbaxis->getCtrlPointList()->size()-1));
  PolylinePtr cpaxis = dynamic_pointer_cast<Polyline>(axis);
  if (is_valid_ptr(cpaxis)) return NurbsCurvePtr(new NurbsCurve(cpaxis->getPointList(), 1));
  return NurbsCurvePtr(0);
}

// Compute the derivative.
Vector3 Extrusion::getSecondDerivativeUUAt(real_t u, real_t v) const
{
    return getSecondDerivativeUUAt(u,v,getFrameAt(u));
}

// Compute the derivative.
Vector3 Extrusion::getSecondDerivativeUUAt(real_t u, real_t v, const Matrix3& frame) const
{
   // A''(u) +(R''(u)S(u) + 2 R'(u)'S(u) + R(u)S''(u)) C(v)
  real_t ds = 0.01;

  if ((u+2*ds - getAxis()->getLastKnot()) > -GEOM_EPSILON){
     u = getAxis()->getLastKnot()-ds-GEOM_EPSILON;
  }

  NurbsCurvePtr axis = get_nurbs_axis(getAxis());
  if (is_valid_ptr(axis)) {
    Vector3 avector = axis->getDerivativeAt(u,2);
    Vector3 cvector = getCrossSection()->getPointAt(v);

    Matrix3 framedu = getNextFrameAt(u, frame, ds);
    Matrix3 framedu2 = getNextFrameAt(u+ds, framedu, ds);

    Matrix3 dframeu = (framedu-frame)/ds;
    Matrix3 dframeu2 = (framedu2 - (framedu*2) + frame)/(ds*ds);

    Matrix3 T = dframeu2;

    if(getProfileTransformation()){
      real_t u_trans = getProfileTransformation()->getUMin() + (getProfileTransformation()->getUMax()-getProfileTransformation()->getUMin())*(u - axis->getFirstKnot())/(axis->getLastKnot() - axis->getFirstKnot());
      Matrix3TransformationPtr trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans));
      if (trans){ 
        Matrix3 transf_u = trans->getMatrix();
        trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans+ds));
        Matrix3 transf_udu = trans->getMatrix();
        trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans+2*ds));
        Matrix3 transf_u2du = trans->getMatrix();

        Matrix3 transf_du = (transf_u2du - (transf_udu*2) + transf_u)/(ds*ds);

        T = (dframeu2 * transf_u) + (dframeu * transf_udu*2) + (frame*transf_u2du);

      }
    }

    return avector + T * cvector;
  }
  else  return  Vector3();
}

NurbsCurve2DPtr get_nurbs_cross_section(Curve2DPtr section){
  NurbsCurve2DPtr csection = dynamic_pointer_cast<NurbsCurve2D>(section);
  if (is_valid_ptr(csection)) return csection;
  BezierCurve2DPtr cbsection = dynamic_pointer_cast<BezierCurve2D>(section);
  if (is_valid_ptr(cbsection)) return NurbsCurve2DPtr(new NurbsCurve2D(cbsection->getCtrlPointList(), cbsection->getCtrlPointList()->size()-1));
  Polyline2DPtr cpsection = dynamic_pointer_cast<Polyline2D>(section);
  if (is_valid_ptr(cpsection)) return NurbsCurve2DPtr(new NurbsCurve2D(cpsection->getPointList(), 1));
  return NurbsCurve2DPtr(0);
}

// Compute the derivative.
Vector3 Extrusion::getSecondDerivativeVVAt(real_t u, real_t v) const
{
    return getSecondDerivativeVVAt(u,v,getFrameAt(u));
}

// Compute the derivative.
Vector3 Extrusion::getSecondDerivativeVVAt(real_t u, real_t v, const Matrix3& frame) const
{
  // R(u) S(u) C''(v)
  NurbsCurve2DPtr csection = get_nurbs_cross_section(getCrossSection());
  if (is_valid_ptr(csection)) {
    Vector3 vector = Vector3(csection->getDerivativeAt(v,2),0);

    if(getProfileTransformation()){
      real_t u_trans = getProfileTransformation()->getUMin() + (getProfileTransformation()->getUMax()-getProfileTransformation()->getUMin())*(u - getAxis()->getFirstKnot())/(getAxis()->getLastKnot() - getAxis()->getFirstKnot());
      Matrix3TransformationPtr trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans));
      if (trans){ 
        Matrix3 _transf2D = trans->getMatrix();
        vector = _transf2D * vector;
      }
    }
    return frame * vector;
  }
  return Vector3();
}

// Compute the derivative.
Vector3 Extrusion::getSecondDerivativeUVAt(real_t u, real_t v) const
{
    return getSecondDerivativeUVAt(u,v,getFrameAt(u));
}

// Compute the derivative.
Vector3 Extrusion::getSecondDerivativeUVAt(real_t u, real_t v, const Matrix3& frame) const
{
  // R(u)'S'(u) C'(v)
  real_t ds = 0.01;
  if ((u+ds - getAxis()->getLastKnot()) > -GEOM_EPSILON){
     u = getAxis()->getLastKnot()-ds-GEOM_EPSILON;
  }

  Vector3 cvector = getCrossSection()->getTangentAt(v);

  Matrix3 framedu = getNextFrameAt(u, frame, ds);

  Matrix3 dframeu = (framedu-frame)/ds;


  if(getProfileTransformation()){
    real_t u_trans = getProfileTransformation()->getUMin() + (getProfileTransformation()->getUMax()-getProfileTransformation()->getUMin())*(u - getAxis()->getFirstKnot())/(getAxis()->getLastKnot() - getAxis()->getFirstKnot());
    Matrix3TransformationPtr trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans));
    if (trans){ 
      Matrix3 transf_u = trans->getMatrix();
      trans = dynamic_pointer_cast<Matrix3Transformation>((*getProfileTransformation())(u_trans+ds));
      Matrix3 transf_du = (trans->getMatrix() - transf_u)/ds;

      dframeu = dframeu * transf_du;

    }
  }

  return dframeu * cvector;
}


/* ----------------------------------------------------------------------- */

#undef DEFAULT_ORIENTATION_LIST
#undef DEFAULT_SCALE_LIST

/* ----------------------------------------------------------------------- */
