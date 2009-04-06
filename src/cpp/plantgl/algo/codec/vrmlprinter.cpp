/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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





#include "vrmlprinter.h"
#include <plantgl/algo/base/discretizer.h>

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/scene/shape.h>

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_VRMLPRINT_INCREMENT_INDENT \
  __indent += "    ";


#define GEOM_VRMLPRINT_DECREMENT_INDENT \
  __indent.erase(__indent.end() - 4,__indent.end());

/*
#define GEOM_VRMLPRINT_BEGINSHAPE(obj){ \
  if (obj->isNamed()) { \
    uint_t appid = 0; \
    if(__app)appid= __app->getId(); \
    if (! __shapecache.insert((number(obj->getId())+"_"+number(appid)).c_str()).second){ \
      __geomStream << "USE SHAPE_" << obj->getId() <<"_" << appid << endl; \
      return true; \
    } \
      __geomStream << "DEF SHAPE_" << obj->getId() <<"_" << appid << " Shape { " << endl; \
  } \
  else { \
    __geomStream << "Shape { " << endl; \
  } \
  GEOM_VRMLPRINT_INCREMENT_INDENT; \
  __geomStream << __indent << "appearance Appearance {" << endl; \
  if(__app){ \
  GEOM_VRMLPRINT_INCREMENT_INDENT; \
  __geomStream << __indent << "material "; \
  __app->apply(*this); \
  GEOM_VRMLPRINT_DECREMENT_INDENT \
  } \
  __geomStream << __indent << '}' << endl; \
  __geomStream << __indent << "geometry "; \
 };
*/

/*  if (obj->isNamed()) { \
    uint_t appid = 0; \
    if(__app)appid= __app->getId(); \
    if (!__shapecache.insert(pair<uint_t,uint_t>(obj->getId(),appid)).second ){ \
      __geomStream << "USE SHAPE_" << obj->getId() <<"_" << appid << endl; \
      return true; \
    } \
    else { \
      __geomStream << "DEF SHAPE_" << obj->getId() <<"_" << appid << " Shape { " << endl; \
    } \
  } \
  else { \
    __geomStream << "Shape { " << endl; \
  } \
*/

#define GEOM_VRMLPRINT_BEGINSHAPE(obj) \
  bool shapeused = false; \
  if (obj->isNamed()) { \
    uint_t appid = 0; \
    if(__app)appid= __app->getId(); \
    if (!__shapecache.insert(pair<uint_t,uint_t>(obj->getId(),appid)).second ){ \
      if(__app->isNamed())  \
      __geomStream << "USE SHAPE_" << obj->getName().c_str() <<"_" << __app->getName().c_str() << endl; \
      else __geomStream << "USE SHAPE_" << obj->getName().c_str() <<"_" << appid << endl; \
      shapeused = true; \
    } \
    else { \
      if(__app->isNamed())  \
      __geomStream << "DEF SHAPE_" << obj->getName().c_str() <<"_" << __app->getName().c_str() << " Shape { " << endl; \
      else __geomStream << "DEF SHAPE_" << obj->getName().c_str() <<"_" << appid << " Shape { " << endl; \
    } \
  } \
  else { \
    __geomStream << "Shape { " << endl; \
  } \
  if(!shapeused) { \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   __geomStream << __indent << "appearance Appearance {" << endl; \
   if(__app){ \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   __geomStream << __indent << "material "; \
   __app->apply(*this); \
   GEOM_VRMLPRINT_DECREMENT_INDENT \
   } \
   __geomStream << __indent << '}' << endl; \
   __geomStream << __indent << "geometry "; \
  }


#define GEOM_VRMLPRINT_BEGINOBJ(type,obj) \
  bool used = shapeused; \
  if (obj->isNamed()) { \
    if (! __cache.insert(obj->getId()).second) { \
      __geomStream  <<  "USE " << obj->getName().c_str() << endl; \
      __geomStream  <<  __indent << "#" << obj->getId() << endl; \
      used = true; \
    } \
    else { \
      __geomStream  <<  "DEF " << obj->getName().c_str() << " " << type << " { " << endl; \
      __geomStream  <<  __indent << "#" << obj->getId() << endl; \
       GEOM_VRMLPRINT_INCREMENT_INDENT; \
    } \
  } \
  else{ \
    __geomStream <<  type << " { " << endl; \
    GEOM_VRMLPRINT_INCREMENT_INDENT; \
  } \

