/*---------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
 *
 # ---------------------------------------------------------------------------
 #
 #                      GNU General Public Licence
 #
 #       This program is free software; you can redistribute it and/or
 #       modify it under the terms of the GNU General Public License as
 #       published by the Free Software Foundation; either version 2 of
 #       the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #       but WITHOUT ANY WARRANTY; without even the implied warranty of
 #       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 #       GNU General Public License for more details.
 #
 #       You should have received a copy of the GNU General Public
 #       License along with this program; see the file COPYING. If not,
 #       write to the Free Software Foundation, Inc., 59
 #       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 #
 # ---------------------------------------------------------------------------
 */


#include "pglturtle.h"
// #include <plantgl/gui/viewer/pglapplication.h>
#include <plantgl/scenegraph/transformation/axisrotated.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/geometry/frustum.h>
#include <plantgl/scenegraph/geometry/cone.h>
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
TOOLS_USING_NAMESPACE

#define FABS(a) (a < 0 ? -(a) : a)

/*----------------------------------------------------------*/

Turtle::error_msg_handler_func ERROR_FUNC = NULL;
Turtle::error_msg_handler_func WARNING_FUNC = NULL;

void Turtle::register_error_handler(Turtle::error_msg_handler_func f)
{ ERROR_FUNC = f; }

void Turtle::register_warning_handler(Turtle::error_msg_handler_func f)
{ WARNING_FUNC = f; }

void Turtle::error(const std::string& msg){
    if (ERROR_FUNC != NULL) ERROR_FUNC(msg);
    else std::cerr << "*** ERROR : " << msg << std::endl;
}

void Turtle::warning(const std::string& msg){
    if (WARNING_FUNC != NULL) WARNING_FUNC(msg);
    else std::cerr << "*** WARNING : " << msg << std::endl;
}


/*----------------------------------------------------------*/

TurtleParam::TurtleParam() :
  position(0,0,0),
  heading(0,0,1),
  left(0,-1,0),
  up(1,0,0),
  scale(1,1,1),
  lastId(Shape::NOID),
  color(1),
  texture(0),
  width(0.1f),
  tropism(0,0,1),
  elasticity(0),
  __polygon(false),
  __generalizedCylinder(false),
  customId(Shape::NOID),
  customParentId(Shape::NOID),
  sectionResolution(Cylinder::DEFAULT_SLICES)
{        
}

TurtleParam::~TurtleParam() {}

void 
TurtleParam::reset(){
  position = Vector3(0,0,0);
  heading  = Vector3(0,0,1);
  left     = Vector3(0,-1,0);
  up       = Vector3(1,0,0);
  scale    = Vector3(1,1,1);
  lastId = Shape::NOID;
  customId = Shape::NOID;
  customParentId = Shape::NOID;
  sectionResolution = Cylinder::DEFAULT_SLICES;
  color = 1;
  texture = 0;
  width = 0.1f;
  tropism = Vector3(0,0,1);
  elasticity = 0;
  __polygon = false;
  __generalizedCylinder = false;
  pointList.clear();
  leftList.clear();
  radiusList.clear();
  crossSection = Curve2DPtr();
  initialCrossSection = Curve2DPtr();
  initialColor = -1;
}
    
TurtleParam * TurtleParam::copy(){
  TurtleParam * t = new TurtleParam(*this);
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
  if(!pointList.empty())
	pointList = vector<Vector3>(1,*(pointList.end()-1));
  if(!leftList.empty())
	leftList = vector<Vector3>(1,*(leftList.end()-1));
  if(!radiusList.empty())
    radiusList = vector<real_t>(1,*(radiusList.end()-1));
}
    
void TurtleParam::removePoints(){
  pointList.clear();
  leftList.clear();
  radiusList.clear();
}
    
void TurtleParam::polygon(bool t){
  __polygon = t;
  removePoints();
  if (t){
	initialColor = color;
  }
  else {
	initialColor = -1;
  }
}
    
void TurtleParam::generalizedCylinder(bool t){
  __generalizedCylinder = t;
  removePoints();
  if (t){
	initialCrossSection = crossSection;
	initialColor = color;
  }
  else {
	initialCrossSection = Curve2DPtr();
	initialColor = -1;
  }

}
    
