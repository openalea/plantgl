/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon & C. Godin
 *       (frederic.boudon@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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



#include <iomanip>
#include <string>

#include "actn_vgstarprinter.h"
#include "appe_material.h"
#include "appe_monospectral.h"
#include "appe_multispectral.h"
#include "appe_texture.h"
#include "geom_amapsymbol.h"
#include "geom_asymmetrichull.h"
#include "geom_axisrotated.h"
#include "geom_beziercurve.h"
#include "geom_bezierpatch.h"
#include "geom_box.h"
#include "geom_cone.h"
#include "geom_cylinder.h"
#include "geom_disc.h"
#include "geom_elevationgrid.h"
#include "geom_eulerrotated.h"
#include "geom_extrudedhull.h"
#include "geom_faceset.h"
#include "geom_frustum.h"
#include "geom_group.h"
#include "geom_ifs.h"
#include "geom_extrusion.h"
#include "geom_nurbscurve.h"
#include "geom_nurbspatch.h"
#include "geom_oriented.h"
#include "geom_pointset.h"
#include "geom_polyline.h"
#include "geom_paraboloid.h"
#include "geom_quadset.h"
#include "geom_revolution.h"
#include "geom_swung.h"
#include "geom_scaled.h"
#include "geom_sphere.h"
#include "geom_tapered.h"
#include "geom_translated.h"
#include "geom_triangleset.h"
#include "scne_shape.h"
#include "geom_geometryarray2.h"
#include "Tools/util_math.h"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */
/*
// Geom names corresponding to VGSTAR primitives
// indexed by enums in VgstarPrinter::VGSTAR_TYPE
static char** GEOM_NAMES_FOR_VGSTAR_PRIMITVES = {
  "Not Used",
  "POLYGONE",
  "Sphere",
  "CYLINDER",
  "ELLIPSIS"
};
*/

#define GEOM_VGSTARPRINT_SPACE '\t'

#define GEOM_VGSTARPRINT_BEGIN(stream,nb_objects) \
  stream << nb_objects; \

#define GEOM_VGSTARPRINT_END(stream) \
  stream << endl;

#define GEOM_VGSTARPRINT_COLOR3(stream,val) \
  stream << GEOM_VGSTARPRINT_SPACE << (int)val.getRed() \
         << GEOM_VGSTARPRINT_SPACE << (int)val.getGreen() \
         << GEOM_VGSTARPRINT_SPACE << (int)val.getBlue();


#define GEOM_VGSTARPRINT_VECTOR3(stream,val) \
  stream << GEOM_VGSTARPRINT_SPACE << val.x() \
         << GEOM_VGSTARPRINT_SPACE << val.y() \
         << GEOM_VGSTARPRINT_SPACE << val.z();

/* ----------------------------------------------------------------------- */

bool VgstarPrinter::triangle_process( Geometry* object )
  {
  GEOM_ASSERT(object);
  object->apply(__tesselator);
  TriangleSetPtr _triangleSet = __tesselator.getTriangulation();
  if (_triangleSet) _triangleSet->apply(*this);
  return true;
  }

/* ----------------------------------------------------------------------- */

VgstarPrinter::VgstarPrinter( ostream& vgstarStream, Tesselator& tesselator) :
  MatrixComputer(),
  __vgstarStream(vgstarStream),
  __tesselator(tesselator){
}


/* ----------------------------------------------------------------------- */

bool VgstarPrinter::header(const char * comment){
  __vgstarStream <<
          "Obj\tEchX\tEchY\tEchZ\tTransX\tTransY\tTransZ\tRotX\tRotY\tRotZ\tR\tG\tB\tX1\tY1\tZ1\tX2\tY2\tZ2\tX3\tY3\tZ3" << endl;
    return true;
}

void VgstarPrinter::printTransformation(){
    Vector3 scaling, rotation, translation;

    getTransformation(scaling, rotation, translation);
    rotation *= GEOM_DEG;

//      Matrix4 test;
//      test.setTransformation(scaling,rotation,translation);
//      assert(test == gmatrix);

        Vector3 vgs_rotation(rotation.z(),rotation.y(),rotation.x());

    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,scaling);
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,translation);
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,vgs_rotation);
}

void VgstarPrinter::printNullTransformation(){
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,Vector3::ORIGIN);
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,Vector3::ORIGIN);
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,Vector3::ORIGIN);
}

void VgstarPrinter::printColor(){
    GEOM_VGSTARPRINT_COLOR3(__vgstarStream,__color);
}

void VgstarPrinter::printNullTriangle(){
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,Vector3::ORIGIN);
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,Vector3::ORIGIN);
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,Vector3::ORIGIN);
}

/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process(GeomShape * geomShape) {
  GEOM_ASSERT(geomShape);
  bool b=geomShape->appearance->apply(*this);
  return ( b && (geomShape->geometry->apply(*this)));
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Material * material ) {
  GEOM_ASSERT(material);
  __color = material->getAmbient();
  return true;
}

/* ----------------------------------------------------------------------- */

bool VgstarPrinter::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  __color = texture->getAmbient();
  return true;
}

/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  __color = Color3::BLACK;
  return false;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  __color = Color3::BLACK;
  return false;
}