#define GEOM_VRMLPRINT_BEGINGROUP(type,obj) \
  bool used = false; \
  if (obj->isNamed()) { \
    uint_t appid = 0; \
    if(__app)appid= __app->getId(); \
    if (!__shapecache.insert(pair<uint_t,uint_t>(obj->getId(),appid)).second ){ \
      if(__app->isNamed())  \
      __geomStream << "USE " << obj->getName().c_str() << "_" << __app->getName().c_str() << endl; \
      else __geomStream << "USE " << obj->getName().c_str() << "_" << appid << endl; \
      used = true; \
    } \
    else { \
      if(__app->isNamed())  \
      __geomStream << "DEF " << obj->getName().c_str()  << "_" << __app->getName().c_str() << " " << type << " { " << endl; \
      else __geomStream << "DEF " << obj->getName().c_str()  << "_" << appid << " " << type << " { " << endl; \
       GEOM_VRMLPRINT_INCREMENT_INDENT; \
    } \
  } \
  else{ \
    __geomStream <<  type << " { " << endl; \
    GEOM_VRMLPRINT_INCREMENT_INDENT; \
  } \

#define GEOM_VRMLPRINT_BEGINSIMPLE(type,obj) \
 if(!used){ \
  __geomStream <<  type << " { "; \
  if (obj->isNamed())__geomStream << "# " << obj->getName().c_str(); \
  __geomStream << endl; \
  GEOM_VRMLPRINT_INCREMENT_INDENT; \
 } \

#define GEOM_VRMLPRINT_ENDSIMPLE  \
  if(!used){ \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << __indent << "}" << endl; \
  } \

#define GEOM_VRMLPRINT_ENDOBJ  \
  if(!used){ \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << __indent << "}" << endl; \
  } \

#define GEOM_VRMLPRINT_ENDSHAPE { \
  if(!shapeused){ \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << __indent << "}" << endl; \
  } \
 };

#define GEOM_VRMLPRINT_FIELD(name,val,type) { \
   if(!used){ \
    __geomStream << __indent << name << " "; \
    GEOM_VRMLPRINT_##type(val); \
    __geomStream << endl; \
   } \
  };


#define GEOM_VRMLPRINT_BOOLEAN(val) \
  __geomStream << (val ? "TRUE" : "FALSE");

#define GEOM_VRMLPRINT_ANGLE(val) \
  __geomStream << (val*GEOM_DEG);

#define GEOM_VRMLPRINT_REAL(val) \
  __geomStream << val;

#define GEOM_VRMLPRINT_STRING(val) \
  __geomStream << '"' << val << '"';

#define GEOM_VRMLPRINT_SUBNODE(val) \
  val->apply(*this); \

#define GEOM_VRMLPRINT_CHILDREN(val) \
  __geomStream << " [ " << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
  __geomStream << __indent; \
  val->apply(*this); \
  GEOM_VRMLPRINT_DECREMENT_INDENT; \
  __geomStream << __indent << ']'; \

#define GEOM_VRMLPRINT_CHILDRENARRAY(val) \
  __geomStream << " [ " << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
  __geomStream << __indent; \
  for(uint_t _i = 0 ; _i < val->size(); _i++){ \
   val->getAt(_i)->apply(*this); \
   if(_i != val->size()-1)__geomStream << __indent << ',' << endl << __indent ; \
  } \
  GEOM_VRMLPRINT_DECREMENT_INDENT; \
  __geomStream << __indent << ']'; \


#define GEOM_VRMLPRINT_COLOR3(val) \
  __geomStream << val.getRedClamped() << ' ' << val.getGreenClamped() << ' ' << val.getBlueClamped(); \


#define GEOM_VRMLPRINT_VECTOR2(val) \
  __geomStream << val.y() \
         << " " << val.x();


#define GEOM_VRMLPRINT_VECTOR3(val) \
  __geomStream << val.y() \
         << " " << val.z() \
         << " " << val.x();


#define GEOM_VRMLPRINT_VECTOR4(val) \
  __geomStream << val.y() \
         << " " << val.z() \
         << " " << val.x() \
         << " " << val.w();

#define GEOM_VRMLPRINT_ROT4(val) \
  __geomStream << val.x() \
         << " " << val.y() \
         << " " << val.z() \
         << " " << val.w();

