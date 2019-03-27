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



/*! \file appe_color.h
    \brief Definition of the appearance class Color3 and Color4.
*/


#ifndef __matl_img_h__
#define __matl_img_h__

#include "../sg_config.h"
#include "color.h"
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_hashmap.h>
#include <vector>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/// Image Pointer
class Image;
typedef RCPtr<Image> ImagePtr;


class Image : public RefCountObject{
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

    Color4 getPixelAtUV(real_t u, real_t v, bool repeatu = true, bool repeatv = true) const;

    uint_t width() const { return __width; }
    uint_t height() const { return __height; }
    uint_t nbChannels() const { return __nbchannels; }

    void setNbChannels(uint_t nb = 4);

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

    pgl_hash_map<uint_t,uint_t> histogram() const;
};



/* ----------------------------------------------------------------------- */

// __geom_triangleset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

