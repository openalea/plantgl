/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2018 - Cirad/Inra/Inria
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://github.com/openalea/plantgl
 *
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



#include "util_image.h"

#define cimg_display 0
#include "CImg.h"
/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

Image::Image(uint_t width, uint_t height, uint_t nbChannels, const Color4& defaultColor):
    RefCountObject(),
    __data(width*height*nbChannels), __width(width), __height(height), __nbchannels(nbChannels)
{
    fill(defaultColor);

}

Image::Image(const std::string& fname) :
    RefCountObject(),
    __data(), __width(0), __height(0), __nbchannels(0)
{
    read(fname);
}

Image::~Image()
{

}

void Image::setPixelAt(uint_t x, uint_t y, const Color4 & pixel)
{
    GEOM_ASSERT(x < __width && y < __height);
    std::vector<uchar_t>::iterator itCol = __data.begin();
    itCol += __nbchannels*(y * __width + x);
    if (__nbchannels>0)*itCol = pixel.getRed();
    if (__nbchannels>1){++itCol; *itCol = pixel.getGreen();}
    if (__nbchannels>2){++itCol; *itCol = pixel.getBlue();}
    if (__nbchannels>3){++itCol; *itCol = pixel.getAlpha();}
    
}
void Image::setPixelAt(uint_t x, uint_t y, const Color3 & pixel, uchar_t alpha ){
    setPixelAt(x,y,Color4(pixel,alpha));
}

Color4 Image::getPixelAt(uint_t x, uint_t y) const {
    GEOM_ASSERT(x < __width && y < __height);
    std::vector<uchar_t>::const_iterator itCol = __data.begin();
    itCol += __nbchannels*(y * __width + x);
    return Color4(__nbchannels>0?*itCol:0,__nbchannels>1?*(itCol+1):0,__nbchannels>2?*(itCol+2):0,__nbchannels>3?*(itCol+3):0);
}

void Image::fill(const Color4 & color) 
{
    uint8_t i = 0;
    for (std::vector<uchar_t>::iterator itCol = __data.begin(); itCol != __data.end(); ++itCol, i = (i+1)%__nbchannels){
        *itCol = color[i];
    }

}



bool Image::read(const std::string& fname)
{
    try {
        cimg_library::CImg<unsigned char> cimg(fname.c_str());

        __width = cimg.width();
        __height = cimg.height();
        __nbchannels = cimg.spectrum();
        uint_t size = __width*__height*__nbchannels;
        __data = std::vector<uchar_t>(size);

        for (uint_t i = 0 ; i < cimg.width() ; ++i){
            for (uint_t j = 0 ; j < cimg.height() ;  ++j){
                setPixelAt(i,j, Color4((uchar_t)*cimg.data(i,j,0), 
                                       (uchar_t)(__nbchannels>1?*cimg.data(i,j,1):0), 
                                       (uchar_t)(__nbchannels>2?*cimg.data(i,j,2):0), 
                                       (uchar_t)(__nbchannels>3?*cimg.data(i,j,3):0)));
            }
        }
        //fromNonInterlacedData(cimg.data(), cimg.width(), cimg.height(), cimg.spectrum());
        return true;
    }
    catch (...) {
        return false;
    }
}

void Image::save(const std::string& fname) const
{
    const unsigned char * data = toNonInterlacedData();
    cimg_library::CImg<unsigned char> img(data, __width,__height, 1, __nbchannels);
    delete [] data;
    img.save(fname.c_str());

}

const uchar_t * Image::toNonInterlacedData() const
{
    uint_t size = __width*__height;
    unsigned char * data = new unsigned char[size*__nbchannels];
    unsigned char * itcimg = data;

    std::vector<uchar_t>::const_iterator itimg = __data.begin();
    for (uint8_t c = 0; c < __nbchannels; ++c ){
        itimg = __data.begin()+c;
        for (uint_t i = 0 ; i < size; ++i, ++itcimg, itimg += __nbchannels) {
            *itcimg = *itimg ;
        }
    }
    return data;

}

void Image::fromNonInterlacedData(const uchar_t * input, uint_t width, uint_t height, uchar_t nbChannels) 
{
        __width = width;
        __height = height;
        __nbchannels = nbChannels;
        uint_t size = __width*__height;
        __data = std::vector<uchar_t>(size*__nbchannels);
        const uchar_t * itcimg = input;
        std::vector<uchar_t>::iterator itimg = __data.begin();

        for (uint8_t c = 0; c < __nbchannels; ++c ){
            itimg = __data.begin()+c;
            for (uint_t i = 0 ; i < size; ++i, ++itcimg, itimg += __nbchannels) {
                *itimg = *itcimg ;
            }
        }
}

const uchar_t * Image::toInterlacedData() const
{ return __data.data(); }

void Image::fromInterlacedData(const uchar_t * data, uint_t width, uint_t height, uchar_t nbChannels) 
{
    __width = width;
    __height = height;
    __nbchannels = nbChannels;
    uint_t size = __width*__height*nbChannels;
    __data = std::vector<uchar_t>(data, data+size);
}

