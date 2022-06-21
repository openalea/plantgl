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




#include "xmlprinter.h"
//#ifndef PGL_CORE_WITHOUT_QT
//#include <QtCore/qtextstream.h>
//#endif

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/pointarray.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

#define GEOM_XMLPRINT_INCREMENT_INDENT \
  __indent += "    ";

#ifndef PGL_CORE_WITHOUT_QT

#define GEOM_XMLPRINT_DECREMENT_INDENT \
  __indent.truncate(__indent.length() - 4);

#else

#define GEOM_XMLPRINT_DECREMENT_INDENT \
  __indent.erase(__indent.length() - 4,4);

#endif

#define GEOM_XMLPRINT_COLOR3(val) \
  __stream << val.getRed() << ' ' << val.getGreen() << ' ' << val.getBlue(); \

#define GEOM_XMLPRINT_STRING(val) \
  __stream << val.c_str(); \

#define GEOM_XMLPRINT_GEOMETRY(val,finish) \
  __stream << Qt::endl << __indent; val->apply(*this); \


#define GEOM_XMLPRINT_INT(val) \
  __stream << val; \

#define GEOM_XMLPRINT_REAL(val) \
  __stream << val; \

#define GEOM_XMLPRINT_BOOLEAN(val) \
        __stream << (val?"True":"False"); \

#define GEOM_XMLPRINT_VECTOR2(val) \
  __stream << val.x() << ' ' << val.y(); \

#define GEOM_XMLPRINT_VECTOR3(val) \
  __stream << val.x() << ' ' << val.y() << ' ' << val.z(); \

#define GEOM_XMLPRINT_VECTOR4(val) \
  __stream << val.x() << ' ' << val.y() << ' ' << val.z() << ' ' << val.w(); \

#define GEOM_XMLPRINT_FIELD(name,val,type) { \
    __stream << Qt::endl << __indent << name << "=\""; \
    GEOM_XMLPRINT_##type(val); \
    __stream << "\" "; \
  };

#define GEOM_XMLPRINT_FIELD_GEOMETRY(name,val,type,finish) { \
        if(!finish)GEOM_XMLPRINT_FINISHOBJ(finish); \
    __stream << Qt::endl << __indent << "<" << name << " >"; \
        GEOM_XMLPRINT_INCREMENT_INDENT; \
    GEOM_XMLPRINT_GEOMETRY(val); \
        GEOM_XMLPRINT_DECREMENT_INDENT; \
    __stream << __indent << "</" << name << " >"; \
  };

#define GEOM_XMLPRINT_FIELD_ARRAY(name,val,type) { \
    __stream << Qt::endl << __indent << name << "=\""; \
    GEOM_XMLPRINT_ARRAY(val,type); \
    __stream << "\" "; \
  };

#define GEOM_XMLPRINT_REAL(val) \
  __stream << val;

#define GEOM_XMLPRINT_INT(val) \
  __stream << val;

#define GEOM_XMLPRINT_ARRAY(val,type){ \
   for(uint_t _i = 0;_i < val->size();_i++){ \
    GEOM_XMLPRINT_##type(val->getAt(_i)); \
    if(_i != (val->size()-1)){ \
      __stream << " "; \
    } \
   } \
 };

//      if(_i != 0 && _i % 3 == 0 )__stream << Qt::endl << __indent;


#define GEOM_XMLPRINT_INDEX3(val) \
  __stream << val.getAt(0) << " " << val.getAt(1) << " " << val.getAt(2); \

#define GEOM_XMLPRINT_INDEX4(val) \
  __stream << val.getAt(0) << " " << val.getAt(1) << " " << val.getAt(2) << " " << val.getAt(3); \

#define GEOM_XMLPRINT_INDEX(val) \
   for(Index::const_iterator _j = val.begin() ; _j != val.end() ; _j++) \
      __stream << (*_j) << ", "; \
   __stream << ", -1"; \

#define GEOM_XMLPRINT_BEGINOBJ(type,obj,finish) \
  GEOM_ASSERT( obj ); \
  if (obj->isNamed()) { \
    if (! __cache.insert(obj->getObjectId()).second) { \
      __stream  <<  __indent << "<" << type << " Name=\"" << obj->getName().c_str() << "\" > " << Qt::endl; \
      return true; \
    } \
    else { \
      __stream  <<  __indent << "<" << type  << " Name=\"" << obj->getName().c_str() << "\" "; \
      GEOM_XMLPRINT_INCREMENT_INDENT; \
    } \
  } \
  bool finish = false; \

#define GEOM_XMLPRINT_FINISHOBJ(finish) \
        __stream << " />" << Qt::endl; \
        finish = true; \


