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


#include "pglturtle.h"
// #include <plantgl/gui/viewer/pglapplication.h>
#include <plantgl/scenegraph/transformation/axisrotated.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/screenprojected.h>
#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/quadset.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/geometry/frustum.h>
#include <plantgl/scenegraph/geometry/cone.h>
#include <plantgl/scenegraph/geometry/box.h>
#include <plantgl/scenegraph/geometry/sphere.h>
#include <plantgl/scenegraph/geometry/disc.h>
#include <plantgl/scenegraph/geometry/group.h>
#include <plantgl/scenegraph/geometry/text.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/math/util_math.h>
#include <plantgl/tool/util_types.h>
#include <plantgl/tool/util_string.h>

#include <sstream>

using namespace std;
PGL_USING_NAMESPACE

#define FABS(a) (a < 0 ? -(a) : a)

/*----------------------------------------------------------*/

Turtle::error_msg_handler_func ERROR_FUNC = NULL;
Turtle::error_msg_handler_func WARNING_FUNC = NULL;

void Turtle::register_error_handler(Turtle::error_msg_handler_func f)
{ ERROR_FUNC = f; }

void Turtle::register_warning_handler(Turtle::error_msg_handler_func f)
{ WARNING_FUNC = f; }

void Turtle::error(const std::string& msg){
  if (warn_on_error){
      if (ERROR_FUNC != NULL) { ERROR_FUNC(msg); }
      else { std::cerr << "*** ERROR : " << msg << std::endl; }
  }
}

void Turtle::warning(const std::string& msg){
  if (warn_on_error){
      if (WARNING_FUNC != NULL) { WARNING_FUNC(msg); }
      else { std::cerr << "*** WARNING : " << msg << std::endl; }
  }
}


/*----------------------------------------------------------*/
/*
bool curve_orientation(Curve2DPtr curve) {
  real_t _firstK = curve->getFirstKnot();
  real_t _lastK = curve->getLastKnot();
  Vector2 firstPosition = curve->getPointAt(_firstK);
  Vector2 lastPosition  = curve->getPointAt(_lastK);
  uint_t _size = curve->getStride();
  real_t _step =  (_lastK-_firstK) / (real_t) _size;
  Vector2 deltaPosition1, deltaPosition2;
  if (norm(firstPosition - lastPosition) < GEOM_EPSILON) {
      // closed contour
      lastPosition   = curve->getPointAt(_lastK - _step);
      _size -= 1;

  }
  Vector2 midPosition   = firstPosition+lastPosition;
  _firstK += _step;
  for (uint_t _i = 1; _i < _size-1; _i++) {
     midPosition += curve->getPointAt(_firstK);
     _firstK += _step;
  }
  midPosition /= _size;
  deltaPosition1   = firstPosition - midPosition;
  deltaPosition2   = lastPosition - midPosition;
  real_t cp = cross(deltaPosition1,deltaPosition2);
  printf("cp=%f\n",cp);
  if (fabs(cp) > GEOM_EPSILON) return cp > 0;
  else return (deltaPosition2.x() - deltaPosition1.x()) > 0;

}
*/
/*----------------------------------------------------------*/

TurtlePath::~TurtlePath() { }

Turtle2DPath::Turtle2DPath(Curve2DPtr curve, real_t totalLength, real_t actualLength, bool orientation, bool ccw, QuantisedFunctionPtr arclengthParam) :
    TurtlePath(totalLength,actualLength?actualLength:curve->getLength(),arclengthParam),
    __path(curve),
    __orientation(orientation),
    __lastHeading(orientation?0:1,orientation?1:0)
{

      if(is_null_ptr(arclengthParam))__arclengthParam = curve->getArcLengthToUMapping();
      __lastPosition = curve->getPointAt(curve->getFirstKnot());
      __ccw = ccw;
      if (__ccw) {
          if(orientation) { __lastHeading = Vector2(0,-1); }
          else { __lastHeading = Vector2(-1,0); }
      }
}

TurtlePathPtr Turtle2DPath::copy() const
{ return TurtlePathPtr(new Turtle2DPath(*this)); }

void Turtle2DPath::setPosition(real_t t)
{
    __actualT = t / __totalLength;
    if  (__actualT <= 0) {__actualT = 0;}
    else if (__actualT >= 1.0) {__actualT = 1.0;}
    real_t actualU = __arclengthParam->getValue(__actualT);
    __lastPosition = __path->getPointAt(actualU);
    __lastHeading = __path->getTangentAt(actualU);
    __lastHeading.normalize();
}


Turtle3DPath::Turtle3DPath(LineicModelPtr curve, real_t totalLength, real_t actualLength, QuantisedFunctionPtr arclengthParam) :
    TurtlePath(totalLength,actualLength?actualLength:curve->getLength(),arclengthParam), __path(curve), __lastHeading(0,0,1), __lastUp(1,0,0), __lastLeft(0,-1,0)
{
    if(is_null_ptr(arclengthParam))__arclengthParam = curve->getArcLengthToUMapping();
    __lastPosition = curve->getPointAt(curve->getFirstKnot());
}

void Turtle3DPath::setPosition(real_t t)
{
    __actualT = t / __totalLength;
    if  (__actualT <= 0) {__actualT = 0;}
    else if (__actualT >= 1.0){ __actualT = 1.0;}
    real_t actualU = __arclengthParam->getValue(__actualT);
    __lastPosition = __path->getPointAt(actualU);
    __lastHeading = __path->getTangentAt(actualU);
    __lastHeading.normalize();
    __lastUp = __path->getNormalAt(actualU);
    __lastUp.normalize();
    __lastLeft = cross(__lastUp,__lastHeading);
    __lastLeft.normalize();
}

TurtlePathPtr Turtle3DPath::copy() const
{ return TurtlePathPtr(new Turtle3DPath(*this)); }

/*----------------------------------------------------------*/
TurtleDrawParameter::TurtleDrawParameter():
  color(1),
  customMaterial(),
  crossSection(),
  crossSectionCCW(true),
  defaultSection(true),
  texCoordScale(1,1),
  texCoordTranslation(0,0),
  texCoordRotCenter(0.5,0.5),
  texCoordRotAngle(0),
  texBaseColor(255,255,255,0),
  axialLength(0)
{
}

void TurtleDrawParameter::reset(){
  color = 1;
  customMaterial = AppearancePtr();
  texCoordScale = Vector2(1,1);
  texCoordTranslation = Vector2(0,0);
  texCoordRotCenter = Vector2(0.5,0.5);
  texCoordRotAngle = 0;
  texBaseColor = Color4(255,255,255,0);
  crossSection = Curve2DPtr();
  crossSectionCCW = true;
  defaultSection = true;
  axialLength = 0;
}


TurtleParam::TurtleParam() :
  TurtleDrawParameter(),
  position(0,0,0),
  heading(0,0,1),
  left(0,-1,0),
  up(1,0,0),
  scale(1,1,1),
  reflection(1,1,1),
  lastId(Shape::NOID),
  width(0.1f),
  tropism(0,0,1),
  elasticity(0),
  screenCoordinates(false),
  __polygon(false),
  __generalizedCylinder(false),
  pointList(new Point3Array()),
  customId(Shape::NOID),
  customParentId(Shape::NOID),
  sectionResolution(Cylinder::DEFAULT_SLICES),
  initial()
{
}

TurtleParam::~TurtleParam() {}

void
TurtleParam::reset(){
  TurtleDrawParameter::reset();
  position = Vector3(0,0,0);
  heading  = Vector3(0,0,1);
  left     = Vector3(0,-1,0);
  up       = Vector3(1,0,0);
  scale    = Vector3(1,1,1);
  reflection = Vector3(1,1,1);
  lastId = Shape::NOID;
  customId = Shape::NOID;
  customParentId = Shape::NOID;
  sectionResolution = Cylinder::DEFAULT_SLICES;
  width = 0.1f;
  tropism = Vector3(0,0,1);
  elasticity = 0;
  screenCoordinates = false;
  __polygon = false;
  __generalizedCylinder = false;
  pointList->clear();
  leftList.clear();
  radiusList.clear();
  initial.reset();
  guide = TurtlePathPtr();
}

TurtleParam * TurtleParam::copy(){
  TurtleParam * t = new TurtleParam(*this);
  if(t->guide)t->guide = t->guide->copy();
  if(!__polygon)t->pointList = new Point3Array(*t->pointList);
  return t;
}

void TurtleParam::dump(){
   std::cerr << "Position : " << position << std::endl;
   std::cerr << "Heading  : " << heading  << std::endl;
   std::cerr << "Left     : " << left << std::endl;
   std::cerr << "Up       : " << up << std::endl;
   std::cerr << "Color    : " << color << std::endl;
   std::cerr << "Width    : " << width << std::endl;
}

void TurtleParam::transform(const Matrix3& m){
   heading = m * heading;
   left = m * left;
   up = m * up;
}

