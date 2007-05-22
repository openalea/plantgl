/*---------------------------------------------------------------------------
 #
 #       File author(s): F. Boudon (frederic.boudon@loria.fr)
 #
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
#include <gui/viewer/pglapplication.h>
#include <scenegraph/transformation/axisrotated.h>
#include <scenegraph/transformation/oriented.h>
#include <scenegraph/transformation/translated.h>
#include <scenegraph/transformation/scaled.h>
#include <scenegraph/geometry/polyline.h>
#include <scenegraph/geometry/extrusion.h>
#include <scenegraph/geometry/triangleset.h>
#include <scenegraph/geometry/cylinder.h>
#include <scenegraph/geometry/frustum.h>
#include <scenegraph/geometry/cone.h>
#include <scenegraph/geometry/sphere.h>
#include <scenegraph/geometry/disc.h>
#include <scenegraph/geometry/text.h>
#include <scenegraph/container/pointarray.h>
#include <scenegraph/container/indexarray.h>
#include <scenegraph/scene/shape.h>
#include <math/util_math.h>

#include <sstream>

using namespace std;
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#define FABS(a) (a < 0 ? -(a) : a)

/*----------------------------------------------------------*/

Turtle::error_msg_handler_func ERROR_FUNC = NULL;

void Turtle::register_error_handler(Turtle::error_msg_handler_func f)
{ ERROR_FUNC = f; }

void Turtle::error(const std::string& msg){
    if (ERROR_FUNC != NULL) ERROR_FUNC(msg);
    else std::cerr << "*** ERROR : " << msg << std::endl;
}

void Turtle::warning(const std::string& msg){
    std::cerr << "*** WARNING : " << msg << std::endl;
}


/*----------------------------------------------------------*/

TurtleParam::TurtleParam() :
  position(0,0,0),
  heading(0,0,1),
  left(0,-1,0),
  up(1,0,0),
  scale(1,1,1),
  color(1),
  texture(0),
  width(0.1f),
  __polygon(false),
  __generalizedCylinder(false)
{        
}

void 
TurtleParam::reset(){
  position = Vector3(0,0,0);
  heading  = Vector3(0,0,1);
  left     = Vector3(0,-1,0);
  up       = Vector3(1,0,0);
  scale    = Vector3(1,1,1);
  color = 1;
  texture = 0;
  width = 0.1f;
  __polygon = false;
  __generalizedCylinder = false;
  pointList.clear();
  radiusList.clear();
}
    
