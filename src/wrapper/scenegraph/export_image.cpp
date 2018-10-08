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
#include <QtGui/QImage>
#include <QtCore/QResource>

PGL_USING_NAMESPACE
using namespace boost::python;

#define bp boost::python
#define np boost::python::numpy

DEF_POINTEE(Image)

np::ndarray img_to_array(Image * img)
{
    np::dtype dt = np::dtype::get_builtin<uint8_t>();
    size_t s = sizeof(uint8_t);

    ImagePtr tr = img->transpose();
    np::ndarray array = np::from_data(tr->toNonInterlacedData(), 
                                      dt,
                                      bp::make_tuple(tr->width(), tr->height(), tr->nbChannels()),
                                      bp::make_tuple(s, tr->width()*s, tr->width()*tr->height()*s),
                                      bp::object());
    return array;
}

np::ndarray img_to_interlaced_array(Image * img)
{
    np::dtype dt = np::dtype::get_builtin<uint8_t>();
    size_t s = sizeof(uint8_t);

    ImagePtr tr = img; 
    np::ndarray array = np::from_data(tr->toInterlacedData(), 
                                      dt,
                                      bp::make_tuple(tr->width(), tr->height(), tr->nbChannels()),
                                      bp::make_tuple(tr->height()*tr->nbChannels()*s, tr->nbChannels()*s, s),
                                      bp::object());
    return array;
}


bool  from_array(Image * img,  np::ndarray array) {
    if (array.get_nd() != 3) { return false; }
    np::dtype dt = np::dtype::get_builtin<uint8_t>();
    if (array.get_dtype() != dt) { return false;}

    img->fromData((const uchar_t *)array.get_data(), 
                  array.shape(1),   array.shape(0),    array.shape(2), 
                  array.strides(1), array.strides(0),  array.strides(2));
    return true;
}

Image * img_from_array(np::ndarray array){
    Image * img = new Image();
    if(!from_array(img, array)) { delete img; return NULL; }
    return img;

}

boost::python::object py_histogram(Image * img){
    pgl_hash_map<uint_t,uint_t> res = img->histogram();
    boost::python::list bres;
    for(pgl_hash_map<uint_t,uint_t>::const_iterator _it = res.begin(); _it != res.end(); ++_it){
      bres.append(boost::python::make_tuple(_it->first,_it->second));
    }
    return bres;
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
        .def( "to_interlaced_array", &img_to_interlaced_array )
        .def( "from_array", &from_array )
        .def( "transpose", &Image::transpose )
        .def( "histogram", &py_histogram )
       // .def( "_register_image_content_in_qt", &register_image_content_in_qt)

    ;

  implicitly_convertible<ImagePtr, RefCountObjectPtr >();
}

