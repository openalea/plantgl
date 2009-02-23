#include "pyprinter.h"

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_container.h>
#include <plantgl/scenegraph/geometry/profile.h>
#include <plantgl/tool/util_string.h>


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

template <typename T>
ostream& print_field(ostream& os, string name, string str, const T& value)
{
	os << name << '.' << str << " = ";
	print_value(os,value);
	return os << endl;
}

template <typename T>
ostream& print_arg_field(ostream& os, string str, const T& value)
{
	os << '\t' << str << " = ";
	print_value(os,value);
	os << " , ";
	return os << endl;
}

template <typename T>
ostream& print_arg_field(ostream& os, const T& value)
{
	os << '\t';
	print_value(os,value);
	os << " , ";
	return os << endl;
}

template <typename T>
ostream& print_field(ostream& os, string name, string str, const T& value, bool in_constructor)
{
	if (in_constructor) return print_arg_field(os,str,value);
	else return print_field(os,name, str,value);
}

inline ostream& print_value(ostream& os, const bool& value)
{
	return os << (value ? "True" : "False");
}

inline  ostream& print_value(ostream& os, const int_t& value)
{
	return os << value;
}

inline ostream& print_value(ostream& os, const uint_t& value)
{
	return os << value;
}

inline ostream& print_value(ostream& os, const real_t& value)
{
	return os << value;
} 

inline ostream& print_value(ostream& os, const string& value)
{
	return os << "\"" << value << "\"";
} 

inline ostream& print_value(ostream& os, const Vector2& value)
{
	return os << "Vector2(" << value.x() << ", " << value.y() <<  ")";
} 

inline ostream& print_value(ostream& os, const Vector3& value)
{
	return os << "Vector3(" << value.x() << ", " << value.y() << ", " << value.z() << ")";
} 

inline ostream& print_value(ostream& os, const Vector4& value)
{
	return os << "Vector4(" << value.x() << ", " << value.y() << ", " << value.z() << ", " << value.w() << ")";
} 

inline ostream& print_value(ostream& os, const Color3& value)
{	
	os << "Color3(" << (uint16_t)value.getRed() 
         << "," << (uint16_t)value.getGreen() 
         << "," << (uint16_t)value.getBlue() << ")";
	
	return os;
} 


ostream& print_value(ostream& os, const Color4ArrayPtr& value)
{	
	os << "Color4Array([";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _sizei; _i++) {
		Color4 c = value->getAt(_i);
		os << "(" << (uint16_t)c.getRed() 
			<< ", " << (uint16_t)c.getGreen() 
			<< ", " << (uint16_t)c.getBlue()
			<< ", " << (uint16_t)c.getAlpha() << "), ";
	}
	os << "])";
	return os;
} 

ostream& print_value(ostream& os, const Curve2DArrayPtr& value)
{
	os << "[";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _sizei; _i++) {
		os << value->getAt(_i)->getName() << ", ";
	}
	os << "]";
	return os;
}

ostream& print_value(ostream& os, const GeometryArrayPtr& value)
{
	os << "[";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _sizei; _i++) {
		os << value->getAt(_i)->getName() << ", ";
	}
	os << "]";
	return os;
}

ostream& print_value(ostream& os, const Index3ArrayPtr& value)
{
	os << "Index3Array([";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _sizei; _i++) {
		os << "(" 
			<< value->getAt(_i).getAt(0) << "," << value->getAt(_i).getAt(1) << ","
			<< value->getAt(_i).getAt(2) << "), ";
	}
	os << "])";
	return os;
}

ostream& print_value(ostream& os, const Index4ArrayPtr& value)
{
	os << "Index4Array([";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _sizei; _i++) {
		os << "(" 
			<< value->getAt(_i).getAt(0) << "," << value->getAt(_i).getAt(1) << ","
			<< value->getAt(_i).getAt(2) << "," << value->getAt(_i).getAt(3)
			<< "), ";
	}
	os << "])";
	return os;
}

ostream& print_value(ostream& os, const RealArrayPtr& value)
{
	//os << "RealArray([";
	os << "[";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _sizei; _i++) {
		os << value->getAt(_i) << ", ";
	}
	os << "]";
	return os;
}

ostream& print_value(ostream& os, const RealArray2Ptr& value)
{
	uint_t _cols = value->getColsNb();
	os << "[[";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _sizei; _i++) {
		os << value->getAt(_i / _cols ,_i % _cols) << ",";
		if (_i != (_sizei - 1)){ 
          if (_i !=0 && (_i+1) % (_cols) ==0){ 
			  os << "], ";
              os << "[";} 
          else  os << ", "; 
		}
	}
	os << "]]";
	return os;
}

ostream& print_value(ostream& os, const Point2ArrayPtr& value)
{
	os << "Point2Array([";
	uint_t _sizei = value->getSize();
        for(Point2Array::const_iterator it = value->getBegin(); it != value->getEnd(); ++it)
		print_value(os,*it) << ',';
	os << "])";
	return os;
}

ostream& print_value(ostream& os, const Point3ArrayPtr& value)
{
	os << "Point3Array([";
	uint_t _sizei = value->getSize();
        for(Point3Array::const_iterator it = value->getBegin(); it != value->getEnd(); ++it)
		print_value(os,*it) << ',';
	os << "])";
	return os;
}

ostream& print_value(ostream& os, const Point4ArrayPtr& value)
{
	os << "Point4Array([";
	uint_t _sizei = value->getSize();
        for(Point4Array::const_iterator it = value->getBegin(); it != value->getEnd(); ++it)
		print_value(os,*it) << ',';
	os << "])";
	return os;
}

ostream& print_value(ostream& os, const Point4MatrixPtr& value)
{
	uint_t _cols = value->getColsNb();
	uint_t _rows = value->getRowsNb();
	os << "[";
	uint_t _sizei = value->getSize();
	for (uint_t _i = 0; _i < _rows; ++_i) {
           os << "[";
   	   for (uint_t _j = 0; _j < _cols; ++_j) 
		print_value(os,value->getAt(_i ,_j)) << ',';
 	   os << "], ";
	}
	os << "]";
	return os;
}


