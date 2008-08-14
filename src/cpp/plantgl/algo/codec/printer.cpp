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





#include "printer.h"

#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>
#include <plantgl/tool/util_enviro.h>
#include <plantgl/tool/dirnames.h>
#include <time.h>
#include <iostream>

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_container.h>
#include <plantgl/scenegraph/geometry/profile.h>




TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_PRINT_INCREMENT_INDENT \
  __indent += "    ";


#define GEOM_PRINT_DECREMENT_INDENT \
  __indent.erase(__indent.end() - 4,__indent.end());


#define GEOM_PRINT_BEGIN(stream,type,obj) \
  if (obj->isNamed()) { \
    if (! __cache.insert(obj->getId()).second) { \
      stream << obj->getName().c_str() << endl; \
      return true; \
    }; \
    stream << type << " " << obj->getName().c_str() << " { " << endl; \
  } \
  else \
    stream << type << " { " << endl; \
  GEOM_PRINT_INCREMENT_INDENT;

#define GEOM_PRINT_END(stream) \
  GEOM_PRINT_DECREMENT_INDENT; \
  stream << __indent << "}";


#define GEOM_PRINT_APPEARANCE(stream,val) \
  val->apply(*this);


#define GEOM_PRINT_ANGLE(stream,val) \
  stream << (val * GEOM_DEG);


#define GEOM_PRINT_BOOLEAN(stream,val) \
  stream << (val ? "True" : "False");


#define GEOM_PRINT_COLOR3(stream,val) \
  if(val == Color3::BLACK)stream << "Black"; \
  else if(val == Color3::BLUE)stream << "Blue"; \
  else if(val == Color3::CYAN)stream << "Cyan"; \
  else if(val == Color3::GREEN)stream << "Green"; \
  else if(val == Color3::MAGENTA)stream << "Magenta"; \
  else if(val == Color3::RED)stream << "Red"; \
  else if(val == Color3::WHITE)stream << "White"; \
  else if(val == Color3::YELLOW)stream << "Yellow"; \
  else if(val.getRed() == val.getGreen() && val.getRed() == val.getBlue())stream << (uint16_t)val.getRed(); \
  else stream << "<" << (uint16_t)val.getRed() \
         << "," << (uint16_t)val.getGreen() \
         << "," << (uint16_t)val.getBlue() << ">";

#define GEOM_PRINT_COLOR4(stream,val) \
  if(val == Color4::BLACK)stream << "Black"; \
  else if(val == Color4::BLUE)stream << "Blue"; \
  else if(val == Color4::CYAN)stream << "Cyan"; \
  else if(val == Color4::GREEN)stream << "Green"; \
  else if(val == Color4::MAGENTA)stream << "Magenta"; \
  else if(val == Color4::RED)stream << "Red"; \
  else if(val == Color4::WHITE)stream << "White"; \
  else if(val == Color4::YELLOW)stream << "Yellow"; \
  else if(val.getRed() == val.getGreen() && val.getRed() == val.getBlue()&& val.getRed() == val.getAlpha())stream << (uint16_t)val.getRed(); \
  else stream << "<" << (uint16_t)val.getRed() \
         << "," << (uint16_t)val.getGreen() \
         << "," << (uint16_t)val.getBlue() \
         << "," << (uint16_t)val.getAlpha() << ">";


#define GEOM_PRINT_GEOMETRY(stream,val) \
  val->apply(*this);


#define GEOM_PRINT_INDEX3(stream,val) \
  stream << "[" << val.getAt(0) \
           << "," << val.getAt(1) \
           << "," << val.getAt(2) << "]";


#define GEOM_PRINT_INDEX4(stream,val) \
  stream << "[" << val.getAt(0) \
           << "," << val.getAt(1) \
           << "," << val.getAt(2) \
           << "," << val.getAt(3) << "]";


#define GEOM_PRINT_INDEXN(stream,val) { \
    stream << "["; \
    uint_t _sizej = val.getSize(); \
    for (uint_t _j = 0; _j < _sizej; _j++) \
      stream << val.getAt(_j) << (_j == (_sizej - 1) ? "]" : ","); \
  }


#define GEOM_PRINT_INTEGER(stream,val) \
  stream << (int_t)val;


#define GEOM_PRINT_REAL(stream,val) \
  stream << (real_t)val;


#define GEOM_PRINT_STRING(stream,val) \
  stream << "\"" << val.c_str() << "\"";


#define GEOM_PRINT_VECTOR2(stream,val) \
  stream << "<" << val.x() \
           << "," << val.y() << ">";


#define GEOM_PRINT_VECTOR3(stream,val) \
  stream << "<" << val.x() \
           << "," << val.y() \
           << "," << val.z() << ">";

#define GEOM_PRINT_VECTOR3P(stream,val) \
  stream << "<" << number(val.x()) \
           << "," << number(val.y()) \
           << "," << number(val.z()) << ">";


#define GEOM_PRINT_VECTOR4(stream,val) \
  stream << "<" << val.x() \
           << "," << val.y() \
           << "," << val.z() \
           << "," << val.w()<< ">";

