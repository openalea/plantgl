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


#include "pglturtledrawer.h"
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

/*
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
*/

void PglTurtleDrawer::customGeometry(const uint_t id,
                                AppearancePtr appearance,
                                const FrameInfo& frameinfo, 
                                const GeometryPtr smb, 
                                real_t scale = 1.0)
{
    if( FABS(norm(frameinfo.scaling)) > GEOM_EPSILON){
        PlanarModelPtr _2Dtest = dynamic_pointer_cast<PlanarModel>(smb); 
        if (is_valid_ptr(_2Dtest) && frameinfo.screenprojection)
            _addToScene(transform(frameinfo.position, frameinfo.heading, frameinfo.left, frameinfo.up, frameinfo.scaling, GeometryPtr(new Oriented(Vector3(0,1,0),Vector3(0,0,1),smb))), id, appearance, frameinfo.screenprojection);
        else _addToScene(transform(position, heading, left, up scaling, smb));
    }
}

GeometryPtr
PglTurtleDrawer::transform(const Vector3& position, 
                           const Vector3& heading, 
                           const Vector3& left, 
                           const Vector3& up, 
                           const GeometryPtr& obj) const {
  if ( up != Vector3::OX ||
       left   != -Vector3::OY )
       obj = GeometryPtr(new Oriented(up,-left,obj));
  if ( position != Vector3::ORIGIN )
       obj = GeometryPtr(new Translated(position,obj));
  return obj;
}

GeometryPtr
PglTurtleDrawer::transform_n_scale(const Vector3& position, 
                                   const Vector3& heading, 
                                   const Vector3& left, 
                                   const Vector3& up, 
                                   const Vector3& scaling,
                                   const GeometryPtr& obj) const {
   if ( scaling !=  Vector3(1,1,1) &&
      (scaling.x() != scaling.y() ||
       scaling.y() != scaling.z() ))
       obj = GeometryPtr(new Scaled(scaling,obj));
   return transform(position, heading, left, up, obj);
}




void PglTurtleDrawer::_addToScene(const GeometryPtr geom, id_pair idpair, AppearancePtr app, bool projection)
{
    GeometryPtr mgeom = geom;
    if (projection)
      mgeom = GeometryPtr(new ScreenProjected(GeometryPtr(new Oriented(Vector3(0,0,1),Vector3(1,0,0),mgeom)),false));

    __scene->add(Shape3DPtr(new Shape(mgeom, app, idpair.id, idpair.parentId)));
}

void PglTurtleDrawer::frustum(id_pair idpair,
                          AppearancePtr appearance,
                          const FrameInfo& frameinfo, 
                          real_t length, 
                          real_t baseradius, 
                          real_t topradius, 
                          uint_t sectionResolution) {
  if (fabs(length) > GEOM_EPSILON) {
    GeometryPtr a;

    if(FABS(baseradius) > GEOM_EPSILON){
        real_t taper = topradius/baseradius;
        if (frameinfo.scaling !=  Vector3(1,1,1) &&
            (frameinfo.scaling.x() == frameinfo.scaling.y()))
            baseradius *= frameinfo.scaling.x();
        if (FABS(taper) < GEOM_EPSILON)
            a = GeometryPtr(new Cone(baseradius,length*frameinfo.scaling.z(), false, sectionResolution));
        else if (FABS(taper-1.0) < GEOM_EPSILON)
            a = GeometryPtr(new Cylinder(baseradius, length*frameinfo.scaling.z(), false, sectionResolution));
        else
            a = GeometryPtr(new Frustum(baseradius, length*frameinfo.scaling.z(), taper, false, sectionResolution));
                  _addToScene(transform(GeometryPtr(new Scaled(getScale()*scale,GeometryPtr(new Oriented(Vector3(0,1,0),Vector3(0,0,1),smb)))),false));

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

void PglTurtleDrawer::void cylinder(const Vector3& position, 
                          const Vector3& heading, 
                          const Vector3& left, 
                          const Vector3& up, 
                          const Vector3& scaling, 
                          const uint_t id,
                          AppearancePtr appearance,
                          real_t length,
                          real_t radius,
                          uint_t sectionResolution){
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

void PglTurtleDrawer::box(real_t length, real_t botradius,  real_t topradius){
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

void PglTurtleDrawer::quad(real_t length, real_t botradius,  real_t topradius){
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



GeometryPtr PglTurtleDrawer::DEFAULT_SPHERE;

void PglTurtleDrawer::sphere(real_t radius){
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

GeometryPtr PglTurtleDrawer::getCircle(real_t radius) const{
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
PglTurtleDrawer::circle(real_t radius){
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

void PglTurtleDrawer::surface(const string& name,real_t scale){
  SurfaceMap::const_iterator it = __surfList.find(name);
  if (it == __surfList.end()){
    error("Unknown surface '" + name + '\'');
  }
  GeometryPtr obj(new Scaled(getScale()*scale,it->second));
  _addToScene(transform(obj,false));
}

#include "plantgl/algo/base/tesselator.h"

void
PglTurtleDrawer::polygon(const Point3ArrayPtr& pointList, bool concavetest){
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
PglTurtleDrawer::generalizedCylinder(const Point3ArrayPtr& points,
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
PglTurtleDrawer::label(const string& text, int size ){
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

AppearancePtr PglTurtleDrawer::HEADING_FRAME_MATERIAL(new Material("HEADING_FRAME_MATERIAL",Color3(250,50,50),1));
AppearancePtr PglTurtleDrawer::UP_FRAME_MATERIAL(new Material("UP_FRAME_MATERIAL",Color3(50,50,250),1));
AppearancePtr PglTurtleDrawer::LEFT_FRAME_MATERIAL(new Material("LEFT_FRAME_MATERIAL",Color3(50,250,50),1));

void PglTurtleDrawer::frame(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio, real_t color, real_t transparency) {
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


AppearancePtr PglTurtleDrawer::getCurrentMaterial() const{
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

AppearancePtr PglTurtleDrawer::getCurrentInitialMaterial() const{
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

ScenePtr PglTurtleDrawer::partialView(const uint_t id,
                                      AppearancePtr appearance,
                                      bool generalizedCylinderOn,
                                      Point3ArrayPtr& pointList,
                                      std::vector<Vector3>& leftList,
                                      std::vector<real_t>& radiusList,
                                      TurtleDrawParameter& initial) {
    ScenePtr currentscene = new Scene(*__scene);
    if(generalizedCylinderOn && pointList->size() > 1){
        this->generalizedCylinder(id, appearance, pointList, leftList, radiusList, initial.crossSection, initial.crossSectionCCW);
    }
    frame();
    ScenePtr result = __scene;
    __scene = currentscene;
    return result;
}