template<class T>
inline string compute_name(T * obj) {  
	if(obj->isNamed()) return obj->getName();
	
	if(obj->use_count() > 1){
		string _name;
		_name = "PGL_"+number(obj->getId());
		obj->setName(_name);
	}
	else return "anobject";

	return obj->getName();
}

template<class T>
inline string compute_name(RCPtr<T> obj)
{ return compute_name(obj.get()); }

inline ostream& print_value(ostream& os, SceneObjectPtr value)
{
	return os << compute_name(value);
}


inline void print_begin(ostream& os, string name, string type)
{
	os << name << " = " << type << "()" << endl;
}

inline void print_cons_begin(ostream& os, string name, string type)
{
	os << name << " = " << type << "(" << endl;
}
inline void print_cons_end(ostream& os, SceneObjectPtr obj, string name)
{
	os << ")" << endl;
	if ( obj->isNamed())
		print_field (os, name, "name", obj->getName());
}

inline void print_end(ostream& os)
{
	os << endl;
}

PyPrinter::PyPrinter(ostream& stream) : Printer(stream)
{
}

PyPrinter::~PyPrinter()
{
}

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

	print_cons_begin(__shapeStream, name, "Shape");

	if( shape->geometry ){
		print_arg_field (__shapeStream, "geometry", SceneObjectPtr(shape->geometry));
		if( shape->appearance )
		{
			print_arg_field (__shapeStream, "appearance", SceneObjectPtr(shape->appearance));
			if(shape->id != Shape::NOID)
			{
				print_arg_field (__shapeStream, "id", (shape->id));
				if(shape->parentId != Shape::NOID)
					print_arg_field (__shapeStream, "parentId", (shape->parentId));
			}
			else
			{
				if(shape->parentId != Shape::NOID)
				{
					print_cons_end(__shapeStream, shape, name);
					print_field (__shapeStream, name, "parentId", (shape->parentId));
					print_end(__shapeStream);
					return true;
				}
			}
		}
		print_cons_end(__shapeStream, shape, name);
	}
	
	else
	{
		print_cons_end(__shapeStream, shape, name);
		if( shape->appearance )
			print_field (__shapeStream, name, "appearance", SceneObjectPtr(shape->appearance));
		if(shape->id != Shape::NOID)
			print_field (__shapeStream, name, "id", (shape->id));
		if(shape->parentId != Shape::NOID)
			print_field (__shapeStream, name, "parentId", (shape->parentId));
	}
	
	print_end(__shapeStream);
	return true;
}

/* ----------------------------------------------------------------------- */

