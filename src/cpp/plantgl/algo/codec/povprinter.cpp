/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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
#include <plantgl/math/util_math.h>

#include "povprinter.h"
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>

#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/algo/base/tesselator.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/container/pointarray.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_POVPRINT_INCREMENT_INDENT \
  __indent += "  ";


#define GEOM_POVPRINT_DECREMENT_INDENT \
  __indent.erase(__indent.end() - 2,__indent.end());


#define GEOM_POVPRINT_DECLARE(stream,obj) \
  if (! __cache.insert(obj->getId()).second) return true; \
  stream << __indent << "#declare " << obj->getName() << " = ";


#define GEOM_POVPRINT_BEG_(stream,type) \
  stream << __indent << type << " {" << endl; \
  GEOM_POVPRINT_INCREMENT_INDENT; \

#define GEOM_POVPRINT_END_(stream) \
  GEOM_POVPRINT_DECREMENT_INDENT; \
  stream << __indent << "}" << endl;


#define GEOM_POVPRINT_BEGIN(stream,type,obj) \
    if (obj->isNamed()){ \
	  if(!(__cache.insert(obj->getId()).second)){ \
	    GEOM_POVPRINT_BEG_(stream,"object"); \
		stream << __indent << obj->getName() << endl; \
		GEOM_POVPRINT_TEXTURE; \
		GEOM_POVPRINT_END_(stream); \
		return true; \
	  } \
	  else { \
		stream << __indent << "#declare " << obj->getName() << " = "; \
		GEOM_POVPRINT_BEG_(stream,type) \
	  } \
	} \
	else { \
	    GEOM_POVPRINT_BEG_(stream,type); \
	} \

#define GEOM_POVPRINT_END(stream,obj) \
  GEOM_POVPRINT_END_(stream) \
  if (obj->isNamed()){ \
	    GEOM_POVPRINT_BEG_(stream,"object"); \
		stream << __indent << obj->getName() << endl; \
		GEOM_POVPRINT_END_(stream); \
  } \


#define GEOM_POVPRINT_BEGIN_DIS(stream,obj) \
    if (obj->isNamed()){ \
	  if(!(__cache.insert(obj->getId()).second)){ \
	    GEOM_POVPRINT_BEG_(stream,"object"); \
		stream << __indent << obj->getName() << endl; \
		/* GEOM_POVPRINT_TEXTURE;*/  \
		GEOM_POVPRINT_END_(stream); \
		return true; \
	  } \
	} \


#define GEOM_POVPRINT_DISCRETIZE(stream,obj) \
  GEOM_ASSERT(obj); \
  GEOM_POVPRINT_BEGIN_DIS(stream,obj) \
  obj->apply(__tesselator); \
  TriangleSetPtr _triangleSet = __tesselator.getTriangulation(); \
  bool res = false; \
  if (_triangleSet) res = _triangleSet->apply(*this); \
  return res; \


#define GEOM_POVPRINT_TEXTURE \
  __geomStream << __indent << "texture { " << __texture << " }" << endl;


#define GEOM_POVPRINT_ANGLE(stream,val) \
  stream << (val * GEOM_DEG);


#define GEOM_POVPRINT_COLOR3(stream,val) \
  stream << "<" << ((real_t)val.getAt(0) / 255.0) \
         << "," << ((real_t)val.getAt(1) / 255.0) \
         << "," << ((real_t)val.getAt(2) / 255.0) << ">";


#define GEOM_POVPRINT_COLOR4(stream,val) \
  stream << "<" << ((real_t)val.getAt(0) / 255.0) \
         << "," << ((real_t)val.getAt(1) / 255.0) \
         << "," << ((real_t)val.getAt(2) / 255.0) \
         << "," << ((real_t)val.getAt(3) / 255.0) << ">";


#define GEOM_POVPRINT_VECTOR2(stream,val) \
  stream << "<" << val.x() \
         << "," << val.y() << ">";


