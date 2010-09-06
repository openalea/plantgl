#include "pyprinter.h"

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_container.h>
#include <plantgl/scenegraph/geometry/profile.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/dirnames.h>


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

template<class T>
inline string compute_name(T * obj) {  
	if(obj->isNamed()) return obj->getName();
	
	string _name;
	_name = "PGL_"+number(obj->getId());
	if(obj->use_count() > 1){
		obj->setName(_name);
	}
	return _name;

}

template<class T>
inline string compute_name(RCPtr<T> obj)
{ return compute_name(obj.get()); }

/* ----------------------------------------------------------------------- */

inline std::string _pgltype(const std::string& pglnamespace, const std::string& pgltypename)
{
	if (pglnamespace.empty()) return pgltypename;
	else return pglnamespace+"."+pgltypename;
}

inline std::string PyPrinter::pgltype(const std::string& pgltypename) const
{
	return _pgltype(__pglnamespace,pgltypename);
}

/* ----------------------------------------------------------------------- */

inline ostream& print_value(ostream& os, const bool& value, const std::string& pglnamespace)
{
	return os << (value ? "True" : "False");
}

inline  ostream& print_value(ostream& os, const int_t& value, const std::string& pglnamespace)
{
	return os << value;
}

inline ostream& print_value(ostream& os, const uint_t& value, const std::string& pglnamespace)
{
	return os << value;
}

inline ostream& print_value(ostream& os, const real_t& value, const std::string& pglnamespace)
{
	return os << value;
} 

inline ostream& print_value(ostream& os, const string& value, const std::string& pglnamespace)
{
	return os << "\"" << value << "\"";
} 

inline ostream& print_value(ostream& os, const Vector2& value, const std::string& pglnamespace)
{
	return os << "(" << value.x() << ", " << value.y() <<  ")";
} 

inline ostream& print_value(ostream& os, const Vector3& value, const std::string& pglnamespace)
{
	return os << "(" << value.x() << ", " << value.y() << ", " << value.z() << ")";
} 

inline ostream& print_value(ostream& os, const Vector4& value, const std::string& pglnamespace)
{
	return os << "(" << value.x() << ", " << value.y() << ", " << value.z() << ", " << value.w() << ")";
} 

inline ostream& print_value(ostream& os, const Color3& value, const std::string& pglnamespace)
{	
	os << "(" << (uint16_t)value.getRed() 
         << "," << (uint16_t)value.getGreen() 
         << "," << (uint16_t)value.getBlue() << ")";
	
	return os;
} 

inline ostream& print_value(ostream& os, const Color4& value, const std::string& pglnamespace)
{	
	os << "(" << (uint16_t)value.getRed() 
         << "," << (uint16_t)value.getGreen() 
         << "," << (uint16_t)value.getBlue()
         << "," << (uint16_t)value.getAlpha() << ")";
	
	return os;
} 

inline ostream& print_value(ostream& os, const Index3& value, const std::string& pglnamespace)
{	
	os << "(" << value[0]  << "," << value[1]  << "," << value[2] << ")";
	return os;
} 

inline ostream& print_value(ostream& os, const Index4& value, const std::string& pglnamespace)
{	
	os << "(" << value[0]  << "," << value[1]  << "," << value[2] << "," << value[3] << ")";
	return os;
} 

ostream& print_value(ostream& os, const Curve2DArrayPtr& value, const std::string& pglnamespace)
{
	os << "[";
	if (!value->empty()){
		uint_t _sizei = value->size();
		os << compute_name(value->getAt(0));
		for (uint_t _i = 1; _i < _sizei; _i++) {
			os << ", " << compute_name(value->getAt(_i));
		}
	}
	os << "]";
	return os;
}

ostream& print_value(ostream& os, const GeometryArrayPtr& value, const std::string& pglnamespace)
{
	os << "[";
	if (!value->empty()){
		uint_t _sizei = value->size();
		os << compute_name(value->getAt(0));
		for (uint_t _i = 1; _i < _sizei; _i++) {
			os << ", " << compute_name(value->getAt(_i));
		}
	}
	os << "]";
	return os;
}

template<class array>
ostream& print_value_array(ostream& os, const array& value, const std::string& pglnamespace)
{
	os << "([";
	if (!value->empty()){
		uint_t _sizei = value->size();
		print_value(os,value->getAt(0),pglnamespace);
		for (uint_t _i = 1; _i < _sizei; _i++) {
			os << ','; print_value(os,value->getAt(_i),pglnamespace);
		}
	}
	os << "])";
	return os;
}

ostream& print_value(ostream& os, const Index3ArrayPtr& value, const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"Index3Array");
	print_value_array(os,value,pglnamespace);
	return os;
}

