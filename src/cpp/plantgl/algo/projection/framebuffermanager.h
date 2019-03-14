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

/*! \file ZBufferEngine.h
    \brief Definition of Rendering Engine based on ZBuffer.
*/



#ifndef __FrameBufferManager_h__
#define __FrameBufferManager_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/appearance/util_image.h>
#include "../algo_config.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ZBufferEngine
    \brief A Rendering Engine based on ZBuffer.
*/

/* ----------------------------------------------------------------------- */


class FrameBufferManager;
typedef RCPtr<FrameBufferManager> FrameBufferManagerPtr;

class FrameBufferManager : public RefCountObject {
public:
    FrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, uint8_t nbChannel, const Color3& backGroundColor) {}
    FrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, uint8_t nbChannel, const Color4& backGroundColor) {}
    virtual ~FrameBufferManager() {}
    
    virtual void setPixelAt(uint_t x, uint_t y, const Color3& color) = 0;
    virtual Color3 getPixelAt(uint_t x, uint_t y) const = 0;

    virtual void setPixel4At(uint_t x, uint_t y, const Color4& color) = 0;
    virtual Color4 getPixel4At(uint_t x, uint_t y) const = 0;

    virtual uint16_t width() const = 0;
    virtual uint16_t height() const = 0;
    virtual FrameBufferManagerPtr deepcopy() const = 0;
};



class PglFrameBufferManager : public FrameBufferManager {
public:
    PglFrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, uint8_t nbChannel, const Color3& backGroundColor);
    PglFrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, uint8_t nbChannel, const Color4& backGroundColor);
    virtual ~PglFrameBufferManager();

    virtual void setPixelAt(uint_t x, uint_t y, const Color3& color);
    virtual Color3 getPixelAt(uint_t x, uint_t y) const ;
    
    virtual void setPixel4At(uint_t x, uint_t y, const Color4& color);
    virtual Color4 getPixel4At(uint_t x, uint_t y) const ;
    
    virtual uint16_t width() const;
    virtual uint16_t height() const;
    virtual FrameBufferManagerPtr deepcopy() const;

    ImagePtr getImage() const { return __image; }    

    ImagePtr __image;
};

typedef RCPtr<PglFrameBufferManager> PglFrameBufferManagerPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