#define GEOM_POVPRINT_VECTOR3(stream,val) \
  stream << "<" << val.x() \
         << "," << val.y() \
         << "," << val.z() << ">";


#define GEOM_POVPRINT_VECTOR4(stream,val) \
  stream << "<" << val.x() \
         << "," << val.y() \
         << "," << val.z() \
         << "," << val.w()<< ">";


#define GEOM_POVPRINT_TRANSLATE(stream,vector) \
  stream << __indent << "translate "; \
  GEOM_POVPRINT_VECTOR3(stream,vector); \
  stream << endl;


#define GEOM_POVPRINT_SCALE(stream,vector) \
  stream << __indent << "scale "; \
  GEOM_POVPRINT_VECTOR3(stream,vector); \
  stream << endl;


#define GEOM_POVPRINT_ROTATE(stream,vector) \
  stream << __indent << "rotate "; \
  GEOM_POVPRINT_VECTOR3(stream,vector); \
  stream << endl;


#define GEOM_POVPRINT_MATRIX(stream,matrix) \
  stream << __indent << "matrix <"; \
  stream << matrix(0,0) << "," << matrix(1,0) << "," << matrix(2,0) << ","; \
  stream << matrix(0,1) << "," << matrix(1,1) << "," << matrix(2,1) << ","; \
  stream << matrix(0,2) << "," << matrix(1,2) << "," << matrix(2,2) << ","; \
  stream << matrix(0,3) << "," << matrix(1,3) << "," << matrix(2,3) << ">"; \
  stream << endl;


/* ----------------------------------------------------------------------- */


PovPrinter::PovPrinter( ostream& povStream,
                        Tesselator& tesselator,
						const BoundingBox& bbox ) :
  Printer(povStream,povStream,povStream),
  __tesselator(tesselator),
  __linewidth(0.1),
  __linecache(false),
  __pointcache(false){
  __geomStream << "#declare CENTER = ";
  GEOM_POVPRINT_VECTOR3(__geomStream, bbox.getCenter());
  __geomStream << ';' << endl;
}

PovPrinter::PovPrinter( ostream& povStream, Tesselator& tesselator) :
  Printer(povStream,povStream,povStream),
  __tesselator(tesselator),
  __linewidth(0.1),
  __linecache(false),
  __pointcache(false){
}

PovPrinter::~PovPrinter( ) {
}

/* ----------------------------------------------------------------------- */

bool PovPrinter::setCenter(const BoundingBox& bbox ){
  __geomStream << "#declare CENTER = ";
  GEOM_POVPRINT_VECTOR3(__geomStream, bbox.getCenter());
  __geomStream << ';' << endl;
  return true;
}


bool PovPrinter::setCamera(const Vector3& location, 
						   const Vector3& direction, 
						   const Vector3& up, 
						   const Vector3& right){
	assert(direction.isOrthogonalTo(up));
	__geomStream << endl << "camera {" << endl;
	__geomStream << "   perspective" << endl;
	__geomStream << "    location ";
    GEOM_POVPRINT_VECTOR3(__geomStream,location);
    __geomStream << setprecision(20);
    __geomStream << endl << "    direction ";
    GEOM_POVPRINT_VECTOR3(__geomStream,direction);
    __geomStream << endl << "    up ";
    GEOM_POVPRINT_VECTOR3(__geomStream,up);
     Vector3 _right = right*(4.0f/3.0f);
    __geomStream << endl << "    right ";
    GEOM_POVPRINT_VECTOR3(__geomStream,_right);
    __geomStream << setprecision(6);
    __geomStream << endl << '}'  << endl << endl;
    return true;
}