ostream& print_value(ostream& os, const Index4ArrayPtr& value, const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"Index4Array");
	print_value_array(os,value,pglnamespace);
	return os;
}

ostream& print_value(ostream& os, const RealArrayPtr& value, const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"RealArray");
	print_value_array(os,value,pglnamespace);
	return os;
}

ostream& print_value(ostream& os, const Color4ArrayPtr& value, const std::string& pglnamespace)
{	
	os << _pgltype(pglnamespace,"Color4Array");
	print_value_array(os,value,pglnamespace);
	return os;
} 


ostream& print_value(ostream& os, const Point2ArrayPtr& value, const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"Point2Array");
	print_value_array(os,value,pglnamespace);
	return os;
}

ostream& print_value(ostream& os, const Point3ArrayPtr& value, const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"Point3Array");
	print_value_array(os,value,pglnamespace);
	return os;
}

ostream& print_value(ostream& os, const Point4ArrayPtr& value, const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"Point4Array");
	print_value_array(os,value,pglnamespace);
	return os;
}

template<class array>
ostream& print_value_array2(ostream& os, const array& value, const std::string& pglnamespace)
{
	os << "([[";
	if (!value->empty()){
		uint_t _sizei = value->size();
		uint_t _cols = value->getColsNb();
		for (uint_t _i = 0; _i < _sizei; _i++) {
			print_value(os,value->getAt(_i / _cols ,_i % _cols),pglnamespace) ;
			if (_i != (_sizei - 1)){ 
				if (_i !=0 && (_i+1) % (_cols) ==0){ 
					os << "], [";
				} 
				else  os << ", "; 
			}
		}
	}
	os << "]])";
	return os;
}


ostream& print_value(ostream& os, const RealArray2Ptr& value,const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"RealArray2");
	print_value_array2(os,value,pglnamespace);
	return os;
}


ostream& print_value(ostream& os, const Point4MatrixPtr& value,const std::string& pglnamespace)
{
	os << _pgltype(pglnamespace,"Point4Matrix");
	print_value_array2(os,value,pglnamespace);
	return os;
}


inline ostream& print_value(ostream& os, SceneObjectPtr value,const std::string& pglnamespace)
{
	return os << compute_name(value);
}

inline ostream& print_value(ostream& os, GeometryPtr value,const std::string& pglnamespace)
{
	return os << compute_name(value);
}

inline ostream& print_value(ostream& os, AppearancePtr value,const std::string& pglnamespace)
{
	return os << compute_name(value);
}

inline ostream& print_value(ostream& os, PolylinePtr value,const std::string& pglnamespace)
{
	return os << compute_name(value);
}

/* ----------------------------------------------------------------------- */


template <typename T>
ostream& PyPrinter::print_field(ostream& os, const string& name, const string& field, const T& value, bool newline)
{
	if(newline) os << __indentation; 
	os << name << '.' << field << " = ";
	print_value(os,value,__pglnamespace);
	if(newline) os << endl;
	return os;
}

template <typename T>
ostream& PyPrinter::print_arg_field(ostream& os, const string& field, const T& value, bool newline)
{
	if(newline) os << __indentation; 
	os << field << " = ";
	print_value(os,value,__pglnamespace);
	os << " , ";
	if(newline) os << endl;
	return os;
}


template <typename T>
ostream& PyPrinter::print_unamed_arg_field(ostream& os, const T& value, bool newline)
{
	if(newline) os << __indentation; 
	print_value(os,value,__pglnamespace);
	os << " , ";
	if(newline) os << endl;
	return os;
}

template <typename T>
ostream& PyPrinter::print_field(ostream& os, const string& name, const string& str, const T& value, bool in_constructor, bool newline)
{
	if (in_constructor) return print_arg_field(os,str,value);
	else return print_field(os,name, str,value);
}


/* ----------------------------------------------------------------------- */


inline void PyPrinter::print_constructor_begin(ostream& os, const string& name, const string& type, bool newline)
{
	os << __indentation; 
	os << name << " = " << pgltype(type) << "(" ;
	if(newline) os << __indentation << endl; 
	// increment tabulation
	incrementIndentation();
}

inline void PyPrinter::print_constructor_end(ostream& os, 
											 SceneObjectPtr obj, 
											 const string& name, 
											 bool newline)
{
	if(newline) os << __indentation; 
	os << ")" ;
	os << endl;
	// decrement tabulation
	decrementIndentation();
	// if obj is named, set property name to this value.
	if ( obj->isNamed())
		print_field (os, name, "name", obj->getName());
}

inline void PyPrinter::print_object_end(ostream& os)
{
	for (size_t i = 0; i < __line_between_object; ++i)
		os << __indentation << endl;
}