#define GEOM_VRMLPRINT_POINT3ARRAY(val){ \
   __geomStream << "Coordinate {" << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   __geomStream << __indent  << "point ["  << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   for(Point3Array::iterator _i = val->begin();_i!=val->end();_i++){ \
      __geomStream << __indent << _i->y() \
         << " " << _i->z() \
         << " " << _i->x(); \
      if(_i!=val->end()-1)__geomStream << ',' << endl; \
   } \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << endl << __indent << ']' << endl; \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << __indent << '}'; \
 };

#define GEOM_VRMLPRINT_REALMATRIX(val){ \
   __geomStream << '[' << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   __geomStream << __indent; \
    uint_t _j =0 ; \
    for(RealArray2::const_iterator _i = val->begin();_i!=val->end();_i++){ \
      _j++; \
      GEOM_VRMLPRINT_REAL(*_i); \
      if(_i!=val->end()-1){ \
        __geomStream << " , "; \
        if(_j == val->getColsNb() ){ \
            _j = 0; \
            __geomStream << endl << __indent; \
        } \
      } \
   } \
   __geomStream  << ']' << endl; \
 };

#define GEOM_VRMLPRINT_NORMAL3ARRAY(val){ \
   __geomStream << "Normal {" << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   __geomStream << __indent  << "vector ["  << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   for(Point3Array::const_iterator _i = val->begin();_i!=val->end();_i++){ \
      __geomStream << __indent << _i->y() \
         << " " << _i->z() \
         << " " << _i->x(); \
      if(_i!=val->end()-1)__geomStream << ',' << endl; \
   } \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << endl << __indent << ']' << endl; \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << __indent << '}'; \
 };

#define GEOM_VRMLPRINT_INDEXARRAY(val){ \
   __geomStream << '[' << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   uint_t _sizej = val->size(); \
   for(uint_t _j = 0 ; _j < _sizej; _j++){ \
     __geomStream << __indent; \
     uint_t _sizek = val->getAt(_j).size(); \
     for(uint_t _k = 0 ; _k < _sizek ; _k++){ \
       __geomStream << (val->getAt(_j).getAt(_k)) << " , "; \
     } \
     __geomStream << "-1" ; \
     if(_j != _sizej-1)__geomStream << " , " << endl; \
   } \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << endl << __indent << ']'; \
 };

#define GEOM_VRMLPRINT_INDEXARRAY3(val){ \
   __geomStream << '[' << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   uint_t _sizej = val->size(); \
   for(uint_t _j = 0 ; _j < _sizej; _j++){ \
     __geomStream << __indent; \
     for(uint_t _k = 0 ; _k < 3 ; _k++){ \
       __geomStream << (val->getAt(_j).getAt(_k)) << " , "; \
     } \
     __geomStream << "-1" ; \
     if(_j != _sizej-1)__geomStream << " , "<< endl; \
   } \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << endl << __indent << ']'; \
 };

#define GEOM_VRMLPRINT_INDEXARRAY4(val){ \
   __geomStream << '[' << endl; \
   GEOM_VRMLPRINT_INCREMENT_INDENT; \
   uint_t _sizej = val->size(); \
   for(uint_t _j = 0 ; _j < _sizej; _j++){ \
     __geomStream << __indent; \
     for(uint_t _k = 0 ; _k < 4 ; _k++){ \
       __geomStream << (val->getAt(_j).getAt(_k)) << " , "; \
     } \
     __geomStream << "-1" ; \
     if(_j != _sizej-1)__geomStream << " , "<< endl; \
   } \
   GEOM_VRMLPRINT_DECREMENT_INDENT; \
   __geomStream << endl << __indent << ']'; \
 };

#define GEOM_VRMLDISCRETIZE(obj){ \
  obj->apply(__discretizer); \
  ExplicitModelPtr _explicit = __discretizer.getDiscretization(); \
  if (_explicit){ \
   if(obj->isNamed()) \
    _explicit->setName(obj->getName()); \
   return _explicit->apply(*this); \
  } \
  else return false; \
};

/* ----------------------------------------------------------------------- */


VrmlPrinter::VrmlPrinter( ostream& vrmlStream, Discretizer& discretizer ) :
  Printer(vrmlStream,vrmlStream,vrmlStream),
  __discretizer(discretizer),
  __app(){
}

VrmlPrinter::~VrmlPrinter( ) {
}