bool PovPrinter::setCamera(const Vector3& eye, 
						   const Vector3& distance,
						   const double&  ang, 
						   const double&  az, 
						   const double&  el){
	__geomStream << endl << "camera {" << endl;
	__geomStream << "   perspective" << endl;
	__geomStream << "    location ";
	Vector3 v = eye-distance;
    GEOM_POVPRINT_VECTOR3(__geomStream,v);
    __geomStream << endl << "    direction ";
        Vector3 dir = - Vector3::OX;
    GEOM_POVPRINT_VECTOR3(__geomStream,dir);
    __geomStream << endl << "    up ";
    GEOM_POVPRINT_VECTOR3(__geomStream,Vector3::OZ);
    __geomStream << endl << "    right <0,4/3,0>";
    __geomStream << endl << "    angle " << ang;
    __geomStream << endl << "    rotate " ;
        Vector3 rotation(0,-el,az);
    GEOM_POVPRINT_VECTOR3(__geomStream,rotation);
    __geomStream << endl << '}'  << endl ;
    return true;
}

void PovPrinter::beginHeader(){
	__geomStream << endl;
	__geomStream << "#ifndef (__camera_definition__)" << endl;
	__geomStream << "#declare __camera_definition__ = true;" << endl;
}

void PovPrinter::endHeader(){
	__geomStream << endl;
	__geomStream << "#end // __camera_definition__" << endl;
}

bool PovPrinter::setLight(const TOOLS(Vector3)& position, const Color3& color){
        __geomStream << endl << "light_source {" << endl << "     ";
    GEOM_POVPRINT_VECTOR3(__geomStream,position);
        __matStream << endl << "    color rgb ";
        GEOM_POVPRINT_COLOR3(__matStream,color);
    __geomStream << endl << '}'  << endl << endl;
        return true;
}

bool PovPrinter::header(const char * comment){
    __geomStream << "/*" << endl;
	__geomStream << " * A povray file generated with GEOM." << endl;
    __geomStream << " * Example of use : povray -Ifile.pov -Ofile.tga +FT +H400 +W400." << endl;
    if(comment) __geomStream << " * " << comment << '.' << endl;
    __geomStream << " */" << endl << endl;
    return true;
}

/* ----------------------------------------------------------------------- */

bool PovPrinter::setBackGround(const Color3& color){
    __geomStream << "background { color rgb ";
    GEOM_POVPRINT_COLOR3(__geomStream,color);
    __geomStream << " }"  << endl << endl;
    return true;
}

/* ----------------------------------------------------------------------- */


bool PovPrinter::process(Shape * Shape) {
  GEOM_ASSERT(Shape);
  bool b=Shape->appearance->apply(*this);
  return ( b && (Shape->geometry->apply(*this)));
}

bool PovPrinter::process( Inline * geomInline ) {
  GEOM_ASSERT(geomInline);
  if(geomInline->getScene() && geomInline->getScene()->getSize()>1){
	GEOM_POVPRINT_BEG_(__geomStream,"union");
  }
  else {
	GEOM_POVPRINT_BEG_(__geomStream,"object");
  }

  geomInline->getScene()->apply(*this) ;

  const Vector3 _scale = geomInline->getScale();
  GEOM_POVPRINT_SCALE(__geomStream,_scale);

  const Vector3 _translation = geomInline->getTranslation();
  GEOM_POVPRINT_TRANSLATE(__geomStream,_translation);

  GEOM_POVPRINT_END_(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Material * material ) {
  GEOM_ASSERT(material);

  __texture = material->getName();

  GEOM_POVPRINT_DECLARE(__matStream,material);

  GEOM_POVPRINT_BEG_(__matStream,"texture");

  GEOM_POVPRINT_BEG_(__matStream,"pigment");

  Color4 _color(material->getAmbient(),(uchar_t)(material->getTransparency() * 255.0) );
  __matStream << __indent << "color rgbt ";
  GEOM_POVPRINT_COLOR4(__matStream,_color);
  __matStream << endl;

  GEOM_POVPRINT_END_(__matStream);

  __matStream << __indent;
  GEOM_POVPRINT_BEG_(__matStream,"finish");

  __matStream << __indent << "ambient 1" << endl;
  __matStream << __indent << "diffuse " << material->getDiffuse() << endl;
  real_t spec = material->getSpecular().getAverageClamped();
  __matStream << __indent << "specular " << spec << endl;

  GEOM_POVPRINT_END_(__matStream);

  GEOM_POVPRINT_END_(__matStream);

  __tesselator.computeTexCoord(false);

  return true;
}


/* ----------------------------------------------------------------------- */
bool PovPrinter::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);

  __texture = texture->getName();

  GEOM_POVPRINT_DECLARE(__matStream,texture);

  GEOM_POVPRINT_BEG_(__matStream,"texture");

  GEOM_POVPRINT_BEG_(__matStream,"pigment");

  GEOM_POVPRINT_BEG_(__matStream,"uv_mapping image_map");

  __matStream << __indent << "png \"" << texture->getFilename() << '"' << endl;;

  GEOM_POVPRINT_END_(__matStream);

  GEOM_POVPRINT_END_(__matStream);

  GEOM_POVPRINT_END_(__matStream);

  __tesselator.computeTexCoord(true);

  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);

  __texture = monoSpectral->getName();

  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);

  __texture = multiSpectral->getName();

  return true;
}



