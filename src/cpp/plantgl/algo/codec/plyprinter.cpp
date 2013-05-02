/* -*-c++-*-
 * ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
 *
 * ----------------------------------------------------------------------------
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
 * ----------------------------------------------------------------------------
 */


#include <plantgl/tool/bfstream.h>
#include <plantgl/tool/util_string.h>

#include "plyprinter.h"
#include <plantgl/algo/base/discretizer.h>

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

#define GEOM_PLY_MESH(obj,gindex,len) \
  if( __pass == 1 ){ \
    __vertex += obj->getPointList()->size(); \
    __face += obj->getIndexList()->size(); \
  } \
  else if( __pass == 2 ){ \
    for(Point3Array::const_iterator _it = obj->getPointList()->begin(); \
        _it != obj->getPointList()->end(); _it++){ \
         stream << _it->x() << ' ' <<_it->y()  << ' ' << _it->z()  << ' ' << __red   << ' ' << __green   << ' ' << __blue << endl; \
    } \
  } \
  else if( __pass == 3 ){ \
    for(gindex##Array::const_iterator _it = obj->getIndexList()->begin(); \
        _it != obj->getIndexList()->end(); _it++){ \
       stream << len; \
       for(gindex::const_iterator _it2 = _it->begin(); \
          _it2 != _it->end(); _it2++) \
          stream << ' ' << ((*_it2)+__index); \
       stream << endl; \
    } \
    __index += obj->getPointList()->size(); \
  } \
  return true; \


#define GEOM_PLYB_MESH(obj,gindex,len) \
  if( __pass == 1 ){ \
    __vertex += obj->getPointList()->size(); \
    __face += obj->getIndexList()->size(); \
  } \
  else if( __pass == 2 ){ \
    for(Point3Array::const_iterator _it = obj->getPointList()->begin(); \
        _it != obj->getPointList()->end(); _it++){ \
      stream << (float)_it->x() << (float)_it->y()  << (float)_it->z() << (uchar_t)__red  << (uchar_t)__green  << (uchar_t)__blue; \
    } \
  } \
  else if( __pass == 3 ){ \
    for(gindex##Array::const_iterator _it = obj->getIndexList()->begin(); \
        _it != obj->getIndexList()->end(); _it++){ \
       stream << (uchar_t)len; \
       for(gindex::const_iterator _it2 = _it->begin(); \
          _it2 != _it->end(); _it2++) \
         stream << (int)((*_it2)+__index); \
    } \
    __index += obj->getPointList()->size(); \
  } \
  return true; \

/* ----------------------------------------------------------------------- */


PlyPrinter::PlyPrinter( ostream& stream , Discretizer& discretizer) :
  Printer(stream,stream,stream),
  __discretizer(discretizer),
  __pass(0),
  __vertex(0),
  __face(0),
  __red(160),
  __green(160),
  __blue(160),
  __index(0)
{
}

PlyPrinter::~PlyPrinter()
{
}

#define stream __geomStream
/* ----------------------------------------------------------------------- */


bool
PlyPrinter::beginProcess()
{
  return true;
}

bool
PlyPrinter::endProcess()
{
  return true;
}


/* ----------------------------------------------------------------------- */

bool
PlyPrinter::header( const char * comment ){
  stream << "ply" << endl;
  stream << "format ascii 1.0" << endl;
  stream << "comment author ";
#ifdef __GNUC__
  stream << getenv("LOGNAME");
#endif
  stream << endl;
  if(comment)stream << "comment " << comment << endl;
  stream << "element vertex " << __vertex << endl; /// number of vertex
  stream << "property float x" << endl; /// vertex coordinates
  stream << "property float y" << endl;
  stream << "property float z" << endl;
  stream << "property uchar_t diffuse_red" << endl; /// vertex color
  stream << "property uchar_t diffuse_green" << endl;
  stream << "property uchar_t diffuse_blue" << endl;
  stream << "element face " << __face << endl;/// number of face
  stream << "property list uchar_t int vertex_indices " << endl; /// number of vertices for each face
  stream << "end_header" << endl;
  return true;
}

/* ----------------------------------------------------------------------- */



bool
PlyPrinter::process( Inline * geominline )
{
  GEOM_ASSERT( geominline );
  return geominline->getScene()->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Shape * Shape )
{
  GEOM_ASSERT( Shape );
  if( __pass == 2 ){
    Shape->getAppearance()->apply(*this);
    if(Shape->getGeometry()->apply(__discretizer))
      return __discretizer.getDiscretization()->apply(*this);
    else return false;
  }
  else if( __pass == 1 || __pass == 3 ){
    if(Shape->getGeometry()->apply(__discretizer))
      return __discretizer.getDiscretization()->apply(*this);
    else return false;
  }
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Material * material )
{
  GEOM_ASSERT( material );
  __red = int(material->getAmbient().getRed());
  __green = int(material->getAmbient().getGreen());
  __blue = int(material->getAmbient().getBlue());
  return true;
}