bool VrmlPrinter::header(const char * comment){
  __geomStream << "#VRML V2.0 utf8" << endl << endl;
  __geomStream << "# File create with GEOM." << endl;
  if(comment)__geomStream << "# " << comment << endl;
  __geomStream << endl;
  __geomStream << "WorldInfo {" << endl;
  bool used = false;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  string title = "A GEOM Scene";
  GEOM_VRMLPRINT_FIELD("title",title,STRING);
  if(comment)__geomStream << __indent << "info [ \"" << comment << "\" ]" << endl;
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << "}" << endl << endl;
  return true;
}

bool VrmlPrinter::setBackGround(const Color3& sky){
  __geomStream << "Background {" << endl;
  bool used = false;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  GEOM_VRMLPRINT_FIELD("skyColor",sky,COLOR3);
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << "}" << endl << endl;
  return true;
}

bool VrmlPrinter::setCamera(const Vector3& position,
                                                        const real_t& az,
                                                        const real_t& el,
                                                        const string& name){
  bool used = false;
  if(fabs(az) > GEOM_EPSILON ){
    __geomStream << "Transform {" << endl;
    GEOM_VRMLPRINT_INCREMENT_INDENT;
        Vector4 rot(Vector3::OZ, az * GEOM_RAD);
        GEOM_VRMLPRINT_FIELD("rotation",rot,VECTOR4);
  __geomStream << __indent << "children ";
  }

  if(fabs(el) > GEOM_EPSILON ){
    __geomStream << "Transform {" << endl;
    GEOM_VRMLPRINT_INCREMENT_INDENT;
        Vector4 rot(Vector3::OY, -el * GEOM_RAD);
        GEOM_VRMLPRINT_FIELD("rotation",rot,VECTOR4);
    __geomStream << __indent << "children ";
  }

  __geomStream << "Viewpoint {" << endl;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  GEOM_VRMLPRINT_FIELD("position",position,VECTOR3);
  GEOM_VRMLPRINT_FIELD("description",name,STRING);
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << __indent << "}" << endl;
  if(fabs(el) > GEOM_EPSILON ){
        GEOM_VRMLPRINT_DECREMENT_INDENT;
        __geomStream << "}" << endl;
  }
  if(fabs(az) > GEOM_EPSILON ){
        GEOM_VRMLPRINT_DECREMENT_INDENT;
        __geomStream << __indent << "}" << endl;
  }
  __geomStream << endl;
  return true;

}

#define MIN3(a,b,c) min(a,min(b,c))

bool VrmlPrinter::setLight(const TOOLS(Vector3)& location,
                                                   const Color3& ambient,
                                                   const Color3& diffuse,
                                                   const real_t& radius){
  bool used = false;
  __geomStream << "PointLight {" << endl;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  real_t diff = MIN3(( 255.0 / (real_t)diffuse.getRed()   ),
                                   ( 255.0 / (real_t)diffuse.getGreen() ),
                                   ( 255.0 / (real_t)diffuse.getBlue()  ) );
  Color3 col(diffuse.getRed()*diff,diffuse.getGreen()*diff,diffuse.getBlue()*diff);
  diff = 1.0 / diff;
  GEOM_VRMLPRINT_FIELD("color",col,COLOR3);
  GEOM_VRMLPRINT_FIELD("intensity",diff,REAL);
  real_t amb =(( (real_t)ambient.getRed()   / (real_t)diffuse.getRed()   ) +
                         ( (real_t)ambient.getGreen() / (real_t)diffuse.getGreen() ) +
                         ( (real_t)ambient.getBlue()  / (real_t)diffuse.getBlue()  ) );
  amb /= 3;
  amb *= diff;
  GEOM_VRMLPRINT_FIELD("ambientIntensity",amb,REAL);
  GEOM_VRMLPRINT_FIELD("location",location,VECTOR3);
  GEOM_VRMLPRINT_FIELD("radius",radius,REAL);
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << __indent << "}" << endl << endl;
  return true;
}