Matrix3 TurtleParam::getOrientationMatrix() const{
   return Matrix3(heading,left,up);
}

Matrix4 TurtleParam::getTransformationMatrix() const{
   return Matrix4(heading*scale.x(),left*scale.y(),up*scale.z(),position);
}

bool TurtleParam::isValid() const{
   return heading.isNormalized() &&
          left.isNormalized() &&
          up.isNormalized() &&
          heading.isOrthogonalTo(left) &&
          heading.isOrthogonalTo(up) &&
          left.isOrthogonalTo(up);
}

void TurtleParam::keepLastPoint(){
  if(!pointList->empty()){
    Vector3 lastp = *(pointList->end()-1);
    pointList->clear();
    pointList->push_back(lastp);
    // pointList = vector<Vector3>(1,*(pointList.end()-1));
    // pointList = vector<Vector3>(1,*(pointList.end()-1));
  }
  if(!leftList.empty())
    leftList = vector<Vector3>(1,*(leftList.end()-1));
  if(!radiusList.empty())
    radiusList = vector<real_t>(1,*(radiusList.end()-1));
}

void TurtleParam::removePoints(){
  pointList->clear();
  leftList.clear();
  radiusList.clear();
}

void TurtleParam::polygon(bool t){
  __polygon = t;
  removePoints();
  if (t){
    initial.color = color;
    initial.customMaterial = customMaterial;
  }
  else {
    initial.reset();
  }
}

void TurtleParam::generalizedCylinder(bool t){
  __generalizedCylinder = t;
  removePoints();
  if (t){
    initial = TurtleDrawParameter(*this);
  }
  else {
    initial.reset();
  }

}

void TurtleParam::pushPosition(){
  pointList->push_back(position);
  if(__generalizedCylinder) {
    leftList.push_back(left);
    radiusList.push_back(width);
  }
}

void TurtleParam::pushRadius(){
    *(radiusList.end()-1) = width;
}

/*----------------------------------------------------------*/

Turtle::Turtle(TurtleParam * params):
  __params( params ? params : new TurtleParam()),
  angle_increment(60),
  width_increment(1),
  color_increment(1),
  scale_multiplier(0.5),
  default_step(1),
  id(Shape::NOID),
  parentId(Shape::NOID),
  warn_on_error(true),
  path_info_cache_enabled(true)
{
    if (!__params->crossSection) setDefaultCrossSection();
    assert (__params->crossSection && "Failed to initialize cross section");
}

Turtle::~Turtle() { }

string
Turtle::str() const {
    stringstream ss;
    ss << "Turtle : "
      << getPosition() << ","
      << getHeading() << ","
      << getLeft() << ","
      << getUp() << std::ends;
    return ss.str();
}

void Turtle::reset(){
  resetValues();
}

void Turtle::resetValues(){
    __params->reset();
    if (!__params->crossSection) setDefaultCrossSection();
    __paramstack = stack<TurtleParam *>();
    __pathinfos.clear();
}

void Turtle::dump() const{
    __params->dump();
}

bool Turtle::isValid() const{
    return __params->isValid();
}

void Turtle::start(){
    reset();
}

void Turtle::incId(uint_t i)
{
    if (getId() <= UINT32_MAX-i) setId(getId()+i);
    else error("Id should be a valid positive value.");
}

void Turtle::decId(uint_t i)
{
    if (getId() >= i) setId(getId()-i);
    else error("Id should be a valid positive value.");
}

void Turtle::setNoId(){
    setId(Shape::NOID);
}

uint_t Turtle::popId()
{
    uint_t nid = id;
    parentId = id;
    return nid;
}

void Turtle::stop(){
  if(__params->isGeneralizedCylinderOn()){
      if(__params->pointList->size() > 1){
        _generalizedCylinder(__params->pointList,
                           __params->leftList,
                           __params->radiusList,
                           __params->initial.crossSection,
                           __params->initial.crossSectionCCW);
      }
    __params->removePoints();
  }
}

  void Turtle::push(){
    __params->lastId = parentId;
    __paramstack.push(__params->copy());
    if(__params->isGeneralizedCylinderOn()){
        __params->keepLastPoint();
        __params->customParentId = __params->customId;
        __params->customId = getId();
        parentId = getId();
    }
  }

  void Turtle::pop(){
    if (__params->isGeneralizedCylinderOn()){
        if(__params->pointList->size() > 1){
            _generalizedCylinder(__params->pointList,
                                 __params->leftList,
                                 __params->radiusList,
                                 __params->initial.crossSection,
                                 __params->initial.crossSectionCCW);
        }
    }
    if(!__paramstack.empty()){
      delete __params;
      __params = __paramstack.top();
      __paramstack.pop();
      parentId = __params->lastId;
    }
    else {
      error("Empty Turtle Stack: Cannot pop");
    }
  }

  void Turtle::f(real_t length){
      // if(length > 0){
          if (length > 0 && __params->guide) _applyGuide(length);
          if (__params->elasticity > GEOM_EPSILON){
              _applyTropism();
          }
          __params->position += __params->heading*length*getScale().z();
          // __params->axialLength += length;
          if (length > 0 && __params->guide) _ajustToGuide();
      // }
      // else if(fabs(length) < GEOM_EPSILON) warning("f length should be non null.");
      // else error("f length should be positive.");
  }

  void Turtle::F(real_t length,real_t topradius){
      if(length > 0){
          if (__params->guide) _applyGuide(length);
          if (__params->elasticity > GEOM_EPSILON){
              _applyTropism();
          }
          if (length > GEOM_EPSILON){
              if (!__params->isGeneralizedCylinderOn()){
                      if(__params->defaultSection && __params->width > -GEOM_EPSILON){
                        if (topradius < -GEOM_EPSILON)  _cylinder(length);
                        else _frustum(length,topradius);
                      }
                      else { _sweep(length,topradius < -GEOM_EPSILON?getWidth():topradius); }
              }
              __params->position += __params->heading*length*getScale().z();
              __params->axialLength += length;
          }
          if (__params->guide) _ajustToGuide();
          if (length > GEOM_EPSILON && (__params->isGeneralizedCylinderOn() || __params->isPolygonOn()))
              __params->pushPosition();
          if (topradius > -GEOM_EPSILON ) {
              setWidth(topradius);
          }
      }
      // else if(fabs(length) < GEOM_EPSILON) warning("F length should be non null.");
      // else error("F length should be positive.");
      else if(length < -GEOM_EPSILON) {
          turnAround(); F(-length,topradius); turnAround();

      }
  }

void Turtle::nF(real_t length, real_t dl)
{
    uint32_t nbSteps = uint32_t(length / dl);
    real_t deltaStep = length - nbSteps * dl;
    for (uint32_t i = 1; i <= nbSteps; ++i){ F(dl); }
    if(deltaStep > GEOM_EPSILON) F(deltaStep);
}

void Turtle::nF(real_t length, real_t dl, real_t radius, const QuantisedFunctionPtr radiusvariation)
{
    uint32_t nbSteps = uint32_t(length / dl);
    real_t deltaStep = length - nbSteps * dl;
    if (!is_null_ptr(radiusvariation)){
        real_t fx = radiusvariation->getFirstX(), lx = radiusvariation->getLastX();
        real_t du = (dl / length) * (lx-fx);
        real_t uiter = fx;
        setWidth(radiusvariation->getValue(uiter)*radius);
        for (uint32_t i = 1; i <= nbSteps; ++i){
            uiter += du;
            F(dl,radiusvariation->getValue(uiter)*radius);
        }
        if(deltaStep > GEOM_EPSILON) F(deltaStep,radius*radiusvariation->getValue(lx));
    }
    else {
        real_t w = getWidth();
        real_t dw = (radius - w) / length;
        real_t cw = w;
        for (uint32_t i = 1; i <= nbSteps; ++i){ cw += dw*dl; F(dl, cw); }
        if(deltaStep > GEOM_EPSILON) F(deltaStep, radius);
    }
}

void Turtle::left(real_t angle){
  real_t ra = angle * GEOM_RAD *__params->reflection.y();
  Matrix3 m = Matrix3::axisRotation(__params->up,ra);
  __params->heading = m*__params->heading;
  __params->left = m*__params->left;
}

void Turtle::down(real_t angle){
    real_t ra = angle * GEOM_RAD;
    Matrix3 m = Matrix3::axisRotation(__params->left,ra) *__params->reflection.z();
    __params->heading = m*__params->heading;
    __params->up = m*__params->up;
  }

void Turtle::rollL(real_t angle){
    real_t ra = angle * GEOM_RAD;
    Matrix3 m = Matrix3::axisRotation(__params->heading,ra) *__params->reflection.x();
    __params->up = m*__params->up;
    __params->left = m*__params->left;
  }