/* ----------------------------------------------------------------------- */

bool
PlyPrinter::process( Texture2D * texture )
{
  GEOM_ASSERT( texture );
  __red = 0;
  __green = 0;
  __blue = 0;
  return true;
}


/* ----------------------------------------------------------------------- */

bool
PlyPrinter::process( ImageTexture * texture )
{
  GEOM_ASSERT( texture );
  return true;
}


/* ----------------------------------------------------------------------- */

bool
PlyPrinter::process( Texture2DTransformation * texture )
{
  GEOM_ASSERT( texture );
  return true;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( MonoSpectral * monoSpectral )
{
  GEOM_ASSERT( monoSpectral );
  __red = 160;
  __green = 160;
  __blue = 160;
  return true;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( MultiSpectral * multiSpectral )
{
  GEOM_ASSERT( multiSpectral );
  __red = 160;
  __green = 160;
  __blue = 160;
  return true;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( AmapSymbol * amapSymbol )
{
  GEOM_ASSERT( amapSymbol );
  GEOM_PLY_MESH( amapSymbol ,Index,_it->size() );
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( AsymmetricHull * asymmetricHull )
{
  GEOM_ASSERT( asymmetricHull );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( AxisRotated * axisRotated )
{
  GEOM_ASSERT( axisRotated );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( BezierCurve * bezierCurve )
{
  GEOM_ASSERT( bezierCurve );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( BezierPatch * bezierPatch )
{
  GEOM_ASSERT( bezierPatch );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Box * box )
{
  GEOM_ASSERT( box );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Cone * cone )
{
  GEOM_ASSERT( cone );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Cylinder * cylinder )
{
  GEOM_ASSERT( cylinder );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( ElevationGrid * elevationGrid )
{
  GEOM_ASSERT( elevationGrid );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( EulerRotated * eulerRotated )
{
  GEOM_ASSERT( eulerRotated );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( ExtrudedHull * extrudedHull )
{
  GEOM_ASSERT( extrudedHull );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Extrusion * extrusion )
{
  GEOM_ASSERT( extrusion );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( FaceSet * faceSet )
{
  GEOM_ASSERT( faceSet );
  GEOM_PLY_MESH( faceSet ,Index,_it->size() );
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Frustum * frustum )
{
  GEOM_ASSERT( frustum );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Group * group )
{
  GEOM_ASSERT( group );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( NurbsCurve * nurbsCurve )
{
  GEOM_ASSERT( nurbsCurve );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( NurbsPatch * nurbsPatch )
{
  GEOM_ASSERT( nurbsPatch );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Oriented * oriented )
{
  GEOM_ASSERT( oriented );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Paraboloid * paraboloid )
{
  GEOM_ASSERT( paraboloid );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( PointSet * pointSet )
{
  GEOM_ASSERT( pointSet );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Polyline * polyline )
{
  GEOM_ASSERT( polyline );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Revolution * revolution )
{
  GEOM_ASSERT( revolution );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( QuadSet * quadSet )
{
  GEOM_ASSERT( quadSet );
  GEOM_PLY_MESH( quadSet , Index4 , 4 );
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Scaled * scaled )
{
  GEOM_ASSERT( scaled );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( ScreenProjected * scp )
{
  GEOM_ASSERT( scp );
  return false;
}

/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Sphere * sphere )
{
  GEOM_ASSERT( sphere );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Tapered * tapered )
{
  GEOM_ASSERT( tapered );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Translated * translated )
{
  GEOM_ASSERT( translated );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( TriangleSet * triangleSet )
{
  GEOM_ASSERT( triangleSet );
  GEOM_PLY_MESH( triangleSet ,Index3 , 3 );
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( BezierCurve2D * bezierCurve )
{
  GEOM_ASSERT( bezierCurve );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Disc * disc )
{
  GEOM_ASSERT( disc );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( NurbsCurve2D * nurbsCurve )
{
  GEOM_ASSERT( nurbsCurve );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( PointSet2D * pointSet )
{
  GEOM_ASSERT( pointSet );
  return false;
}


/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process( Polyline2D * polyline )
{
  GEOM_ASSERT( polyline );
  return false;
}


/* ----------------------------------------------------------------------- */

bool
PlyPrinter::process( Text * text )
{
  GEOM_ASSERT( text );
  return true;
}

/* ----------------------------------------------------------------------- */
bool
PlyPrinter::process( Font * font )
{
  GEOM_ASSERT( font );
  return true;
}

/* ----------------------------------------------------------------------- */


bool
PlyPrinter::process(ScenePtr scene, const char * comment)
{
  GEOM_ASSERT(scene);
  __pass = 1;
  scene->apply(*this);
  header(comment);
  __pass = 2;
  scene->apply(*this);
  __pass = 3;
  scene->apply(*this);
  return true;
}


/* ----------------------------------------------------------------------- */

#undef stream

/* ----------------------------------------------------------------------- */


bool
PlyPrinter::print(ScenePtr scene,string filename,const char * comment, ply_format format )
{
  if(format == ply_ascii){
    ofstream stream(filename.c_str());
    if(!stream)return false;
    else {
      Discretizer discretizer;
      PlyPrinter printer(stream,discretizer);
      return printer.process(scene,comment);
    }
  }
  else if (format == ply_binary_little_endian){
    leofstream stream(filename.c_str());
    if(!stream)return false;
    else {
      Discretizer discretizer;
      PlyBinaryPrinter printer(stream,discretizer,format);
      return printer.process(scene,comment);
//      return true;
    }
  }
  else {
    beofstream stream(filename.c_str());
    if(!stream)return false;
    else {
      Discretizer discretizer;
      PlyBinaryPrinter printer(stream,discretizer,format);
      return printer.process(scene,comment);
//      return true;
    }
  }
}



bool
PlyPrinter::print(ScenePtr scene,Discretizer & discretizer,
      string filename,const char * comment, ply_format format )
{
  if(format == ply_ascii){
    ofstream stream(filename.c_str());
    if(!stream)return false;
    else {
      PlyPrinter printer(stream,discretizer);
      return printer.process(scene,comment);
    }
  }
  else if (format == ply_binary_little_endian){
    leofstream stream(filename.c_str());
    if(!stream)return false;
    else {
      PlyBinaryPrinter printer(stream,discretizer,format);
      return printer.process(scene,comment);
//      return true;
    }
  }
  else {
    beofstream stream(filename.c_str());
    if(!stream)return false;
    else {
      PlyBinaryPrinter printer(stream,discretizer,format);
      return printer.process(scene,comment);
//      return true;
    }
  }
}


/* ----------------------------------------------------------------------- */


PlyBinaryPrinter::PlyBinaryPrinter( bofstream& stream ,
                                    Discretizer& discretizer,
                                    ply_format format ) :
  PlyPrinter(stream.getStream(),discretizer),
  stream(stream),
  __format(format)
{
  assert(format != ply_ascii);
}



PlyBinaryPrinter::~PlyBinaryPrinter()
{
}


/* ----------------------------------------------------------------------- */


bool
PlyBinaryPrinter::header( const char * comment ){
  string header = "ply\nformat ";
  if(__format == ply_binary_little_endian)
    header += "binary_little_endian";
  else header += "binary_big_endian";
  header += " 1.0" + string("\n") +"comment author ";
#ifdef __GNUC__
  header += getenv("LOGNAME");
#endif
  header += '\n';
  if(comment){
    header += string("comment ") + comment;
    header += '\n';
  }
  header += "element vertex " + number(__vertex);
  header += '\n';
  header += "property float x";
  header += '\n';
  header += "property float y";
  header += '\n';
  header += "property float z";
  header += '\n';
  header += "property uchar_t diffuse_red";
  header += '\n';
  header += "property uchar_t diffuse_green";
  header += '\n';
  header += "property uchar_t diffuse_blue";
  header += '\n';
  header += "element face ";
  header += number(__face);
  header += '\n';
  header += "property list uchar_t int vertex_indices";
  header += '\n';
  header += "end_header \n";
  stream << header;
  return true;
}

/* ----------------------------------------------------------------------- */


bool
PlyBinaryPrinter::process( AmapSymbol * amapSymbol )
{
  GEOM_ASSERT( amapSymbol );
  GEOM_PLYB_MESH( amapSymbol ,Index,_it->size() );
}


/* ----------------------------------------------------------------------- */


bool
PlyBinaryPrinter::process( FaceSet * faceSet )
{
  GEOM_ASSERT( faceSet );
  GEOM_PLYB_MESH( faceSet ,Index,_it->size() );
}


/* ----------------------------------------------------------------------- */


bool
PlyBinaryPrinter::process( QuadSet * quadSet )
{
  GEOM_ASSERT( quadSet );
  GEOM_PLYB_MESH( quadSet , Index4 , 4 );
}


/* ----------------------------------------------------------------------- */


bool
PlyBinaryPrinter::process( TriangleSet * triangleSet )
{
  GEOM_ASSERT( triangleSet );
  GEOM_PLYB_MESH( triangleSet ,Index3 , 3 );
}


/* ----------------------------------------------------------------------- */

bool
PlyBinaryPrinter::process(ScenePtr scene, const char * comment)
{
  GEOM_ASSERT(scene);
  __pass = 1;
  scene->apply(*this);
  header(comment);
  __pass = 2;
  scene->apply(*this);
  __pass = 3;
  scene->apply(*this);
  return true;
}


/* ----------------------------------------------------------------------- */