/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process(Shape * Shape) {
  GEOM_ASSERT(Shape);
  __app = Shape->appearance;
  return (Shape->geometry->apply(*this));
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Material * material ) {
  GEOM_ASSERT(material);

  bool shapeused = false;
  GEOM_VRMLPRINT_BEGINOBJ("Material",material);
  Color3 a(uchar_t(material->getDiffuse()*material->getAmbient().getRed()),
           uchar_t(material->getDiffuse()*material->getAmbient().getGreen()),
           uchar_t(material->getDiffuse()*material->getAmbient().getBlue()));
  GEOM_VRMLPRINT_FIELD("diffuseColor",a,COLOR3);
  real_t b = 0.0;
  if(fabs(material->getDiffuse())>GEOM_EPSILON) b = (real_t)1.0 / (real_t)material->getDiffuse();
  GEOM_VRMLPRINT_FIELD("ambientIntensity",b,REAL);
  GEOM_VRMLPRINT_FIELD("specularColor",material->getSpecular(),COLOR3);
  GEOM_VRMLPRINT_FIELD("emissiveColor",material->getEmission(),COLOR3);
  GEOM_VRMLPRINT_FIELD("shininess",material->getShininess(),REAL);
  GEOM_VRMLPRINT_FIELD("transparency",material->getTransparency(),REAL);

  GEOM_VRMLPRINT_ENDOBJ;

  return true;
}


/* ----------------------------------------------------------------------- */
bool VrmlPrinter::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);

  bool shapeused = false;
  GEOM_VRMLPRINT_BEGINOBJ("Material",texture);
  Color3 a(uchar_t(texture->getDiffuse()*texture->getAmbient().getRed()),
           uchar_t(texture->getDiffuse()*texture->getAmbient().getGreen()),
           uchar_t(texture->getDiffuse()*texture->getAmbient().getBlue()));
  GEOM_VRMLPRINT_FIELD("diffuseColor",a,COLOR3);
  real_t b = 0.0;
  if(fabs(texture->getDiffuse())>GEOM_EPSILON) b = (real_t)1.0 / (real_t)texture->getDiffuse();
  GEOM_VRMLPRINT_FIELD("ambientIntensity",b,REAL);
  GEOM_VRMLPRINT_FIELD("specularColor",texture->getSpecular(),COLOR3);
  GEOM_VRMLPRINT_FIELD("emissiveColor",texture->getEmission(),COLOR3);
  GEOM_VRMLPRINT_FIELD("shininess",texture->getShininess(),REAL);
  GEOM_VRMLPRINT_FIELD("transparency",texture->getTransparency(),REAL);

  GEOM_VRMLPRINT_ENDOBJ;

  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  bool shapeused = false;
  GEOM_VRMLPRINT_BEGINOBJ("Material",monoSpectral);

  GEOM_VRMLPRINT_ENDOBJ;

  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  bool shapeused = false;
  GEOM_VRMLPRINT_BEGINOBJ("Material",multiSpectral);

  GEOM_VRMLPRINT_ENDOBJ;

  return true;
}