void Turtle::iRollL(real_t angle){
    real_t ra = angle * GEOM_RAD *__params->reflection.x();
    Matrix3 m = Matrix3::axisRotation(__params->heading,ra);
    __params->up = m*__params->up;
    __params->left = m*__params->left;
    if (__params->guide && !__params->guide->is2D()){
        Turtle3DPath * guide = (Turtle3DPath *)__params->guide.get();
        Matrix3 m2 = Matrix3::axisRotation(guide->__lastHeading,ra);
        guide->__lastUp   = m2*guide->__lastUp;
        guide->__lastLeft = m2*guide->__lastLeft;
    }
    else warning("No appropriate turtle embedding (Guide) specified for intrinsic roll.");
}

void Turtle::rollToVert(real_t alpha, const Vector3& top){
    if (alpha <= 0.0) return;
    if (alpha >= 1.0) {
        __params->left = cross(top,__params->heading);
        if (norm(__params->left) < GEOM_EPSILON )
          __params->left = Vector3(0,-1,0);
        else{
          __params->left.normalize();
        }
        __params->up = cross(__params->heading,__params->left);
    }
    else {
        Vector3 nleft = cross(top,__params->heading);
        nleft.normalize();
        real_t rotangle = angle(__params->left,nleft,__params->heading);
        rollR(rotangle * alpha * GEOM_DEG);

    }
}

    /// Roll such as head vector comes in the horizontal plane
void Turtle::rollToHorizontal(real_t alpha, const Vector3& top) {
    if (alpha <= 0.0) return;
    if (alpha >= 1.0) alpha = 1.0;
    real_t nonhorizontal = dot(__params->heading, top);
    if (fabs(nonhorizontal) > GEOM_EPSILON){
        Vector3 targetheading = __params->heading - nonhorizontal*top;
        if (norm(targetheading) < GEOM_EPSILON ) targetheading = Vector3(1,0,0);
        else targetheading.normalize();
        Vector3 rotaxis = cross(__params->heading,targetheading);
        rotaxis.normalize();
        real_t rotangle = angle(__params->heading,targetheading,rotaxis);
        Matrix3 m = Matrix3::axisRotation(rotaxis,rotangle* alpha);
        transform(m);
    }
}


void Turtle::transform(const Matrix3& matrix)
{
    __params->transform(matrix);
}

void Turtle::setHead(const Vector3& head, const Vector3& up){
    Vector3 h = head;
    real_t lh = h.normalize();
    Vector3 u = up;
    real_t lu = u.normalize();
    if (lh < GEOM_EPSILON){
      error("Heading Vector in setHead is NULL");
      return;
    }
    if (lu < GEOM_EPSILON){
      error("Up Vector in setHead is NULL");
    }
    Vector3 l = cross(u,h);
    if (norm(l) < GEOM_EPSILON){
      error("Heading and Up Vector in setHead are colinear.");
    }

    if(!h.isOrthogonalTo(u)) {
        u = cross(h,l);
        warning("Re-Orthogonalizing Up vector in setHead");
    }
    __params->heading = h;
    __params->up = u;
    __params->left = l;
  }

  void Turtle::eulerAngles(real_t azimuth, real_t elevation, real_t roll)
  {
    Matrix3 m = Matrix3::eulerRotationZYX(Vector3(azimuth * GEOM_RAD, - elevation * GEOM_RAD, roll * GEOM_RAD));
    __params->heading = m * Vector3(1,0,0);
    __params->up = m * Vector3(0,0,1);
    __params->left = cross(__params->up,__params->heading);
  }

  void Turtle::move(const Vector3& pos){
    if (__params->screenCoordinates)  __params->position = Vector3(pos.z(),pos.x(),pos.y());
    else  __params->position = pos;
  }

  void Turtle::shift(const Vector3& pos){
    if (__params->screenCoordinates)  __params->position += Vector3(pos.z(),pos.x(),pos.y());
    else __params->position += pos;
  }

  void Turtle::scale(const Vector3& s){
    __params->scale = s;
  }

  void Turtle::setColor(int v){
      if (0 <= v && v < getColorListSize()){
        __params->color = v;
        __params->customMaterial = AppearancePtr();
        __params->axialLength = 0;
        if(__params->isGCorPolygonOnInit()) __params->initial.color = v;
      }
    else {
      stringstream st;
      int maxcolor = getColorListSize();
      st << "Invalid Color value " << v << " in setColor (maximum is " <<  maxcolor - 1 << ')' <<  std::flush;
      warning(st.str());
      return;
    }
  }

void Turtle::interpolateColors(int val1, int val2, real_t alpha){

}

void Turtle::setCustomAppearance(const AppearancePtr app)
{
    __params->customMaterial = app;
    __params->axialLength = 0;
    if(__params->isGCorPolygonOnInit()) {
        if (is_valid_ptr(app)) __params->initial.customMaterial = app;
        else __params->initial.color = __params->color;
    }
}

void Turtle::setTextureScale(const Vector2& t)
{
  __params->texCoordScale = t;
  if(__params->isGCorPolygonOnInit()) __params->initial.texCoordScale = t;
}

void Turtle::setTextureUScale(real_t u)
{
  __params->texCoordScale.x() = u;
  if(__params->isGCorPolygonOnInit()) __params->initial.texCoordScale.x() = u;
}

void Turtle::setTextureVScale(real_t v)
{
  __params->texCoordScale.y() = v;
  if(__params->isGCorPolygonOnInit()) __params->initial.texCoordScale.y() = v;
}

void Turtle::setTextureRotation(real_t angle,
                                const Vector2& center)
{
  __params->texCoordRotAngle = angle;
  __params->texCoordRotCenter = center;
  if(__params->isGCorPolygonOnInit()){
      __params->initial.texCoordRotAngle = angle;
      __params->initial.texCoordRotCenter = center;
  }
}

void Turtle::setTextureTranslation(const Vector2& t)
{
  __params->texCoordTranslation = t;
  if(__params->isGCorPolygonOnInit()){
      __params->initial.texCoordTranslation = t;
  }
}

void Turtle::setTextureTransformation(const Vector2& scaling, const Vector2& translation,
                                      real_t angle, const Vector2& rotcenter)
{
  setTextureScale(scaling);
  setTextureTranslation(translation);
  setTextureRotation(angle,rotcenter);
}

void Turtle::setTextureBaseColor(const Color4& v)
{
    __params->texBaseColor = v;
}

void Turtle::setTextureBaseColor(int val1){
}

void Turtle::interpolateTextureBaseColors(int val1, int val2, real_t alpha){
}

void Turtle::setWidth(real_t v){
    if (v > -GEOM_EPSILON){
        __params->width = v;
        if(__params->isGeneralizedCylinderOn())
            __params->pushRadius();
    }
    else {
        warning("Invalid width value in setWidth. Must be positive");
    }
}

  void Turtle::lineTo(const Vector3 & v, real_t topradius){
      Vector3 heading = __params->heading;
      Vector3 left = __params->left;
      Vector3 up = __params->up;

      oLineTo(v,topradius);

      __params->heading = heading;
      __params->left = left;
      __params->up = up;
}

  void Turtle::lineRel(const Vector3 & v, real_t topradius){
      Vector3 heading = __params->heading;
      Vector3 left = __params->left;
      Vector3 up = __params->up;

      oLineRel(v,topradius);

      __params->heading = heading;
      __params->left = left;
      __params->up = up;
  }

/// Trace line toward v and change the orientation
void Turtle::oLineTo(const Vector3& v, real_t topradius )
{
    Vector3 h = v - getPosition();
    real_t l = h.normalize();
    if (l > GEOM_EPSILON){
        _tendTo(h);
        F(l,topradius);
    }
}



  void Turtle::oLineRel(const Vector3& v, real_t topradius){
      // Vector3 h = v.x()*getUp()-v.y()*getLeft()+v.z()*getHeading();

      Vector3 h = (__params->screenCoordinates ? Vector3(v.z(),v.x(),v.y()): v);
      real_t l = h.normalize();
      if (l > GEOM_EPSILON){
          _tendTo(h);
          F(l,topradius);
      }
  }

  void  Turtle::pinpoint(const Vector3 & v){
      Vector3 h = (__params->screenCoordinates ? Vector3(v.z(),v.x(),v.y()):v) - getPosition();
      real_t l = h.normalize();
      if (l > GEOM_EPSILON){
          _tendTo(h);
      }
  }

  void Turtle::pinpointRel(const Vector3 & v){
      // Vector3 h = v.x()*getUp()-v.y()*getLeft()+v.z()*getHeading();
    Vector3 h = (__params->screenCoordinates ? Vector3(v.z(),v.x(),v.y()):v);
    real_t l = h.normalize();
    if (l > GEOM_EPSILON){
        _tendTo(h);
    }
  }

  void Turtle::startPolygon(){
      __params->polygon(true);
    __params->customId = popId();
    __params->customParentId = parentId;
    // __params->pushPosition();
  }

  void Turtle::stopPolygon(bool concavetest){
      if(__params->pointList->size() > 2)_polygon(__params->pointList,concavetest);
      __params->polygon(false);
      __params->customId = Shape::NOID;
      __params->customParentId = Shape::NOID;
  }

  void Turtle::polygonPoint(){
      __params->pushPosition();
  }

  void Turtle::startGC(){
      if (!__params->crossSection) setDefaultCrossSection();
      __params->generalizedCylinder(true);
      __params->customId = popId();
      __params->customParentId = parentId;
      __params->pushPosition();
  }