TurtleParam * TurtleParam::copy(){
  TurtleParam * t = new TurtleParam();
  t->position = position;
  t->heading = heading;
  t->left = left;
  t->up = up;
  t->color = color;
  t->texture = texture;
  t->width = width;
  t->polygon(__polygon);
  t->generalizedCylinder(__generalizedCylinder);
  t->pointList = vector<Vector3>(pointList.begin(),pointList.end());
  t->radiusList = vector<real_t>(radiusList.begin(),radiusList.end());
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
    
Matrix3 TurtleParam::getMatrix() const{
   return Matrix3(heading,left,up);
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
  if(!radiusList.empty())
    radiusList = vector<real_t>(1,*(radiusList.end()-1));
}
    
void TurtleParam::removePoints(){
  pointList.clear();
  radiusList.clear();
}
    
void TurtleParam::polygon(bool t){
  __polygon = t;
  removePoints();
}
    
void TurtleParam::generalizedCylinder(bool t){
  __generalizedCylinder = t;
  removePoints();
}
    
void TurtleParam::pushPosition(){
  pointList.push_back(position);
  if(__generalizedCylinder)
	radiusList.push_back(width);
}

/*----------------------------------------------------------*/

Turtle::Turtle(TurtleParam * params):
  __params( params ? params : new TurtleParam()),
  angle_increment(60),
  width_increment(1),
  color_increment(1),
  scale_multiplier(0.5),
  default_step(1){
}

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
  
void Turtle::stop(){
  if(__params->isGeneralizedCylinderOn()){
     if(__params->pointList.size() > 1)
	  _generalizedCylinder(__params->pointList,
						   __params->radiusList);
    __params->removePoints();
  }
}
  
  void Turtle::push(){
	__paramstack.push(__params->copy());
	if(__params->isGeneralizedCylinderOn())
	  __params->keepLastPoint();
  }
  
  void Turtle::pop(){
	if (__params->isGeneralizedCylinderOn()){
     if(__params->pointList.size() > 1)
	  _generalizedCylinder(__params->pointList,
						   __params->radiusList);
      __params->removePoints();
	}
	if(!__paramstack.empty()){
	  delete __params;
	  __params = __paramstack.top();
	  __paramstack.pop();
	}
	else {
	  error("Empty Turtle Stack: Cannot pop");
	}
  }
  
  void Turtle::f(float length){
	__params->position += __params->heading*length;
	if (__params->isGeneralizedCylinderOn() ||
	  __params->isPolygonOn())
	  __params->pushPosition();
  }
  
  void Turtle::F(real_t length,real_t topdiam){
	if (!__params->isGeneralizedCylinderOn() && 
	  ! __params->isPolygonOn()){
	  if (topdiam < GEOM_EPSILON) _cylinder(length);
	  else _frustum(length,topdiam);
	}
	if (topdiam > GEOM_EPSILON ) setWidth(topdiam);
	f(length);
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
  
void Turtle::rollR(real_t angle){
	real_t ra = angle * GEOM_RAD;
	Matrix3 m = Matrix3::axisRotation(__params->heading,ra);
	__params->up = m*__params->up;
	__params->left = m*__params->left;
  }
  
  void Turtle::rollToVert(){
	Vector3 zvec = Vector3::OZ;
	__params->left = cross(zvec,__params->heading);
	if (norm(__params->left) < GEOM_EPSILON )
	  __params->left = Vector3(0,-1,0);
	else{
	  __params->left.normalize();
	  __params->up = cross(__params->heading,__params->left);
	}
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
	__params->pushPosition();
  }
  
  void Turtle::stopPolygon(){
	if(__params->pointList.size() > 2)_polygon(__params->pointList);
	__params->polygon(false);
  }
  
  void Turtle::startGC(){
    __params->generalizedCylinder(true);
    __params->pushPosition();
  }
  
  void Turtle::stopGC(){
    if(__params->pointList.size() > 1)
	  _generalizedCylinder(__params->pointList,
						   __params->radiusList);
    __params->generalizedCylinder(false);
  }
  
/*----------------------------------------------------------*/

PglTurtle::PglTurtle(TurtleParam * param):
  Turtle(param),
  __scene(new Scene()){
   defaultValue();
}
 
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
  __appList.push_back(new Material("default"));
  __appList.push_back(new Material("Brown",Color3(65,45,15),3)); // Brown
  __appList.push_back(new Material("Green",Color3(30,60,10),3)); // Green
  __appList.push_back(new Material("Red",Color3(60,0,0),3));     // Red
  __appList.push_back(new Material("Yellow",Color3(60,60,15),3));// Yellow
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
    __appList.push_back(new Material("Default"));
  if (__appList.size() == pos)
    __appList.push_back(mat);
  else __appList[pos] = mat;
}

void PglTurtle::setColor(size_t pos, const Color3& mat){
  while (__appList.size() < pos)
    __appList.push_back(new Material("Default"));
  if (__appList.size() == pos)
    __appList.push_back(new Material(mat));
  else __appList[pos] = AppearancePtr(new Material(mat));
}

void 
PglTurtle::appendColor(uint32_t red, uint32_t green, uint32_t blue)
{ appendColor(Color3(red,green,blue)); }

void 
PglTurtle::appendColor(float red, float green, float blue)
{ appendColor(Color3((uchar_t)red*255,(uchar_t)green*255,(uchar_t)blue*255)); }

void 
PglTurtle::appendColor(const Color3& mat)
{ __appList.push_back(AppearancePtr(new Material(mat))); }
    
void 
PglTurtle::setColor(size_t pos, uint32_t red, uint32_t green, uint32_t blue )
{ setColor(pos,Color3(red,green,blue)); }

void 
PglTurtle::setColor(size_t pos, float red, float green, float blue )
{ setColor(pos,Color3((uchar_t)red*255,(uchar_t)green*255,(uchar_t)blue*255)); }


void PglTurtle::removeColor(size_t pos){
  if (__appList.size() > pos){
	__appList.erase(__appList.begin()+pos);
  }
}

void PglTurtle::removeSurface(const string& name){
  SurfaceMap::iterator it = __surfList.find(name);
  if(it != __surfList.end())__surfList.erase(it);
}
    
AppearancePtr PglTurtle::getMaterial() const{
   if (getColor() < __appList.size())
            return __appList[getColor()];
   else return AppearancePtr(new Material("Default"));
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

void PglTurtle::_frustum(real_t length, real_t topdiam){
  GeometryPtr a;
  real_t width = getWidth();
  if(FABS(width) > GEOM_EPSILON){
	real_t taper = topdiam/width;
  if ( getScale() !=  Vector3(1,1,1) &&
	  (getScale().x() == getScale().y() && 
	   getScale().y() == getScale().z() ))
	   width *= getScale().x();
	if (FABS(taper) < GEOM_EPSILON)
	  a = GeometryPtr(new Cone(width,length,false));
	else if (FABS(taper-1.0) < GEOM_EPSILON)
	  a = GeometryPtr(new Cylinder(width,length,false));
	else
	  a = GeometryPtr(new Frustum(width,length,taper,false));
	 __scene->add(Shape(transform(a),getMaterial()));
  }
  else {
	if (FABS(topdiam) < GEOM_EPSILON){
	  Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(2,getPosition()));
	  pts->setAt(1,getPosition()+getHeading()*length);
	  a = GeometryPtr(new Polyline(pts));
	}
	else {
	  real_t _topdiam = topdiam;
	  if ( getScale() !=  Vector3(1,1,1) &&
		  (getScale().x() == getScale().y() && 
		  getScale().y() == getScale().z() ))
		  topdiam *= getScale().x();
	  a = GeometryPtr(new Cone(_topdiam,length,false));
	  if (getLeft() != Vector3::OX || 
		  getUp() != -Vector3::OY)
        a = GeometryPtr(new Oriented(getLeft(),-getUp(),a));
      a = GeometryPtr(new Translated(getPosition()+getHeading()*length,a));
	}
	__scene->add(Shape(a,getMaterial()));
  }
}

void PglTurtle::_cylinder(real_t length){
  real_t width = getWidth();
  if(FABS(width) < GEOM_EPSILON){
	  Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(2,getPosition()));
	  pts->setAt(1,getPosition()+getHeading()*length);
	  __scene->add(Shape(GeometryPtr(new Polyline(pts)),getMaterial()));
  }
  else {
	if ( getScale() !=  Vector3(1,1,1) &&
		(getScale().x() == getScale().y() && 
		 getScale().y() == getScale().z() ))
		width *= getScale().x();
	__scene->add(Shape(transform(GeometryPtr(new Cylinder(width,length,false))),
					  getMaterial()));
  }
}
    