#define GEOM_XMLPRINT_ENDOBJ(type,finish) \
        GEOM_XMLPRINT_DECREMENT_INDENT; \
        if(!finish){ \
                GEOM_XMLPRINT_FINISHOBJ(finish); \
        } \
        else { __stream  << Qt::endl << __indent << "</" << type << ">" << Qt::endl ;} \

/* ----------------------------------------------------------------------- */


XMLPrinter::XMLPrinter(QTextStream& stream) :
  Action(),__stream(stream)
{
}

XMLPrinter::~XMLPrinter()
{
}


bool XMLPrinter::header(const char * comment){
  __stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << Qt::endl;
  __stream << "<!-- File create with GEOM.-->" << Qt::endl;
  if(comment)__stream << "<!--" << comment << "-->" << Qt::endl;
  __stream << Qt::endl;
  return true;
}

void XMLPrinter::setIndent(const QString& s)
{ __indent = s;}

const QString& XMLPrinter::getIndent() const
{ return __indent; }

void XMLPrinter::clear()
{ __cache.clear(); }


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::beginProcess()
{
  return true;
}

bool
XMLPrinter::endProcess()
{
  return true;
}


/* ----------------------------------------------------------------------- */



bool
XMLPrinter::process( Inline * geominline )
{
  GEOM_ASSERT( geominline );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Shape * Shape )
{
  GEOM_ASSERT( Shape );
  Shape->appearance->apply(*this);
  return (Shape->geometry->apply(*this));
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Material * material )
{
  GEOM_ASSERT( material );
  return false;
/*
  if (material->isNamed()) {
    if (! __cache.insert(material->getObjectId()).second) {
      __stream  <<  __indent << "<Appearance USE " << material->getName().c_str() << " > " << Qt::endl;
      return true;
    }
    else {
      __stream  <<  __indent << "<Appearance DEF " << material->getName().c_str() << " >  " << Qt::endl;
      GEOM_XMLPRINT_INCREMENT_INDENT;
      __stream  <<  __indent << "<material>" << Qt::endl;
      GEOM_XMLPRINT_INCREMENT_INDENT;
    }
  }
  __stream  <<  __indent << "<Material " << Qt::endl;
  GEOM_XMLPRINT_INCREMENT_INDENT;
  GEOM_XMLPRINT_BEG

  Color3 a(uchar(material->getDiffuse()*material->getAmbient().getRed()),
           uchar(material->getDiffuse()*material->getAmbient().getGreen()),
           uchar(material->getDiffuse()*material->getAmbient().getBlue()));
  __stream  <<__indent;
  GEOM_XMLPRINT_FIELD("diffuseColor",a,COLOR3);
  __stream  << Qt::endl << __indent;
  real_t b = 0.0;
  if(fabs(material->getDiffuse())>GEOM_EPSILON) b = (real_t)1.0 / (real_t)material->getDiffuse();
  GEOM_XMLPRINT_FIELD("ambientIntensity",b,REAL);
  __stream  << Qt::endl << __indent;
  GEOM_XMLPRINT_FIELD("specularColor",material->getSpecular(),COLOR3);
  __stream  << Qt::endl << __indent;
  GEOM_XMLPRINT_FIELD("emissiveColor",material->getEmission(),COLOR3);
  __stream  << Qt::endl << __indent;
  GEOM_XMLPRINT_FIELD("shininess",material->getShininess(),REAL);
  __stream  << Qt::endl << __indent;
  GEOM_XMLPRINT_FIELD("transparency",material->getTransparency(),REAL);
  __stream  << "/>" << Qt::endl;
  GEOM_XMLPRINT_DECREMENT_INDENT;
  if (material->isNamed()) {
    GEOM_XMLPRINT_DECREMENT_INDENT;
    __stream << __indent << "</material>" << Qt::endl;
    GEOM_XMLPRINT_DECREMENT_INDENT;
    __stream << __indent << "</Appearance>  " << Qt::endl;
  }
  return true;*/
}


/* ----------------------------------------------------------------------- */

bool
XMLPrinter::process( Texture2D * texture)
{
  GEOM_ASSERT( texture );
  return false;
}

/* ----------------------------------------------------------------------- */

bool
XMLPrinter::process( ImageTexture * texture )
{
  GEOM_ASSERT( texture );
  return false;
}

/* ----------------------------------------------------------------------- */

bool
XMLPrinter::process( Texture2DTransformation * texture )
{
  GEOM_ASSERT( texture );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( MonoSpectral * monoSpectral )
{
  GEOM_ASSERT( monoSpectral );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( MultiSpectral * multiSpectral )
{
  GEOM_ASSERT( multiSpectral );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( AmapSymbol * amapSymbol )
{
//  GEOM_XMLPRINT_INDFACESET( amapSymbol, INDEX );
        return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( AsymmetricHull * asymmetricHull )
{
  GEOM_ASSERT( asymmetricHull );
  GEOM_XMLPRINT_BEGINOBJ("AsymmetricHull",asymmetricHull,finish);
  GEOM_XMLPRINT_FIELD("PosXRadius",asymmetricHull->getPosXRadius(),REAL);
  GEOM_XMLPRINT_FIELD("PosYRadius",asymmetricHull->getPosYRadius(),REAL);
  GEOM_XMLPRINT_FIELD("NegXRadius",asymmetricHull->getNegXRadius(),REAL);
  GEOM_XMLPRINT_FIELD("NegYRadius",asymmetricHull->getNegYRadius(),REAL);
  GEOM_XMLPRINT_FIELD("PosXHeight",asymmetricHull->getPosXHeight(),REAL);
  GEOM_XMLPRINT_FIELD("PosYHeight",asymmetricHull->getPosYHeight(),REAL);
  GEOM_XMLPRINT_FIELD("NegXHeight",asymmetricHull->getNegXHeight(),REAL);
  GEOM_XMLPRINT_FIELD("NegYHeight",asymmetricHull->getNegYHeight(),REAL);
  GEOM_XMLPRINT_FIELD("Bottom",asymmetricHull->getBottom(),VECTOR3);
  GEOM_XMLPRINT_FIELD("Top",asymmetricHull->getTop(),VECTOR3);
  GEOM_XMLPRINT_FIELD("BottomShape",asymmetricHull->getBottomShape(),REAL);
  GEOM_XMLPRINT_FIELD("TopShape",asymmetricHull->getTopShape(),REAL);
  GEOM_XMLPRINT_FIELD("Slices",asymmetricHull->getSlices(),INT);
  GEOM_XMLPRINT_FIELD("Stacks",asymmetricHull->getStacks(),INT);

  GEOM_XMLPRINT_ENDOBJ("AsymmetricHull",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( AxisRotated * axisRotated )
{
  GEOM_ASSERT( axisRotated );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( BezierCurve * bezierCurve )
{
  GEOM_ASSERT( bezierCurve );
  GEOM_XMLPRINT_BEGINOBJ("BezierCurve",bezierCurve,finish);
  GEOM_XMLPRINT_FIELD("Stride",bezierCurve->getStride(),INT);
  GEOM_XMLPRINT_FIELD_ARRAY("CtrlPointList",bezierCurve->getCtrlPointList(),VECTOR4);
  GEOM_XMLPRINT_ENDOBJ("BezierCurve",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( BezierPatch * bezierPatch )
{
  GEOM_ASSERT( bezierPatch );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Box * box )
{
  GEOM_ASSERT( box );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Cone * cone )
{
  GEOM_ASSERT( cone );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Cylinder * cylinder )
{
  GEOM_ASSERT( cylinder );
  GEOM_XMLPRINT_BEGINOBJ("Cylinder",cylinder,finish);
  GEOM_XMLPRINT_FIELD("Radius",cylinder->getRadius(),REAL);
  GEOM_XMLPRINT_FIELD("Height",cylinder->getHeight(),REAL);
  GEOM_XMLPRINT_FIELD("Solid",cylinder->getSolid(),BOOLEAN);
  GEOM_XMLPRINT_ENDOBJ("Cylinder",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( ElevationGrid * elevationGrid )
{
  GEOM_ASSERT( elevationGrid );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( EulerRotated * eulerRotated )
{
  GEOM_ASSERT( eulerRotated );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( ExtrudedHull * extrudedHull )
{
  GEOM_ASSERT( extrudedHull );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Extrusion * extrusion )
{
  GEOM_ASSERT( extrusion );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( FaceSet * faceSet )
{
//  GEOM_XMLPRINT_INDFACESET( faceSet, INDEX );
        return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Frustum * frustum )
{
  GEOM_ASSERT( frustum );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Group * group )
{
  GEOM_ASSERT( group );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( IFS * ifs)
{
  GEOM_ASSERT( ifs );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( NurbsCurve * nurbsCurve )
{
  GEOM_ASSERT( nurbsCurve );
  GEOM_XMLPRINT_BEGINOBJ("NurbsCurve",nurbsCurve,finish);
  GEOM_XMLPRINT_FIELD("Stride",nurbsCurve->getStride(),INT);
  GEOM_XMLPRINT_FIELD("Degree",nurbsCurve->getDegree(),INT);
  GEOM_XMLPRINT_FIELD_ARRAY("CtrlPointList",nurbsCurve->getCtrlPointList(),VECTOR4);
  if(!nurbsCurve->isKnotListToDefault())
  GEOM_XMLPRINT_FIELD_ARRAY("KnotList",nurbsCurve->getKnotList(),REAL);

  GEOM_XMLPRINT_ENDOBJ("NurbsCurve",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( NurbsPatch * nurbsPatch )
{
  GEOM_ASSERT( nurbsPatch );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Oriented * oriented )
{
  GEOM_ASSERT( oriented );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Paraboloid * paraboloid )
{
  GEOM_ASSERT( paraboloid );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( PointSet * pointSet )
{
  GEOM_ASSERT( pointSet );
  GEOM_XMLPRINT_BEGINOBJ("PointSet",pointSet,finish);
  GEOM_XMLPRINT_FIELD_ARRAY("PointList",pointSet->getPointList(),VECTOR3);

  GEOM_XMLPRINT_ENDOBJ("Polyline",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Polyline * polyline )
{
  GEOM_ASSERT( polyline );
  GEOM_XMLPRINT_BEGINOBJ("Polyline",polyline,finish);
  GEOM_XMLPRINT_FIELD_ARRAY("PointList",polyline->getPointList(),VECTOR3);

  GEOM_XMLPRINT_ENDOBJ("Polyline",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Revolution * revolution )
{
  GEOM_ASSERT( revolution );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( QuadSet * quadSet )
{
//  GEOM_XMLPRINT_INDFACESET( quadSet, INDEX4 );
        return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Scaled * scaled )
{
  GEOM_ASSERT( scaled );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Sphere * sphere )
{
  GEOM_ASSERT( sphere );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Swung * swung)
{
  GEOM_ASSERT( swung );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Tapered * tapered )
{
  GEOM_ASSERT( tapered );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Translated * translated )
{
  GEOM_ASSERT( translated );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( TriangleSet * triangleSet )
{
//  GEOM_PRINT_INDFACESET( triangleSet, INDEX3 );
        return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( BezierCurve2D * bezierCurve )
{
  GEOM_ASSERT( bezierCurve );
  GEOM_XMLPRINT_BEGINOBJ("BezierCurve2D",bezierCurve,finish);
  GEOM_XMLPRINT_FIELD("Stride",bezierCurve->getStride(),INT);
  GEOM_XMLPRINT_FIELD_ARRAY("CtrlPointList",bezierCurve->getCtrlPointList(),VECTOR3);

  GEOM_XMLPRINT_ENDOBJ("BezierCurve2D",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Disc * disc )
{
  GEOM_ASSERT( disc );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( NurbsCurve2D * nurbsCurve )
{
  GEOM_ASSERT( nurbsCurve );
  GEOM_XMLPRINT_BEGINOBJ("NurbsCurve2D",nurbsCurve,finish);
  GEOM_XMLPRINT_FIELD("Stride",nurbsCurve->getStride(),INT);
  GEOM_XMLPRINT_FIELD("Degree",nurbsCurve->getDegree(),INT);
  GEOM_XMLPRINT_FIELD_ARRAY("CtrlPointList",nurbsCurve->getCtrlPointList(),VECTOR3);
  GEOM_XMLPRINT_FIELD_ARRAY("KnotList",nurbsCurve->getKnotList(),REAL);

  GEOM_XMLPRINT_ENDOBJ("NurbsCurve2D",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( PointSet2D * pointSet )
{
  GEOM_ASSERT( pointSet );
  GEOM_XMLPRINT_BEGINOBJ("PointSet2D",pointSet,finish);
  GEOM_XMLPRINT_FIELD_ARRAY("PointList",pointSet->getPointList(),VECTOR2);

  GEOM_XMLPRINT_ENDOBJ("Polyline2D",finish);
  return true;
}


/* ----------------------------------------------------------------------- */


bool
XMLPrinter::process( Polyline2D * polyline )
{
  GEOM_ASSERT( polyline );
  GEOM_XMLPRINT_BEGINOBJ("Polyline2D",polyline,finish);
  GEOM_XMLPRINT_FIELD_ARRAY("PointList",polyline->getPointList(),VECTOR2);

  GEOM_XMLPRINT_ENDOBJ("Polyline2D",finish);
  return true;
}


/* ----------------------------------------------------------------------- */

bool
XMLPrinter::process( Text * text )
{
  GEOM_ASSERT( text );
  return false;
}

/* ----------------------------------------------------------------------- */

bool
XMLPrinter::process( Font * font )
{
  GEOM_ASSERT( font );
  return false;
}

/* ----------------------------------------------------------------------- */
