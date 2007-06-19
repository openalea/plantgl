#include <plantgl/scenegraph/geometry/text.h>

#include <boost/python.hpp>
#include <string>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"


using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE(Font)

SETGET(Font,Family,std::string);
SETGET(Font,Size,uint32_t);
SETGET(Font,Bold,bool);
SETGET(Font,Italic,bool);

void export_Font()
{
  class_< Font, FontPtr, bases< SceneObject >,boost::noncopyable >
    ("Font",init<optional<std::string,int,bool,bool> >
     (args("family","size","bold","italic"),
	  "Font([family,size,bold,italic])"))
    .def( "copy", &Font::copy )
	.DEC_SETGET(family,Font,Family,std::string)
	.DEC_SETGET_WD(size,Font,Size,uint32_t)
	.DEC_SETGET_WD(bold,Font,Bold,boll)
	.DEC_SETGET_WD(italic,Font,Italic,boll)

    ;

  implicitly_convertible< FontPtr, SceneObjectPtr >();
}


DEF_POINTEE(Text)

SETGET(Text,String,std::string);
SETGET(Text,FontStyle,FontPtr);
SETGET(Text,Position,Vector3);
SETGET(Text,ScreenCoordinates,bool);

void export_Text()
{
  class_< Text, TextPtr, bases< Geometry >,boost::noncopyable >
    ("Text",init<std::string,optional<const TOOLS(Vector3)&,bool, const FontPtr&> >
     (args("string","position","screencoordinates","fontstyle"),
	  "Text(str string[, Vector3 position, bool screencoordinates, Font fontstyle])"))
    .def( "copy", &Text::copy )
	.DEC_SETGET(string,Text,String,std::string)
	.DEC_SETGET_WD(fontstyle,Text,FontStyle,FontPtr)
	.DEC_SETGET_WD(position,Text,Position,Vector3)
	.DEC_SETGET_WD(screencoordinates,Text,ScreenCoordinates,bool)

    ;

  implicitly_convertible< TextPtr,GeometryPtr >();
  
}