void Turtle::stopGC(){
    if(__params->pointList->size() > 1){
        _generalizedCylinder(__params->pointList,
                             __params->leftList,
                             __params->radiusList,
                             __params->initial.crossSection,
                             __params->initial.crossSectionCCW);
    }
    __params->generalizedCylinder(false);
    __params->customId = Shape::NOID;
    __params->customParentId = Shape::NOID;
  }

void Turtle::setCrossSection(const Curve2DPtr& curve, bool ccw)
{
    _setCrossSection(curve, ccw, false);
}

void Turtle::setDefaultCrossSection(size_t slices)
{
    _setCrossSection(Curve2DPtr(Polyline2D::Circle(1,slices)),true,true);
}

void Turtle::_setCrossSection(const Curve2DPtr& curve, bool ccw, bool defaultSection)
{
    __params->crossSection = curve;
    __params->crossSectionCCW = ccw;
    __params->defaultSection = defaultSection;
    if(__params->isGeneralizedCylinderOnInit())
    {
        __params->initial.crossSection = curve;
        __params->initial.crossSectionCCW = ccw;
        __params->initial.defaultSection = defaultSection;
    }
}

void  Turtle::setSectionResolution(uint_t resolution)
{
    getParameters().sectionResolution = resolution;
    if ((__params->crossSection) && __params->defaultSection)
        setDefaultCrossSection(resolution);

}

#define GET_PATH_INFO(curve) \
    real_t actuallength = 0; \
    QuantisedFunctionPtr arclengthParam; \
    if (path_info_cache_enabled) {  \
        size_t curveid = ptr_to_size_t(curve); \
        PathInfoMap::const_iterator it = __pathinfos.find(curveid); \
        if (it != __pathinfos.end()) { \
            actuallength = it->second.length; \
            arclengthParam = it->second.arclengthParam; \
        } \
        else { \
            actuallength = curve->getLength(); \
            arclengthParam = curve->getArcLengthToUMapping(); \
            PathInfo pi; \
            pi.length = actuallength; \
            pi.arclengthParam = arclengthParam; \
            __pathinfos[curveid] = pi; \
        } \
    }  \
    else {  \
            actuallength = curve->getLength(); \
            arclengthParam = curve->getArcLengthToUMapping(); \
    } \

void Turtle::setGuide(const Curve2DPtr& path, real_t length, bool yorientation, bool ccw)
{
    GET_PATH_INFO(path)
    getParameters().guide = TurtlePathPtr(new Turtle2DPath(path,length,actuallength,yorientation,ccw,arclengthParam));
}

void Turtle::setGuide(const LineicModelPtr& path, real_t length)
{
    GET_PATH_INFO(path)
    getParameters().guide = TurtlePathPtr(new Turtle3DPath(path,length,actuallength,arclengthParam));
}


void Turtle::setPositionOnGuide(real_t t)
{
    if(__params->guide) getParameters().guide->setPosition(t);
    else warning("Guide not set. Cannot set position on it.");
}


void Turtle::leftReflection()
{
    __params->reflection.y() = -__params->reflection.y();
}

void Turtle::upReflection()
{
    __params->reflection.z() = -__params->reflection.z();
}

void Turtle::headingReflection()
{
    __params->reflection.x() = -__params->reflection.x();
    // __params->heading = - __params->heading;
}


void Turtle::_applyTropism() {
    _tendTo(getTropism(),getElasticity());
}

void Turtle::_applyGuide(real_t& l) {
        bool local_ajustement = (fabs(l) < GEOM_EPSILON);
        real_t current = __params->guide->__actualT;
        if( current >= 1.0 && !local_ajustement) {
            __params->guide = TurtlePathPtr();
            return;
        }
        if(__params->guide->is2D()){
            Turtle2DPath * guide = (Turtle2DPath *)__params->guide.get();
            // compute position parameter
            Vector2 tangent;
            if (local_ajustement) {
                // In case of length 0, local tangent is used for redirection.
                // assert(current < (1.0+GEOM_EPSILON));
                tangent = guide->__path->getTangentAt(guide->__arclengthParam->getValue(current));
            }
            else {
                // In case of displacement with significant length, direction to next point is used as new direction
                real_t dt = (l/guide->__totalLength);
                real_t next = current + dt;
                real_t reminder = next - 1.0;
                if (reminder > GEOM_EPSILON) { next = 1.0 ; }
                real_t nextU = guide->__arclengthParam->getValue(next);
                Vector2 lastpos = guide->__lastPosition;
                Vector2 nextpos = guide->__path->getPointAt(nextU);
                // look if an offset is required to take into account the extra length out of the guide
                real_t offset_length = 0;
                Vector2 offset;
                if (reminder > GEOM_EPSILON) {
                    // we work here in the space of the curve. so we use actualLength
                    offset_length = reminder * guide->__actualLength;
                    Vector2 endTangent = guide->__path->getTangentAt(nextU) ;
                    endTangent.normalize();
                    offset = endTangent * offset_length;
                }
                tangent = (nextpos + offset) - lastpos ;

                // resize the length to be the equal to euclidian distance between the 2 points on the curve
                l = (tangent.normalize() * guide->__scale);  // we translate this length in space of turtle by multiplying by scale

                current = next;
                // save position parameter
                guide->__lastPosition = nextpos;
                guide->__actualT = current;

            }
            // Estimate deviation and turn left accordingly
            real_t deviation = angle(guide->__lastHeading,tangent);
            if (guide->__ccw) deviation *= -1;
            if (guide->__orientation == false){
                Matrix3 m = Matrix3::axisRotation(__params->left,-deviation);
                __params->heading = m*__params->heading;
                __params->up = m*__params->up;
            }
            else {
                Matrix3 m = Matrix3::axisRotation(__params->up,deviation);
                __params->heading = m*__params->heading;
                __params->left = m*__params->left;
            }
            // save position parameter
            guide->__lastHeading = tangent;
        }
        else {
            Turtle3DPath * guide = (Turtle3DPath *)__params->guide.get();
            // compute position parameter
            real_t current = guide->__actualT;
            Vector3 tangent;
            if (local_ajustement) {
                // In case of length 0, local tangent is used for redirection.
                tangent = guide->__path->getTangentAt(guide->__arclengthParam->getValue(current));
                tangent.normalize();
            }
            else {
                // In case of displacement with significant length, direction to next point is used as new direction
                real_t dt = (l/guide->__totalLength);
                real_t next = current + dt;
                real_t reminder = next - 1.0;
                if (reminder > GEOM_EPSILON) { next = 1.0 ; }
                Vector3 lastpos = guide->__lastPosition;
                real_t nextU = guide->__arclengthParam->getValue(next);
                Vector3 nextpos = guide->__path->getPointAt(nextU);
                // look if an offset is required to take into account the extra length out of the guide
                real_t offset_length = 0;
                Vector3 offset;
                if (reminder > GEOM_EPSILON) {
                    // we work here in the space of the curve. so we use actualLength
                    offset_length = reminder * guide->__actualLength ;
                    Vector3 endTangent = guide->__path->getTangentAt(nextU) ;
                    endTangent.normalize();
                    offset = endTangent * offset_length;
                }

                tangent =  (nextpos + offset) - lastpos;

                // resize the length to be the equal to euclidian distance between the 2 points on the curve
                l = (tangent.normalize() * guide->__scale);  // we translate this length in space of turtle by multiplying by scale

                current = next;
                // save position parameter
                guide->__lastPosition = nextpos;
            }

            // Previous reference frame on the curve
            const Vector3& prevH = guide->__lastHeading;
            const Vector3& prevL = guide->__lastLeft;
            const Vector3& prevU = guide->__lastUp;

            // Estimate new direction in Turtle Frame
            Vector3 projected_tangent = Vector3(dot(tangent,prevH),
                                                  dot(tangent,prevL),
                                                  dot(tangent,prevU));
            projected_tangent = __params->getOrientationMatrix() * projected_tangent;
            projected_tangent.normalize();

            // Estimate deviation and turn accordingly

            const Vector3 turtleHeading = getHeading();
            // Estimate axis of rotation for turtle and frame on curve
            Vector3 cp  = cross(turtleHeading,projected_tangent);
            Vector3 cp2 = cross(prevH,tangent);

            // Estimate angle of rotation (is similar in both case)
            real_t sinus = norm( cp );
            if (sinus > GEOM_EPSILON){
                cp /= sinus;
                real_t cosinus = dot( turtleHeading,projected_tangent);
                real_t ang = atan2( sinus, cosinus );
                // Transform turtle frame
                transform(Matrix3::axisRotation(cp,ang));
                // Compute curve frame transformation
                cp2.normalize();
                Matrix3 m = Matrix3::axisRotation(cp2,ang);
                // compute new reference frame on the curve and save them in the guide
                guide->__lastLeft = m * prevL;
                guide->__lastLeft.normalize();
                guide->__lastUp = m * prevU;
                guide->__lastUp.normalize();
            }

            // save curve parameter
            guide->__lastHeading = tangent;
            guide->__actualT = current;
        }
}

void Turtle::_ajustToGuide()
{
    // Do a applyGuide of length 0.
    // In this case local tangent will be used.
    static real_t l = 0;
    _applyGuide(l);
}


void Turtle::_tendTo(const Vector3& t, real_t strength)
{
    Vector3 h = getHeading();
    Vector3 cp = cross(h,t);
    real_t sinus = norm( cp );
    if (sinus > GEOM_EPSILON){
        cp /= sinus;
        real_t cosinus = dot( h,t );
        real_t ang = atan2( sinus, cosinus );
        transform(Matrix3::axisRotation(cp,ang*strength));
    }
    else {
        real_t cosinus = dot( h,t );
        if (cosinus < GEOM_EPSILON){
            real_t ang = GEOM_PI;
            transform(Matrix3::axisRotation(getUp(),ang*strength));
        }
    }
}


void Turtle::sphere(real_t radius )
{
  if (radius < -GEOM_EPSILON)
  { warning("Invalid radius for sphere"); }
  else if (radius > GEOM_EPSILON) _sphere(radius);
}


void Turtle::circle(real_t radius )
{
  if (radius < -GEOM_EPSILON)
  { warning("Invalid radius for circle"); }
  else if (radius > GEOM_EPSILON) _circle(radius);
}

void Turtle::box(real_t length,real_t topradius){
      if(length > 0){
          if (__params->guide) _applyGuide(length);
          if (__params->elasticity > GEOM_EPSILON) _applyTropism();

          _box(length, getWidth(), topradius);

          __params->position += __params->heading*length*getScale().z();
          __params->axialLength += length;
          if (length > 0 && __params->guide) _ajustToGuide();

          if (topradius > -GEOM_EPSILON ) {
              setWidth(topradius);

          }
      }
      // else if(fabs(length) < GEOM_EPSILON) warning("F length should be non null.");
      // else error("F length should be positive.");
      else if(length < -GEOM_EPSILON) {
          turnAround(); box(-length,topradius); turnAround();

      }
}

void Turtle::quad(real_t length,real_t topradius){
      if(length > 0){
          if (__params->guide) _applyGuide(length);
          if (__params->elasticity > GEOM_EPSILON){
              _applyTropism();
          }
          _quad(length, getWidth(), topradius);

          __params->position += __params->heading*length*getScale().z();
          __params->axialLength += length;
          if (length > 0 && __params->guide) _ajustToGuide();

          if (topradius > -GEOM_EPSILON ) {
              setWidth(topradius);

          }
      }
      // else if(fabs(length) < GEOM_EPSILON) warning("F length should be non null.");
      // else error("F length should be positive.");
      else if(length < -GEOM_EPSILON) {
          turnAround(); quad(-length,topradius); turnAround();

      }
  }

void Turtle::label(const std::string& text, int size )
{
    if(!text.empty())_label(text, size);
    else warning("Invalid text for label");
}

void Turtle::surface(const std::string& name, real_t scale)
{
    if (!name.empty() && scale > GEOM_EPSILON) _surface(name,scale);
    else {
        if(name.empty()) warning("Invalid name for surface");
        if(scale < -GEOM_EPSILON) warning("Invalid scale for surface");
    }
}

void Turtle::frame(real_t scale, real_t cap_heigth_ratio, real_t cap_radius_ratio, real_t color, real_t transparency )
{
    color = max<real_t>(0,min<real_t>(1,color));
    transparency = max<real_t>(0,min<real_t>(1,transparency));
    if (scale > GEOM_EPSILON && cap_heigth_ratio < 1 && cap_heigth_ratio > 0) _frame(scale,cap_heigth_ratio,cap_radius_ratio,color, transparency);
    else {
        if(scale < GEOM_EPSILON) warning("Invalid scale for frame. Should be positive");
        if(cap_heigth_ratio > 1 || cap_heigth_ratio < 0) warning("Invalid cap_heigth_ratio for frame. Should be in [0,1].");
        if(cap_radius_ratio < GEOM_EPSILON) warning("Invalid cap_radius_ratio for frame. Should be positive.");
    }
}

void Turtle::arrow(real_t scale, real_t cap_heigth_ratio, real_t cap_radius_ratio )
{
    if (scale > GEOM_EPSILON && cap_heigth_ratio < 1 && cap_heigth_ratio > 0) _arrow(scale,cap_heigth_ratio,cap_radius_ratio);
    else {
        if(scale < GEOM_EPSILON) warning("Invalid scale for arrow. Should be positive");
        if(cap_heigth_ratio > 1 || cap_heigth_ratio < 0) warning("Invalid cap_heigth_ratio for arrow. Should be in [0,1].");
        if(cap_radius_ratio < GEOM_EPSILON) warning("Invalid cap_radius_ratio for arrow. Should be positive.");
    }
}

void Turtle::sweep(const Curve2DPtr& path, const Curve2DPtr& section, real_t length, real_t dl, real_t radius, const QuantisedFunctionPtr radiusvariation)
{
    setGuide(path,length); setCrossSection(section); nF(length,dl,radius,radiusvariation);
}

void Turtle::sweep(const LineicModelPtr& path, const Curve2DPtr& section, real_t length, real_t dl, real_t radius, const QuantisedFunctionPtr radiusvariation)
{
    setGuide(path,length); setCrossSection(section); nF(length,dl,radius,radiusvariation);
}


void Turtle::_sweep(real_t length, real_t topradius)
{
    // default sweep
    Point3ArrayPtr points(new Point3Array(getPosition(),getPosition()+getHeading()*length));
    std::vector<Vector3> left;
    left.push_back(getLeft());
    left.push_back(getLeft());
    std::vector<real_t> radius;
    radius.push_back(getWidth());
    radius.push_back(topradius);
    _generalizedCylinder(points, left, radius,
                         __params->crossSection,
                         __params->crossSectionCCW, true);
}
/*----------------------------------------------------------*/

//Polyline2DPtr PglTurtle::DEFAULT_CROSS_SECTION(0);

/*----------------------------------------------------------*/

PglTurtle::PglTurtle(TurtleParam * param):
  Turtle(param),
  __scene(new Scene()){
   defaultValue();
}

PglTurtle::~PglTurtle() { }

void PglTurtle::reset(){
  resetValues();
  __scene = ScenePtr(new Scene());
}

void PglTurtle::clear(){
   reset();
   __appList.clear();
   __surfList.clear();
}

void PglTurtle::interpolateColors(int val1, int val2, real_t alpha){
    AppearancePtr a1 = getMaterial(val1);
    AppearancePtr a2 = getMaterial(val2);
    MaterialPtr m1 = dynamic_pointer_cast<Material>(a1);
    MaterialPtr m2 = dynamic_pointer_cast<Material>(a2);
    if (is_null_ptr(m1) || is_null_ptr(m2))
        error("Can only interpolate material. Not texture.");
    setCustomAppearance(AppearancePtr(interpolate(m1,m2,alpha)));
}

void PglTurtle::setTextureBaseColor(const Color4& v)
{
    __params->texBaseColor = v;
}

void PglTurtle::setTextureBaseColor(int val1){
    AppearancePtr a1 = getMaterial(val1);
    MaterialPtr m1 = dynamic_pointer_cast<Material>(a1);
    if (is_null_ptr(m1))
        error("Can only set base color from material. Not texture.");
    setTextureBaseColor(m1->getDiffuseColor());
}

void PglTurtle::interpolateTextureBaseColors(int val1, int val2, real_t alpha){
    AppearancePtr a1 = getMaterial(val1);
    AppearancePtr a2 = getMaterial(val2);
    MaterialPtr m1 = dynamic_pointer_cast<Material>(a1);
    MaterialPtr m2 = dynamic_pointer_cast<Material>(a2);
    if (is_null_ptr(m1) || is_null_ptr(m2))
        error("Can only interpolate material. Not texture.");
    Color3 icol3 = Color3::interpolate(m1->getDiffuseColor(),m2->getDiffuseColor(),alpha);
    setTextureBaseColor(Color4(icol3,m1->getTransparency()*(1-alpha) + m2->getTransparency()*alpha ));
}


void PglTurtle::clearColorList(){
   __appList.clear();
}

const vector<AppearancePtr>&
PglTurtle::getColorList() const{
   return __appList;
}