#define GEOM_PRINT_SCALE(stream,scale) { \
  stream << __indent << "Scale" << " "; \
  GEOM_PRINT_VECTOR3(stream,scale); \
  stream << endl; \
  };

#define GEOM_PRINT_TRANSLATE(stream,translate) { \
  stream << __indent << "Translation" << " "; \
  GEOM_PRINT_VECTOR3(stream,translate); \
  stream << endl; \
  };

#define GEOM_PRINT_ROTATE(stream,rotate) { \
  stream << __indent << "EulerRotation " << " { "<<endl; \
  GEOM_PRINT_INCREMENT_INDENT; \
  stream << __indent << "Azimuth" << " "; \
  GEOM_PRINT_ANGLE(stream,rotate.x()); \
  stream << endl; \
  stream << __indent << "Elevation" << " "; \
  GEOM_PRINT_ANGLE(stream,rotate.y()); \
  stream << endl; \
  stream << __indent << "Roll" << " "; \
  GEOM_PRINT_ANGLE(stream,rotate.z()); \
  GEOM_PRINT_DECREMENT_INDENT; \
  stream << endl << __indent << "}" << endl; \
  };

#define GEOM_PRINT_TRANSFO(stream,transfo) { \
  stream << "Transfo " << " { "<<endl; \
  GEOM_PRINT_INCREMENT_INDENT; \
  Vector3 scale, rotate, translate; \
  transfo->getTransformation( scale, rotate, translate );\
  GEOM_PRINT_TRANSLATE(stream,translate); \
  GEOM_PRINT_SCALE(stream,scale); \
  if( normLinf(rotate) > GEOM_TOLERANCE ) { \
    GEOM_PRINT_ROTATE(stream,rotate); } \
  GEOM_PRINT_DECREMENT_INDENT; \
  stream << __indent << " }"; \
  };



#define GEOM_PRINT_FIELD(stream,obj,field,type) { \
    stream << __indent << #field << " "; \
    GEOM_PRINT_##type(stream,obj->get##field()); \
    stream << endl; \
  };


#define GEOM_PRINT_FIELD_ARRAY(stream,obj,field,type) { \
    stream << __indent << #field << " [ " << endl; \
    GEOM_PRINT_INCREMENT_INDENT; \
    stream << __indent; \
    uint_t _sizei = obj->get##field()->getSize(); \
    for (uint_t _i = 0; _i < _sizei; _i++) { \
      GEOM_PRINT_##type(stream,obj->get##field()->getAt(_i)); \
      if (_i != (_sizei - 1)) stream << ", " << endl << __indent ; \
    }; \
    GEOM_PRINT_DECREMENT_INDENT; \
    stream << endl << __indent << "]" << endl; \
  };


#define GEOM_PRINT_FIELD_MATRIX(stream,obj,field,type) { \
    uint_t _cols =obj->get##field()->getColsNb(); \
    stream << __indent << #field << " [" << endl; \
    GEOM_PRINT_INCREMENT_INDENT; \
    stream << __indent << " [ " ; \
    uint_t _sizei = obj->get##field()->getSize(); \
    for (uint_t _i = 0; _i < _sizei; _i++) { \
      GEOM_PRINT_##type(stream,obj->get##field()->getAt(_i / _cols ,_i % _cols)); \
      if (_i != (_sizei - 1)){ \
          if (_i !=0 && (_i+1) % (_cols) ==0){ stream << " ]," << endl;\
                         stream  << __indent << " [ ";} \
          else  stream << ", "; \
      } \
    }; \
    stream  << " ]" << endl; \
    GEOM_PRINT_DECREMENT_INDENT; \
    stream << __indent << "]" << endl; \
  };

#define GEOM_PRINT_SCENE(stream,val) \
  stream << '{' << endl; \
  Printer p(__shapeStream,__shapeStream,__shapeStream); \
  p.addIndent(__indent.size() + 4); \
  val->apply(p); \
  stream << __indent <<'}' << endl; \


/* ----------------------------------------------------------------------- */


Printer::Printer( ) :
  Action(),
  __shapeStream(cout),
  __geomStream(cout),
  __matStream(cout),
  __cache(),
  __indent() {
}

Printer::Printer( ostream& stream  ) :
  Action(),
  __shapeStream(stream),
  __geomStream(stream),
  __matStream(stream),
  __cache(),
  __indent() {
}

Printer::Printer( ostream& shapeStream, ostream& geomStream, ostream& matStream ) :
  Action(),
  __shapeStream(shapeStream),
  __geomStream(geomStream),
  __matStream(matStream),
  __cache(),
  __indent() {
}

Printer::~Printer( ) {
}

void Printer::clear( ) {
  __cache.clear();
}

void Printer::addIndent(uint_t i){
  __indent = "";
  for(uint_t j = 0; j<i; j++)__indent += " ";
}

bool Printer::header(const char * comment){
  return header(__shapeStream,NULL,comment);
}

