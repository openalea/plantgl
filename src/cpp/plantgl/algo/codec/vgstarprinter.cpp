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




#include <iomanip>
#include <string>

#include "vgstarprinter.h"
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

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

    __matrix.getTransformation(scaling, rotation, translation);
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


bool VgstarPrinter::process(Shape * Shape) {
  GEOM_ASSERT(Shape);
  bool b=Shape->appearance->apply(*this);
  return ( b && (Shape->geometry->apply(*this)));
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Material * material ) {
  GEOM_ASSERT(material);
  __color = material->getAmbient();
  return true;
}

/* ----------------------------------------------------------------------- */

bool VgstarPrinter::process( Texture2D * texture ) {
  GEOM_ASSERT(texture);
  __color = Color3::BLACK;;
  return true;
}

/* ----------------------------------------------------------------------- */

bool VgstarPrinter::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  return true;
}

/* ----------------------------------------------------------------------- */

bool VgstarPrinter::process( Texture2DTransformation * texture ) {
  GEOM_ASSERT(texture);
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

  __matrix.getTransformation(scaling, rotation, translation);

  if(scaling.x() == scaling.y()){

          __matrix.push();
          __matrix.axisRotation(Vector3::OY,-GEOM_HALF_PI);

          real_t diam_base= cone->getRadius( ) * 2.;
          Vector3 scales(cone->getHeight(),diam_base,0.0001);
          __matrix.scale(scales);

          GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"33");

          printTransformation();
          printColor();
          printNullTriangle();

          GEOM_VGSTARPRINT_END(__vgstarStream);
          __matrix.pop();
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

  __matrix.push();

  real_t r = cylinder->getRadius()*2;
  Vector3 scales(r,r,cylinder->getHeight());
  __matrix.scale(scales);

  __matrix.axisRotation(Vector3::OY,-GEOM_HALF_PI);


  GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"32");

  printTransformation();
  printColor();
  printNullTriangle();

  GEOM_VGSTARPRINT_END(__vgstarStream);
  __matrix.pop();

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

  __matrix.getTransformation(scaling, rotation, translation);

  if(scaling.x() == scaling.y()){

          __matrix.push();
          __matrix.axisRotation(Vector3::OY,-GEOM_HALF_PI);

          real_t diam_base= cone->getRadius( ) * 2.;
          real_t diam_up= cone->getRadius( ) * cone->getTaper() * 2.;
          Vector3 scales(cone->getHeight(),diam_base,diam_up);
          __matrix.scale(scales);

          GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"33");

          printTransformation();
          printColor();
          printNullTriangle();

          GEOM_VGSTARPRINT_END(__vgstarStream);
          __matrix.pop();
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
  for (uint_t _i = 0; _i < points->size(); ++_i)
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


bool VgstarPrinter::process( Polyline * polyline ) {
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

  __matrix.push();

  real_t r = sphere->getRadius()*2;
  Vector3 scales(r,r,r);
  __matrix.scale(scales);

  GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"31");

  printTransformation();
  printColor();
  printNullTriangle();

  GEOM_VGSTARPRINT_END(__vgstarStream);
  __matrix.pop();

  return true;
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( Tapered * tapered ) {
  return triangle_process(tapered);
}


/* ----------------------------------------------------------------------- */


bool VgstarPrinter::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);

  for (uint_t _i = 0; _i < triangleSet->getIndexListSize(); _i++)
    {
    GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"0");
        printNullTransformation();
        printColor();
    Vector3 _vertex1 = triangleSet->getFacePointAt(_i,0);
        _vertex1 = getMatrix() * _vertex1 ;
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex1);
    Vector3 _vertex2 = triangleSet->getFacePointAt(_i,1);
        _vertex2 = getMatrix() * _vertex2;
    GEOM_VGSTARPRINT_VECTOR3(__vgstarStream,_vertex2);
    Vector3 _vertex3 = triangleSet->getFacePointAt(_i,2);
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
  __matrix.push();

  real_t r = disc->getRadius()*2;
  Vector3 scales(r,r,r);
  __matrix.scale(scales);
  __matrix.translate(Vector3(-0.5,0,0));

  GEOM_VGSTARPRINT_BEGIN(__vgstarStream,"21");

  printTransformation();
  printColor();
  printNullTriangle();

  GEOM_VGSTARPRINT_END(__vgstarStream);
  __matrix.pop();
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


bool VgstarPrinter::process( Polyline2D * polyline ) {
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