/* ----------------------------------------------------------------------- */


bool PovPrinter::process( AmapSymbol * amapSymbol ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,amapSymbol);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( AsymmetricHull * asymmetricHull ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);
  GEOM_POVPRINT_BEGIN(__geomStream,"object",axisRotated);

  axisRotated->getGeometry()->apply(*this);

  /*Matrix4TransformationPtr _transform;
  _transform.cast(axisRotated->getTransformation());
  GEOM_ASSERT(_transform);
  Matrix4 _matrix = _transform->getMatrix();
  GEOM_POVPRINT_MATRIX(__geomStream,_matrix);*/

  OrthonormalBasis3DPtr basis = axisRotated->getOrthonormalBasis();
  Matrix3 m = basis->getMatrix3();

  Vector3 rotation = m.eulerAnglesZYX();
  rotation *= GEOM_DEG;
  Vector3 rot = Vector3(rotation.z(),rotation.y(),rotation.x());
  __geomStream << __indent << "rotate ";
  GEOM_POVPRINT_VECTOR3(__geomStream,rot);
  __geomStream << endl;

  GEOM_POVPRINT_END(__geomStream,axisRotated);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

  GEOM_POVPRINT_BEGIN_DIS(__geomStream,bezierCurve)
  Discretizer d;
  bool res = bezierCurve->apply(d); 
  if (d.getDiscretization()) res = d.getDiscretization()->apply(*this); 
  return res; 
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( BezierPatch * bezierPatch ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,bezierPatch);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Box * box ) {
  GEOM_ASSERT(box);
  GEOM_POVPRINT_BEGIN(__geomStream,"box",box);

  Vector3 _ll = box->getFrontLowerLeftCorner();
  Vector3 _ur = box->getBackUpperRightCorner();

  __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,_ll);
  GEOM_POVPRINT_VECTOR3(__geomStream,_ur);
  __geomStream << endl;

  GEOM_POVPRINT_TEXTURE;

  GEOM_POVPRINT_END(__geomStream,box);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Cone * cone ) {
  GEOM_ASSERT(cone);
  GEOM_POVPRINT_BEGIN(__geomStream,"cone",cone);

  __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,Vector3::ORIGIN);
  __geomStream << ", " << cone->getRadius() << endl;

  __geomStream << __indent;
  Vector3 _out = Vector3(0,0,cone->getHeight());
  GEOM_POVPRINT_VECTOR3(__geomStream,_out);
  __geomStream << ", 0" << endl;

  if (! cone->getSolid())
    __geomStream << __indent << "open" << endl;

  GEOM_POVPRINT_TEXTURE;

  GEOM_POVPRINT_END(__geomStream,cone);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);
  GEOM_POVPRINT_BEGIN(__geomStream,"cylinder",cylinder);

  __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,Vector3::ORIGIN);
  __geomStream << endl;

  Vector3 _out = Vector3(0,0,cylinder->getHeight());
  __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,_out);
  __geomStream << endl;

  __geomStream << __indent << cylinder->getRadius() << endl;

  if (! cylinder->getSolid())
    __geomStream << __indent << "open" << endl;

  GEOM_POVPRINT_TEXTURE;

  GEOM_POVPRINT_END(__geomStream,cylinder);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( ElevationGrid * elevationGrid ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);
  GEOM_POVPRINT_BEGIN(__geomStream,"object",eulerRotated);

  eulerRotated->getGeometry()->apply(*this);

  Vector3 rotation(eulerRotated->getRoll() * GEOM_DEG,
                    eulerRotated->getElevation() * GEOM_DEG,
                    eulerRotated->getAzimuth() * GEOM_DEG);
  __geomStream << __indent << "rotate ";
  GEOM_POVPRINT_VECTOR3(__geomStream,rotation);
  __geomStream << endl;

  GEOM_POVPRINT_END(__geomStream,eulerRotated);

  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( ExtrudedHull * extrudedHull ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( FaceSet * faceSet ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,faceSet);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);
  GEOM_POVPRINT_BEGIN(__geomStream,"cone",frustum);

  __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,Vector3::ORIGIN);
  __geomStream << ", " << frustum->getRadius() << endl;

  __geomStream << __indent;
  Vector3 _out = Vector3(0,0,frustum->getHeight());
  GEOM_POVPRINT_VECTOR3(__geomStream,_out);
  __geomStream << ", " << (frustum->getRadius() * frustum->getTaper()) << endl;

  if (! frustum->getSolid())
    __geomStream << __indent << "open" << endl;

  GEOM_POVPRINT_TEXTURE;

  GEOM_POVPRINT_END(__geomStream,frustum);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Extrusion * extrusion   ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,extrusion);
}

