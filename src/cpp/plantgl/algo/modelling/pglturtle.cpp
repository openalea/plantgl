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

PglTurtle::PglTurtle(TurtleDrawerPtr drawer, TurtleParam * param):
  Turtle(drawer, param){
   defaultValue();
}

PglTurtle::~PglTurtle() { }

void PglTurtle::reset(){
  resetValues();
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

/*
void PglTurtle::customGeometry(const GeometryPtr smb, real_t scale)
{
    if( FABS(scale) > GEOM_EPSILON){
    PlanarModelPtr _2Dtest = dynamic_pointer_cast<PlanarModel>(smb);
    if (is_valid_ptr(_2Dtest) && __params->screenCoordinates)
      _addToScene(transform(GeometryPtr(new Scaled(getScale()*scale,GeometryPtr(new Oriented(Vector3(0,1,0),Vector3(0,0,1),smb)))),false));
    else _addToScene(transform(GeometryPtr(new Scaled(getScale()*scale,smb)),false));
  }
}*/

void PglTurtle::pglShape(const GeometryPtr smb, real_t scale)
{
    __drawer->customGeometry(getIdPair(), getCurrentMaterial(), __params->frame_info, smb, scale);
}

void PglTurtle::pglShape(const ShapePtr shape, real_t scale)
{

    AppearancePtr prevapp  = __params->customMaterial;
    setCustomAppearance(shape->getAppearance());
    __drawer->customGeometry(getIdPair(), getCurrentMaterial(), __params->frame_info, shape->getGeometry());
    setCustomAppearance(prevapp);
}
void PglTurtle::pglShape(const ScenePtr scene, real_t scale)
{
    AppearancePtr prevapp  = __params->customMaterial;
    for(Scene::const_iterator it = scene->begin(); it != scene->end(); ++it){
        ShapePtr sh = dynamic_pointer_cast<Shape>(*it);
        if (sh) {
            setCustomAppearance(sh->getAppearance());
            __drawer->customGeometry(getIdPair(), getCurrentMaterial(), __params->frame_info, sh->getGeometry());
        }
    }
    setCustomAppearance(prevapp);
}


void PglTurtle::surface(const string& name, real_t scale){
  SurfaceMap::const_iterator it = __surfList.find(name);
  if (it == __surfList.end()){
    error("Unknown surface '" + name + '\'');
  }
  if(scale < -GEOM_EPSILON) warning("Invalid scale for surface");
  __drawer->customGeometry(getIdPair(), getCurrentMaterial(), __params->frame_info, it->second, scale);
}

#include "plantgl/algo/base/tesselator.h"

ScenePtr PglTurtle::partialView(){
    auto drawer = dynamic_pointer_cast<PglTurtleDrawer>(__drawer);
    if(drawer) {
        return drawer->partialView(
                getIdPair(),
                getCurrentMaterial(),
                __params->frame_info,
                __params->isGeneralizedCylinderOn(),
                __params->sectionResolution,
                __params->pointList,
                __params->leftList,
                __params->radiusList,
                __params->initial
                );
    }
    return {};
}