bool Printer::header(ostream & _ostream,const char * filename,const char * comment){
  if(!_ostream)return false;
  _ostream << "(#--------------------------------------------------" << endl;
  _ostream << " #  " << endl;
  if(filename){
    _ostream << " #  File : " << filename << endl;
    _ostream << " #  " << endl;
  }
#ifdef __GNUC__
  _ostream << " #  File Author : " << getenv("LOGNAME") << endl;
  time_t temps;
  struct tm * tm;
  time (& temps);
  tm = localtime ( &temps);
  _ostream << " #  Date : " << tm->tm_mday << "/" << tm->tm_mon << "/" << (1900 + (tm->tm_year))
           << " at " << tm->tm_hour << ":" << tm->tm_min << ":"  << tm->tm_sec << endl;
  _ostream << " #  " << endl;
#endif
  _ostream << " #  A GEOM file generated with GEOM library" << endl;
  _ostream << " #  Published under the GNU General Public Licence. " << endl;
  _ostream << " #  " << endl;
  _ostream << " # --------------------------------------------------" << endl;
  _ostream << " #)" << endl << endl;
  return true;
}

bool Printer::isPrinted(SceneObjectPtr obj){
  if (!obj->isNamed()) return false;
  return !( (__cache.find(obj->getId())) == (__cache.end()));
}

/* ----------------------------------------------------------------------- */
bool Printer::process(Inline * geomInline){
    GEOM_ASSERT(geomInline);
    __shapeStream << __indent;
    GEOM_PRINT_BEGIN(__shapeStream,"Inline",geomInline);
    GEOM_PRINT_FIELD(__shapeStream,geomInline,Scene,SCENE);

    if(!geomInline->isTranslationToDefault())
       GEOM_PRINT_FIELD(__shapeStream,geomInline,Translation,VECTOR3);

    if(!geomInline->isScaleToDefault())
       GEOM_PRINT_FIELD(__shapeStream,geomInline,Scale,VECTOR3);

    GEOM_PRINT_END(__shapeStream);
     __shapeStream << endl;
    return true;
}

/* ----------------------------------------------------------------------- */


bool Printer::process(Shape * Shape){
    GEOM_ASSERT(Shape);

    if( Shape->geometry ){
        if ( (__cache.find(Shape->geometry->getId())) == (__cache.end())) {
            if(!Shape->geometry->isNamed()){
                Shape->geometry->setName("Geometry_"+number(Shape->geometry->getId()));
            }
            __geomStream << __indent;
            Shape->geometry->apply(*this);
            __geomStream << endl;
            __geomStream << endl;
        }
    }
    if(Shape->appearance){
      if ( (__cache.find(Shape->appearance->getId())) == (__cache.end())) {
	  if(!Shape->appearance->isNamed()){
		Shape->geometry->setName("Appearance_"+number(Shape->appearance->getId()));
	  }
        __matStream << __indent;
        Shape->appearance->apply(*this);
        __matStream << endl;
        __matStream << endl;
      }
    }
    __shapeStream << __indent << "Shape ";
    if(Shape->isNamed())__shapeStream << Shape->getName().c_str();
    __shapeStream << " { " << endl;
    GEOM_PRINT_INCREMENT_INDENT;
    if(Shape->id != Shape::NOID)
        __shapeStream << __indent << "Id  " << (Shape->id) << endl;
    if(Shape->parentId != Shape::NOID)
        __shapeStream << __indent << "ParentId  " << (Shape->parentId) << endl;
    if (Shape->geometry)
        __shapeStream << __indent << "Geometry  " <<
            (Shape->geometry->getName().c_str()) << endl;
    if(Shape->appearance)
        __shapeStream << __indent << "Appearance  " <<
            (Shape->appearance->getName().c_str()) << endl;
    GEOM_PRINT_END(__shapeStream);
     __shapeStream << endl;
     __shapeStream << endl;
    return true;
}

/* ----------------------------------------------------------------------- */