void
PglTurtle::setColorList(const std::vector<AppearancePtr>& applist){
    __appList = applist;
}

void PglTurtle::clearSurfaceList(){
   __surfList.clear();
}

const PglTurtle::SurfaceMap&
PglTurtle::getSurfaceList() const {
   return __surfList;
}

void PglTurtle::defaultValue(){
  __appList.clear();
  __surfList.clear();
  __appList.push_back(AppearancePtr(new Material("Color_0")));
  __appList.push_back(AppearancePtr(new Material("Color_1",Color3(65,45,15),3))); // Brown
  __appList.push_back(AppearancePtr(new Material("Color_2",Color3(30,60,10),3))); // Green
  __appList.push_back(AppearancePtr(new Material("Color_3",Color3(60,0,0),3)));     // Red
  __appList.push_back(AppearancePtr(new Material("Color_4",Color3(60,60,15),3)));// Yellow
  __appList.push_back(AppearancePtr(new Material("Color_5",Color3(0,0,60),3)));    // Blue
  __appList.push_back(AppearancePtr(new Material("Color_6",Color3(60,0,60),3))); // Purple
  Point3ArrayPtr points= Point3ArrayPtr(new Point3Array(7,Vector3(0,0,0.5)));
  points->setAt(1,Vector3(0,0,0));
  points->setAt(2,Vector3(0,-0.25,1./3));
  points->setAt(3,Vector3(0,-0.25,2./3));
  points->setAt(4,Vector3(0,0,1));
  points->setAt(5,Vector3(0,0.25,2./3.));
  points->setAt(6,Vector3(0,0.25,1./3.));
  Index3ArrayPtr indices= Index3ArrayPtr(new Index3Array(6));
  indices->setAt(0,Index3(0,2,1));
  indices->setAt(1,Index3(0,3,2));
  indices->setAt(2,Index3(0,4,3));
  indices->setAt(3,Index3(0,5,4));
  indices->setAt(4,Index3(0,6,5));
  indices->setAt(5,Index3(0,1,6));
  __surfList["l"] = GeometryPtr(new TriangleSet(points, indices));
}

/*
void PglTurtle::plot() const{
  PGLViewerApplication::display(__scene);
}
*/

void PglTurtle::appendMaterial(const AppearancePtr& mat)
{ if(mat)__appList.push_back(mat); }

void PglTurtle::insertMaterial(size_t pos, const AppearancePtr& mat)
{ if(mat)__appList.insert(__appList.begin()+pos,mat); }

void PglTurtle::setMaterial(size_t pos, const AppearancePtr& mat){
  while (__appList.size() < pos)
    __appList.push_back(AppearancePtr(new Material("Color_"+TOOLS(number(pos)))));
  if (__appList.size() == pos)
    __appList.push_back(mat);
  else __appList[pos] = mat;
}

AppearancePtr PglTurtle::getMaterial(size_t pos){
    if (pos >= __appList.size()) {
        size_t i = __appList.size();
        while (i <= pos)
            __appList.push_back(AppearancePtr(new Material("Color_"+TOOLS(number(i++)))));
    }
    return __appList[pos];
}

void PglTurtle::setColorAt(size_t pos, const Color3& mat){
  size_t i = __appList.size();
  while (i < pos)
    __appList.push_back(AppearancePtr(new Material("Color_"+TOOLS(number(i++)))));
  if (__appList.size() == pos)
    __appList.push_back(AppearancePtr(new Material("Color_"+TOOLS(number(__appList.size())),mat)));
  else __appList[pos] = AppearancePtr(new Material("Color_"+TOOLS(number(pos)),mat));
}

void
PglTurtle::appendColor(uint_t red, uint_t green, uint_t blue)
{ appendColor(Color3(red,green,blue)); }

void
PglTurtle::appendColor(float red, float green, float blue)
{ appendColor(Color3((uchar_t)red*255,(uchar_t)green*255,(uchar_t)blue*255)); }

void
PglTurtle::appendColor(const Color3& mat)
{ __appList.push_back(AppearancePtr(new Material(mat))); }

void
PglTurtle::setColorAt(size_t pos, uint_t red, uint_t green, uint_t blue )
{ setColorAt(pos,Color3(red,green,blue)); }

void
PglTurtle::setColorAt(size_t pos, float red, float green, float blue )
{ setColorAt(pos,Color3((uchar_t)red*255,(uchar_t)green*255,(uchar_t)blue*255)); }


void PglTurtle::removeColor(size_t pos){
  if (__appList.size() > pos){
    __appList.erase(__appList.begin()+pos);
  }
}

void PglTurtle::removeSurface(const string& name){
  SurfaceMap::iterator it = __surfList.find(name);
  if(it != __surfList.end())__surfList.erase(it);
}

GeometryPtr PglTurtle::getSurface(const string& name){
  SurfaceMap::iterator it = __surfList.find(name);
  if(it != __surfList.end())return it->second;
  else return GeometryPtr();
}

AppearancePtr PglTurtle::getCurrentMaterial() const{
    if (__params->customMaterial) return __params->customMaterial;
    if (getColor() < __appList.size()){
        AppearancePtr res = __appList[getColor()];
        if (res->isTexture()){
            real_t texshiftY = __params->axialLength;
            Vector2 texshift(0,texshiftY*__params->texCoordScale.y());
            if ( norm(texshift) > GEOM_EPSILON || __params->texCoordScale != Vector2(1,1) ||
                 __params->texCoordTranslation != Vector2(0,0) ||
                 __params->texCoordRotAngle != 0){

                Texture2DPtr tex = new Texture2D(
                    dynamic_pointer_cast<Texture2D>(res)->getImage(),
                    new Texture2DTransformation(__params->texCoordScale,
                                            __params->texCoordTranslation + texshift,
                                            __params->texCoordRotCenter,
                                            __params->texCoordRotAngle*GEOM_RAD),
                    __params->texBaseColor);
               return AppearancePtr(tex);
            }
            else if(__params->texBaseColor != Texture2D::DEFAULT_BASECOLOR){
                Texture2DPtr tex = new Texture2D(
                    dynamic_pointer_cast<Texture2D>(res)->getImage(),
                    Texture2DTransformationPtr(0),
                    __params->texBaseColor);
               return AppearancePtr(tex);

            }
        }
        return res;
    }
   else return AppearancePtr(new Material("Color_"+TOOLS(number(getColor()))));
}

AppearancePtr PglTurtle::getCurrentInitialMaterial() const{
    if (__params->initial.customMaterial) return __params->initial.customMaterial;
    if (__params->initial.color < __appList.size()){
        AppearancePtr res = __appList[__params->initial.color];
        if (res->isTexture() ){

            if (__params->initial.texCoordScale != Vector2(1,1) ||
                __params->initial.texCoordTranslation != Vector2(0,0) ||
                __params->initial.texCoordRotAngle != 0){
                    Texture2DPtr tex = new Texture2D(
                        dynamic_pointer_cast<Texture2D>(res)->getImage(),
                        new Texture2DTransformation(__params->initial.texCoordScale,
                                                    __params->initial.texCoordTranslation,
                                                    __params->initial.texCoordRotCenter,
                                                    __params->initial.texCoordRotAngle*GEOM_RAD),
                        __params->initial.texBaseColor);
                    return AppearancePtr(tex);
            }
            else if(__params->initial.texBaseColor != Texture2D::DEFAULT_BASECOLOR){
                Texture2DPtr tex = new Texture2D(
                    dynamic_pointer_cast<Texture2D>(res)->getImage(),
                    Texture2DTransformationPtr(0),
                    __params->initial.texBaseColor);
                return AppearancePtr(tex);

            }
        }
        return res;
    }
   else return AppearancePtr(new Material("Color_"+TOOLS(number(__params->initial.color))));
}

void PglTurtle::customGeometry(const GeometryPtr smb, real_t scale)
{
    if( FABS(scale) > GEOM_EPSILON){
    PlanarModelPtr _2Dtest = dynamic_pointer_cast<PlanarModel>(smb);
    if (is_valid_ptr(_2Dtest) && __params->screenCoordinates)
      _addToScene(transform(GeometryPtr(new Scaled(getScale()*scale,GeometryPtr(new Oriented(Vector3(0,1,0),Vector3(0,0,1),smb)))),false));
    else _addToScene(transform(GeometryPtr(new Scaled(getScale()*scale,smb)),false));
  }
}

void PglTurtle::pglShape(const GeometryPtr smb, real_t scale)
{
    customGeometry(smb,scale);
}

