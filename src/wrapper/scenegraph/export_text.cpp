#include <plantgl/scenegraph/geometry/text.h>

#include <boost/python.hpp>
#include <string>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"


using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE(Font)

void export_Font()
{
  class_< Font, FontPtr, bases< SceneObject >,boost::noncopyable >
    ("Font",init<optional<std::string,int,bool,bool> >
     (args("family","size","bold","italic"),
	  "Font([family,size,bold,italic])"))
    .def( "copy", &Font::copy )
	.DEC_BT_PROPERTY(family,Font,Family,std::string)
	.DEC_BT_NR_PROPERTY_WD(size,Font,Size,uint32_t)
	.DEC_BT_NR_PROPERTY_WD(bold,Font,Bold,bool)
	.DEC_BT_NR_PROPERTY_WD(italic,Font,Italic,bool)

    ;

  implicitly_convertible< FontPtr, SceneObjectPtr >();
}


DEF_POINTEE(Text)

void export_Text()
{
  class_< Text, TextPtr, bases< Geometry >,boost::noncopyable >
    ("Text",init<std::string,optional<const TOOLS(Vector3)&,bool, const FontPtr&> >
     (args("string","position","screencoordinates","fontstyle"),
	  "Text(str string[, Vector3 position, bool screencoordinates, Font fontstyle])"))
    .def( "copy", &Text::copy )
	.DEC_BT_PROPERTY(string,Text,String,std::string)
	.DEC_PTR_PROPERTY_WD(fontstyle,Text,FontStyle,FontPtr)
	.DEC_CT_PROPERTY_WD(position,Text,Position,Vector3)
	.DEC_BT_NR_PROPERTY_WD(screencoordinates,Text,ScreenCoordinates,bool)

    ;

  implicitly_convertible< TextPtr,GeometryPtr >();
  
}