void PglTurtle::_sphere(real_t radius){
  real_t rad = radius;
  if ( getScale() !=  Vector3(1,1,1) &&
		(getScale().x() == getScale().y() && 
		 getScale().y() == getScale().z() ))
		rad *= getScale().x();
  __scene->add(Shape(transform(GeometryPtr(new Sphere(rad))),
					getMaterial()));
}

GeometryPtr PglTurtle::getCircle(real_t radius) const{
   Point3ArrayPtr pts = Point3ArrayPtr(new Point3Array(17,Vector3(0,1,0)));
   real_t angdelta = GEOM_PI/8.0;
   real_t angle = 0;
   for (int i = 1; i < 16; i++){
	 angle += angdelta;
     pts->setAt(i,Vector3(0,cos(angle),sin(angle)));
   }
   pts->setAt(16,Vector3(0,1,0));
   Vector3 scale = getScale()* radius;
   return GeometryPtr(new Scaled(scale,GeometryPtr(new Polyline(pts))));
}
    
void 
PglTurtle::_circle(real_t radius){
  real_t rad = radius;
  if ( getScale() !=  Vector3(1,1,1) &&
		(getScale().x() == getScale().y() && 
		 getScale().y() == getScale().z() ))
		rad *= getScale().x();
  __scene->add(Shape(transform(GeometryPtr(new AxisRotated(Vector3::OX,GEOM_HALF_PI,GeometryPtr(new Disc(rad))))),
					getMaterial()));
//   __scene->add(Shape(transform(getCircle(radius),false),getMaterial()));
}
    
void PglTurtle::_surface(const string& name,real_t scale){
  SurfaceMap::const_iterator it = __surfList.find(name);
  if (it == __surfList.end()){
 	error("Unknown surface '" + name + '\'');
  }
  GeometryPtr obj(new Scaled(getScale()*scale,it->second));
  __scene->add(Shape(transform(obj,false),getMaterial()));
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
	__scene->add(Shape(t,getMaterial()));
  }
}

void 
PglTurtle::_generalizedCylinder(const vector<Vector3>& points,
								const vector<real_t>& radiusList){
  LineicModelPtr axis = LineicModelPtr(new Polyline(Point3ArrayPtr(
						  new Point3Array(points.begin(),points.end()))));
  Point2ArrayPtr radius(new Point2Array(radiusList.size()));
  Point2Array::iterator it2 = radius->getBegin();
  for (std::vector<real_t>::const_iterator it = radiusList.begin();
		it != radiusList.end(); it++){
		  *it2 = Vector2(*it,*it); it2++;
  }
  Point2ArrayPtr pts = Point2ArrayPtr(new Point2Array(17,Vector2(1,0)));
  real_t angdelta = GEOM_PI/8.0;
  real_t angle = 0;
  for (int i = 1; i < 16; i++){
	  angle += angdelta;
	  pts->setAt(i,Vector2(cos(angle),sin(angle)));
  }
  pts->setAt(16,Vector2(1,0));
  Curve2DPtr crossSection = Curve2DPtr(new Polyline2D(pts));
  __scene->add(Shape(GeometryPtr(new Extrusion(axis,crossSection,radius)),
						 getMaterial()));
}

void
PglTurtle::_label(const string& text ){
  GeometryPtr obj(new Text(text));
  if ( getPosition() != Vector3::ORIGIN )
     obj = GeometryPtr(new Translated(getPosition(),obj));
  __scene->add(Shape(obj,getMaterial()));
}