#define PYPRINT_ARG(stream,obj, name, pyattribute, cattribute,in_constructor) \
  if (! obj->is##cattribute##ToDefault()){ \
	print_field (stream, name, #pyattribute, obj->get##cattribute(),in_constructor); \
  } \
  else if (in_constructor) { \
	print_cons_end (stream, obj, name); \
	in_constructor = false; \
  } \


bool PyPrinter::process( Material * material ) {
  GEOM_ASSERT(material);
  
  string name = compute_name(material);
  bool in_constructor = true; // tell if the constructor of the object is still described.

  print_cons_begin(__matStream, name, "Material");

  PYPRINT_ARG(__matStream, material, name, ambient,  Ambient,  in_constructor)
  PYPRINT_ARG(__matStream, material, name, diffuse,  Diffuse,  in_constructor)
  PYPRINT_ARG(__matStream, material, name, specular, Specular, in_constructor)
  PYPRINT_ARG(__matStream, material, name, emission, Emission, in_constructor)
  PYPRINT_ARG(__matStream, material, name, shininess, Shininess, in_constructor)
  PYPRINT_ARG(__matStream, material, name, transparency, Transparency, in_constructor)

  if (in_constructor) print_cons_end (__matStream, material, name);
  
  print_end(__matStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  
  string name = compute_name(texture);
  print_cons_begin(__geomStream, name, "ImageTexture");
    print_arg_field(__geomStream, name, texture->getFilename());
  print_cons_end (__geomStream, texture, name);

  if (! texture->isMipmapingToDefault())
	print_field (__geomStream, name, "mipmaping", texture->getMipmaping());
  if (! texture->isAmbientToDefault())
    print_field (__geomStream, name, "ambient", texture->getAmbient());
  if (! texture->isDiffuseToDefault())
    print_field (__geomStream, name, "diffuse", texture->getDiffuse());
  if (! texture->isSpecularToDefault())
    print_field (__geomStream, name, "specular", texture->getSpecular());
  if (! texture->isEmissionToDefault())
    print_field (__geomStream, name, "emission", texture->getEmission());
  if (! texture->isShininessToDefault())
    print_field (__geomStream, name, "shininess", texture->getShininess());
  if (! texture->isTransparencyToDefault())
    print_field (__geomStream, name, "transparency", texture->getTransparency());

  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);

  string name = compute_name(asymmetricHull);
  print_cons_begin(__geomStream, name, "AsymmetricHull");
  if (! asymmetricHull->isNegXRadiusToDefault())
  {
	print_arg_field (__geomStream, "negXRadius", asymmetricHull->getNegXRadius());
	if (! asymmetricHull->isPosXRadiusToDefault())
	{
		print_arg_field (__geomStream, "posXRadius", asymmetricHull->getPosXRadius());
		if (! asymmetricHull->isNegYRadiusToDefault())
		{
			print_arg_field (__geomStream, "negYRadius", asymmetricHull->getNegYRadius());
			if (! asymmetricHull->isPosYRadiusToDefault())
			{
				print_arg_field (__geomStream, "posYRadius", asymmetricHull->getPosYRadius());
				if (! asymmetricHull->isNegXHeightToDefault())
				{
					print_arg_field (__geomStream, "negXHeight", asymmetricHull->getNegXHeight());
					if (! asymmetricHull->isPosXHeightToDefault())
					{
						print_arg_field (__geomStream, "posXHeight", asymmetricHull->getPosXHeight());
						if (! asymmetricHull->isNegYHeightToDefault())
						{
							print_arg_field (__geomStream, "negYHeight", asymmetricHull->getNegYHeight());
							if (! asymmetricHull->isPosYHeightToDefault())
							{
								print_arg_field (__geomStream, "posYHeight", asymmetricHull->getPosYHeight());
								if (! asymmetricHull->isBottomToDefault())
								{
									print_arg_field (__geomStream, "bottom", asymmetricHull->getBottom());
									if (! asymmetricHull->isTopToDefault())
									{
										print_arg_field (__geomStream, "top", asymmetricHull->getTop());
										if (! asymmetricHull->isBottomShapeToDefault())
										{
											print_arg_field (__geomStream, "bottomShape", asymmetricHull->getBottomShape());
											if (! asymmetricHull->isTopShapeToDefault())
											{
												print_arg_field (__geomStream, "topShape", asymmetricHull->getTopShape());
												if (! asymmetricHull->isSlicesToDefault())
												{
													print_arg_field (__geomStream, "slices", asymmetricHull->getSlices());
													if (! asymmetricHull->isStacksToDefault())
														print_arg_field (__geomStream, "stacks", asymmetricHull->getStacks());
												}
												else  // Slices is default
												{
													print_cons_end(__geomStream, asymmetricHull, name);	
													if (! asymmetricHull->isStacksToDefault())
														print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
													print_end(__geomStream);
													return true;
												}
											}
											else  // TopShape is default
											{
												print_cons_end(__geomStream, asymmetricHull, name);	
												if (! asymmetricHull->isSlicesToDefault())
													print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
												if (! asymmetricHull->isStacksToDefault())
													print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
												print_end(__geomStream);
												return true;
											}
										}
										else  // BottomShape is default
										{
											print_cons_end(__geomStream, asymmetricHull, name);						
											if (! asymmetricHull->isTopShapeToDefault())
												print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
											if (! asymmetricHull->isSlicesToDefault())
												print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
											if (! asymmetricHull->isStacksToDefault())
												print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
											print_end(__geomStream);
											return true;
										}
									}
									else  // Top is default
									{
										print_cons_end(__geomStream, asymmetricHull, name);						
										if (! asymmetricHull->isBottomShapeToDefault())
											print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
										if (! asymmetricHull->isTopShapeToDefault())
											print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
										if (! asymmetricHull->isSlicesToDefault())
											print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
										if (! asymmetricHull->isStacksToDefault())
											print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
										print_end(__geomStream);
										return true;
									}
								}
								else  // Bottom is default
								{
									print_cons_end(__geomStream, asymmetricHull, name);
									if (! asymmetricHull->isTopToDefault())
										print_field (__geomStream, name, "top", asymmetricHull->getTop());
									if (! asymmetricHull->isBottomShapeToDefault())
										print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
									if (! asymmetricHull->isTopShapeToDefault())
										print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
									if (! asymmetricHull->isSlicesToDefault())
										print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
									if (! asymmetricHull->isStacksToDefault())
										print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
									print_end(__geomStream);
									return true;
								}
							}
							else  // PosYHeight is default
							{
								print_cons_end(__geomStream, asymmetricHull, name);
								if (! asymmetricHull->isBottomToDefault())
									print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
								if (! asymmetricHull->isTopToDefault())
									print_field (__geomStream, name, "top", asymmetricHull->getTop());
								if (! asymmetricHull->isBottomShapeToDefault())
									print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
								if (! asymmetricHull->isTopShapeToDefault())
									print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
								if (! asymmetricHull->isSlicesToDefault())
									print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
								if (! asymmetricHull->isStacksToDefault())
									print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
								print_end(__geomStream);
								return true;
							}
						}
						else  // NegYHeight is dedault
						{
							print_cons_end(__geomStream, asymmetricHull, name);
							if (! asymmetricHull->isPosYHeightToDefault())
								print_field (__geomStream, name, "posYHeight", asymmetricHull->getPosYHeight());
							if (! asymmetricHull->isBottomToDefault())
								print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
							if (! asymmetricHull->isTopToDefault())
								print_field (__geomStream, name, "top", asymmetricHull->getTop());
							if (! asymmetricHull->isBottomShapeToDefault())
								print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
							if (! asymmetricHull->isTopShapeToDefault())
								print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
							if (! asymmetricHull->isSlicesToDefault())
								print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
							if (! asymmetricHull->isStacksToDefault())
								print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
							print_end(__geomStream);
							return true;
						}
					}
					else  // PosXHeight is default
					{
						print_cons_end(__geomStream, asymmetricHull, name);
						if (! asymmetricHull->isNegYHeightToDefault())
							print_field (__geomStream, name, "negYHeight", asymmetricHull->getNegYHeight());
						if (! asymmetricHull->isPosYHeightToDefault())
							print_field (__geomStream, name, "posYHeight", asymmetricHull->getPosYHeight());
						if (! asymmetricHull->isBottomToDefault())
							print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
						if (! asymmetricHull->isTopToDefault())
							print_field (__geomStream, name, "top", asymmetricHull->getTop());
						if (! asymmetricHull->isBottomShapeToDefault())
							print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
						if (! asymmetricHull->isTopShapeToDefault())
							print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
						if (! asymmetricHull->isSlicesToDefault())
							print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
						if (! asymmetricHull->isStacksToDefault())
							print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
						print_end(__geomStream);
						return true;
					}
				}
				else  // NegXHeight is default
				{
					print_cons_end(__geomStream, asymmetricHull, name);
					if (! asymmetricHull->isPosXHeightToDefault())
						print_field (__geomStream, name, "posXHeight", asymmetricHull->getPosXHeight());
					if (! asymmetricHull->isNegYHeightToDefault())
						print_field (__geomStream, name, "negYHeight", asymmetricHull->getNegYHeight());
					if (! asymmetricHull->isPosYHeightToDefault())
						print_field (__geomStream, name, "posYHeight", asymmetricHull->getPosYHeight());
					if (! asymmetricHull->isBottomToDefault())
						print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
					if (! asymmetricHull->isTopToDefault())
						print_field (__geomStream, name, "top", asymmetricHull->getTop());
					if (! asymmetricHull->isBottomShapeToDefault())
						print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
					if (! asymmetricHull->isTopShapeToDefault())
						print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
					if (! asymmetricHull->isSlicesToDefault())
						print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
					if (! asymmetricHull->isStacksToDefault())
						print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
					print_end(__geomStream);
					return true;
				}
			}
			else  // PosYRadius is default
			{
				print_cons_end(__geomStream, asymmetricHull, name);
				if (! asymmetricHull->isNegXHeightToDefault())
					print_field (__geomStream, name, "negXHeight", asymmetricHull->getNegXHeight());
				if (! asymmetricHull->isPosXHeightToDefault())
					print_field (__geomStream, name, "posXHeight", asymmetricHull->getPosXHeight());
				if (! asymmetricHull->isNegYHeightToDefault())
					print_field (__geomStream, name, "negYHeight", asymmetricHull->getNegYHeight());
				if (! asymmetricHull->isPosYHeightToDefault())
					print_field (__geomStream, name, "posYHeight", asymmetricHull->getPosYHeight());
				if (! asymmetricHull->isBottomToDefault())
					print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
				if (! asymmetricHull->isTopToDefault())
					print_field (__geomStream, name, "top", asymmetricHull->getTop());
				if (! asymmetricHull->isBottomShapeToDefault())
					print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
				if (! asymmetricHull->isTopShapeToDefault())
					print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
				if (! asymmetricHull->isSlicesToDefault())
					print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
				if (! asymmetricHull->isStacksToDefault())
					print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
				print_end(__geomStream);
				return true;
			}
		}
		else  // NegYRadius is default
		{
			print_cons_end(__geomStream, asymmetricHull, name);
			if (! asymmetricHull->isPosYRadiusToDefault())
			    print_field (__geomStream, name, "posYRadius", asymmetricHull->getPosYRadius());
			if (! asymmetricHull->isNegXHeightToDefault())
				print_field (__geomStream, name, "negXHeight", asymmetricHull->getNegXHeight());
			if (! asymmetricHull->isPosXHeightToDefault())
				print_field (__geomStream, name, "posXHeight", asymmetricHull->getPosXHeight());
			if (! asymmetricHull->isNegYHeightToDefault())
				print_field (__geomStream, name, "negYHeight", asymmetricHull->getNegYHeight());
			if (! asymmetricHull->isPosYHeightToDefault())
				print_field (__geomStream, name, "posYHeight", asymmetricHull->getPosYHeight());
			if (! asymmetricHull->isBottomToDefault())
				print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
			if (! asymmetricHull->isTopToDefault())
				print_field (__geomStream, name, "top", asymmetricHull->getTop());
			if (! asymmetricHull->isBottomShapeToDefault())
				print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
			if (! asymmetricHull->isTopShapeToDefault())
				print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
			if (! asymmetricHull->isSlicesToDefault())
				print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
			if (! asymmetricHull->isStacksToDefault())
				print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
			print_end(__geomStream);
			return true;
		}		
	}
	else // PosXRadius is default
	{
	  print_cons_end(__geomStream, asymmetricHull, name);
	  if (! asymmetricHull->isNegYRadiusToDefault())
		  print_field (__geomStream, name, "negYRadius", asymmetricHull->getNegYRadius());
	  if (! asymmetricHull->isPosYRadiusToDefault())
		  print_field (__geomStream, name, "posYRadius", asymmetricHull->getPosYRadius());
	  if (! asymmetricHull->isNegXHeightToDefault())
		  print_field (__geomStream, name, "negXHeight", asymmetricHull->getNegXHeight());
	  if (! asymmetricHull->isPosXHeightToDefault())
		  print_field (__geomStream, name, "posXHeight", asymmetricHull->getPosXHeight());
	  if (! asymmetricHull->isNegYHeightToDefault())
		  print_field (__geomStream, name, "negYHeight", asymmetricHull->getNegYHeight());
	  if (! asymmetricHull->isPosYHeightToDefault())
		  print_field (__geomStream, name, "posYHeight", asymmetricHull->getPosYHeight());
	  if (! asymmetricHull->isBottomToDefault())
		  print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
	  if (! asymmetricHull->isTopToDefault())
		  print_field (__geomStream, name, "top", asymmetricHull->getTop());
	  if (! asymmetricHull->isBottomShapeToDefault())
		  print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
	  if (! asymmetricHull->isTopShapeToDefault())
		  print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
	  if (! asymmetricHull->isSlicesToDefault())
		  print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
	  if (! asymmetricHull->isStacksToDefault())
		  print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
	  print_end(__geomStream);
	  return true;
	}

	print_cons_end(__geomStream, asymmetricHull, name);
  }
  else // NegXRadius is default
  {
	  print_cons_end(__geomStream, asymmetricHull, name);
	  if (! asymmetricHull->isPosXRadiusToDefault())
		  print_field (__geomStream, name, "posXRadius", asymmetricHull->getPosXRadius());
	  if (! asymmetricHull->isNegYRadiusToDefault())
		  print_field (__geomStream, name, "negYRadius", asymmetricHull->getNegYRadius());
	  if (! asymmetricHull->isPosYRadiusToDefault())
		  print_field (__geomStream, name, "posYRadius", asymmetricHull->getPosYRadius());
	  if (! asymmetricHull->isNegXHeightToDefault())
		  print_field (__geomStream, name, "negXHeight", asymmetricHull->getNegXHeight());
	  if (! asymmetricHull->isPosXHeightToDefault())
		  print_field (__geomStream, name, "posXHeight", asymmetricHull->getPosXHeight());
	  if (! asymmetricHull->isNegYHeightToDefault())
		  print_field (__geomStream, name, "negYHeight", asymmetricHull->getNegYHeight());
	  if (! asymmetricHull->isPosYHeightToDefault())
		  print_field (__geomStream, name, "posYHeight", asymmetricHull->getPosYHeight());
	  if (! asymmetricHull->isBottomToDefault())
		  print_field (__geomStream, name, "bottom", asymmetricHull->getBottom());
	  if (! asymmetricHull->isTopToDefault())
		  print_field (__geomStream, name, "top", asymmetricHull->getTop());
	  if (! asymmetricHull->isBottomShapeToDefault())
		  print_field (__geomStream, name, "bottomShape", asymmetricHull->getBottomShape());
	  if (! asymmetricHull->isTopShapeToDefault())
		  print_field (__geomStream, name, "topShape", asymmetricHull->getTopShape());
	  if (! asymmetricHull->isSlicesToDefault())
		  print_field (__geomStream, name, "slices", asymmetricHull->getSlices());
	  if (! asymmetricHull->isStacksToDefault())
		  print_field (__geomStream, name, "stacks", asymmetricHull->getStacks());
  }

  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);
  
  string name = compute_name(axisRotated);

  GeometryPtr obj = axisRotated->getGeometry();
  obj->apply(*this);


  print_cons_begin(__geomStream, name, "AxisRotated");
  print_arg_field (__geomStream, "axis", axisRotated->getAxis());
  print_arg_field (__geomStream, "angle", axisRotated->getAngle());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));
  print_cons_end(__geomStream, axisRotated, name);
  
  /*
  if (! axisRotated->isAxisToDefault())
  {
	print_arg_field (__geomStream, "axis", axisRotated->getAxis());
    if (! axisRotated->isAngleToDefault())
	{
		print_arg_field (__geomStream, "angle", axisRotated->getAngle());
		print_arg_field (__geomStream, "geometry", geometryname);
	}
	else  // angle is default
	{
		print_cons_end(__geomStream, axisRotated, name);
		print_field (__geomStream, name, "geometry", geometryname);
		print_end(__geomStream);
		return true;
	}
    print_cons_end(__geomStream, axisRotated, name);
  }
  else // axis is default
  {
	print_cons_end(__geomStream, axisRotated, name);
	if (! axisRotated->isAngleToDefault())
		print_field (__geomStream, name, "angle", axisRotated->getAngle());
	print_field (__geomStream, name, "geometry", geometryname);
  }*/
  
  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */



bool PyPrinter::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  
  string name = compute_name(bezierCurve);
  print_cons_begin(__geomStream, name, "BezierCurve");
  print_arg_field (__geomStream, bezierCurve->getCtrlPointList());
  if (! bezierCurve->isStrideToDefault())
	  print_arg_field (__geomStream, "stride", bezierCurve->getStride());

  print_cons_end(__geomStream, bezierCurve, name);
  print_field (__geomStream, name, "degree", bezierCurve->getDegree());
  print_end(__geomStream);

  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);
  
  string name = compute_name(bezierPatch);
  print_cons_begin(__geomStream, name, "BezierPatch");

  print_arg_field (__geomStream, bezierPatch->getCtrlPointMatrix());

  print_cons_end(__geomStream, bezierPatch, name);
  print_end(__geomStream);
  
  /*
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
  */
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Box * box )
{
  GEOM_ASSERT(box);

  string name = compute_name(box);
  print_cons_begin(__geomStream, name, "Box");

  if (! box->isSizeToDefault())
    print_arg_field (__geomStream, "size", box->getSize());

  print_cons_end(__geomStream, box, name);

  print_end(__geomStream);
  
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Cone * cone ) {
  GEOM_ASSERT(cone);
  
  string name = compute_name(cone);
  print_cons_begin(__geomStream, name, "Cone");
  if (! cone->isRadiusToDefault())
  {
	print_arg_field (__geomStream, "radius", cone->getRadius());
	if (! cone->isHeightToDefault())
	{
		print_arg_field (__geomStream, "height", cone->getHeight());
		if (! cone->isSolidToDefault())
		{
			print_arg_field (__geomStream, "solid", cone->getSolid());
			if (! cone->isSlicesToDefault())
				print_arg_field (__geomStream, "slices", cone->getSlices());
		} 
		else // solid is default
		{
			print_cons_end (__geomStream, cone, name);
			if (! cone->isSlicesToDefault())
				print_field (__geomStream, name, "slices", cone->getSlices());
			print_end(__geomStream);
			return true;
		}
	}
	else  // height is default
	{
		print_cons_end (__geomStream, cone, name);
		if (! cone->isSolidToDefault())
			print_field (__geomStream, name, "solid", cone->getSolid());
		if (! cone->isSlicesToDefault())
			print_field (__geomStream, name, "slices", cone->getSlices());
		print_end(__geomStream);
		return true;
	}
	print_cons_end (__geomStream, cone, name);
  }
  else // radius is default
  {
    print_cons_end(__geomStream, cone, name);
	if (! cone->isHeightToDefault())
		print_field (__geomStream, name, "height", cone->getHeight());
	if (! cone->isSolidToDefault())
		print_field (__geomStream, name, "solid", cone->getSolid());
    if (! cone->isSlicesToDefault())
		print_field (__geomStream, name, "slices", cone->getSlices());
  }
 
  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);

  string name = compute_name(cylinder);
  print_cons_begin(__geomStream, name, "Cylinder");
  if (! cylinder->isRadiusToDefault())
  {
	print_arg_field (__geomStream, "radius", cylinder->getRadius());
	if (! cylinder->isHeightToDefault())
	{
		print_arg_field (__geomStream, "height", cylinder->getHeight());
		if (! cylinder->isSolidToDefault())
		{
			print_arg_field (__geomStream, "solid", cylinder->getSolid());
			if (! cylinder->isSlicesToDefault())
				print_arg_field (__geomStream, "slices", cylinder->getSlices());
		} 
		else // solid is default
		{
			print_cons_end (__geomStream, cylinder, name);
			if (! cylinder->isSlicesToDefault())
				print_field (__geomStream, name, "slices", cylinder->getSlices());
			print_end(__geomStream);
			return true;
		}
	}
	else  // height is default
	{
		print_cons_end (__geomStream, cylinder, name);
		if (! cylinder->isSolidToDefault())
			print_field (__geomStream, name, "solid", cylinder->getSolid());
		if (! cylinder->isSlicesToDefault())
			print_field (__geomStream, name, "slices", cylinder->getSlices());
		print_end(__geomStream);
		return true;
	}
	print_cons_end (__geomStream, cylinder, name);
  }
  else // radius is default
  {
    print_cons_end(__geomStream, cylinder, name);
	if (! cylinder->isHeightToDefault())
		print_field (__geomStream, name, "height", cylinder->getHeight());
	if (! cylinder->isSolidToDefault())
		print_field (__geomStream, name, "solid", cylinder->getSolid());
    if (! cylinder->isSlicesToDefault())
		print_field (__geomStream, name, "slices", cylinder->getSlices());
  }
 
  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);
  
  string name = compute_name(elevationGrid);
  print_cons_begin(__geomStream, name, "ElevationGrid");
  
  print_arg_field (__geomStream, "heightList", elevationGrid->getHeightList());
  
  print_cons_end (__geomStream, elevationGrid, name);
  if (! elevationGrid->isXSpacingToDefault())
	  print_field(__geomStream, name, "xspacing", elevationGrid->getXSpacing());
  if (! elevationGrid->isYSpacingToDefault())
	  print_field(__geomStream, name, "yspacing", elevationGrid->getYSpacing());
  if (! elevationGrid->isCCWToDefault())
	  print_field(__geomStream, name, "ccw", elevationGrid->getCCW());

  print_end(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);
  
  string name = compute_name(eulerRotated);

  GeometryPtr obj = eulerRotated->getGeometry();
  obj->apply(*this);

  print_cons_begin(__geomStream, name, "EulerRotated");
  print_arg_field (__geomStream, "azimuth", eulerRotated->getAzimuth());
  print_arg_field (__geomStream, "elevation", eulerRotated->getElevation());
  print_arg_field (__geomStream, "roll", eulerRotated->getRoll());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));

  print_cons_end(__geomStream, eulerRotated, name);
  print_end(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */


bool PyPrinter::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);
  
  Curve2DPtr vertical_obj = extrudedHull->getVertical();
  Curve2DPtr horizontal_obj = extrudedHull->getHorizontal();
  vertical_obj->apply(*this);
  horizontal_obj->apply(*this);
  
  string name = compute_name(extrudedHull);
 
  print_cons_begin(__geomStream, name, "ExtrudedHull");

  print_arg_field(__geomStream,SceneObjectPtr(vertical_obj));
  print_arg_field(__geomStream,SceneObjectPtr(horizontal_obj));
  if (! extrudedHull->isCCWToDefault())
	  print_arg_field(__geomStream, extrudedHull->getCCW());

  print_cons_end(__geomStream, extrudedHull, name);
  
  print_end(__geomStream);

  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Group * group  ) {
  GEOM_ASSERT(group);

  string name = compute_name(group);

  GeometryArrayPtr obj = group->getGeometryList();
  obj->apply(*this);

  print_cons_begin(__geomStream, name, "Group");
    print_arg_field (__geomStream, "geometryList", group->getGeometryList());
  print_cons_end(__geomStream, group, name);
  print_end(__geomStream);

  /*
  GEOM_PRINT_BEGIN(__geomStream,"Group",group);

  GEOM_PRINT_FIELD_ARRAY(__geomStream,group,GeometryList,GEOMETRY);

  if (! group->isSkeletonToDefault())
    GEOM_PRINT_FIELD(__geomStream,group,Skeleton,GEOMETRY);

  GEOM_PRINT_END(__geomStream);*/

  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  
  string name = compute_name(nurbsCurve);

  print_cons_begin(__geomStream, name, "NurbsCurve");
  print_arg_field (__geomStream, "ctrlPointList", nurbsCurve->getCtrlPointList());
  if (! nurbsCurve->isKnotListToDefault())
  {
	  print_arg_field (__geomStream, "knotList", nurbsCurve->getKnotList());
	  if (! nurbsCurve->isDegreeToDefault())
	  {
		print_arg_field (__geomStream, "degree", nurbsCurve->getDegree());
		if (! nurbsCurve->isStrideToDefault())
			print_arg_field (__geomStream, "strides", nurbsCurve->getStride());
	  }
	  else  // degree is default
	  {
		print_cons_end(__geomStream, nurbsCurve, name);
		if (! nurbsCurve->isStrideToDefault())
			print_field (__geomStream, name, "strides", nurbsCurve->getStride());
		print_end(__geomStream);
		return true;
	  }
	  print_cons_end(__geomStream, nurbsCurve, name);
  }
  else  // knotlist is default
  {
	print_cons_end(__geomStream, nurbsCurve, name);
	if (! nurbsCurve->isDegreeToDefault())
		print_field (__geomStream, name, "degree", nurbsCurve->getDegree());
	if (! nurbsCurve->isStrideToDefault())
		print_field (__geomStream, name, "strides", nurbsCurve->getStride());
  }
  
  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  string name = compute_name(nurbsPatch);
  print_cons_begin(__geomStream, name, "NurbsPatch");
  
  print_arg_field (__geomStream, nurbsPatch->getCtrlPointMatrix()); 
  if (! nurbsPatch->isUKnotListToDefault())
  {
	print_arg_field (__geomStream, nurbsPatch->getUKnotList()); 
	if (! nurbsPatch->isVKnotListToDefault())
	{
		print_arg_field (__geomStream, nurbsPatch->getVKnotList());
		if (! nurbsPatch->isUDegreeToDefault())
		{
			print_arg_field (__geomStream, nurbsPatch->getUDegree());
			if (! nurbsPatch->isVDegreeToDefault())
			{
				print_arg_field (__geomStream, nurbsPatch->getVDegree());
				if (! nurbsPatch->isUStrideToDefault())
				{
					print_arg_field (__geomStream, nurbsPatch->getUStride());
					if (! nurbsPatch->isVStrideToDefault())
					{
						print_arg_field (__geomStream, nurbsPatch->getVStride());
						if (! nurbsPatch->isCCWToDefault())
							print_arg_field (__geomStream, nurbsPatch->getCCW());
					}
					else  // Vstride is default
					{
						print_cons_end(__geomStream, nurbsPatch, name);
						if (! nurbsPatch->isCCWToDefault())
							print_field (__geomStream, name, "ccw", nurbsPatch->getCCW());
						print_end(__geomStream);
						return true;
					}
				}
				else  // Ustride is default
				{
					print_cons_end(__geomStream, nurbsPatch, name);
					if (! nurbsPatch->isVStrideToDefault())
						print_field (__geomStream, name, "vstride", nurbsPatch->getVStride());
					if (! nurbsPatch->isCCWToDefault())
						print_field (__geomStream, name, "ccw", nurbsPatch->getCCW());
					print_end(__geomStream);
					return true;
				}
			}
			else  // VDegree is default
			{
				print_cons_end(__geomStream, nurbsPatch, name);
				if (! nurbsPatch->isUStrideToDefault())
					print_field (__geomStream, name, "ustride", nurbsPatch->getUStride());
				if (! nurbsPatch->isVStrideToDefault())
					print_field (__geomStream, name, "vstride", nurbsPatch->getVStride());
				if (! nurbsPatch->isCCWToDefault())
					print_field (__geomStream, name, "ccw", nurbsPatch->getCCW());
				print_end(__geomStream);
				return true;
			}
		}
		else  // UDegree is default
		{
			print_cons_end(__geomStream, nurbsPatch, name);
			if (! nurbsPatch->isVDegreeToDefault())
				print_field (__geomStream, name, "vdegree", nurbsPatch->getVDegree());
			if (! nurbsPatch->isUStrideToDefault())
				print_field (__geomStream, name, "ustride", nurbsPatch->getUStride());
			if (! nurbsPatch->isVStrideToDefault())
				print_field (__geomStream, name, "vstride", nurbsPatch->getVStride());
			if (! nurbsPatch->isCCWToDefault())
				print_field (__geomStream, name, "ccw", nurbsPatch->getCCW());
			print_end(__geomStream);
			return true;
		}
	}
	else  // VKnotList is default
	{
		print_cons_end(__geomStream, nurbsPatch, name);
		if (! nurbsPatch->isUDegreeToDefault())
			print_field (__geomStream, name, "udegree", nurbsPatch->getUDegree());
		if (! nurbsPatch->isVDegreeToDefault())
			print_field (__geomStream, name, "vdegree", nurbsPatch->getVDegree());
		if (! nurbsPatch->isUStrideToDefault())
			print_field (__geomStream, name, "ustride", nurbsPatch->getUStride());
		if (! nurbsPatch->isVStrideToDefault())
			print_field (__geomStream, name, "vstride", nurbsPatch->getVStride());
		if (! nurbsPatch->isCCWToDefault())
			print_field (__geomStream, name, "ccw", nurbsPatch->getCCW());
		print_end(__geomStream);
		return true;
	}
	print_cons_end(__geomStream, nurbsPatch, name);
  }
  else // UKnotList is default
  {
	print_cons_end(__geomStream, nurbsPatch, name);
	if (! nurbsPatch->isVKnotListToDefault())
		print_field (__geomStream, name, "vknotList", nurbsPatch->getUKnotList()); 
	if (! nurbsPatch->isUDegreeToDefault())
		print_field (__geomStream, name, "udegree", nurbsPatch->getUDegree());
	if (! nurbsPatch->isVDegreeToDefault())
		print_field (__geomStream, name, "vdegree", nurbsPatch->getVDegree());
	if (! nurbsPatch->isUStrideToDefault())
		print_field (__geomStream, name, "ustride", nurbsPatch->getUStride());
	if (! nurbsPatch->isVStrideToDefault())
		print_field (__geomStream, name, "vstride", nurbsPatch->getVStride());
	if (! nurbsPatch->isCCWToDefault())
		print_field (__geomStream, name, "ccw", nurbsPatch->getCCW());
  }

  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);

  string name = compute_name(pointSet);
  print_cons_begin(__geomStream, name, "PointSet");
  print_arg_field (__geomStream, "pointList", pointSet->getPointList());
  if(! pointSet->isColorListToDefault())
	  print_arg_field (__geomStream, "colorList", pointSet->getColorList());
  print_cons_end(__geomStream, pointSet, name);
  
  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  
  string name = compute_name(polyline);
  print_cons_begin(__geomStream, name, "Polyline");
  print_arg_field (__geomStream, "pointList", polyline->getPointList());
  if(! polyline->isColorListToDefault())
	  print_arg_field (__geomStream, "colorList", polyline->getColorList());
  print_cons_end(__geomStream, polyline, name);
  
  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process(Sphere * sphere)
{
  GEOM_ASSERT(sphere);
  
  string name = compute_name(sphere);
  print_cons_begin(__geomStream, name, "Sphere");

  if (! sphere->isRadiusToDefault())
  {
	print_arg_field (__geomStream, "radius", sphere->getRadius());  
	if (! sphere->isSlicesToDefault())
	{
		print_arg_field (__geomStream, "slices", sphere->getSlices());
		if (! sphere->isStacksToDefault())
			print_arg_field (__geomStream, "stacks", sphere->getStacks());
	}
	else 
	{
		if (! sphere->isStacksToDefault())
		{
			print_cons_end(__geomStream, sphere, name);
			print_field (__geomStream, name, "stacks", sphere->getStacks());
			print_end(__geomStream);
			return true;
		}
	}
	print_cons_end(__geomStream, sphere, name);
  }
  else
  {
	print_cons_end(__geomStream, sphere, name);
 
	if (! sphere->isSlicesToDefault())
		print_field<int_t> (__geomStream, name, "slices", sphere->getSlices());
	if (! sphere->isStacksToDefault())
		print_field<int_t> (__geomStream, name, "stacks", sphere->getStacks());
  }

  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);

  string name = compute_name(scaled);

  GeometryPtr obj = scaled->getGeometry();
  obj->apply(*this);

  print_cons_begin(__geomStream, name, "Scaled");
  print_arg_field (__geomStream, "scale", scaled->getScale());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));

  print_cons_end(__geomStream, scaled, name);

  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */


bool PyPrinter::process( Swung * swung )
{
  GEOM_ASSERT(swung);
  
  Curve2DArrayPtr obj = swung->getProfileList();
  obj->apply(*this);

  string name = compute_name(swung);

  print_cons_begin(__geomStream, name, "Swung");

  print_arg_field (__geomStream, "profileList", swung->getProfileList());
  print_arg_field (__geomStream, "angleList", swung->getAngleList());

  if (! swung->isSlicesToDefault())
  {
	print_arg_field (__geomStream, "slices", swung->getSlices());
	if(! swung->isCCWToDefault() )
	{
		print_arg_field (__geomStream, "ccw", swung->getCCW());
		if (! swung->isDegreeToDefault())
		{
			print_arg_field (__geomStream, "degree", swung->getDegree());
			if (! swung->isStrideToDefault())
				print_arg_field (__geomStream, "stride", swung->getStride());
		}
		else  // degree is default
		{
			print_cons_end(__geomStream, swung, name);
			if (! swung->isStrideToDefault())
				print_field (__geomStream, name, "stride", swung->getStride());
			print_end(__geomStream);
			return true;
		}
	}
	else  // ccw is default
	{
		print_cons_end(__geomStream, swung, name);
		if (! swung->isDegreeToDefault())
			print_field (__geomStream, name, "degree", swung->getDegree());
		if (! swung->isStrideToDefault())
			print_field (__geomStream, name, "stride", swung->getStride());
		print_end(__geomStream);
		return true;
	}
	print_cons_end(__geomStream, swung, name);
  }
  else  // slices is default
  {
	print_cons_end(__geomStream, swung, name);
	if(! swung->isCCWToDefault() )
		print_field (__geomStream, name, "ccw", swung->getCCW());
	if (! swung->isDegreeToDefault())
		print_field (__geomStream, name, "degree", swung->getDegree());
	if (! swung->isStrideToDefault())
		print_field (__geomStream, name, "stride", swung->getStride());
  }

  print_end(__geomStream);
  return true;
}