/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Group * group ) {
  GEOM_ASSERT(group);
  GEOM_POVPRINT_BEGIN(__geomStream,"union",group);
  group->getGeometryList()->apply(*this) ;
  GEOM_POVPRINT_END(__geomStream,group);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( IFS * ifs )
{
//todo OK
  GEOM_ASSERT(ifs);
  GEOM_TRACE("process IFS");

  ITPtr transfos;
  transfos.cast( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);

  GEOM_POVPRINT_BEGIN(__geomStream,"union",ifs);

  Matrix4Array::const_iterator matrix= matrixList->getBegin();
  while( matrix != matrixList->getEnd() )
    {
    GEOM_POVPRINT_BEG_(__geomStream,"object");
    ifs->getGeometry()->apply(*this);
    GEOM_POVPRINT_MATRIX(__geomStream,(*matrix));
    GEOM_POVPRINT_END_(__geomStream);
    matrix++;
    }
  GEOM_POVPRINT_END(__geomStream,ifs);

  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  GEOM_POVPRINT_BEGIN_DIS(__geomStream,nurbsCurve)
  Discretizer d;
  bool res = nurbsCurve->apply(d); 
  if (d.getDiscretization()) res = d.getDiscretization()->apply(*this); 
  return res; 

  return true;
}

/* ----------------------------------------------------------------------- */


bool PovPrinter::process( NurbsPatch * nurbsPatch ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);
  GEOM_POVPRINT_BEGIN(__geomStream,"object",oriented);

  oriented->getGeometry()->apply(*this);

  /*MatrixTransformationPtr _transform(oriented->getTransformation());
  GEOM_ASSERT(_transform);
  Matrix4 _matrix = _transform->getMatrix();
  GEOM_POVPRINT_MATRIX(__geomStream,_matrix);*/

  OrthonormalBasis3DPtr basis = oriented->getOrthonormalBasis();
  Matrix3 m = basis->getMatrix3();

  Vector3 rotation = m.eulerAnglesZYX();
  rotation *= GEOM_DEG;
  Vector3 rot = Vector3(rotation.z(),rotation.y(),rotation.x());
  __geomStream << __indent << "rotate ";
  GEOM_POVPRINT_VECTOR3(__geomStream,rot);
  __geomStream << endl;

  GEOM_POVPRINT_END(__geomStream,oriented);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Paraboloid * paraboloid ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,paraboloid);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);

  if(!__pointcache){
	__geomStream << "#ifndef (PointWidth)" << endl;
	__geomStream << "#declare PointWidth = " << __linewidth << ';' << endl;
	__geomStream << "#end // PointWidth" << endl;
	__pointcache = true;
  }

  GEOM_POVPRINT_BEGIN(__geomStream,"union",pointSet);
  for (Point3Array::const_iterator _i = pointSet->getPointList()->getBegin();
	_i != pointSet->getPointList()->getEnd(); _i++) {
    const Vector3& _vertex1 = *_i;
 	GEOM_POVPRINT_BEG_(__geomStream,"sphere");
	__geomStream << __indent;
    GEOM_POVPRINT_VECTOR3(__geomStream,_vertex1);
    __geomStream << ", PointWidth" << endl;
	GEOM_POVPRINT_TEXTURE;
	GEOM_POVPRINT_END_(__geomStream);
  };
  // nothing to do
  GEOM_POVPRINT_END(__geomStream,pointSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);

  if(!__linecache){
	__geomStream << "#ifndef (LineWidth)" << endl;
	__geomStream << "#declare LineWidth = " << __linewidth << ';' << endl;
	__geomStream << "#end // LineWidth" << endl;
	__linecache = true;
  }

  GEOM_POVPRINT_BEGIN(__geomStream,"union",polyline);
  for (Point3Array::const_iterator _i = polyline->getPointList()->getBegin();
	_i != polyline->getPointList()->getEnd()-1; _i++) {
    const Vector3& _vertex1 = *_i;
    const Vector3& _vertex2 = *(_i+1);
	if(norm(_vertex1-_vertex2) > GEOM_EPSILON){
 	GEOM_POVPRINT_BEG_(__geomStream,"cylinder");
	__geomStream << __indent;
    GEOM_POVPRINT_VECTOR3(__geomStream,_vertex1);
    __geomStream << ", ";
    GEOM_POVPRINT_VECTOR3(__geomStream,_vertex2);
    __geomStream << ", LineWidth" << endl;
	GEOM_POVPRINT_TEXTURE;
	GEOM_POVPRINT_END_(__geomStream);
	}
  };
  // nothing to do
  GEOM_POVPRINT_END(__geomStream,polyline);

  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);
  GEOM_POVPRINT_BEGIN(__geomStream,"lathe",revolution);

  const Curve2DPtr prof = revolution->getProfile();
  real_t fk = prof->getFirstKnot();
  real_t lk = prof->getLastKnot();
  uint32_t st = prof->getStride();
  real_t step = (lk - fk)/st;
  __geomStream << __indent << "quadratic_spline " << st << "," << endl;

