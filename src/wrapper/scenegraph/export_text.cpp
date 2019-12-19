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



#include <plantgl/scenegraph/geometry/text.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE

DEF_POINTEE(Font)

void export_Font()
{
  class_< Font, FontPtr, bases< SceneObject >,boost::noncopyable >
    ("Font","Font describes how text is displayed.",init<optional<std::string,int,bool,bool> >
     ("Font([family,size,bold,italic])",
     (bp::arg("family") = "",
      bp::arg("size")   = Font::DEFAULT_SIZE,
      bp::arg("bold")   = Font::DEFAULT_BOLD,
      bp::arg("italic") = Font::DEFAULT_ITALIC)))
    .DEF_PGLBASE(Font)
    .DEC_BT_PROPERTY(family,Font,Family,std::string)
    .DEC_BT_NR_PROPERTY_WDV(size,Font,Size,uint_t,DEFAULT_SIZE)
    .DEC_BT_NR_PROPERTY_WDV(bold,Font,Bold,bool,DEFAULT_BOLD)
    .DEC_BT_NR_PROPERTY_WDV(italic,Font,Italic,bool,DEFAULT_ITALIC)

    ;

  implicitly_convertible< FontPtr, SceneObjectPtr >();
}


DEF_POINTEE(Text)

void export_Text()
{
  class_< Text, TextPtr, bases< Geometry >,boost::noncopyable >
    ("Text","Text with font. It support display in screen or world coordinates.",init<std::string,optional<const Vector3&,bool, const FontPtr&> >
     ("Text(str string[, Vector3 position, bool screencoordinates, Font fontstyle])",
     (bp::arg("string"),
      bp::arg("position") = Text::DEFAULT_POSITION,
      bp::arg("screencoordinates") = Text::DEFAULT_SCREEN_COORDINATES,
      bp::arg("fontstyle") = Text::DEFAULT_FONT)))
    .DEF_PGLBASE(Text)
    .DEC_BT_PROPERTY(string,Text,String,std::string)
    .DEC_PTR_PROPERTY_WDV(fontstyle,Text,FontStyle,FontPtr,DEFAULT_FONT)
    .DEC_CT_PROPERTY_WDV(position,Text,Position,Vector3,DEFAULT_POSITION)
    .DEC_BT_NR_PROPERTY_WDV(screencoordinates,Text,ScreenCoordinates,bool,DEFAULT_SCREEN_COORDINATES)

    ;

  implicitly_convertible< TextPtr,GeometryPtr >();

}