/* ----------------------------------------------------------------------- */

bool PyPrinter::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  
  string name = compute_name(quadSet);
  
  print_cons_begin(__geomStream, name, "QuadSet");
  print_arg_field (__geomStream, "pointList", quadSet->getPointList());
  print_arg_field (__geomStream, "indexList", quadSet->getIndexList());

  print_cons_end(__geomStream, quadSet, name);
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
  //if (! quadSet->isSkeletonToDefault()){
	
  print_end(__geomStream);
  return true;  
}



/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Translated * translated ) {
  GEOM_ASSERT(translated);
  
  string name = compute_name(translated);

  GeometryPtr obj = translated->getGeometry();
  obj->apply(*this);

  // string geometryname = compute_name(obj);
  
  print_cons_begin(__geomStream, name, "Translated");
  print_arg_field (__geomStream, "translation", translated->getTranslation());
  print_arg_field (__geomStream, "geometry", SceneObjectPtr(obj));

  print_cons_end(__geomStream, translated, name);


  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */

bool PyPrinter::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);

  string name = compute_name(triangleSet);
  
  print_cons_begin(__geomStream, name, "TriangleSet");
  print_arg_field (__geomStream, "pointList", triangleSet->getPointList());
  print_arg_field (__geomStream, "indexList", triangleSet->getIndexList());

  print_cons_end(__geomStream, triangleSet, name);
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
  //if (! quadSet->isSkeletonToDefault()){

  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

  string name = compute_name(bezierCurve);
  print_cons_begin(__geomStream, name, "BezierCurve2D");
  print_arg_field (__geomStream, bezierCurve->getCtrlPointList());
  if (! bezierCurve->isStrideToDefault())
	  print_arg_field (__geomStream, "stride", bezierCurve->getStride());

  print_cons_end(__geomStream, bezierCurve, name);
  print_field (__geomStream, name, "degree", bezierCurve->getDegree());
  print_end(__geomStream);

  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Disc * disc ) {
  GEOM_ASSERT(disc);
  
  string name = compute_name(disc);
  print_cons_begin(__geomStream, name, "Disc");

  if (! disc->isRadiusToDefault())
  {
	print_arg_field (__geomStream, "radius", disc->getRadius());
	if (! disc->isSlicesToDefault())
		print_arg_field (__geomStream, "slices", disc->getSlices());
	print_cons_end(__geomStream, disc, name);
  }
  else
  {
	print_cons_end(__geomStream, disc, name);
	if (! disc->isSlicesToDefault())
		print_field (__geomStream, name, "slices", disc->getSlices());
  }

  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  string name = compute_name(nurbsCurve);
  print_cons_begin(__geomStream, name, "NurbsCurve2D");
  print_arg_field (__geomStream, "ctrlPointList", nurbsCurve->getCtrlPointList());

  if (! nurbsCurve->isKnotListToDefault())
  {
	print_arg_field (__geomStream, "knotList", nurbsCurve->getKnotList());
	if (! nurbsCurve->isDegreeToDefault())
	{
		print_arg_field (__geomStream, "degree", nurbsCurve->getDegree());
		if (! nurbsCurve->isStrideToDefault())
			print_arg_field (__geomStream, "strides", nurbsCurve->getStride());
	}
	else
	{
		if (! nurbsCurve->isStrideToDefault())
		{
			print_cons_end(__geomStream, nurbsCurve, name);
			print_field (__geomStream, name, "stride", nurbsCurve->getStride());
			print_end(__geomStream);
			return true;
		}
	}
	print_cons_end(__geomStream, nurbsCurve, name);
  }
  else
  {
	print_cons_end(__geomStream, nurbsCurve, name);
	if (! nurbsCurve->isDegreeToDefault())
		print_field (__geomStream, name, "degree", nurbsCurve->getDegree());
	if (! nurbsCurve->isStrideToDefault())
		print_field (__geomStream, name, "stride", nurbsCurve->getStride());
  }
  
  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);

  string name = compute_name(pointSet);
  print_cons_begin(__geomStream, name, "PointSet2D");
  print_arg_field (__geomStream, pointSet->getPointList());
  
  print_cons_end(__geomStream, pointSet, name);
  print_end(__geomStream);
  return true;
}


/* ----------------------------------------------------------------------- */

bool PyPrinter::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);

  string name = compute_name(polyline);
  print_cons_begin(__geomStream, name, "Polyline2D");
  print_arg_field (__geomStream, polyline->getPointList());
  
  print_cons_end(__geomStream, polyline, name); 
  print_end(__geomStream);
  return true;
}

/* ----------------------------------------------------------------------- */