/*  for (real_t _i = fk;
       _i <= lk; _i+=step) {
    const Vector2& _point = prof->getPointAt(_i);
    __geomStream << __indent;
    GEOM_POVPRINT_VECTOR2(__geomStream,_point);
    __geomStream << (_i != lk ? "," : "") << endl;
  };*/

  for (uint32_t _i = 0;
       _i < st; _i++) {
    Vector2 _point = prof->getPointAt(fk);
    __geomStream << __indent;
    GEOM_POVPRINT_VECTOR2(__geomStream,_point);
    __geomStream << endl;
	fk += step;
  };

  GEOM_POVPRINT_TEXTURE;

  __geomStream << __indent << "rotate <90,0,0>" << endl;

  GEOM_POVPRINT_END(__geomStream,revolution);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Swung * swung )
{
  GEOM_POVPRINT_DISCRETIZE(__geomStream,swung);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( QuadSet * quadSet ) {
  GEOM_POVPRINT_DISCRETIZE(__geomStream,quadSet);
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);
  GEOM_POVPRINT_BEGIN(__geomStream,"object",scaled);

  scaled->getGeometry()->apply(*this);

  const Vector3& _scale = scaled->getScale();
  GEOM_POVPRINT_SCALE(__geomStream,_scale);

  GEOM_POVPRINT_END(__geomStream,scaled);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);
  GEOM_POVPRINT_BEGIN(__geomStream,"sphere",sphere);

   __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,Vector3::ORIGIN);
  __geomStream << ", " << sphere->getRadius() << endl;

  GEOM_POVPRINT_TEXTURE;

  GEOM_POVPRINT_END(__geomStream,sphere);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);

  Transformation3DPtr _taper(tapered->getTransformation());
  GEOM_ASSERT(_taper);

  tapered->getPrimitive()->apply(__tesselator);
  TriangleSetPtr _triangleSet = __tesselator.getTriangulation();
  if (_triangleSet) {
    TriangleSetPtr _tTriangleSet;
    _tTriangleSet.cast(_triangleSet->transform(_taper));
    _tTriangleSet->apply(*this);
  }
  else
    {
      GEOM_POVPRINT_BEGIN(__geomStream,"cone",tapered);

      __geomStream << __indent;
      GEOM_POVPRINT_VECTOR3(__geomStream,Vector3::ORIGIN);
      __geomStream << " " << tapered->getBaseRadius() << endl;

      Vector3 _out(0,0,1);
      __geomStream << __indent;
      GEOM_POVPRINT_VECTOR3(__geomStream,_out);
      __geomStream << " " << tapered->getTopRadius() << endl;

      GEOM_POVPRINT_END(__geomStream,tapered);
    }

  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Translated * translated ) {
  GEOM_ASSERT(translated);
  GEOM_POVPRINT_BEGIN(__geomStream,"object",translated);

  translated->getGeometry()->apply(*this);

  const Vector3& _translation = translated->getTranslation();
  GEOM_POVPRINT_TRANSLATE(__geomStream,_translation);

  GEOM_POVPRINT_END(__geomStream,translated);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);

  GEOM_POVPRINT_BEGIN(__geomStream,"mesh",triangleSet);

  triangleSet->checkNormalList();
  bool normalV = triangleSet->getNormalPerVertex();
  for (uint32_t _i = 0; _i < triangleSet->getIndexListSize(); _i++) {
	  if(normalV){
		  __geomStream << __indent << "smooth_triangle { ";
		  const Vector3& _vertex1 = triangleSet->getPointAt(_i,0);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_vertex1);
		  __geomStream << ", ";
		  const Vector3& _normal1 = triangleSet->getNormalAt(_i,0);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_normal1);
		  __geomStream << ", ";
		  const Vector3& _vertex2 = triangleSet->getPointAt(_i,1);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_vertex2);
		  __geomStream << ", ";
		  const Vector3& _normal2 = triangleSet->getNormalAt(_i,1);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_normal2);
		  __geomStream << ", ";
		  const Vector3& _vertex3 = triangleSet->getPointAt(_i,2);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_vertex3);
		  __geomStream << ", ";
		  const Vector3& _normal3 = triangleSet->getNormalAt(_i,2);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_normal3);
	  }
	  else {
		  __geomStream << __indent << "triangle { ";
		  const Vector3& _vertex1 = triangleSet->getPointAt(_i,0);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_vertex1);
		  __geomStream << ", ";
		  const Vector3& _vertex2 = triangleSet->getPointAt(_i,1);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_vertex2);
		  __geomStream << ", ";
		  const Vector3& _vertex3 = triangleSet->getPointAt(_i,2);
		  GEOM_POVPRINT_VECTOR3(__geomStream,_vertex3);
	  }
	  if (__tesselator.texCoordComputed() && triangleSet->getTexCoordList())
	  {
		  __geomStream << " uv_vectors ";
		  const Vector2& _vertex1 = triangleSet->getTexCoordAt(_i,0);
		  GEOM_POVPRINT_VECTOR2(__geomStream,_vertex1);
		  __geomStream << ", ";
		  const Vector2& _vertex2 = triangleSet->getTexCoordAt(_i,1);
		  GEOM_POVPRINT_VECTOR2(__geomStream,_vertex2);
		  __geomStream << ", ";
		  const Vector2& _vertex3 = triangleSet->getTexCoordAt(_i,2);
		  GEOM_POVPRINT_VECTOR2(__geomStream,_vertex3);
	  }
    __geomStream << "}" << endl;
  };
  GEOM_POVPRINT_TEXTURE;

  GEOM_POVPRINT_END(__geomStream, triangleSet);

  return true;
}