/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( AmapSymbol * amapSymbol ) {
  return triangle_process(amapSymbol);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( AsymmetricHull * asymmetricHull ) {
  return triangle_process(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  return false;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( BezierPatch * bezierPatch ) {
  return triangle_process(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Box * box ) {
  return triangle_process(box);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Cone * cone ) {
  GEOM_ASSERT(cone);

  Vector3 scaling, rotation, translation;

  getTransformation(scaling, rotation, translation);

  if(scaling.x() == scaling.y()){

          pushMatrix();
          axisRotate(Vector3::OY,-GEOM_HALF_PI);

          real_t diam_base= cone->getRadius( ) * 2.;
          Vector3 scales(cone->getHeight(),diam_base,0.0001);
          scale(scales);

          GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"33");

          printTransformation();
          printColor();
          printNullTriangle();

          GEOM_VGSTARPRINT_END(__vgstarStream);
          popMatrix();
          return true;
  }
  else {
          return triangle_process(cone);
  }
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Cylinder * cylinder ) {
//  return triangle_process(cylinder);
  GEOM_ASSERT(cylinder);

  pushMatrix();

  real_t r = cylinder->getRadius()*2;
  Vector3 scales(r,r,cylinder->getHeight());
  scale(scales);

  axisRotate(Vector3::OY,-GEOM_HALF_PI);


  GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"32");

  printTransformation();
  printColor();
  printNullTriangle();

  GEOM_VGSTARPRINT_END(__vgstarStream);
  popMatrix();

  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( ElevationGrid * elevationGrid ) {
  return triangle_process(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( ExtrudedHull * extrudedHull ) {
  return triangle_process(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( FaceSet * faceSet ) {
  return triangle_process(faceSet);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Frustum * cone ) {
  GEOM_ASSERT(cone);

  Vector3 scaling, rotation, translation;
  Matrix4 gmatrix = getMatrix();

  getTransformation(scaling, rotation, translation);

  if(scaling.x() == scaling.y()){

          pushMatrix();
          axisRotate(Vector3::OY,-GEOM_HALF_PI);

          real_t diam_base= cone->getRadius( ) * 2.;
          real_t diam_up= cone->getRadius( ) * cone->getTaper() * 2.;
          Vector3 scales(cone->getHeight(),diam_base,diam_up);
          scale(scales);

          GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"33");

          printTransformation();
          printColor();
          printNullTriangle();

          GEOM_VGSTARPRINT_END(__vgstarStream);
          popMatrix();
  }
  else {
          return triangle_process(cone);
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Extrusion * extrusion   ) {
  return triangle_process(extrusion);
}

/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Group * group ) {
  GEOM_ASSERT(group);

  group->getGeometryList()->apply(*this) ;

  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  // nothing to do

  return true;
}

/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( NurbsPatch * nurbsPatch ) {
  return triangle_process(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Paraboloid * paraboloid ) {
  return triangle_process(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);

  bool hasColor = pointSet->hasColorList();
  Color3 oldcolor = __color;
  const Point3ArrayPtr& points = pointSet->getPointList();
  for (uint32_t _i = 0; _i < points->getSize(); ++_i)
  {
	  GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"40");
	  printNullTransformation();
	  if(hasColor){
		  const Color4& col = pointSet->getColorList()->getAt(_i);
		  __color = Color3(col.getRed(),col.getGreen(),col.getBlue());
	  }
	  printColor();
	  Vector3 _vertex1 = points->getAt(_i);
	  _vertex1 = getMatrix() * _vertex1 ;
	  GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex1);
	  GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex1);
	  GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex1);
	  GEOM_VGSTARPRINT_END(__vgstarStream);
  }
  __color = oldcolor;
  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( GeomPolyline * polyline ) {
  GEOM_ASSERT(polyline);

  // nothing to do

  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Revolution * revolution ) {
  return triangle_process(revolution);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Swung * swung )
{
  return triangle_process(swung);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( QuadSet * quadSet ) {
  return triangle_process(quadSet);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);

  pushMatrix();

  real_t r = sphere->getRadius()*2;
  Vector3 scales(r,r,r);
  scale(scales);

  GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"31");

  printTransformation();
  printColor();
  printNullTriangle();

  GEOM_VGSTARPRINT_END(__vgstarStream);
  popMatrix();

  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Tapered * tapered ) {
  return triangle_process(tapered);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);

  for (uint32_t _i = 0; _i < triangleSet->getIndexListSize(); _i++)
    {
    GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"0");
        printNullTransformation();
        printColor();
    Vector3 _vertex1 = triangleSet->getPointAt(_i,0);
        _vertex1 = getMatrix() * _vertex1 ;
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex1);
    Vector3 _vertex2 = triangleSet->getPointAt(_i,1);
        _vertex2 = getMatrix() * _vertex2;
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex2);
    Vector3 _vertex3 = triangleSet->getPointAt(_i,2);
    _vertex3 = getMatrix() * _vertex3;
        GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex3);
    GEOM_VGSTARPRINT_END(__vgstarStream);
    }


  return true;
}


/* ------------------------------------------------------------------------- */


bool VgstarPrinter::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Disc * disc ) {
//  return triangle_process(disc);
  pushMatrix();

  real_t r = disc->getRadius()*2;
  Vector3 scales(r,r,r);
  scale(scales);
  translate(Vector3(-0.5,0,0));

  GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"21");

  printTransformation();
  printColor();
  printNullTriangle();

  GEOM_VGSTARPRINT_END(__vgstarStream);
  popMatrix();
  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  // nothing to do
  return true;
}

/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do
  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( GeomPolyline2D * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do
  return true;
}


/* ----------------------------------------------------------------------- */

bool VgstarPrinter::process( Text * text ) {
  GEOM_ASSERT(text);
  // nothing to do
  return true;
}

/* ----------------------------------------------------------------------- */

bool VgstarPrinter::process( Font * font ) {
  GEOM_ASSERT(font);
  // nothing to do
  return true;
}

/* ----------------------------------------------------------------------- */