void PglTurtle::pglShape(const ShapePtr shape, real_t scale)
{

    AppearancePtr prevapp  = __params->customMaterial;
    setCustomAppearance(shape->getAppearance());
    customGeometry(shape->getGeometry());
    setCustomAppearance(prevapp);
}
void PglTurtle::pglShape(const ScenePtr scene, real_t scale)
{
    AppearancePtr prevapp  = __params->customMaterial;
    for(Scene::const_iterator it = scene->begin(); it != scene->end(); ++it){
        ShapePtr sh = dynamic_pointer_cast<Shape>(*it);
        if (sh) {
            setCustomAppearance(sh->getAppearance());
            customGeometry(sh->getGeometry());
        }
    }
    setCustomAppearance(prevapp);
}


GeometryPtr
PglTurtle::transform(const GeometryPtr& o, bool scaled) const{
  GeometryPtr obj = o;
  if ( scaled && getScale() !=  Vector3(1,1,1) &&
      (getScale().x() != getScale().y() ||
       getScale().y() != getScale().z() ))
       obj = GeometryPtr(new Scaled(getScale(),obj));
  if ( getUp() != Vector3::OX ||
       getLeft()   != -Vector3::OY )
       obj = GeometryPtr(new Oriented(getUp(),-getLeft(),obj));
  if ( getPosition() != Vector3::ORIGIN )
       obj = GeometryPtr(new Translated(getPosition(),obj));
  return obj;
}



void PglTurtle::_addToScene(const GeometryPtr geom, bool custompid, AppearancePtr app, bool projection)
{
  GeometryPtr mgeom = geom;
  if (projection && getParameters().screenCoordinates)
      mgeom = GeometryPtr(new ScreenProjected(GeometryPtr(new Oriented(Vector3(0,0,1),Vector3(1,0,0),mgeom)),false));

   if (custompid)
       __scene->add(Shape3DPtr(new Shape(mgeom,app?app:getCurrentInitialMaterial(),__params->customId,__params->customParentId)));
   else{
     uint_t pid = parentId;
     __scene->add(Shape3DPtr(new Shape(mgeom,app?app:getCurrentMaterial(),popId(),pid)));
   }
}

void PglTurtle::_frustum(real_t length, real_t topradius){
  if (fabs(length) > GEOM_EPSILON) {
    GeometryPtr a;

    real_t width = getWidth();
    if(FABS(width) > GEOM_EPSILON){
        real_t taper = topradius/width;
        if ( getScale() !=  Vector3(1,1,1) &&
            (getScale().x() == getScale().y() ))
            width *= getScale().x();
        if (FABS(taper) < GEOM_EPSILON)
            a = GeometryPtr(new Cone(width,length*getScale().z(),false,getParameters().sectionResolution));
        else if (FABS(taper-1.0) < GEOM_EPSILON)
            a = GeometryPtr(new Cylinder(width,length*getScale().z(),false,getParameters().sectionResolution));
        else
            a = GeometryPtr(new Frustum(width,length*getScale().z(),taper,false,getParameters().sectionResolution));
        _addToScene(transform(a));
    }
    else {
        if (FABS(topradius) < GEOM_EPSILON){
            Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(2,getPosition()));
            pts->setAt(1,getPosition()+getHeading()*length*getScale().z());
            a = GeometryPtr(new Polyline(pts));

        }
        else {
            real_t _topradius = topradius;
            if ( getScale() !=  Vector3(1,1,1) &&
                (getScale().x() == getScale().y() &&
                getScale().y() == getScale().z() ))
                _topradius *= getScale().x();
            a = GeometryPtr(new Cone(_topradius,length*getScale().z(),false,getParameters().sectionResolution));
            if (getLeft() != Vector3::OX ||
                getUp() != -Vector3::OY)
                a = GeometryPtr(new Oriented(getLeft(),-getUp(),a));
            a = GeometryPtr(new Translated(getPosition()+getHeading()*length*getScale().z(),a));
        }
        _addToScene(a);
    }
  }
}

void PglTurtle::_cylinder(real_t length){
  if (fabs(length) > GEOM_EPSILON) {
      real_t width = getWidth();
      if(FABS(width) < GEOM_EPSILON){
          Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(2,getPosition()));
          pts->setAt(1,getPosition()+getHeading()*length*getScale().z());
          _addToScene(GeometryPtr(new Polyline(pts)));
      }
      else {
        if ( getScale() !=  Vector3(1,1,1) &&
            (getScale().x() == getScale().y() ))
            width *= getScale().x();
        _addToScene(transform(GeometryPtr(new Cylinder(width,length*getScale().z(),false,getParameters().sectionResolution))));
      }
  }
}

void PglTurtle::_box(real_t length, real_t botradius,  real_t topradius){
    GeometryPtr a;
    if((FABS(botradius) < GEOM_EPSILON)&& (FABS(topradius) < GEOM_EPSILON)){
            Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(2,getPosition()));
            pts->setAt(1,getPosition()+getHeading()*length*getScale().z());
            a = GeometryPtr(new Polyline(pts));
            _addToScene(a);
   }
    else{
        if ( getScale() !=  Vector3(1,1,1) &&
            (getScale().x() == getScale().y() )){
            botradius *= getScale().x();
            topradius *= getScale().x();
        }

        if (FABS(topradius-botradius) < GEOM_EPSILON){
            real_t l = length*getScale().z()/2;
            a = GeometryPtr(new Translated(Vector3(0,0,l),GeometryPtr(new Box(Vector3(botradius,botradius,l)))));
        }
        else {
            real_t l = length*getScale().z()/2;
            a = GeometryPtr(new Translated(Vector3(0,0,l),GeometryPtr(new Tapered(botradius, topradius,PrimitivePtr(new Box(Vector3(1,1,l)))))));
         }
        _addToScene(transform(a));
    }
}

void PglTurtle::_quad(real_t length, real_t botradius,  real_t topradius){
    GeometryPtr a;
    if ( getScale() !=  Vector3(1,1,1) &&
        (getScale().x() == getScale().y() )){
        botradius *= getScale().x();
        topradius *= getScale().x();
    }
    Point3ArrayPtr points(new Point3Array());
    if (FABS(botradius) < GEOM_EPSILON){
        points->push_back(getPosition());
    }
    else {
        points->push_back(getPosition()+getLeft()*botradius);
        points->push_back(getPosition()-getLeft()*botradius);

    }
    Vector3 toppos = getPosition()+getHeading()*length*getScale().z();
    if (FABS(topradius) < GEOM_EPSILON){
        points->push_back(toppos);
    }
    else {
        points->push_back(toppos-getLeft()*topradius);
        points->push_back(toppos+getLeft()*topradius);
    }

    switch (points->size()){
        case 2:
            a = GeometryPtr(new Polyline(points));
            break;
        case 3:
            {
                Index3ArrayPtr ind(new Index3Array());
                ind->push_back(Index3(0,1,2));
                a = GeometryPtr(new TriangleSet(points,ind));
            }
            break;
        default:
        case 4:
            {
                Index4ArrayPtr ind(new Index4Array());
                ind->push_back(Index4(0,1,2,3));
                a = GeometryPtr(new QuadSet(points,ind));
            }
            break;
    }
     _addToScene(a);
}



GeometryPtr PglTurtle::DEFAULT_SPHERE;

void PglTurtle::_sphere(real_t radius){
  if (getParameters().sectionResolution == Cylinder::DEFAULT_SLICES){
      if (is_null_ptr(DEFAULT_SPHERE))DEFAULT_SPHERE = GeometryPtr(new Sphere(1));
      _addToScene(transform(GeometryPtr(new Scaled(getScale() * radius,DEFAULT_SPHERE)),false));

  }
  else {
      bool anisotropicscaling = true;
      if ( getScale() !=  Vector3(1,1,1) &&
            (getScale().x() == getScale().y() &&
             getScale().y() == getScale().z() ))
      {
            anisotropicscaling = false;
      }
      if (anisotropicscaling)
            _addToScene(transform(GeometryPtr(new Sphere(radius * getScale().x(),getParameters().sectionResolution,getParameters().sectionResolution)),false));
      else
          _addToScene(transform(GeometryPtr(new Sphere(radius,getParameters().sectionResolution,getParameters().sectionResolution))));
  }
}

GeometryPtr PglTurtle::getCircle(real_t radius) const{
   int res = getParameters().sectionResolution;
   Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(res+1,Vector3(0,1,0)));
   real_t angdelta = (2*GEOM_PI)/res;
   real_t angle = 0;
   for (int i = 1; i < res; i++){
     angle += angdelta;
     pts->setAt(i,Vector3(0,cos(angle),sin(angle)));
   }
   pts->setAt(res,Vector3(0,1,0));
   Vector3 scale = getScale()* radius;
   return GeometryPtr(new Scaled(scale,GeometryPtr(new Polyline(pts))));
}

void
PglTurtle::_circle(real_t radius){
  if (radius < GEOM_EPSILON)
  { warning("Invalid radius for circle"); return; }
  real_t rad = radius;
  if ( getScale() !=  Vector3(1,1,1) &&
        (getScale().x() == getScale().y() &&
         getScale().y() == getScale().z() ))
        rad *= getScale().x();
  _addToScene(transform(GeometryPtr(new AxisRotated(Vector3::OY,GEOM_HALF_PI,GeometryPtr(new Disc(rad,getParameters().sectionResolution)) )) ));
//   __scene->add(Shape(transform(getCircle(radius),false),getCurrentMaterial()));
}