/* ------------------------------------------------------------------------- */


bool PovPrinter::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

  GEOM_POVPRINT_BEGIN_DIS(__geomStream,bezierCurve)
  Discretizer d;
  bool res = bezierCurve->apply(d); 
  if (d.getDiscretization()) res = d.getDiscretization()->apply(*this); 
  return res; 
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Disc * disc ) {
  GEOM_ASSERT(disc);
  GEOM_POVPRINT_BEGIN(__geomStream,"object", disc);

  GEOM_POVPRINT_BEG_(__geomStream,"cylinder");

  __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,Vector3::ORIGIN);
  __geomStream << endl;

  __geomStream << __indent;
  GEOM_POVPRINT_VECTOR3(__geomStream,Vector3(0,0,0.01f));
  __geomStream << endl;

  __geomStream << __indent << disc->getRadius() << endl;

  GEOM_POVPRINT_TEXTURE;

  GEOM_POVPRINT_END_(__geomStream);

  __geomStream << __indent << "rotate <90,0,0>" << endl;

  GEOM_POVPRINT_END(__geomStream, disc);
  return true;
}


/* ----------------------------------------------------------------------- */


bool PovPrinter::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  GEOM_POVPRINT_BEGIN_DIS(__geomStream,nurbsCurve)
  Discretizer d;
  bool res = nurbsCurve->apply(d); 
  if (d.getDiscretization()) res = d.getDiscretization()->apply(*this); 
  return res; 
}

