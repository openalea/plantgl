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




#include <plantgl/scenegraph/appearance/util_image.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#if PGL_WITH_BOOST_NUMPY
    #include <boost/python/numpy.hpp>
#endif
#include <QtGui/QImage>
#include <QtCore/QResource>

PGL_USING_NAMESPACE
using namespace boost::python;

#define bp boost::python

#if PGL_WITH_BOOST_NUMPY
#define np boost::python::numpy
#endif

DEF_POINTEE(Image)

#if PGL_WITH_BOOST_NUMPY
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
#endif

boost::python::object py_histogram(Image * img){
    pgl_hash_map<uint_t,uint_t> res = img->histogram();
    boost::python::list bres;
    for(pgl_hash_map<uint_t,uint_t>::const_iterator _it = res.begin(); _it != res.end(); ++_it){
      bres.append(boost::python::make_tuple(_it->first,_it->second));
    }
    return bres;
}


boost::python::object py_getpixelat(Image * img, uint_t x, uint_t y){
   if (img->nbChannels() == 3) { return boost::python::object(img->getPixel3At(x,y)); }
   else return boost::python::object(img->getPixelAt(x,y));
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
#if PGL_WITH_BOOST_NUMPY
        .def( "__init__", make_constructor( img_from_array ) )
#endif
        .def( "setPixelAt", (void (Image::*)(uint_t , uint_t, const Color4 &))&Image::setPixelAt )
        .def( "setPixelAt", (void (Image::*)(uint_t , uint_t, const Color3 &, uchar_t))&Image::setPixelAt )
        .def( "getPixelAt", &py_getpixelat )
        .def( "getPixel4At", &Image::getPixelAt )
        .def( "getPixel3At", &Image::getPixel3At )
        .def( "width", &Image::width )
        .def( "height", &Image::height )
        .def( "nbChannels", &Image::nbChannels )
        .def( "fill", &Image::fill )
        .def( "read", &Image::read )
        .def( "save", &Image::save )
#if PGL_WITH_BOOST_NUMPY
        .def( "to_array", &img_to_array )
        .def( "to_interlaced_array", &img_to_interlaced_array )
        .def( "from_array", &from_array )
#endif
        .def( "transpose", &Image::transpose )
        .def( "histogram", &py_histogram )
       // .def( "_register_image_content_in_qt", &register_image_content_in_qt)

    ;

  implicitly_convertible<ImagePtr, RefCountObjectPtr >();
}