/* ----------------------------------------------------------------------- */

#define GEOM_BEGIN(obj) \
	GEOM_ASSERT(obj) \
    if (obj->use_count() > 1) { \
		if (! __cache.insert(obj->getId()).second) return true; \
	} \

/* ----------------------------------------------------------------------- */

PyPrinter::PyPrinter(ostream& stream) : Printer(stream), scene_name(), __indentation_increment("    "), __pglnamespace(), __line_between_object(1) 
{
}

PyPrinter::~PyPrinter()
{
}


bool PyPrinter::beginProcess()
{
	scene_name = "result";
	__shapeStream << __indentation << "def create_scene():";
	incrementIndentation();
	__shapeStream << __indentation << "result = Scene()\n";
	return true;
}

bool PyPrinter::endProcess()
{
	__shapeStream << __indentation << "return result\n";
	decrementIndentation();
	__shapeStream << __indentation << "result = create_scene()\n";
	scene_name.clear();
	return true;
}

/* ----------------------------------------------------------------------- */

#define PYPRINT_ARG(stream,obj, name, pyattribute, cattribute,in_constructor) \
  if (! obj->is##cattribute##ToDefault()){ \
	print_field (stream, name, #pyattribute, obj->get##cattribute(),in_constructor, true); \
  } \
  else if (in_constructor) { \
	print_constructor_end (stream, obj, name); \
	in_constructor = false; \
  } \

#define PYPRINT_ARG_WITHCOND(stream,obj, name, pyattribute, cattribute, cond, in_constructor) \
  if (cond){ \
	print_field (stream, name, #pyattribute, obj->get##cattribute(),in_constructor,true); \
  } \
  else if (in_constructor) { \
	print_constructor_end (stream, obj, name); \
	in_constructor = false; \
  } \

#define PYPRINT_NAMEDARG(stream,obj, name, pyattribute, cattribute,newline) \
  if (! obj->is##cattribute##ToDefault()){ \
	print_field (stream, name, #pyattribute, obj->get##cattribute(),true,newline); \
  } \

/* ----------------------------------------------------------------------- */


bool PyPrinter::process(Shape * shape)
{
	GEOM_ASSERT(shape);

	string name = compute_name(shape);

	if( shape->geometry ){
		shape->geometry->apply(*this);
	}

	if(shape->appearance){
		shape->appearance->apply(*this);
	}

	bool in_constructor = true; // tell if the constructor of the object is still described.

	print_constructor_begin(__shapeStream, name, "Shape");

	PYPRINT_ARG_WITHCOND(__shapeStream, shape, name, geometry,  Geometry,  is_valid_ptr(shape->getGeometry()), in_constructor)
	PYPRINT_ARG_WITHCOND(__shapeStream, shape, name, appearance,  Appearance,  is_valid_ptr(shape->getAppearance()), in_constructor)
	PYPRINT_ARG_WITHCOND(__shapeStream, shape, name, id, Id, shape->getId() !=  Shape::NOID, in_constructor)
	PYPRINT_ARG_WITHCOND(__shapeStream, shape, name, parentId, ParentId, shape->getParentId() !=  Shape::NOID, in_constructor)

	if (in_constructor) print_constructor_end (__shapeStream, shape, name);

	if (!scene_name.empty()) __shapeStream  << scene_name << ".add(" << name << ")" << std::endl;


	return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Material * material ) {
  GEOM_BEGIN(material);
  
  string name = compute_name(material);
  bool in_constructor = true; // tell if the constructor of the object is still described.

  print_constructor_begin(__matStream, name, "Material", false);

  print_unamed_arg_field(__matStream, name, false);
  PYPRINT_NAMEDARG(__matStream, material, name, ambient,  Ambient, false)
  PYPRINT_NAMEDARG(__matStream, material, name, diffuse,  Diffuse, false)
  PYPRINT_NAMEDARG(__matStream, material, name, specular, Specular, false)
  PYPRINT_NAMEDARG(__matStream, material, name, emission, Emission, false)
  PYPRINT_NAMEDARG(__matStream, material, name, shininess, Shininess, false)
  PYPRINT_NAMEDARG(__matStream, material, name, transparency, Transparency, false)

  print_constructor_end (__matStream, material, name, false);
  
  print_object_end(__matStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Texture2D * texture ) {
  GEOM_BEGIN(texture);
  
  if(texture->getImage())
	  texture->getImage()->apply(*this);

  if(texture->getTransformation())
	  texture->getTransformation()->apply(*this);

  string name = compute_name(texture);
  print_constructor_begin(__matStream, name, "Texture2D", false);
	if(texture->getImage())
		print_arg_field (__matStream, "image", SceneObjectPtr(texture->getImage()),false);
    if(texture->getTransformation())
		print_arg_field (__matStream, "transformation", SceneObjectPtr(texture->getTransformation()),false);
  print_constructor_end (__matStream, texture, name, false);

  print_object_end(__matStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( ImageTexture * texture ) {
  GEOM_BEGIN(texture);
  
  bool in_constructor = true; // tell if the constructor of the object is still described.
  string name = compute_name(texture);

  std::string filename = getfilename(texture->getFilename());

  print_constructor_begin(__matStream, name, "ImageTexture", false);
    print_unamed_arg_field(__matStream, name, false);
    print_unamed_arg_field(__matStream, filename, false);
	PYPRINT_NAMEDARG(__matStream, texture, name, mipmaping,  Mipmaping, false)
	PYPRINT_NAMEDARG(__matStream, texture, name, repeatS,  RepeatS,  false)
	PYPRINT_NAMEDARG(__matStream, texture, name, repeatT,  RepeatT,  false)
	PYPRINT_NAMEDARG(__matStream, texture, name, transparency,  Transparency,  false)
  print_constructor_end (__matStream, texture, name, false);

  print_object_end(__matStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Texture2DTransformation * texturetransfo ) {
  GEOM_BEGIN(texturetransfo);
  
  bool in_constructor = true; // tell if the constructor of the object is still described.
  string name = compute_name(texturetransfo);

  print_constructor_begin(__matStream, name, "TextureTransformation",false);
    print_unamed_arg_field(__matStream, name, false);
	PYPRINT_NAMEDARG(__matStream, texturetransfo, name, scale,  Scale,  false)
	PYPRINT_NAMEDARG(__matStream, texturetransfo, name, translation,  Translation,  false)
	PYPRINT_NAMEDARG(__matStream, texturetransfo, name, rotationCenter,  RotationCenter,  false)
	PYPRINT_NAMEDARG(__matStream, texturetransfo, name, rotationAngle,  RotationAngle,  false)
  print_constructor_end (__matStream, texturetransfo, name, false);

  print_object_end(__matStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( AsymmetricHull * asymmetricHull ) {
  GEOM_BEGIN(asymmetricHull);

  string name = compute_name(asymmetricHull);
  bool in_constructor = true; // tell if the constructor of the object is still described.

  print_constructor_begin(__geomStream, name, "AsymmetricHull");

  PYPRINT_ARG(__geomStream, asymmetricHull, name, negXRadius,  NegXRadius,  in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, posXRadius,  PosXRadius,  in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, negYRadius, NegYRadius, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, posYRadius, PosYRadius, in_constructor)

  PYPRINT_ARG(__geomStream, asymmetricHull, name, negXHeight, NegXHeight, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, posXHeight, PosXHeight, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, negYHeight, NegYHeight, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, posYHeight, PosYHeight, in_constructor)

  PYPRINT_ARG(__geomStream, asymmetricHull, name, bottom, Bottom, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, top, Top, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, bottomShape, BottomShape, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, topShape, TopShape, in_constructor)

  PYPRINT_ARG(__geomStream, asymmetricHull, name, slices, Slices, in_constructor)
  PYPRINT_ARG(__geomStream, asymmetricHull, name, stacks, Stacks, in_constructor)

  if (in_constructor) print_constructor_end (__geomStream, asymmetricHull, name);

  print_object_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( AxisRotated * axisRotated ) {
  GEOM_BEGIN(axisRotated);
  
  string name = compute_name(axisRotated);

  GeometryPtr obj = axisRotated->getGeometry();
  obj->apply(*this);


  print_constructor_begin(__geomStream, name, "AxisRotated");
  print_arg_field (__geomStream, "axis", axisRotated->getAxis());
  print_arg_field (__geomStream, "angle", axisRotated->getAngle());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));
  print_constructor_end(__geomStream, axisRotated, name);
  
  
  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */



bool PyPrinter::process( BezierCurve * bezierCurve ) {
  GEOM_BEGIN(bezierCurve);
  
  string name = compute_name(bezierCurve);
  print_constructor_begin(__geomStream, name, "BezierCurve");
  print_unamed_arg_field (__geomStream, bezierCurve->getCtrlPointList());
  if (! bezierCurve->isStrideToDefault())
	  print_arg_field (__geomStream, "stride", bezierCurve->getStride());

  print_constructor_end(__geomStream, bezierCurve, name);
  print_field (__geomStream, name, "degree", bezierCurve->getDegree());
  print_object_end(__geomStream);

  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( BezierPatch * bezierPatch ) {
  GEOM_BEGIN(bezierPatch);
  
  string name = compute_name(bezierPatch);
  print_constructor_begin(__geomStream, name, "BezierPatch");

  print_unamed_arg_field (__geomStream, bezierPatch->getCtrlPointMatrix());

  print_constructor_end(__geomStream, bezierPatch, name);
  print_object_end(__geomStream);
  
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Box * box )
{
  GEOM_BEGIN(box);

  string name = compute_name(box);
  print_constructor_begin(__geomStream, name, "Box");

  if (! box->isSizeToDefault())
    print_arg_field (__geomStream, "size", box->getSize());

  print_constructor_end(__geomStream, box, name);

  print_object_end(__geomStream);
  
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Cone * cone ) {
  GEOM_BEGIN(cone);
  
  string name = compute_name(cone);
  print_constructor_begin(__geomStream, name, "Cone",false);
  if (! cone->isRadiusToDefault())
  	print_arg_field (__geomStream, "radius", cone->getRadius(),false);

  if (! cone->isHeightToDefault())
	print_arg_field (__geomStream, "height", cone->getHeight(),false);

  if (! cone->isSolidToDefault())
	print_arg_field (__geomStream, "solid", cone->getSolid(),false);

  if (! cone->isSlicesToDefault())
	print_arg_field (__geomStream, "slices", cone->getSlices(),false);
  
  print_constructor_end (__geomStream, cone, name,false);
  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Cylinder * cylinder ) {
  GEOM_BEGIN(cylinder);

  string name = compute_name(cylinder);
  print_constructor_begin(__geomStream, name, "Cylinder",false);
  if (! cylinder->isRadiusToDefault())
	print_arg_field (__geomStream, "radius", cylinder->getRadius(),false);

  if (! cylinder->isHeightToDefault())
	print_arg_field (__geomStream, "height", cylinder->getHeight(),false);

  if (! cylinder->isSolidToDefault())
	print_arg_field (__geomStream, "solid", cylinder->getSolid(),false);

  if (! cylinder->isSlicesToDefault())
	print_arg_field (__geomStream, "slices", cylinder->getSlices(),false);

  print_constructor_end (__geomStream, cylinder, name,false);
  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( ElevationGrid * elevationGrid ) {
  GEOM_BEGIN(elevationGrid);
  
  string name = compute_name(elevationGrid);
  print_constructor_begin(__geomStream, name, "ElevationGrid");
  
  print_arg_field (__geomStream, "heightList", elevationGrid->getHeightList());
  
  if (! elevationGrid->isXSpacingToDefault())
	  print_arg_field(__geomStream, "xspacing", elevationGrid->getXSpacing(),false);
  if (! elevationGrid->isYSpacingToDefault())
	  print_arg_field(__geomStream, "yspacing", elevationGrid->getYSpacing(),false);
  if (! elevationGrid->isCCWToDefault())
	  print_arg_field(__geomStream, "ccw", elevationGrid->getCCW(),false);
  print_constructor_end (__geomStream, elevationGrid, name);

  print_object_end(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( EulerRotated * eulerRotated ) {
  GEOM_BEGIN(eulerRotated);
  
  string name = compute_name(eulerRotated);

  GeometryPtr obj = eulerRotated->getGeometry();
  obj->apply(*this);

  print_constructor_begin(__geomStream, name, "EulerRotated");
  print_arg_field (__geomStream, "azimuth", eulerRotated->getAzimuth());
  print_arg_field (__geomStream, "elevation", eulerRotated->getElevation());
  print_arg_field (__geomStream, "roll", eulerRotated->getRoll());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));

  print_constructor_end(__geomStream, eulerRotated, name);
  print_object_end(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */


bool PyPrinter::process( ExtrudedHull * extrudedHull ) {
  GEOM_BEGIN(extrudedHull);
  
  Curve2DPtr vertical_obj = extrudedHull->getVertical();
  Curve2DPtr horizontal_obj = extrudedHull->getHorizontal();
  vertical_obj->apply(*this);
  horizontal_obj->apply(*this);
  
  string name = compute_name(extrudedHull);
 
  print_constructor_begin(__geomStream, name, "ExtrudedHull");

  print_arg_field(__geomStream,"vertical",SceneObjectPtr(vertical_obj),false);
  print_arg_field(__geomStream,"horizontal",SceneObjectPtr(horizontal_obj),false);
  if (! extrudedHull->isCCWToDefault())
	  print_arg_field(__geomStream, "ccw",extrudedHull->getCCW(),false);

  print_constructor_end(__geomStream, extrudedHull, name);
  
  print_object_end(__geomStream);

  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Group * group  ) {
  GEOM_BEGIN(group);

  string name = compute_name(group);

  GeometryArrayPtr obj = group->getGeometryList();
  obj->apply(*this);

  if (! group->isSkeletonToDefault())
	group->getSkeleton()->apply(*this);

  print_constructor_begin(__geomStream, name, "Group");
  print_arg_field (__geomStream, "geometryList", group->getGeometryList());
  if (! group->isSkeletonToDefault())
    print_arg_field(__geomStream, "skeleton", group->getSkeleton());
  print_constructor_end(__geomStream, group, name);
  print_object_end(__geomStream);


  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( NurbsCurve * nurbsCurve ) {
  GEOM_BEGIN(nurbsCurve);
  
  string name = compute_name(nurbsCurve);

  print_constructor_begin(__geomStream, name, "NurbsCurve");
  print_arg_field (__geomStream, "ctrlPointList", nurbsCurve->getCtrlPointList());

  if (! nurbsCurve->isKnotListToDefault())
	  print_arg_field (__geomStream, "knotList", nurbsCurve->getKnotList());

  if (! nurbsCurve->isDegreeToDefault())
	  print_arg_field (__geomStream, "degree", nurbsCurve->getDegree(),false);

  if (! nurbsCurve->isStrideToDefault())
	  print_arg_field (__geomStream, "strides", nurbsCurve->getStride(),false);

  print_constructor_end(__geomStream, nurbsCurve, name);
  
  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( NurbsPatch * nurbsPatch ) {
  GEOM_BEGIN(nurbsPatch);

  string name = compute_name(nurbsPatch);
  print_constructor_begin(__geomStream, name, "NurbsPatch");
  
    print_unamed_arg_field (__geomStream, nurbsPatch->getCtrlPointMatrix()); 
	if (! nurbsPatch->isVKnotListToDefault())
		print_arg_field (__geomStream, "uknotList", nurbsPatch->getUKnotList()); 
	if (! nurbsPatch->isVKnotListToDefault())
		print_arg_field (__geomStream, "vknotList", nurbsPatch->getVKnotList()); 
	if (! nurbsPatch->isUDegreeToDefault())
		print_arg_field (__geomStream, "udegree", nurbsPatch->getUDegree(),false);
	if (! nurbsPatch->isVDegreeToDefault())
		print_arg_field (__geomStream, "vdegree", nurbsPatch->getVDegree(),false);
	if (! nurbsPatch->isUStrideToDefault())
		print_arg_field (__geomStream, "ustride", nurbsPatch->getUStride(),false);
	if (! nurbsPatch->isVStrideToDefault())
		print_arg_field (__geomStream, "vstride", nurbsPatch->getVStride(),false);
	if (! nurbsPatch->isCCWToDefault())
		print_arg_field (__geomStream, "ccw", nurbsPatch->getCCW(),false);
	print_constructor_end(__geomStream, nurbsPatch, name);

  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( PointSet * pointSet ) {
  GEOM_BEGIN(pointSet);

  string name = compute_name(pointSet);
  print_constructor_begin(__geomStream, name, "PointSet");
  print_arg_field (__geomStream, "pointList", pointSet->getPointList());
  if(! pointSet->isColorListToDefault())
	  print_arg_field (__geomStream, "colorList", pointSet->getColorList());
  print_constructor_end(__geomStream, pointSet, name);
  
  print_object_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Polyline * polyline ) {
  GEOM_BEGIN(polyline);
  
  string name = compute_name(polyline);
  print_constructor_begin(__geomStream, name, "Polyline");
  print_arg_field (__geomStream, "pointList", polyline->getPointList());
  if(! polyline->isColorListToDefault())
	  print_arg_field (__geomStream, "colorList", polyline->getColorList());
  print_constructor_end(__geomStream, polyline, name);
  
  print_object_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process(Sphere * sphere)
{
  GEOM_BEGIN(sphere);
  
  string name = compute_name(sphere);
  print_constructor_begin(__geomStream, name, "Sphere",false);

  if (! sphere->isRadiusToDefault())
  	print_arg_field (__geomStream, "radius", sphere->getRadius(),false);  

  if (! sphere->isSlicesToDefault())
	print_arg_field (__geomStream, "slices", sphere->getSlices(),false);

  if (! sphere->isStacksToDefault())
	print_arg_field (__geomStream, "stacks", sphere->getStacks(),false);
  
  print_constructor_end(__geomStream, sphere, name,false);
  print_object_end(__geomStream);

  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Scaled * scaled ) {
  GEOM_BEGIN(scaled);

  string name = compute_name(scaled);

  GeometryPtr obj = scaled->getGeometry();
  obj->apply(*this);

  print_constructor_begin(__geomStream, name, "Scaled");
  print_arg_field (__geomStream, "scale", scaled->getScale());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));

  print_constructor_end(__geomStream, scaled, name);

  print_object_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Swung * swung )
{
  GEOM_BEGIN(swung);
  
  Curve2DArrayPtr obj = swung->getProfileList();
  obj->apply(*this);

  string name = compute_name(swung);

  print_constructor_begin(__geomStream, name, "Swung");

  print_arg_field (__geomStream, "profileList", swung->getProfileList());
  print_arg_field (__geomStream, "angleList", swung->getAngleList());

  if (! swung->isSlicesToDefault())
  	print_arg_field (__geomStream, "slices", swung->getSlices(),false);

  if(! swung->isCCWToDefault() )
	print_arg_field (__geomStream, "ccw", swung->getCCW(),false);

  if (! swung->isDegreeToDefault())
	print_arg_field (__geomStream, "degree", swung->getDegree(),false);

  if (! swung->isStrideToDefault())
	print_arg_field (__geomStream, "stride", swung->getStride(),false);

  print_constructor_end(__geomStream, swung, name);
  print_object_end(__geomStream);
  return true;
}



/* ----------------------------------------------------------------------- */

bool PyPrinter::process( QuadSet * quadSet ) {
  GEOM_BEGIN(quadSet);

  if (! quadSet->isSkeletonToDefault())
	quadSet->getSkeleton()->apply(*this);
  
  string name = compute_name(quadSet);
  
  print_constructor_begin(__geomStream, name, "QuadSet");
  print_arg_field (__geomStream, "pointList", quadSet->getPointList());
  print_arg_field (__geomStream, "indexList", quadSet->getIndexList());

  print_constructor_end(__geomStream, quadSet, name);
  if (!quadSet->isNormalPerVertexToDefault())
	print_field (__geomStream, name, "normalPerVertex", quadSet->getNormalPerVertex());
  if (!quadSet->isNormalListToDefault())
    print_field (__geomStream, name, "normalList", quadSet->getNormalList());
  if (!quadSet->isNormalIndexListToDefault())
    print_field (__geomStream, name, "normalIndexList", quadSet->getNormalIndexList());
  if (!quadSet->isColorPerVertexToDefault())
	print_field (__geomStream, name, "colorPerVertex", quadSet->getColorPerVertex());
  if (!quadSet->isColorListToDefault())
	print_field (__geomStream, name, "colorList", quadSet->getColorList());
  if (!quadSet->isColorIndexListToDefault())
	print_field (__geomStream, name, "colorIndexList", quadSet->getColorIndexList());
  if (! quadSet->isTexCoordListToDefault())
    print_field (__geomStream, name, "texCoordList", quadSet->getTexCoordList());
  if (! quadSet->isTexCoordIndexListToDefault())
    print_field (__geomStream, name, "texCoordIndexList", quadSet->getTexCoordIndexList()); 
  if (! quadSet->isCCWToDefault())
    print_field (__geomStream, name, "ccw", quadSet->getCCW());
  if (! quadSet->isSolidToDefault())
    print_field (__geomStream, name, "solid", quadSet->getSolid());
  if (! quadSet->isSkeletonToDefault())
    print_field (__geomStream, name, "skeleton", SceneObjectPtr(quadSet->getSkeleton()));
	
  print_object_end(__geomStream);
  return true;  
}



/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Translated * translated ) {
  GEOM_BEGIN(translated);
  
  string name = compute_name(translated);

  GeometryPtr obj = translated->getGeometry();
  obj->apply(*this);

  // string geometryname = compute_name(obj);
  
  print_constructor_begin(__geomStream, name, "Translated");
  print_arg_field (__geomStream, "translation", translated->getTranslation());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));

  print_constructor_end(__geomStream, translated, name);


  print_object_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( TriangleSet * triangleSet ) {
  GEOM_BEGIN(triangleSet);

  if (! triangleSet->isSkeletonToDefault())
	triangleSet->getSkeleton()->apply(*this);

  string name = compute_name(triangleSet);
  
  print_constructor_begin(__geomStream, name, "TriangleSet");
  print_arg_field (__geomStream, "pointList", triangleSet->getPointList());
  print_arg_field (__geomStream, "indexList", triangleSet->getIndexList());

  print_constructor_end(__geomStream, triangleSet, name);
  if (!triangleSet->isNormalPerVertexToDefault())
	print_field (__geomStream, name, "normalPerVertex", triangleSet->getNormalPerVertex());
  if (!triangleSet->isNormalListToDefault())
    print_field (__geomStream, name, "normalList", triangleSet->getNormalList());
  if (!triangleSet->isNormalIndexListToDefault())
    print_field (__geomStream, name, "normalIndexList", triangleSet->getNormalIndexList());
  if (!triangleSet->isColorPerVertexToDefault())
	print_field (__geomStream, name, "colorPerVertex", triangleSet->getColorPerVertex());
  if (!triangleSet->isColorListToDefault())
	print_field (__geomStream, name, "colorList", triangleSet->getColorList());
  if (!triangleSet->isColorIndexListToDefault())
	print_field (__geomStream, name, "colorIndexList", triangleSet->getColorIndexList());
  if (! triangleSet->isTexCoordListToDefault())
    print_field (__geomStream, name, "texCoordList", triangleSet->getTexCoordList());
  if (! triangleSet->isTexCoordIndexListToDefault())
    print_field (__geomStream, name, "texCoordIndexList", triangleSet->getTexCoordIndexList()); 
  if (! triangleSet->isCCWToDefault())
    print_field (__geomStream, name, "ccw", triangleSet->getCCW());
  if (! triangleSet->isSolidToDefault())
    print_field (__geomStream, name, "solid", triangleSet->getSolid());
  if (! triangleSet->isSkeletonToDefault())
    print_field (__geomStream, name, "skeleton", SceneObjectPtr(triangleSet->getSkeleton()));

  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( BezierCurve2D * bezierCurve ) {
  GEOM_BEGIN(bezierCurve);

  string name = compute_name(bezierCurve);
  print_constructor_begin(__geomStream, name, "BezierCurve2D");
  print_unamed_arg_field (__geomStream, bezierCurve->getCtrlPointList());
  if (! bezierCurve->isStrideToDefault())
	  print_arg_field (__geomStream, "stride", bezierCurve->getStride());
  print_constructor_end(__geomStream, bezierCurve, name);
  print_object_end(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Disc * disc ) {
  GEOM_BEGIN(disc);
  
  string name = compute_name(disc);
  print_constructor_begin(__geomStream, name, "Disc");

  if (! disc->isRadiusToDefault())
	print_arg_field (__geomStream, "radius", disc->getRadius());
  if (! disc->isSlicesToDefault())
	print_arg_field (__geomStream, "slices", disc->getSlices());
  print_constructor_end(__geomStream, disc, name);
  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_BEGIN(nurbsCurve);

  string name = compute_name(nurbsCurve);
  print_constructor_begin(__geomStream, name, "NurbsCurve2D");
  print_arg_field (__geomStream, "ctrlPointList", nurbsCurve->getCtrlPointList());

  if (! nurbsCurve->isKnotListToDefault())
	print_arg_field (__geomStream, "knotList", nurbsCurve->getKnotList());
  if (! nurbsCurve->isDegreeToDefault())
	print_arg_field (__geomStream, "degree", nurbsCurve->getDegree());
  if (! nurbsCurve->isStrideToDefault())
	print_arg_field (__geomStream, "strides", nurbsCurve->getStride());
  print_constructor_end(__geomStream, nurbsCurve, name);
  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( PointSet2D * pointSet ) {
  GEOM_BEGIN(pointSet);

  string name = compute_name(pointSet);
  print_constructor_begin(__geomStream, name, "PointSet2D");
  print_unamed_arg_field (__geomStream, pointSet->getPointList());
  
  print_constructor_end(__geomStream, pointSet, name);
  print_object_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Polyline2D * polyline ) {
  GEOM_BEGIN(polyline);

  string name = compute_name(polyline);
  print_constructor_begin(__geomStream, name, "Polyline2D");
  print_unamed_arg_field (__geomStream, polyline->getPointList());
  
  print_constructor_end(__geomStream, polyline, name); 
  print_object_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */

std::string PyPrinter::getfilename(const std::string& filename) const {
  if (__referencedir.empty()) return filename;

  string path = short_dirname(get_dirname(filename));
  string reference_dir = absolute_filename(__referencedir);

  if(path.size() < reference_dir.size()) return filename;
  path = string(path.begin(),path.begin()+reference_dir.size());

  std::string file = filename;
  if(similar_dir(path,reference_dir)){
	    file = absolute_filename(filename);
		std::string res = std::string (file.begin()+reference_dir.size(),file.end());
		if (res[0] != '/' && res[0] != '\\') res = "./"+res;
		else res = "."+res;
		return res;

/*
        int count = 0;
		// count number of nested dir in path
        for(string::const_iterator _i = path.begin(); _i != path.end(); _i++)
			if(*_i == '\\' || *_i == '/')count++;

		// remove
	    file = absolute_filename(filename);
        string::iterator _j = file.begin();
        for(;_j != file.end() && count>0; _j++)
			if(*_j == '\\' || *_j == '/')count--;

        if(*(path.end()-1) != '\\' && *(path.end()-1) != '/'){
            while(_j != file.begin() && *_j != '\\' && *_j != '/')_j--;
        }
        file = string(_j,file.end()); */
  }
  return file;
}