/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  GEOM_VRMLPRINT_BEGINSHAPE(amapSymbol);
  GEOM_VRMLPRINT_BEGINOBJ("IndexedFaceSet",amapSymbol);
  GEOM_VRMLPRINT_FIELD("coord",amapSymbol->getPointList(),POINT3ARRAY);
  GEOM_VRMLPRINT_FIELD("coordIndex",amapSymbol->getIndexList(),INDEXARRAY);
  if(amapSymbol->getNormalList())
	GEOM_VRMLPRINT_FIELD("normal",amapSymbol->getNormalList(),NORMAL3ARRAY);
  GEOM_VRMLPRINT_FIELD("normalPerVertex",true,BOOLEAN);
  GEOM_VRMLPRINT_FIELD("solid",amapSymbol->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);
  GEOM_VRMLDISCRETIZE(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);
  GEOM_VRMLPRINT_BEGINGROUP("Transform",axisRotated);

  Vector4 v(axisRotated->getAxis(),axisRotated->getAngle());
  GEOM_VRMLPRINT_FIELD("rotation",v,VECTOR4);
  GEOM_VRMLPRINT_FIELD("children",axisRotated->getGeometry(),CHILDREN);

  GEOM_VRMLPRINT_ENDOBJ;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  GEOM_VRMLDISCRETIZE(bezierCurve);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);
  GEOM_VRMLDISCRETIZE(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Box * box ) {
  GEOM_ASSERT(box);
  GEOM_VRMLPRINT_BEGINSHAPE(box);
  GEOM_VRMLPRINT_BEGINOBJ("Box",box);
  Vector3 size = box->getSize()*2;
  GEOM_VRMLPRINT_FIELD("size",size,VECTOR3);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Cone * cone ) {
  GEOM_ASSERT(cone);
  __geomStream <<  "Transform { "<< endl;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  Vector3 a(0,0,cone->getHeight()/2);
  __geomStream << __indent << "translation ";
  GEOM_VRMLPRINT_VECTOR3(a);
  __geomStream << endl;
  __geomStream << __indent << "children [" << endl;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  __geomStream << __indent;
  GEOM_VRMLPRINT_BEGINSHAPE(cone);
  GEOM_VRMLPRINT_BEGINOBJ("Cone",cone);
  GEOM_VRMLPRINT_FIELD("bottomRadius",cone->getRadius(),REAL);
  GEOM_VRMLPRINT_FIELD("height",cone->getHeight(),REAL);
  GEOM_VRMLPRINT_FIELD("side",true,BOOLEAN);
  GEOM_VRMLPRINT_FIELD("bottom",cone->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << __indent << "]" << endl;
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << __indent << "}" << endl;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);
  __geomStream <<  "Transform { "<< endl;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  Vector3 a(0,0,cylinder->getHeight()/2);
  __geomStream << __indent << "translation ";
  GEOM_VRMLPRINT_VECTOR3(a);
  __geomStream << endl;
  __geomStream << __indent << "children [" << endl;
  GEOM_VRMLPRINT_INCREMENT_INDENT;
  __geomStream << __indent;
  GEOM_VRMLPRINT_BEGINSHAPE(cylinder);
  GEOM_VRMLPRINT_BEGINOBJ("Cylinder",cylinder);
  GEOM_VRMLPRINT_FIELD("radius",cylinder->getRadius(),REAL);
  GEOM_VRMLPRINT_FIELD("height",cylinder->getHeight(),REAL);
  GEOM_VRMLPRINT_FIELD("side",true,BOOLEAN);
  GEOM_VRMLPRINT_FIELD("bottom",cylinder->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_FIELD("top",cylinder->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << __indent << "]" << endl;
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << __indent << "}" << endl;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);
  GEOM_VRMLPRINT_BEGINSHAPE(elevationGrid);
  GEOM_VRMLPRINT_BEGINOBJ("ElevationGrid",elevationGrid);
  GEOM_VRMLPRINT_FIELD("xDimension",elevationGrid->getYDim(),REAL);
  GEOM_VRMLPRINT_FIELD("xSpacing",elevationGrid->getYSpacing(),REAL);
  GEOM_VRMLPRINT_FIELD("zDimension",elevationGrid->getXDim(),REAL);
  GEOM_VRMLPRINT_FIELD("zSpacing",elevationGrid->getXSpacing(),REAL);
  GEOM_VRMLPRINT_FIELD("ccw",elevationGrid->getCCW(),BOOLEAN);
  GEOM_VRMLPRINT_FIELD("height",elevationGrid->getHeightList(),REALMATRIX);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);

  Vector4 a(0,0,1,eulerRotated->getAzimuth() );
  Vector4 e(0,1,0,eulerRotated->getElevation());
  Vector4 r(1,0,0,eulerRotated->getRoll());

  GEOM_VRMLPRINT_BEGINGROUP("Transform",eulerRotated);
  GEOM_VRMLPRINT_FIELD("rotation",a,VECTOR4);
  {
  if(!used)    __geomStream << __indent << "children ";
  GEOM_VRMLPRINT_BEGINSIMPLE("Transform",eulerRotated);
  GEOM_VRMLPRINT_FIELD("rotation",e,VECTOR4);
  {
  if(!used)    __geomStream << __indent << "children ";
  GEOM_VRMLPRINT_BEGINSIMPLE("Transform",eulerRotated);
  GEOM_VRMLPRINT_FIELD("rotation",r,VECTOR4);
  GEOM_VRMLPRINT_FIELD("children",eulerRotated->getGeometry(),CHILDREN);
  GEOM_VRMLPRINT_ENDSIMPLE;
  }
  GEOM_VRMLPRINT_ENDSIMPLE;
  }
  GEOM_VRMLPRINT_ENDOBJ;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);
  GEOM_VRMLDISCRETIZE(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  GEOM_VRMLPRINT_BEGINSHAPE(faceSet);
  GEOM_VRMLPRINT_BEGINOBJ("IndexedFaceSet",faceSet);
  GEOM_VRMLPRINT_FIELD("coord",faceSet->getPointList(),POINT3ARRAY);
  GEOM_VRMLPRINT_FIELD("coordIndex",faceSet->getIndexList(),INDEXARRAY);
  if(faceSet->getNormalList())
    GEOM_VRMLPRINT_FIELD("normal",faceSet->getNormalList(),NORMAL3ARRAY);
  GEOM_VRMLPRINT_FIELD("normalPerVertex",faceSet->getNormalPerVertex(),BOOLEAN);
  GEOM_VRMLPRINT_FIELD("solid",faceSet->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);
  GEOM_VRMLDISCRETIZE(frustum);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Extrusion * extrusion   ) {
  GEOM_ASSERT(extrusion);
  GEOM_VRMLDISCRETIZE(extrusion);
}