void TurtleParam::pushPosition(){
  pointList.push_back(position);
  if(__generalizedCylinder) {
	leftList.push_back(left);
	radiusList.push_back(width);
  }
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
  parentId(Shape::NOID)
{
	if (!__params->crossSection)
		setDefaultCrossSection();
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
	if (!__params->crossSection)
		setDefaultCrossSection();
	__paramstack = stack<TurtleParam *>();
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

uint_t Turtle::popId()
{
    uint_t nid = id;
    parentId = id;
    return nid;
}

void Turtle::stop(){
  if(__params->isGeneralizedCylinderOn()){
	  if(__params->pointList.size() > 1){
		_generalizedCylinder(__params->pointList,
						   __params->leftList,
						   __params->radiusList,
						   __params->initialCrossSection);
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
		if(__params->pointList.size() > 1){
			_generalizedCylinder(__params->pointList,
							    __params->leftList,
							    __params->radiusList,
							    __params->initialCrossSection);
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
      if(length > 0){
		  if (__params->elasticity > GEOM_EPSILON){
			  _applyTropism();
		  }
          __params->position += __params->heading*length*getScale().z();
          if (__params->isGeneralizedCylinderOn() ||
              __params->isPolygonOn())
              __params->pushPosition();
      }
      else if(fabs(length) < GEOM_EPSILON) warning("f length should be non null.");
      else error("f length should be positive.");
  }
  
  void Turtle::F(real_t length,real_t topdiam){
      if(length > 0){
		  if (__params->elasticity > GEOM_EPSILON){
			  _applyTropism();
		  }
          if (!__params->isGeneralizedCylinderOn() && 
              ! __params->isPolygonOn()){
                  if (topdiam < GEOM_EPSILON) _cylinder(length);
                  else _frustum(length,topdiam);
          }
          if (topdiam > GEOM_EPSILON ) setWidth(topdiam);
          f(length);
      }
      else if(fabs(length) < GEOM_EPSILON) warning("F length should be non null.");
      else error("F length should be positive.");
  }


void Turtle::left(real_t angle){
  real_t ra = angle * GEOM_RAD;
  Matrix3 m = Matrix3::axisRotation(__params->up,ra);
  __params->heading = m*__params->heading;
  __params->left = m*__params->left;
}
  
void Turtle::down(real_t angle){
	real_t ra = angle * GEOM_RAD;
	Matrix3 m = Matrix3::axisRotation(__params->left,ra);
	__params->heading = m*__params->heading;
	__params->up = m*__params->up;
  }
  
void Turtle::rollL(real_t angle){
	real_t ra = angle * GEOM_RAD;
	Matrix3 m = Matrix3::axisRotation(__params->heading,ra);
	__params->up = m*__params->up;
	__params->left = m*__params->left;
  }
  
void Turtle::rollToVert(const Vector3& top){
	__params->left = cross(top,__params->heading);
	if (norm(__params->left) < GEOM_EPSILON )
	  __params->left = Vector3(0,-1,0);
	else{
	  __params->left.normalize();
	  __params->up = cross(__params->heading,__params->left);
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
 	  error("Heading and Up Vector in setHead is NULL");
	}
	__params->heading = h;
	__params->up = u;
	__params->left = l;
  }
  
  void Turtle::move(const Vector3& pos){
	__params->position = pos;
  }
  
  void Turtle::decal(const Vector3& pos){
	__params->position += pos;
  }
  
  void Turtle::scale(const Vector3& s){
	__params->scale = s;
  }
  
  void Turtle::setColor(int v){
	if (0 <= v && v < getColorListSize())
	  __params->color = v;
	else {
      stringstream st;
      st << "Invalid Color value " << v << " in setColor (maximum is " << getColorListSize() - 1 << ')' <<  std::flush;
      warning(st.str());
	  return;
	}
  }
  
  void Turtle::setTexture(int v){
	if (0 <= v && v < getTextureListSize())
	  __params->texture = v;
	else {
      stringstream st;
      st << "Invalid Texture value " << v << " in setTexture (maximum is " << getTextureListSize() - 1 << ')' << std::flush;
      warning(st.str());
	  __params->texture = 0;
	  return;
	}
  }
  
  void Turtle::setWidth(real_t v){
    if (v > GEOM_EPSILON)
	  __params->width = v;
	else {
 	  error("Invalid width value in setWidth. Must be positive");
	}
  }
  
  void Turtle::traceTo(const Vector3& v){
	Vector3 h = v - __params->position;
	real_t l = h.normalize();
	if (l > GEOM_EPSILON){
      Vector3 c = cross(__params->heading,h);
	  real_t lc = c.normalize();
      if (lc < GEOM_EPSILON)F(l);
	  else {
        real_t a = angle(__params->heading,h);
        Matrix3 m = Matrix3::axisRotation(c,a);
        __params->transform(m);
        F(l);
	  }
	}
  }
  
  void Turtle::startPolygon(){
	__params->polygon(true);
    __params->customId = popId();
    __params->customParentId = parentId;
	__params->pushPosition();
  }
  
  void Turtle::stopPolygon(){
	if(__params->pointList.size() > 2)_polygon(__params->pointList);
	__params->polygon(false);
    __params->customId = Shape::NOID;
    __params->customParentId = Shape::NOID;
  }
  
  void Turtle::startGC(){
	if (!__params->crossSection) setDefaultCrossSection();
    __params->generalizedCylinder(true);
    __params->customId = popId();
    __params->customParentId = parentId;
    __params->pushPosition();
  }
  
void Turtle::stopGC(){
	if(__params->pointList.size() > 1){
		_generalizedCylinder(__params->pointList,
							 __params->leftList,
							 __params->radiusList,
							 __params->initialCrossSection);
	}
    __params->generalizedCylinder(false);
    __params->customId = Shape::NOID;
    __params->customParentId = Shape::NOID;
  }
  
void Turtle::setCrossSection(const Curve2DPtr& curve)
{
	__params->crossSection = curve;
}

void Turtle::setDefaultCrossSection(size_t slices)
{
	setCrossSection(Curve2DPtr(Polyline2D::Circle(1,slices)));
}

void Turtle::_applyTropism() {
	Vector3 cp = cross(getHeading(),getTropism());
	real_t sinus = norm( cp );
	if (sinus > GEOM_EPSILON){
		cp /= sinus;
		real_t cosinus = dot( getHeading(),getTropism() );
		real_t ang = atan2( sinus, cosinus );
		transform(Matrix3::axisRotation(cp,ang*getElasticity()));
	}
}

void Turtle::sphere(real_t radius )
{ 
  if (radius < GEOM_EPSILON)
  { warning("Invalid radius for sphere"); }
  else _sphere(radius); 
}

void Turtle::circle(real_t radius )
{ 
  if (radius < GEOM_EPSILON)
  { warning("Invalid radius for circle"); }
  else _circle(radius); 
}

void Turtle::label(const std::string& text )
{ 
	if(!text.empty())_label(text); 
	else warning("Invalid text for label");
}

void Turtle::surface(const std::string& name, real_t scale)
{ 
	if (!name.empty() && scale > GEOM_EPSILON) _surface(name,scale); 
	else {
		if(name.empty()) warning("Invalid name for surface");
		if(scale < GEOM_EPSILON) warning("Invalid scale for surface");
	}
}

void Turtle::frame(real_t scale, real_t cap_heigth_ratio, real_t cap_radius_ratio )
{
	if (scale > GEOM_EPSILON && cap_heigth_ratio < 1 && cap_heigth_ratio > 0) _frame(scale,cap_heigth_ratio,cap_radius_ratio); 
	else {
		if(scale < GEOM_EPSILON) warning("Invalid scale for frame. Should be positive");
		if(cap_heigth_ratio > 1 || cap_heigth_ratio < 0) warning("Invalid cap_heigth_ratio for frame. Should be in [0,1].");
		if(cap_radius_ratio < GEOM_EPSILON) warning("Invalid cap_radius_ratio for frame. Should be positive.");
	}
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

void PglTurtle::clearColorList(){
   __appList.clear();
}

const vector<AppearancePtr>&
PglTurtle::getColorList() const{
   return __appList;
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
  __appList.push_back(AppearancePtr(new Material("default")));
  __appList.push_back(AppearancePtr(new Material("Brown",Color3(65,45,15),3))); // Brown
  __appList.push_back(AppearancePtr(new Material("Green",Color3(30,60,10),3))); // Green
  __appList.push_back(AppearancePtr(new Material("Red",Color3(60,0,0),3)));     // Red
  __appList.push_back(AppearancePtr(new Material("Yellow",Color3(60,60,15),3)));// Yellow
  Point3ArrayPtr points= Point3ArrayPtr(new Point3Array(7,Vector3(0,0,0.5)));
  points->setAt(1,Vector3(0,0,0));
  points->setAt(2,Vector3(-0.25,0,1./3));
  points->setAt(3,Vector3(-0.25,0,2./3));
  points->setAt(4,Vector3(0,0,1));
  points->setAt(5,Vector3(0.25,0,2./3.));
  points->setAt(6,Vector3(0.25,0,1./3.));
  Index3ArrayPtr indices= Index3ArrayPtr(new Index3Array(6));
  indices->setAt(0,Index3(0,2,1));
  indices->setAt(1,Index3(0,3,2));
  indices->setAt(2,Index3(0,4,3));
  indices->setAt(3,Index3(0,5,4));
  indices->setAt(4,Index3(0,6,5));
  indices->setAt(5,Index3(0,1,6));
  __surfList["l"] = GeometryPtr(new TriangleSet(points, indices));
  points= Point3ArrayPtr(new Point3Array(7,Vector3(0,0,0.5)));
  points->setAt(1,Vector3(0,0,0));
  points->setAt(2,Vector3(0,-0.25,1./3));
  points->setAt(3,Vector3(0,-0.25,2./3));
  points->setAt(4,Vector3(0,0,1));
  points->setAt(5,Vector3(0,0.25,2./3.));
  points->setAt(6,Vector3(0,0.25,1./3.));
  __surfList["s"] = GeometryPtr(new TriangleSet(points, indices));

}

/*
void PglTurtle::plot() const{
  PGLViewerApplication::display(__scene);
}
*/

void PglTurtle::appendMaterial(const AppearancePtr& mat)
{ if(mat)__appList.push_back(mat); }

void PglTurtle::setMaterial(size_t pos, const AppearancePtr& mat){
  while (__appList.size() < pos)
    __appList.push_back(AppearancePtr(new Material("Default")));
  if (__appList.size() == pos)
    __appList.push_back(mat);
  else __appList[pos] = mat;
}

AppearancePtr PglTurtle::getMaterial(size_t pos){
    if (pos >= __appList.size()) {
        while (__appList.size() <= pos)
        __appList.push_back(AppearancePtr(new Material("Default")));
    }
    return __appList[pos];
}

void PglTurtle::setColorAt(size_t pos, const Color3& mat){
  while (__appList.size() < pos)
    __appList.push_back(AppearancePtr(new Material("Default")));
  if (__appList.size() == pos)
    __appList.push_back(AppearancePtr(new Material(mat)));
  else __appList[pos] = AppearancePtr(new Material(mat));
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
   if (getColor() < __appList.size())
            return __appList[getColor()];
   else return AppearancePtr(new Material("Default"));
}

AppearancePtr PglTurtle::getCurrentInitialMaterial() const{
   if (__params->initialColor < __appList.size())
            return __appList[__params->initialColor];
   else return AppearancePtr(new Material("Default"));
}

void PglTurtle::customGeometry(const GeometryPtr smb, real_t scale)
{
  _addToScene(transform(GeometryPtr(new Scaled(Vector3(scale,scale,scale),smb)),true));
}


GeometryPtr 
PglTurtle::transform(const GeometryPtr& o, bool scaled) const{
  GeometryPtr obj = o;
  if ( scaled && getScale() !=  Vector3(1,1,1) &&
	  (getScale().x() != getScale().y() || 
	   getScale().y() != getScale().z() ))
       obj = GeometryPtr(new Scaled(getScale(),obj));
  if ( getLeft() != Vector3::OX || 
	   getUp()   != Vector3::OY )
       obj = GeometryPtr(new Oriented(getLeft(),getUp(),obj));
  if ( getPosition() != Vector3::ORIGIN )
       obj = GeometryPtr(new Translated(getPosition(),obj));
  return obj;
}

void PglTurtle::_addToScene(const GeometryPtr geom, bool custompid, AppearancePtr app)
{
   if (custompid)
	   __scene->add(Shape3DPtr(new Shape(geom,app?app:getCurrentInitialMaterial(),__params->customId,__params->customParentId)));
   else{
     uint_t pid = parentId;
     __scene->add(Shape3DPtr(new Shape(geom,app?app:getCurrentMaterial(),popId(),pid)));
   }
}

void PglTurtle::_frustum(real_t length, real_t topdiam){
  GeometryPtr a;
  real_t width = getWidth();
  if(FABS(width) > GEOM_EPSILON){
	real_t taper = topdiam/width;
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
	if (FABS(topdiam) < GEOM_EPSILON){
	  Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(2,getPosition()));
	  pts->setAt(1,getPosition()+getHeading()*length*getScale().z());
	  a = GeometryPtr(new Polyline(pts));
	}
	else {
	  real_t _topdiam = topdiam;
	  if ( getScale() !=  Vector3(1,1,1) &&
		  (getScale().x() == getScale().y() && 
		  getScale().y() == getScale().z() ))
		  topdiam *= getScale().x();
	  a = GeometryPtr(new Cone(_topdiam,length*getScale().z(),false,getParameters().sectionResolution));
	  if (getLeft() != Vector3::OX || 
		  getUp() != -Vector3::OY)
        a = GeometryPtr(new Oriented(getLeft(),-getUp(),a));
      a = GeometryPtr(new Translated(getPosition()+getHeading()*length*getScale().z(),a));
	}
	_addToScene(transform(a));
  }
}

void PglTurtle::_cylinder(real_t length){
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
    
void PglTurtle::_sphere(real_t radius){
  real_t rad = radius;
  if ( getScale() !=  Vector3(1,1,1) &&
		(getScale().x() == getScale().y() && 
		 getScale().y() == getScale().z() ))
		rad *= getScale().x();
  _addToScene(transform(GeometryPtr(new Sphere(rad,getParameters().sectionResolution,getParameters().sectionResolution))));
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
  _addToScene(transform(GeometryPtr(new AxisRotated(Vector3::OX,GEOM_HALF_PI,GeometryPtr(new Disc(rad,getParameters().sectionResolution))))));
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
    
void 
PglTurtle::_polygon(const vector<Vector3>& pointList){
  size_t s = pointList.size();
  Point3ArrayPtr points ;
  if (norm(pointList[0] - pointList[s-1]) < GEOM_EPSILON){
	points = Point3ArrayPtr(new Point3Array(getParameters().pointList.begin(),getParameters().pointList.end()-1));
	s -= 1;
  }
  else 
	points = Point3ArrayPtr(new Point3Array(getParameters().pointList.begin(),getParameters().pointList.end()));
  if (s > 2){
	Index3ArrayPtr ind(new Index3Array(s-2));
	for (int i=0; i < s-2; i++) 
	  ind->setAt(i,Index3(0,i+1,i+2));
	GeometryPtr t = GeometryPtr(new TriangleSet(points,ind));
    _addToScene(t,true);
  }
}

void 
PglTurtle::_generalizedCylinder(const vector<Vector3>& points,
								const vector<Vector3>& leftList,
								const vector<real_t>& radiusList,
								const Curve2DPtr& crossSection){
  LineicModelPtr axis = LineicModelPtr(new Polyline(Point3ArrayPtr(
						  new Point3Array(points.begin(),points.end()))));
  Point2ArrayPtr radius(new Point2Array(radiusList.size()));
  Point2Array::iterator it2 = radius->begin();
  for (std::vector<real_t>::const_iterator it = radiusList.begin();
		it != radiusList.end(); it++){
		  *it2 = Vector2(*it,*it); it2++;
  }
  if (!crossSection)
	  error("Invalid cross section");
  assert (crossSection);
  Extrusion * extrusion = new Extrusion(axis,crossSection,radius);
  extrusion->getInitialNormal() = leftList[0];
  _addToScene(GeometryPtr(extrusion),true);
}

void
PglTurtle::_label(const string& text ){
  GeometryPtr obj(new Text(text));
  if ( getPosition() != Vector3::ORIGIN )
     obj = GeometryPtr(new Translated(getPosition(),obj));
  _addToScene(obj);
}

AppearancePtr PglTurtle::HEADING_FRAME_MATERIAL(new Material("HEADING_FRAME_MATERIAL",Color3(250,50,50),1));
AppearancePtr PglTurtle::UP_FRAME_MATERIAL(new Material("UP_FRAME_MATERIAL",Color3(50,50,250),1));
AppearancePtr PglTurtle::LEFT_FRAME_MATERIAL(new Material("LEFT_FRAME_MATERIAL",Color3(50,250,50),1));

void PglTurtle::_frame(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio) {
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
  arrow->setName("Frame_"+number(arrow->getId()));
  _addToScene(transform(arrow,false),false,HEADING_FRAME_MATERIAL);
  _addToScene(transform(GeometryPtr(new Oriented(Vector3(0,1,0),Vector3(0,0,1),arrow)),false),false,LEFT_FRAME_MATERIAL);
  _addToScene(transform(GeometryPtr(new Oriented(Vector3(-1,0,0),Vector3(0,0,1),arrow)),false),false,UP_FRAME_MATERIAL);
}