/* ----------------------------------------------------------------------- */


bool PovPrinter::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);

  if(!__pointcache){
	__geomStream << "#ifndef (PointWidth)" << endl;
	__geomStream << "#declare PointWidth = " << __linewidth << ';' << endl;
	__geomStream << "#end // PointWidth" << endl;
	__pointcache = true;
  }

  GEOM_POVPRINT_BEGIN(__geomStream,"union",pointSet);
  Vector3 _vertex1;
  for (Point2Array::const_iterator _i = pointSet->getPointList()->getBegin();
	_i != pointSet->getPointList()->getEnd(); _i++) {
    _vertex1 = Vector3(*_i,0);
 	GEOM_POVPRINT_BEG_(__geomStream,"sphere");
	__geomStream << __indent;
    GEOM_POVPRINT_VECTOR3(__geomStream,_vertex1);
    __geomStream << ", PointWidth" << endl;
	GEOM_POVPRINT_TEXTURE;
	GEOM_POVPRINT_END_(__geomStream);
  };
  // nothing to do
  GEOM_POVPRINT_END(__geomStream,pointSet);

  return true;
}

/* ----------------------------------------------------------------------- */


bool PovPrinter::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do
  return true;
}

/* ----------------------------------------------------------------------- */

bool PovPrinter::process( Text * text ) {
  GEOM_ASSERT(text);
  GEOM_ASSERT(text);
  GEOM_POVPRINT_BEGIN(__geomStream,"text",text);
  __geomStream << __indent << "ttf \"crystal.ttf\"," << endl;
  __geomStream << __indent << '"' << text->getString() << '"' << endl;
  __geomStream << __indent << "2, 0" << endl;
  GEOM_POVPRINT_END(__geomStream, text);
  return true;
}

/* ----------------------------------------------------------------------- */

bool PovPrinter::process( Font * font ) {
  GEOM_ASSERT(font);
  // nothing to do
  return true;
}

/* ----------------------------------------------------------------------- */
