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


/*! \file appe_color.h
    \brief Definition of the appearance class Color3 and Color4.
*/


#ifndef __matl_img_h__
#define __matl_img_h__

#include "../sg_config.h"
#include "color.h"
#include <plantgl/tool/rcobject.h>
#include <vector>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/// Image Pointer
class Image;
typedef RCPtr<Image> ImagePtr;


class Image : public TOOLS(RefCountObject){
protected:
    std::vector<uchar_t> __data;
    uint_t __width;    
    uint_t __height;
    uint_t __nbchannels;


public:
    Image(uint_t width = 800, uint_t height = 600, uint_t nbChannels = 4, const Color4& defaultColor = Color4(0,0,0,0));
    Image(const uchar_t * data, uint_t width = 800, uint_t height = 600, uint_t nbChannels = 4);
    Image(const std::string& fname);

    ~Image();

    void setPixelAt(uint_t x, uint_t y, const Color4 & pixel);    
    void setPixelAt(uint_t x, uint_t y, const Color3 & pixel, uchar_t alpha = 0);
    void setPixelAt(uint_t x, uint_t y, const uchar_t * alpha );

    Color4 getPixelAt(uint_t x, uint_t y) const;
    const uchar_t * getPixelDataAt(uint_t x, uint_t y) const;

    uint_t width() const { return __width; }
    uint_t height() const { return __height; }
    uint_t nbChannels() const { return __nbchannels; }

    ImagePtr transpose() const;

    ///\ brief Food fills the entire image with the given color.
    ///
    void fill(const Color4 & color);

    bool read(const std::string& fname);
    void save(const std::string& fname) const;

    const uchar_t * toNonInterlacedData() const;
    void fromNonInterlacedData(const uchar_t * data, uint_t width, uint_t height, uchar_t nbChannels = 4) ;

    const uchar_t * toInterlacedData() const;
    void fromInterlacedData(const uchar_t * data, uint_t width, uint_t height, uchar_t nbChannels = 4) ;

    void fromData(const uchar_t * data, uint_t width, uint_t height, uchar_t nbChannels, uint_t stridewidth, uint_t strideheight, uchar_t stridechannels) ;

};



/* ----------------------------------------------------------------------- */

// __geom_triangleset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