void PglTurtle::_surface(const string& name,real_t scale){
  SurfaceMap::const_iterator it = __surfList.find(name);
  if (it == __surfList.end()){
    error("Unknown surface '" + name + '\'');
  }
  GeometryPtr obj(new Scaled(getScale()*scale,it->second));
  _addToScene(transform(obj,false));
}

#include "plantgl/algo/base/tesselator.h"

void
PglTurtle::_polygon(const Point3ArrayPtr& pointList, bool concavetest){
  size_t s = pointList->size();
  Point3ArrayPtr points ;
  if (norm(pointList->getAt(0) - pointList->getAt(s-1)) < GEOM_EPSILON){
      points = Point3ArrayPtr(new Point3Array(pointList->begin(),pointList->end()-1));
      s -= 1;
  }
  else  points = Point3ArrayPtr(new Point3Array(*pointList));

  if (s > 2 && concavetest){
      Vector3 v0 = points->getAt(0), v1 = points->getAt(1), v2 = points->getAt(2);
      Vector3 normal = - cross(v1-v0,v2-v0); // - is for clockwise orientation
      normal.normalize();
      assert(fabs(norm(normal) - 1.0) < GEOM_EPSILON);
      Vector3 i = v1 - v0;
      i.normalize();
      assert(fabs(norm(i) - 1.0) < GEOM_EPSILON);
      Vector3 j = cross(normal,i);
      assert(fabs(norm(j) - 1.0) < GEOM_EPSILON);
      Point2ArrayPtr points2(new Point2Array(points->size()));
      points2->setAt(0,Vector2(0,0));
      Point2Array::iterator itP2 = points2->begin()+1;
      for(Point3Array::const_iterator itP = points->begin()+1; itP != points->end(); ++itP, ++itP2){
            Vector3 res = *itP - v0;
            *itP2 = Vector2(dot(res,i),dot(res,j));
            if( itP2 != points2->begin())
                assert(norm(*itP2-*(itP2-1)) > GEOM_EPSILON);
      }
      IndexArrayPtr iarray = polygonization(points2,eConvexTriangulation);
      GeometryPtr t = GeometryPtr(new FaceSet(points,iarray));
      _addToScene(t,true);
  }
  else {
    Index3ArrayPtr ind(new Index3Array(s-2));
    for (int i=0; i < s-2; i++)
        ind->setAt(i,Index3(0,i+1,i+2));
    GeometryPtr t = GeometryPtr(new TriangleSet(points,ind));
    _addToScene(t,true);
  }
}

void
PglTurtle::_generalizedCylinder(const Point3ArrayPtr& points,
                                const vector<Vector3>& leftList,
                                const vector<real_t>& radiusList,
                                const Curve2DPtr& crossSection,
                                bool crossSectionCCW,
                                bool currentcolor){
  if (points->size() == 2 && norm(points->getAt(0) - points->getAt(1)) < GEOM_EPSILON) return;
  LineicModelPtr axis = LineicModelPtr(new Polyline(Point3ArrayPtr(
                          new Point3Array(*points))));
  Point2ArrayPtr radius(new Point2Array(radiusList.size()));
  Point2Array::iterator it2 = radius->begin();
  for (std::vector<real_t>::const_iterator it = radiusList.begin();
        it != radiusList.end(); it++){
          *it2 = Vector2(*it,*it); it2++;
  }
  Curve2DPtr mcrossSection = crossSection;
  if (!mcrossSection) {
      mcrossSection = Curve2DPtr(Polyline2D::Circle(1,__params->sectionResolution));
      crossSectionCCW = true;
      // error("Invalid cross section");
  }
  assert (mcrossSection);
  Extrusion * extrusion = new Extrusion(axis,mcrossSection,radius);
  extrusion->getCCW() = crossSectionCCW;
  extrusion->getInitialNormal() = leftList[0];
  _addToScene(GeometryPtr(extrusion),!currentcolor);
}

void
PglTurtle::_label(const string& text, int size ){
  FontPtr font;
  if (size > 0) font = FontPtr(new Font("",size));
  if (__params->screenCoordinates){
    Vector3 p = (getPosition() + Vector3(1,1,1))*50;
     _addToScene(GeometryPtr(new Text(text, Vector3(p.y(),p.z(),p.x()) , true, font)), false, NULL, false);

  }
  else {
    GeometryPtr obj(new Text(text, Vector3(0,0,0), false, font));
    if ( getPosition() != Vector3::ORIGIN )
       obj = GeometryPtr(new Translated(getPosition(),obj));
    _addToScene(obj);
  }
}

AppearancePtr PglTurtle::HEADING_FRAME_MATERIAL(new Material("HEADING_FRAME_MATERIAL",Color3(250,50,50),1));
AppearancePtr PglTurtle::UP_FRAME_MATERIAL(new Material("UP_FRAME_MATERIAL",Color3(50,50,250),1));
AppearancePtr PglTurtle::LEFT_FRAME_MATERIAL(new Material("LEFT_FRAME_MATERIAL",Color3(50,250,50),1));

void PglTurtle::_frame(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio, real_t color, real_t transparency) {
  GeometryPtr arrow;
  GroupPtr group;
  real_t lengthstick = heigth*(1-cap_heigth_ratio);
  if ( cap_heigth_ratio > 0 && cap_radius_ratio > 0) {
     GeometryPtr cap(new Cone(getWidth()*cap_radius_ratio,heigth*cap_heigth_ratio,true,getParameters().sectionResolution));
     if ( cap_heigth_ratio < 1) {
         group = GroupPtr(new Group(GeometryArrayPtr(new GeometryArray(2))));
         group->getGeometryList()->setAt(0,GeometryPtr(new Translated(Vector3(0,0,lengthstick),cap)));
         arrow = group;
     }
     else arrow = cap;
  }
  if ( lengthstick > GEOM_EPSILON) {
     GeometryPtr stick(new Cylinder(getWidth(),lengthstick,true,getParameters().sectionResolution));
     if (group) group->getGeometryList()->setAt(1,stick);
     else arrow = stick;
  }
  arrow->setName("Frame_"+number(arrow->getObjectId()));
  AppearancePtr hmat(HEADING_FRAME_MATERIAL);
  AppearancePtr umat(UP_FRAME_MATERIAL);
  AppearancePtr lmat(LEFT_FRAME_MATERIAL);
  if( color < (1.0- GEOM_EPSILON) || transparency > GEOM_EPSILON){
      hmat = new Material(Color3(int(250 * color),int(50*color),int(50*color)),transparency);
      umat = new Material(Color3(int(50*color),int(50*color),int(250 * color)),transparency);
      lmat = new Material(Color3(int(50*color),int(250 * color),int(50*color)),transparency);
  }
  _addToScene(transform(arrow,false),false,hmat);
  _addToScene(transform(GeometryPtr(new Oriented(Vector3(1,0,0),Vector3(0,0,1),arrow)),false),false,lmat);
  _addToScene(transform(GeometryPtr(new Oriented(Vector3(0,1,0),Vector3(0,0,1),arrow)),false),false,umat);
}

void PglTurtle::_arrow(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio) {
  GeometryPtr arrow;
  GroupPtr group;
  real_t lengthstick = heigth*(1-cap_heigth_ratio);
  if ( cap_heigth_ratio > 0 && cap_radius_ratio > 0) {
     GeometryPtr cap(new Cone(getWidth()*cap_radius_ratio,heigth*cap_heigth_ratio,true,getParameters().sectionResolution));
     if ( cap_heigth_ratio < 1) {
         group = GroupPtr(new Group(GeometryArrayPtr(new GeometryArray(2))));
         group->getGeometryList()->setAt(0,GeometryPtr(new Translated(Vector3(0,0,lengthstick),cap)));
         arrow = group;
     }
     else arrow = cap;
  }
  if ( lengthstick > GEOM_EPSILON) {
     GeometryPtr stick(new Cylinder(getWidth(),lengthstick,true,getParameters().sectionResolution));
     if (group) group->getGeometryList()->setAt(1,stick);
     else arrow = stick;
  }
  arrow->setName("Arrow_"+number(arrow->getObjectId()));
  _addToScene(transform(arrow,false));
}



ScenePtr PglTurtle::partialView(){
    ScenePtr currentscene = new Scene(*__scene);
    if(__params->isGeneralizedCylinderOn()){
      if(__params->pointList->size() > 1){
        _generalizedCylinder(__params->pointList,
                           __params->leftList,
                           __params->radiusList,
                           __params->initial.crossSection,
                           __params->initial.crossSectionCCW);
      }
    }
    frame();
    ScenePtr result = __scene;
    __scene = currentscene;
    return result;
}