/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Group * group ) {
  GEOM_ASSERT(group);
  GEOM_VRMLPRINT_BEGINGROUP("Group",group);
  GEOM_VRMLPRINT_FIELD("children",group->getGeometryList(),CHILDRENARRAY);
  GEOM_VRMLPRINT_ENDOBJ;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( IFS * ifs ) {
  GEOM_ASSERT(ifs);
  GEOM_VRMLDISCRETIZE(ifs);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  GEOM_VRMLDISCRETIZE(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);
  GEOM_VRMLDISCRETIZE(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);
  OrthonormalBasis3DPtr basis = oriented->getOrthonormalBasis();
  Vector3 rotation = basis->extractEulerAngles();
  if(rotation == Vector3::ORIGIN) return oriented->getGeometry()->apply(*this);
  GEOM_VRMLPRINT_BEGINGROUP("Transform",oriented);

  if(!used){
          __geomStream << __indent << "#Oriented" << endl;

      if(fabs(rotation.x())>GEOM_EPSILON){
          Vector4 a(0,0,1,rotation.x());
          GEOM_VRMLPRINT_FIELD("rotation",a,VECTOR4);
      }
      if(fabs(rotation.y())>GEOM_EPSILON){
          __geomStream << __indent << "children Transform { " << endl;
          GEOM_VRMLPRINT_INCREMENT_INDENT;
          Vector4 b(0,1,0,rotation.y());
          GEOM_VRMLPRINT_FIELD("rotation",b,VECTOR4);
      }
      if(fabs(rotation.z())>GEOM_EPSILON){
          __geomStream << __indent << "children Transform { " << endl;
          GEOM_VRMLPRINT_INCREMENT_INDENT;
          Vector4 c(1,0,0,rotation.z());
          GEOM_VRMLPRINT_FIELD("rotation",c,VECTOR4);
      }

      GEOM_VRMLPRINT_FIELD("children",oriented->getGeometry(),CHILDREN);

      if(fabs(rotation.z())>GEOM_EPSILON){
          GEOM_VRMLPRINT_DECREMENT_INDENT;
          __geomStream << __indent << '}' << endl;
      }
      if(fabs(rotation.y())>GEOM_EPSILON){
          GEOM_VRMLPRINT_DECREMENT_INDENT;
          __geomStream << __indent << '}' << endl;
      }
  }
  GEOM_VRMLPRINT_ENDOBJ;
  return true;

}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);
  GEOM_VRMLDISCRETIZE(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  GEOM_VRMLPRINT_BEGINSHAPE(pointSet);
  GEOM_VRMLPRINT_BEGINOBJ("PointSet",pointSet);
  GEOM_VRMLPRINT_FIELD("coord",pointSet->getPointList(),POINT3ARRAY);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  GEOM_VRMLPRINT_BEGINSHAPE(polyline);
  GEOM_VRMLPRINT_BEGINOBJ("IndexedLineSet",polyline);
  GEOM_VRMLPRINT_FIELD("coord",polyline->getPointList(),POINT3ARRAY);
  if(!used){
   __geomStream << __indent <<"coordIndex [ " << endl;
   GEOM_VRMLPRINT_INCREMENT_INDENT;
   __geomStream << __indent;
   for(uint_t in = 0 ; in < polyline->getPointList()->size(); in++)
     __geomStream << in << " , ";
   GEOM_VRMLPRINT_DECREMENT_INDENT;
   __geomStream << "-1" << endl << __indent << ']' << endl;
  }
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);
  GEOM_VRMLDISCRETIZE(revolution );
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Swung * swung )
{
  GEOM_ASSERT(swung);
  GEOM_VRMLDISCRETIZE(swung);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  GEOM_VRMLPRINT_BEGINSHAPE(quadSet);
  GEOM_VRMLPRINT_BEGINOBJ("IndexedFaceSet",quadSet);
  GEOM_VRMLPRINT_FIELD("coord",quadSet->getPointList(),POINT3ARRAY);
  GEOM_VRMLPRINT_FIELD("coordIndex",quadSet->getIndexList(),INDEXARRAY4);
  if(quadSet->getNormalList())
	GEOM_VRMLPRINT_FIELD("normal",quadSet->getNormalList(),NORMAL3ARRAY);
  GEOM_VRMLPRINT_FIELD("normalPerVertex",quadSet->getNormalPerVertex(),BOOLEAN);
  GEOM_VRMLPRINT_FIELD("solid",quadSet->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);
  GEOM_VRMLPRINT_BEGINGROUP("Transform",scaled);

  GEOM_VRMLPRINT_FIELD("scale",scaled->getScale(),VECTOR3);
  GEOM_VRMLPRINT_FIELD("children",scaled->getGeometry(),CHILDREN);

  GEOM_VRMLPRINT_ENDOBJ;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);
  GEOM_VRMLPRINT_BEGINSHAPE(sphere);
  GEOM_VRMLPRINT_BEGINOBJ("Sphere",sphere);
  GEOM_VRMLPRINT_FIELD("radius",sphere->getRadius(),REAL);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);
  GEOM_VRMLDISCRETIZE(tapered);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Translated * translated ) {
  GEOM_ASSERT(translated);
  GEOM_VRMLPRINT_BEGINGROUP("Transform",translated);

  GEOM_VRMLPRINT_FIELD("translation",translated->getTranslation(),VECTOR3);
  GEOM_VRMLPRINT_FIELD("children",translated->getGeometry(),CHILDREN);

  GEOM_VRMLPRINT_ENDOBJ;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  GEOM_VRMLPRINT_BEGINSHAPE(triangleSet);
  GEOM_VRMLPRINT_BEGINOBJ("IndexedFaceSet",triangleSet);
  GEOM_VRMLPRINT_FIELD("coord",triangleSet->getPointList(),POINT3ARRAY);
  GEOM_VRMLPRINT_FIELD("coordIndex",triangleSet->getIndexList(),INDEXARRAY3);
  if(triangleSet->getNormalList())
	GEOM_VRMLPRINT_FIELD("normal",triangleSet->getNormalList(),NORMAL3ARRAY);
  GEOM_VRMLPRINT_FIELD("normalPerVertex",triangleSet->getNormalPerVertex(),BOOLEAN);
  GEOM_VRMLPRINT_FIELD("solid",triangleSet->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}


/* ------------------------------------------------------------------------- */


bool VrmlPrinter::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  GEOM_VRMLDISCRETIZE(bezierCurve);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( Disc * disc ) {
  GEOM_ASSERT(disc);
  GEOM_VRMLDISCRETIZE(disc);
}


/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  GEOM_VRMLDISCRETIZE(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool VrmlPrinter::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);
  GEOM_VRMLDISCRETIZE(pointSet);
}


/* ----------------------------------------------------------------------- */

bool VrmlPrinter::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);
  GEOM_VRMLDISCRETIZE(polyline);
}

/* ----------------------------------------------------------------------- */

bool VrmlPrinter::process( Text * text ) {
  GEOM_ASSERT(text);
  GEOM_VRMLPRINT_BEGINSHAPE(text);
  GEOM_VRMLPRINT_BEGINOBJ("Text",text);
  GEOM_VRMLPRINT_FIELD("string",text->getString(),STRING);
  GEOM_VRMLPRINT_FIELD("fontStyle",text->getFontStyle(),SUBNODE);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}

/* ----------------------------------------------------------------------- */

bool VrmlPrinter::process( Font * font ) {
  GEOM_ASSERT(font);
  GEOM_VRMLPRINT_BEGINSHAPE(font);
  GEOM_VRMLPRINT_BEGINOBJ("FontStyle",font);
  GEOM_VRMLPRINT_FIELD("family","SERIF",STRING);
  GEOM_VRMLPRINT_ENDOBJ;
  GEOM_VRMLPRINT_ENDSHAPE;
  return true;
}

/* ----------------------------------------------------------------------- */
