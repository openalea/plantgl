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


#include <plantgl/scenegraph/appearance/util_image.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#include <boost/python/numpy.hpp>

PGL_USING_NAMESPACE
using namespace boost::python;

#define bp boost::python
#define np boost::python::numpy

DEF_POINTEE(Image)

np::ndarray img_to_array(Image * img)
{
    np::dtype dt = np::dtype::get_builtin<uint8_t>();
    np::ndarray array = np::from_data(img->toNonInterlacedData(), 
                                      dt,
                                      bp::make_tuple(img->height(),img->width(), img->nbChannels()),
                                      bp::make_tuple(img->width()*img->nbChannels()*sizeof(uint8_t), img->nbChannels()*sizeof(uint8_t), sizeof(uint8_t)),
                                      bp::object());
    return array;

}


bool  from_array(Image * img,  np::ndarray array) {
    if (array.get_nd() != 3) { return false; }
    np::dtype dt = np::dtype::get_builtin<uint8_t>();
    if (array.get_dtype() != dt) { return false;}

    img->fromNonInterlacedData((const uchar_t *)array.get_data(), array.shape(1), array.shape(0), array.shape(2));
    return true;
}

Image * img_from_array(np::ndarray array){
    Image * img = new Image();
    if(!from_array(img, array)) { delete img; return NULL; }
    return img;

}

void export_Image()
{
  class_< Image, ImagePtr, bases< RefCountObject > , boost::noncopyable >
    ("Image", "An image represented as a matrix of Color4.", 
                 init< optional< uint_t,  uint_t, uint_t, const Color4&> >
                 ("Image(width, height, defaultColor)",
                 (bp::arg("width")=800,
                  bp::arg("height")=600,
                  bp::arg("nbChannels")=4,
                  bp::arg("defaultColor")=Color4(0,0,0,0))))
        .def(init< const std::string& >("Image(filename)",(boost::python::arg("filename"))))
        .def( "__init__", make_constructor( img_from_array ) ) 
        .def( "setPixelAt", (void (Image::*)(uint_t , uint_t, const Color4 &))&Image::setPixelAt ) 
        .def( "getPixelAt", &Image::getPixelAt ) 
        .def( "width", &Image::width ) 
        .def( "height", &Image::height ) 
        .def( "nbChannels", &Image::nbChannels ) 
        .def( "fill", &Image::fill ) 
        .def( "read", &Image::read ) 
        .def( "save", &Image::save )
        .def( "to_array", &img_to_array )
        .def( "from_array", &from_array )

    ;

  implicitly_convertible<ImagePtr, RefCountObjectPtr >();
}