bool Printer::process( Material * material ) {
  GEOM_ASSERT(material);
  GEOM_PRINT_BEGIN(__matStream,"Material",material);

  if (! material->isAmbientToDefault())
    GEOM_PRINT_FIELD(__matStream,material,Ambient,COLOR3);

  if (! material->isDiffuseToDefault())
    GEOM_PRINT_FIELD(__matStream,material,Diffuse,REAL);

  if (! material->isSpecularToDefault())
    GEOM_PRINT_FIELD(__matStream,material,Specular,COLOR3);

  if (! material->isEmissionToDefault())
    GEOM_PRINT_FIELD(__matStream,material,Emission,COLOR3);

  if (! material->isShininessToDefault())
    GEOM_PRINT_FIELD(__matStream,material,Shininess,REAL);

  if (! material->isTransparencyToDefault())
    GEOM_PRINT_FIELD(__matStream,material,Transparency,REAL);

  GEOM_PRINT_END(__matStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool Printer::process( ImageTexture * texture ) {
  GEOM_ASSERT(material);
  GEOM_PRINT_BEGIN(__matStream,"ImageTexture",texture);

  GEOM_PRINT_FIELD(__matStream,texture,Filename,STRING);

  if (! texture->isMipmapingToDefault())
    GEOM_PRINT_FIELD(__matStream,texture,Mipmaping,BOOLEAN);

  if (! texture->isAmbientToDefault())
    GEOM_PRINT_FIELD(__matStream,texture,Ambient,COLOR3);

  if (! texture->isDiffuseToDefault())
    GEOM_PRINT_FIELD(__matStream,texture,Diffuse,REAL);

  if (! texture->isSpecularToDefault())
    GEOM_PRINT_FIELD(__matStream,texture,Specular,COLOR3);

  if (! texture->isEmissionToDefault())
    GEOM_PRINT_FIELD(__matStream,texture,Emission,COLOR3);

  if (! texture->isShininessToDefault())
    GEOM_PRINT_FIELD(__matStream,texture,Shininess,REAL);

  if (! texture->isTransparencyToDefault())
    GEOM_PRINT_FIELD(__matStream,texture,Transparency,REAL);

  GEOM_PRINT_END(__matStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool Printer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  GEOM_PRINT_BEGIN(__matStream,"MonoSpectral",monoSpectral);

   if (! monoSpectral->isReflectanceToDefault())
     GEOM_PRINT_FIELD(__matStream,monoSpectral,Reflectance,REAL);

   if (! monoSpectral->isTransmittanceToDefault())
     GEOM_PRINT_FIELD(__matStream,monoSpectral,Transmittance,REAL);

  GEOM_PRINT_END(__matStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  GEOM_PRINT_BEGIN(__matStream,"MultiSpectral",multiSpectral);

  GEOM_PRINT_FIELD_ARRAY(__matStream,multiSpectral,Reflectance,REAL);

  GEOM_PRINT_FIELD_ARRAY(__matStream,multiSpectral,Transmittance,REAL);

  if (! multiSpectral->isFilterToDefault())
    GEOM_PRINT_FIELD(__matStream,multiSpectral,Filter,INDEX3);

  GEOM_PRINT_END(__matStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  GEOM_PRINT_BEGIN(__geomStream,"AmapSymbol",amapSymbol);

  string plantgl_dir = getPlantGLDir();
  string f = amapSymbol->getFileName();
  string pref = short_dirname(get_dirname(f));
  if(pref.size() > plantgl_dir.size())
    pref = string(pref.begin(),pref.begin()+plantgl_dir.size());

  if(similar_dir(pref,plantgl_dir)){
        int count = 0;
        for(string::const_iterator _i = pref.begin();
                _i != pref.end(); _i++)if(*_i == '\\' || *_i == '/')count++;
        string::iterator _j = f.begin();
        for(;_j != f.end() && count>0; _j++)if(*_j == '\\' || *_j == '/')count--;
        if(*(pref.end()-1) != '\\' && *(pref.end()-1) != '/'){
                while(*_j != '\\' && *_j != '/')_j++;
        }
        f = string(_j,f.end());
        __geomStream << __indent << "FileName PLANTGL_DIR+\"";
        __geomStream << f << '"' << endl;
  }
  else GEOM_PRINT_FIELD(__geomStream,amapSymbol,FileName,STRING);

  if (! amapSymbol->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,amapSymbol,Solid,BOOLEAN);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);
  GEOM_PRINT_BEGIN(__geomStream,"AsymmetricHull",asymmetricHull);

  if (! asymmetricHull->isNegXRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,NegXRadius,REAL);

  if (! asymmetricHull->isPosXRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,PosXRadius,REAL);

  if (! asymmetricHull->isNegYRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,NegYRadius,REAL);

  if (! asymmetricHull->isPosYRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,PosYRadius,REAL);

  if (! asymmetricHull->isNegXHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,NegXHeight,REAL);

  if (! asymmetricHull->isPosXHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,PosXHeight,REAL);

  if (! asymmetricHull->isNegYHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,NegYHeight,REAL);

  if (! asymmetricHull->isPosYHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,PosYHeight,REAL);

  if (! asymmetricHull->isBottomToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,Bottom,VECTOR3);

  if (! asymmetricHull->isTopToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,Top,VECTOR3);

  if (! asymmetricHull->isBottomShapeToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,BottomShape,REAL);

  if (! asymmetricHull->isTopShapeToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,TopShape,REAL);

  if (! asymmetricHull->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,Slices,INTEGER);

  if (! asymmetricHull->isStacksToDefault())
    GEOM_PRINT_FIELD(__geomStream,asymmetricHull,Stacks,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);
  GEOM_PRINT_BEGIN(__geomStream,"AxisRotated",axisRotated);

  if (! axisRotated->isAxisToDefault())
    GEOM_PRINT_FIELD(__geomStream,axisRotated,Axis,VECTOR3);

  if (! axisRotated->isAngleToDefault())
    GEOM_PRINT_FIELD(__geomStream,axisRotated,Angle,ANGLE);

  GEOM_PRINT_FIELD(__geomStream,axisRotated,Geometry,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  GEOM_PRINT_BEGIN(__geomStream,"BezierCurve",bezierCurve);

  GEOM_PRINT_FIELD(__geomStream,bezierCurve,Degree,INTEGER);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,bezierCurve,CtrlPointList,VECTOR4);

  if (! bezierCurve->isStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,bezierCurve,Stride,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);
  GEOM_PRINT_BEGIN(__geomStream,"BezierPatch",bezierPatch);

  GEOM_PRINT_FIELD(__geomStream,bezierPatch,UDegree,INTEGER);

  GEOM_PRINT_FIELD(__geomStream,bezierPatch,VDegree,INTEGER);

  GEOM_PRINT_FIELD_MATRIX(__geomStream,bezierPatch,CtrlPointMatrix,VECTOR4);

  if (! bezierPatch->isUStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,bezierPatch,UStride,INTEGER);

  if (! bezierPatch->isVStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,bezierPatch,VStride,INTEGER);

  if (! bezierPatch->isCCWToDefault())
    GEOM_PRINT_FIELD(__geomStream, bezierPatch,CCW,BOOLEAN);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Box * box ) {
  GEOM_ASSERT(box);
  GEOM_PRINT_BEGIN(__geomStream,"Box",box);

  if (! box->isSizeToDefault())
    GEOM_PRINT_FIELD(__geomStream,box,Size,VECTOR3);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Cone * cone ) {
  GEOM_ASSERT(cone);
  GEOM_PRINT_BEGIN(__geomStream,"Cone",cone);

  if (! cone->isRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,cone,Radius,REAL);

  if (! cone->isHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,cone,Height,REAL);

  if (! cone->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,cone,Solid,BOOLEAN);

  if (! cone->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,cone,Slices,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);
  GEOM_PRINT_BEGIN(__geomStream,"Cylinder",cylinder);

  if (! cylinder->isRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,cylinder,Radius,REAL);

  if (! cylinder->isHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,cylinder,Height,REAL);

  if (! cylinder->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,cylinder,Solid,BOOLEAN);

  if (! cylinder->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,cylinder,Slices,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);
  GEOM_PRINT_BEGIN(__geomStream,"ElevationGrid",elevationGrid);

  GEOM_PRINT_FIELD_MATRIX(__geomStream,elevationGrid,HeightList,REAL);

  if (! elevationGrid->isXSpacingToDefault()) {
    GEOM_PRINT_FIELD(__geomStream,elevationGrid,XSpacing,REAL);
  };

  if (! elevationGrid->isYSpacingToDefault()) {
    GEOM_PRINT_FIELD(__geomStream,elevationGrid,YSpacing,REAL);
  };

  if (! elevationGrid->isCCWToDefault())
    GEOM_PRINT_FIELD(__geomStream, elevationGrid,CCW,BOOLEAN);


  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);
  GEOM_PRINT_BEGIN(__geomStream,"EulerRotated",eulerRotated);

  if (! eulerRotated->isAzimuthToDefault())
    GEOM_PRINT_FIELD(__geomStream,eulerRotated,Azimuth,ANGLE);

  if (! eulerRotated->isElevationToDefault())
    GEOM_PRINT_FIELD(__geomStream,eulerRotated,Elevation,ANGLE);

  if (! eulerRotated->isRollToDefault())
    GEOM_PRINT_FIELD(__geomStream,eulerRotated,Roll,ANGLE);

  GEOM_PRINT_FIELD(__geomStream,eulerRotated,Geometry,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);
  GEOM_PRINT_BEGIN(__geomStream,"ExtrudedHull",extrudedHull);

  GEOM_PRINT_FIELD(__geomStream,extrudedHull,Vertical,GEOMETRY);

  GEOM_PRINT_FIELD(__geomStream,extrudedHull,Horizontal,GEOMETRY);

  if (! extrudedHull->isCCWToDefault())
    GEOM_PRINT_FIELD(__geomStream,extrudedHull,CCW,BOOLEAN);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Extrusion * extrusion ) {
  GEOM_ASSERT(extrusion);
  GEOM_PRINT_BEGIN(__geomStream,"Extrusion",extrusion);

  GEOM_PRINT_FIELD(__geomStream,extrusion,Axis,GEOMETRY);

  GEOM_PRINT_FIELD(__geomStream,extrusion,CrossSection,GEOMETRY);

  if(extrusion->getProfileTransformation()){
      ProfileTransformationPtr _p = extrusion->getProfileTransformation();
      if( ! extrusion->isScaleToDefault() )
          GEOM_PRINT_FIELD_ARRAY(__geomStream, _p,Scale,VECTOR2);

      if( ! extrusion->isOrientationToDefault() )
          GEOM_PRINT_FIELD_ARRAY(__geomStream, _p,Orientation,ANGLE);

      if(!  extrusion->isKnotListToDefault() )
          GEOM_PRINT_FIELD_ARRAY(__geomStream, _p,KnotList,REAL);

  }

  if(! extrusion->isSolidToDefault() )
      GEOM_PRINT_FIELD(__geomStream,extrusion,Solid,BOOLEAN);

  if(! extrusion->isCCWToDefault() )
      GEOM_PRINT_FIELD(__geomStream,extrusion,CCW,BOOLEAN);


  GEOM_PRINT_END(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  GEOM_PRINT_BEGIN(__geomStream,"FaceSet",faceSet);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,PointList,VECTOR3);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,IndexList,INDEXN);

  if (!faceSet->isNormalPerVertexToDefault())
	GEOM_PRINT_FIELD(__geomStream,faceSet,NormalPerVertex,BOOLEAN);

  if (!faceSet->isNormalListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,NormalList,VECTOR3);

  if (!faceSet->isNormalIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,NormalIndexList,INDEXN);

  if (!faceSet->isColorPerVertexToDefault())
	  GEOM_PRINT_FIELD(__geomStream,faceSet,ColorPerVertex,BOOLEAN);

  if (!faceSet->isColorListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,ColorList,COLOR4);

  if (!faceSet->isColorIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,ColorIndexList,INDEXN);

  if (! faceSet->isTexCoordListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,TexCoordList,VECTOR2);

  if (! faceSet->isTexCoordIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,faceSet,TexCoordIndexList,INDEXN);

  if (! faceSet->isCCWToDefault())
    GEOM_PRINT_FIELD(__geomStream,faceSet,CCW,BOOLEAN);

  if (! faceSet->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,faceSet,Solid,BOOLEAN);

  if (! faceSet->isSkeletonToDefault())
    GEOM_PRINT_FIELD(__geomStream,faceSet,Skeleton,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);
  GEOM_PRINT_BEGIN(__geomStream,"Frustum",frustum);

  if (! frustum->isRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,frustum,Radius,REAL);

  if (! frustum->isHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,frustum,Height,REAL);

  if (! frustum->isTaperToDefault())
    GEOM_PRINT_FIELD(__geomStream,frustum,Taper,REAL);

  if (! frustum->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,frustum,Solid,BOOLEAN);

  if (! frustum->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,frustum,Slices,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Group * group  ) {
  GEOM_ASSERT(group);
  GEOM_PRINT_BEGIN(__geomStream,"Group",group);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,group,GeometryList,GEOMETRY);

  if (! group->isSkeletonToDefault())
    GEOM_PRINT_FIELD(__geomStream,group,Skeleton,GEOMETRY);

  GEOM_PRINT_END(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( IFS * ifs  ) {
  GEOM_ASSERT(ifs);
GEOM_TRACE("process IFS");
  GEOM_PRINT_BEGIN(__geomStream,"IFS",ifs);
//todo OK

  if( ! ifs->isDepthToDefault() )
    {
    GEOM_PRINT_FIELD(__geomStream,ifs,Depth,INTEGER);
    }

  GEOM_PRINT_FIELD(__geomStream,ifs,Geometry,GEOMETRY);

  // imprimer la liste des transfos...
  GEOM_PRINT_FIELD_ARRAY(__geomStream,ifs,TransfoList,TRANSFO)

  GEOM_PRINT_END(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  GEOM_PRINT_BEGIN(__geomStream,"NurbsCurve",nurbsCurve);

  if (! nurbsCurve->isDegreeToDefault())
    GEOM_PRINT_FIELD(__geomStream,nurbsCurve,Degree,INTEGER);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,nurbsCurve,CtrlPointList,VECTOR4);

  if (! nurbsCurve->isKnotListToDefault())
    GEOM_PRINT_FIELD_ARRAY(__geomStream,nurbsCurve,KnotList,REAL);

  if (! nurbsCurve->isStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,nurbsCurve,Stride,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool Printer::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);
  GEOM_PRINT_BEGIN(__geomStream,"NurbsPatch",nurbsPatch);

  if (! nurbsPatch->isUDegreeToDefault())
  GEOM_PRINT_FIELD(__geomStream,nurbsPatch,UDegree,INTEGER);

  if (! nurbsPatch->isVDegreeToDefault())
  GEOM_PRINT_FIELD(__geomStream,nurbsPatch,VDegree,INTEGER);

  GEOM_PRINT_FIELD_MATRIX(__geomStream,nurbsPatch,CtrlPointMatrix,VECTOR4);

  if (! nurbsPatch->isUKnotListToDefault())
    GEOM_PRINT_FIELD_ARRAY(__geomStream,nurbsPatch,UKnotList,REAL);

  if (! nurbsPatch->isVKnotListToDefault())
    GEOM_PRINT_FIELD_ARRAY(__geomStream,nurbsPatch,VKnotList,REAL);

  if (! nurbsPatch->isUStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,nurbsPatch,UStride,INTEGER);

  if (! nurbsPatch->isVStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,nurbsPatch,VStride,INTEGER);

  if (! nurbsPatch->isCCWToDefault())
    GEOM_PRINT_FIELD(__geomStream, nurbsPatch,CCW,BOOLEAN);

  GEOM_PRINT_END(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool Printer::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);
  GEOM_PRINT_BEGIN(__geomStream,"Oriented",oriented);

  if (! oriented->isPrimaryToDefault())
    GEOM_PRINT_FIELD(__geomStream,oriented,Primary,VECTOR3);

  if (! oriented->isSecondaryToDefault())
    GEOM_PRINT_FIELD(__geomStream,oriented,Secondary,VECTOR3);

  GEOM_PRINT_FIELD(__geomStream,oriented,Geometry,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);
  GEOM_PRINT_BEGIN(__geomStream,"Paraboloid",paraboloid);

  if (! paraboloid->isRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,paraboloid,Radius,REAL);

  if (! paraboloid->isHeightToDefault())
    GEOM_PRINT_FIELD(__geomStream,paraboloid,Height,REAL);

  if (! paraboloid->isShapeToDefault())
    GEOM_PRINT_FIELD(__geomStream,paraboloid,Shape,REAL);

  if (! paraboloid->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,paraboloid,Solid,BOOLEAN);

  if (! paraboloid->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,paraboloid,Slices,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  GEOM_PRINT_BEGIN(__geomStream,"PointSet",pointSet);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,pointSet,PointList,VECTOR3);

  if(! pointSet->isColorListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,pointSet,ColorList,COLOR4);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  GEOM_PRINT_BEGIN(__geomStream,"Polyline",polyline);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,polyline,PointList,VECTOR3);

  if(! polyline->isColorListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,polyline,ColorList,COLOR4);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);
  GEOM_PRINT_BEGIN(__geomStream,"Revolution",revolution);

  GEOM_PRINT_FIELD(__geomStream,revolution,Profile,GEOMETRY);

  if (! revolution->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,revolution,Slices,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Swung * swung )
{
  GEOM_ASSERT(swung);
  GEOM_PRINT_BEGIN(__geomStream,"Swung",swung);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,swung,ProfileList,GEOMETRY);
  GEOM_PRINT_FIELD_ARRAY(__geomStream,swung,AngleList,ANGLE);

  if(! swung->isCCWToDefault() )
      GEOM_PRINT_FIELD(__geomStream,swung,CCW,BOOLEAN);

  if (! swung->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,swung,Slices,INTEGER);

  if (! swung->isDegreeToDefault())
    GEOM_PRINT_FIELD(__geomStream,swung,Degree,INTEGER);

  if (! swung->isStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,swung,Stride,INTEGER);

  GEOM_PRINT_END(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  GEOM_PRINT_BEGIN(__geomStream,"QuadSet",quadSet);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,PointList,VECTOR3);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,IndexList,INDEX4);

  if (!quadSet->isNormalPerVertexToDefault())
	  GEOM_PRINT_FIELD(__geomStream,quadSet,NormalPerVertex,BOOLEAN);

  if (!quadSet->isNormalListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,NormalList,VECTOR3);

  if (!quadSet->isNormalIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,NormalIndexList,INDEX4);

  if (!quadSet->isColorPerVertexToDefault())
	  GEOM_PRINT_FIELD(__geomStream,quadSet,ColorPerVertex,BOOLEAN);

  if (!quadSet->isColorListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,ColorList,COLOR4);

  if (!quadSet->isColorIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,ColorIndexList,INDEX4);

  if (! quadSet->isTexCoordListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,TexCoordList,VECTOR2);

  if (! quadSet->isTexCoordIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,quadSet,TexCoordIndexList,INDEX4);

  if (! quadSet->isCCWToDefault())
    GEOM_PRINT_FIELD(__geomStream,quadSet,CCW,BOOLEAN);

  if (! quadSet->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,quadSet,Solid,BOOLEAN);

  if (! quadSet->isSkeletonToDefault())
    GEOM_PRINT_FIELD(__geomStream,quadSet,Skeleton,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);
  GEOM_PRINT_BEGIN(__geomStream,"Scaled",scaled);

  if (! scaled->isScaleToDefault()) {
    GEOM_PRINT_FIELD(__geomStream,scaled,Scale,VECTOR3);
  };

  GEOM_PRINT_FIELD(__geomStream,scaled,Geometry,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);
  GEOM_PRINT_BEGIN(__geomStream,"Sphere",sphere);

  if (! sphere->isRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,sphere,Radius,REAL);

  if (! sphere->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,sphere,Slices,INTEGER);

  if (! sphere->isStacksToDefault())
    GEOM_PRINT_FIELD(__geomStream,sphere,Stacks,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);
  GEOM_PRINT_BEGIN(__geomStream,"Tapered",tapered);

  if (! tapered->isBaseRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,tapered,BaseRadius,REAL);

  if (! tapered->isTopRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,tapered,TopRadius,REAL);

  GEOM_PRINT_FIELD(__geomStream,tapered,Primitive,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Translated * translated ) {
  GEOM_ASSERT(translated);
  GEOM_PRINT_BEGIN(__geomStream,"Translated",translated);

  if (! translated->isTranslationToDefault())
    GEOM_PRINT_FIELD(__geomStream,translated,Translation,VECTOR3);

  GEOM_PRINT_FIELD(__geomStream,translated,Geometry,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  GEOM_PRINT_BEGIN(__geomStream,"TriangleSet",triangleSet);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,PointList,VECTOR3);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,IndexList,INDEX3);

  if (!triangleSet->isNormalPerVertexToDefault())
	GEOM_PRINT_FIELD(__geomStream,triangleSet,NormalPerVertex,BOOLEAN);

  if (!triangleSet->isNormalListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,NormalList,VECTOR3);

  if (!triangleSet->isNormalIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,NormalIndexList,INDEX3);

  if (!triangleSet->isColorPerVertexToDefault())
	GEOM_PRINT_FIELD(__geomStream,triangleSet,ColorPerVertex,BOOLEAN);

  if (!triangleSet->isColorListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,ColorList,COLOR4);

  if (!triangleSet->isColorIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,ColorIndexList,INDEX3);

  if (! triangleSet->isTexCoordListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,TexCoordList,VECTOR2);

  if (! triangleSet->isTexCoordIndexListToDefault())
	GEOM_PRINT_FIELD_ARRAY(__geomStream,triangleSet,TexCoordIndexList,INDEX3);

  if (! triangleSet->isCCWToDefault())
    GEOM_PRINT_FIELD(__geomStream,triangleSet,CCW,BOOLEAN);

  if (! triangleSet->isSolidToDefault())
    GEOM_PRINT_FIELD(__geomStream,triangleSet,Solid,BOOLEAN);

  if (! triangleSet->isSkeletonToDefault())
    GEOM_PRINT_FIELD(__geomStream,triangleSet,Skeleton,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ------------------------------------------------------------------------- */


bool Printer::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  GEOM_PRINT_BEGIN(__geomStream,"BezierCurve2D",bezierCurve);

  GEOM_PRINT_FIELD(__geomStream,bezierCurve,Degree,INTEGER);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,bezierCurve,CtrlPointList,VECTOR2);

  if (! bezierCurve->isStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,bezierCurve,Stride,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Disc * disc ) {
  GEOM_ASSERT(disc);
  GEOM_PRINT_BEGIN(__geomStream,"Disc",disc);

  if (! disc->isRadiusToDefault())
    GEOM_PRINT_FIELD(__geomStream,disc,Radius,REAL);

  if (! disc->isSlicesToDefault())
    GEOM_PRINT_FIELD(__geomStream,disc,Slices,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  GEOM_PRINT_BEGIN(__geomStream,"NurbsCurve2D",nurbsCurve);

  if (! nurbsCurve->isDegreeToDefault())
    GEOM_PRINT_FIELD(__geomStream,nurbsCurve,Degree,INTEGER);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,nurbsCurve,CtrlPointList,VECTOR2);

  if (! nurbsCurve->isKnotListToDefault())
    GEOM_PRINT_FIELD_ARRAY(__geomStream,nurbsCurve,KnotList,REAL);

  if (! nurbsCurve->isStrideToDefault())
    GEOM_PRINT_FIELD(__geomStream,nurbsCurve,Stride,INTEGER);

  GEOM_PRINT_END(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool Printer::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);
  GEOM_PRINT_BEGIN(__geomStream,"PointSet2D",pointSet);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,pointSet,PointList,VECTOR2);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);
  GEOM_PRINT_BEGIN(__geomStream,"Polyline2D",polyline);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,polyline,PointList,VECTOR2);

  GEOM_PRINT_END(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Printer::process( Text * text ) {
  GEOM_ASSERT(text);
  GEOM_PRINT_BEGIN(__geomStream,"Text",text);

  GEOM_PRINT_FIELD(__geomStream,text,String,STRING);

  if (! text->isPositionToDefault())
	GEOM_PRINT_FIELD(__geomStream,text,Position,VECTOR3);

  if (! text->isScreenCoordinatesToDefault())
	GEOM_PRINT_FIELD(__geomStream,text,ScreenCoordinates,BOOLEAN);

  if (! text->isFontStyleToDefault())
	GEOM_PRINT_FIELD(__geomStream,text,FontStyle,GEOMETRY);

  GEOM_PRINT_END(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool Printer::process( Font * font ) {
  GEOM_ASSERT(text);
  GEOM_PRINT_BEGIN(__geomStream,"Font",font);

  GEOM_PRINT_FIELD(__geomStream,font,Family,STRING);

  if (! font->isSizeToDefault())
	GEOM_PRINT_FIELD(__geomStream,font,Size,INTEGER);

  if (! font->isBoldToDefault())
	GEOM_PRINT_FIELD(__geomStream,font,Bold,BOOLEAN);

  if (! font->isItalicToDefault())
	GEOM_PRINT_FIELD(__geomStream,font,Italic,BOOLEAN);

  GEOM_PRINT_END(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */

