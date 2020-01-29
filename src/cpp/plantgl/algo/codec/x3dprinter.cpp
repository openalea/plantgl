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




#include "x3dprinter.h"

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

#define GEOM_VRMLPRINT_INCREMENT_INDENT \
  __indent += "    ";


#define GEOM_VRMLPRINT_DECREMENT_INDENT \
  __indent.erase(__indent.end() - 4,__indent.end());

#define GEOM_VRMLPRINT_COLOR3(val) \
  __geomStream << val.getRedClamped() << ' ' << val.getGreenClamped() << ' ' << val.getBlueClamped(); \

#define GEOM_VRMLPRINT_VECTOR2(val) \
  __geomStream << val.x() << ' ' << val.y(); \

#define GEOM_VRMLPRINT_VECTOR3(val) \
  __geomStream << val.x() << ' ' << val.y() << ' ' << val.z(); \

#define GEOM_VRMLPRINT_VECTOR4(val) \
  __geomStream << val.x() << ' ' << val.y() << ' ' << val.z() << ' ' << val.w(); \

#define GEOM_VRMLPRINT_FIELD(name,val,type) { \
    __geomStream << name << "='"; \
    GEOM_VRMLPRINT_##type(val); \
    __geomStream << "' "; \
  };

#define GEOM_VRMLPRINT_FIELD_ARRAY(name,val,type) { \
    __geomStream << name << "='"; \
    GEOM_VRMLPRINT_ARRAY(val,type); \
    __geomStream << "' "; \
  };

#define GEOM_VRMLPRINT_REAL(val) \
  __geomStream << val;

#define GEOM_VRMLPRINT_INT(val) \
  __geomStream << val;

#define GEOM_VRMLPRINT_BOOLEAN(val) \
  __geomStream << (val ? "true" : "false");


#define GEOM_VRMLPRINT_ARRAY(val,type){ \
   for(uint_t _i = 0;_i < val->size();_i++){ \
    GEOM_VRMLPRINT_##type(val->getAt(_i)); \
    if(_i != (val->size()-1)){ \
      __geomStream << ", "; \
      if(_i != 0 && _i % 3 == 0 )__geomStream << endl << __indent; \
    } \
   } \
 };

#define GEOM_VRMLPRINT_INDEX3(val) \
  __geomStream << val.getAt(0) << ", " << val.getAt(1) << ", " << val.getAt(2) << ", -1"; \

#define GEOM_VRMLPRINT_INDEX4(val) \
  __geomStream << val.getAt(0) << ", " << val.getAt(1) << ", " << val.getAt(2) << ", " << val.getAt(3) << ", -1"; \

#define GEOM_VRMLPRINT_INDEX(val) \
   for(Index::const_iterator _j = val.begin() ; _j != val.end() ; _j++) \
      __geomStream << (*_j) << ", "; \
   __geomStream << ", -1"; \

#define GEOM_VRMLPRINT_BEGINOBJ(type,obj) \
  GEOM_ASSERT( obj ); \
  if (obj->isNamed()) { \
    if (! __cache.insert(obj->getObjectId()).second) { \
      __geomStream  <<  __indent << "<" << type << " USE name=\"" << obj->getName().c_str() << "\" > " << endl; \
      return true; \
    } \
    else { \
      __geomStream  <<  __indent << "<" << type  << " DEF name=\"" << obj->getName().c_str() << "\" "; \
      GEOM_VRMLPRINT_INCREMENT_INDENT; \
    } \
  } \

#define GEOM_VRMLPRINT_INDFACESET(obj,index) \
  GEOM_ASSERT( obj ); \
  if (obj->isNamed()) { \
    if (! __cache.insert(obj->getObjectId()).second) { \
      __geomStream  <<  __indent << "<IndexedFaceSet USE name=\"" << obj->getName().c_str() << "\" > " << endl; \
      return true; \
    } \
    else { \
      __geomStream  <<  __indent << "<IndexedFaceSet DEF name=\"" << obj->getName().c_str() << "\" "; \
      GEOM_VRMLPRINT_INCREMENT_INDENT; \
    } \
  } \
  \
  GEOM_VRMLPRINT_FIELD_ARRAY("coordIndex",obj->getIndexList(),index); \
  __geomStream << endl << __indent; \
  GEOM_VRMLPRINT_FIELD("normalPerVertex",true,BOOLEAN); \
  __geomStream << endl << __indent; \
  GEOM_VRMLPRINT_FIELD("solid",obj->getSolid(),BOOLEAN); \
  __geomStream << endl << __indent; \
  GEOM_VRMLPRINT_FIELD("ccw",obj->getCCW(),BOOLEAN); \
  \
  __geomStream << " >" << endl; \
  __geomStream << __indent << "<Coordinate point='"; \
  GEOM_VRMLPRINT_INCREMENT_INDENT; \
  GEOM_VRMLPRINT_ARRAY(obj->getPointList(),VECTOR3); \
  GEOM_VRMLPRINT_DECREMENT_INDENT; \
  __geomStream << "' />"  << endl; \
  if(obj->getNormalList()) { \
  __geomStream << __indent << "<Normal vector='"; \
  GEOM_VRMLPRINT_INCREMENT_INDENT; \
  GEOM_VRMLPRINT_ARRAY(obj->getNormalList(),VECTOR3); \
  GEOM_VRMLPRINT_DECREMENT_INDENT; \
  __geomStream << "' />"  << endl; } \
  GEOM_VRMLPRINT_DECREMENT_INDENT; \
  __geomStream << "</IndexedFaceSet>"  << endl; \
  return true; \

/* ----------------------------------------------------------------------- */


X3DPrinter::X3DPrinter(ostream& stream, Discretizer& discretizer) :
  VrmlPrinter(stream,discretizer)
{
}

X3DPrinter::~X3DPrinter()
{
}


bool X3DPrinter::header(const char * comment){
  __geomStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
  __geomStream << "<!-- File create with GEOM.-->" << endl;
  if(comment)__geomStream << "<!--" << comment << "-->" << endl;
  __geomStream << endl;
  return true;
}
/* ----------------------------------------------------------------------- */


bool
X3DPrinter::beginProcess()
{
  return true;
}

bool
X3DPrinter::endProcess()
{
  return true;
}


/* ----------------------------------------------------------------------- */



bool
X3DPrinter::process( Inline * geominline )
{
  GEOM_ASSERT( geominline );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Shape * Shape )
{
  GEOM_ASSERT( Shape );
  __app = Shape->appearance;
  return (Shape->geometry->apply(*this));
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Material * material )
{
  GEOM_ASSERT( material );

  if (material->isNamed()) {
    if (! __cache.insert(material->getObjectId()).second) {
      __geomStream  <<  __indent << "<Appearance USE " << material->getName().c_str() << " > " << endl;
      return true;
    }
    else {
      __geomStream  <<  __indent << "<Appearance DEF " << material->getName().c_str() << " >  " << endl;
      GEOM_VRMLPRINT_INCREMENT_INDENT;
      __geomStream  <<  __indent << "<material>" << endl;
      GEOM_VRMLPRINT_INCREMENT_INDENT;
    }
  }
  __geomStream  <<  __indent << "<Material " << endl;
  GEOM_VRMLPRINT_INCREMENT_INDENT;

  Color3 a(uchar_t(material->getDiffuse()*material->getAmbient().getRed()),
           uchar_t(material->getDiffuse()*material->getAmbient().getGreen()),
           uchar_t(material->getDiffuse()*material->getAmbient().getBlue()));
  __geomStream  <<__indent;
  GEOM_VRMLPRINT_FIELD("diffuseColor",a,COLOR3);
  __geomStream  << endl << __indent;
  real_t b = 0.0;
  if(fabs(material->getDiffuse())>GEOM_EPSILON) b = (real_t)1.0 / (real_t)material->getDiffuse();
  GEOM_VRMLPRINT_FIELD("ambientIntensity",b,REAL);
  __geomStream  << endl << __indent;
  GEOM_VRMLPRINT_FIELD("specularColor",material->getSpecular(),COLOR3);
  __geomStream  << endl << __indent;
  GEOM_VRMLPRINT_FIELD("emissiveColor",material->getEmission(),COLOR3);
  __geomStream  << endl << __indent;
  GEOM_VRMLPRINT_FIELD("shininess",material->getShininess(),REAL);
  __geomStream  << endl << __indent;
  GEOM_VRMLPRINT_FIELD("transparency",material->getTransparency(),REAL);
  __geomStream  << "/>" << endl;
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  if (material->isNamed()) {
    GEOM_VRMLPRINT_DECREMENT_INDENT;
    __geomStream << __indent << "</material>" << endl;
    GEOM_VRMLPRINT_DECREMENT_INDENT;
    __geomStream << __indent << "</Appearance>  " << endl;
  }
  return true;
}


/* ----------------------------------------------------------------------- */

bool
X3DPrinter::process( Texture2D * texture )
{
  GEOM_ASSERT( texture );
  cerr << "Not Yet Implemented" << endl;
  return false;
}

/* ----------------------------------------------------------------------- */

bool
X3DPrinter::process( ImageTexture * texture )
{
  GEOM_ASSERT( texture );
  cerr << "Not Yet Implemented" << endl;
  return false;
}

/* ----------------------------------------------------------------------- */

bool
X3DPrinter::process( Texture2DTransformation * texturetransfo )
{
  GEOM_ASSERT( texturetransfo );
  cerr << "Not Yet Implemented" << endl;
  return false;
}

/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( MonoSpectral * monoSpectral )
{
  GEOM_ASSERT( monoSpectral );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( MultiSpectral * multiSpectral )
{
  GEOM_ASSERT( multiSpectral );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( AmapSymbol * amapSymbol )
{
  GEOM_VRMLPRINT_INDFACESET( amapSymbol, INDEX );
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( AsymmetricHull * asymmetricHull )
{
  GEOM_ASSERT( asymmetricHull );
  GEOM_VRMLPRINT_BEGINOBJ("AsymmetricHull",asymmetricHull);
  GEOM_VRMLPRINT_FIELD("PosXRadius",asymmetricHull->getPosXRadius(),REAL);
  GEOM_VRMLPRINT_FIELD("PosYRadius",asymmetricHull->getPosYRadius(),REAL);
  GEOM_VRMLPRINT_FIELD("NegXRadius",asymmetricHull->getNegXRadius(),REAL);
  GEOM_VRMLPRINT_FIELD("NegYRadius",asymmetricHull->getNegYRadius(),REAL);

  GEOM_VRMLPRINT_FIELD("PosXHeight",asymmetricHull->getPosXHeight(),REAL);
  GEOM_VRMLPRINT_FIELD("PosYHeight",asymmetricHull->getPosYHeight(),REAL);
  GEOM_VRMLPRINT_FIELD("NegXHeight",asymmetricHull->getNegXHeight(),REAL);
  GEOM_VRMLPRINT_FIELD("NegYHeight",asymmetricHull->getNegYHeight(),REAL);

  GEOM_VRMLPRINT_FIELD("Bottom",asymmetricHull->getBottom(),VECTOR3);
  GEOM_VRMLPRINT_FIELD("Top",asymmetricHull->getTop(),VECTOR3);
  GEOM_VRMLPRINT_FIELD("BottomShape",asymmetricHull->getBottomShape(),REAL);
  GEOM_VRMLPRINT_FIELD("TopShape",asymmetricHull->getTopShape(),REAL);

  GEOM_VRMLPRINT_FIELD("Slices",asymmetricHull->getSlices(),INT);
  GEOM_VRMLPRINT_FIELD("Stacks",asymmetricHull->getStacks(),INT);

  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << " />" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( AxisRotated * axisRotated )
{
  GEOM_ASSERT( axisRotated );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( BezierCurve * bezierCurve )
{
  GEOM_ASSERT( bezierCurve );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( BezierPatch * bezierPatch )
{
  GEOM_ASSERT( bezierPatch );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Box * box )
{
  GEOM_ASSERT( box );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Cone * cone )
{
  GEOM_ASSERT( cone );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Cylinder * cylinder )
{
  GEOM_ASSERT( cylinder );
  GEOM_VRMLPRINT_BEGINOBJ("Cylinder",cylinder);
  GEOM_VRMLPRINT_FIELD("radius",cylinder->getRadius(),REAL);
  GEOM_VRMLPRINT_FIELD("height",cylinder->getHeight(),REAL);
  GEOM_VRMLPRINT_FIELD("side",true,BOOLEAN);
  GEOM_VRMLPRINT_FIELD("bottom",cylinder->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_FIELD("top",cylinder->getSolid(),BOOLEAN);
  GEOM_VRMLPRINT_DECREMENT_INDENT;
  __geomStream << " />" << endl;
  return true;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( ElevationGrid * elevationGrid )
{
  GEOM_ASSERT( elevationGrid );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( EulerRotated * eulerRotated )
{
  GEOM_ASSERT( eulerRotated );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( ExtrudedHull * extrudedHull )
{
  GEOM_ASSERT( extrudedHull );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Extrusion * extrusion )
{
  GEOM_ASSERT( extrusion );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( FaceSet * faceSet )
{
  GEOM_VRMLPRINT_INDFACESET( faceSet, INDEX );
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Frustum * frustum )
{
  GEOM_ASSERT( frustum );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Group * group )
{
  GEOM_ASSERT( group );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( NurbsCurve * nurbsCurve )
{
  GEOM_ASSERT( nurbsCurve );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( NurbsPatch * nurbsPatch )
{
  GEOM_ASSERT( nurbsPatch );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Oriented * oriented )
{
  GEOM_ASSERT( oriented );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Paraboloid * paraboloid )
{
  GEOM_ASSERT( paraboloid );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( PointSet * pointSet )
{
  GEOM_ASSERT( pointSet );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Polyline * polyline )
{
  GEOM_ASSERT( polyline );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Revolution * revolution )
{
  GEOM_ASSERT( revolution );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( QuadSet * quadSet )
{
  GEOM_VRMLPRINT_INDFACESET( quadSet, INDEX4 );
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Scaled * scaled )
{
  GEOM_ASSERT( scaled );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Sphere * sphere )
{
  GEOM_ASSERT( sphere );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Tapered * tapered )
{
  GEOM_ASSERT( tapered );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Translated * translated )
{
  GEOM_ASSERT( translated );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( TriangleSet * triangleSet )
{
  GEOM_VRMLPRINT_INDFACESET( triangleSet, INDEX3 );
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( BezierCurve2D * bezierCurve )
{
  GEOM_ASSERT( bezierCurve );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Disc * disc )
{
  GEOM_ASSERT( disc );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( NurbsCurve2D * nurbsCurve )
{
  GEOM_ASSERT( nurbsCurve );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( PointSet2D * pointSet )
{
  GEOM_ASSERT( pointSet );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Polyline2D * polyline )
{
  GEOM_ASSERT( polyline );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */


bool
X3DPrinter::process( Text * text )
{
  GEOM_ASSERT( text );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */

bool
X3DPrinter::process( Font * font )
{
  GEOM_ASSERT( font );
  cerr << "Not Yet Implemented" << endl;
  return false;
}


/* ----------------------------------------------------------------------- */

