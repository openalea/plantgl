/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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
	.DEC_BT_NR_PROPERTY_WDV(size,Font,Size,uint32_t,DEFAULT_SIZE)
	.DEC_BT_NR_PROPERTY_WDV(bold,Font,Bold,bool,DEFAULT_BOLD)
	.DEC_BT_NR_PROPERTY_WDV(italic,Font,Italic,bool,DEFAULT_ITALIC)

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
	.DEC_PTR_PROPERTY_WDV(fontstyle,Text,FontStyle,FontPtr,DEFAULT_FONT)
	.DEC_CT_PROPERTY_WDV(position,Text,Position,Vector3,DEFAULT_POSITION)
	.DEC_BT_NR_PROPERTY_WDV(screencoordinates,Text,ScreenCoordinates,bool,DEFAULT_SCREEN_COORDINATES)

    ;

  implicitly_convertible< TextPtr,GeometryPtr >();
  
}
