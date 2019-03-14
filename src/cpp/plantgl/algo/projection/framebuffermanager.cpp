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


/* ----------------------------------------------------------------------- */

#include "framebuffermanager.h"
/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE


PglFrameBufferManager::PglFrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, uint8_t nbChannels, const Color3& backGroundColor) :
     FrameBufferManager(imageWidth, imageHeight, nbChannels, backGroundColor), 
     __image(new Image(imageWidth, imageHeight, nbChannels, backGroundColor)) 
{}

PglFrameBufferManager::PglFrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, uint8_t nbChannels, const Color4& backGroundColor) :
     FrameBufferManager(imageWidth, imageHeight, nbChannels, backGroundColor), 
     __image(new Image(imageWidth, imageHeight, nbChannels, backGroundColor)) 
{}

PglFrameBufferManager::~PglFrameBufferManager() {}

void PglFrameBufferManager::setPixelAt(uint_t x, uint_t y, const Color3& color) 
{ 
    __image->setPixelAt(x,y,color); 
}

Color3 PglFrameBufferManager::getPixelAt(uint_t x, uint_t y) const
{ 
    return __image->getPixelAt(x,y); 
}

void PglFrameBufferManager::setPixel4At(uint_t x, uint_t y, const Color4& color) 
{ 
    __image->setPixelAt(x,y,color); 
}

Color4 PglFrameBufferManager::getPixel4At(uint_t x, uint_t y) const
{ 
    return __image->getPixelAt(x,y); 
}

uint16_t PglFrameBufferManager::width() const { return __image->width(); }
uint16_t PglFrameBufferManager::height() const { return __image->height(); }

FrameBufferManagerPtr PglFrameBufferManager::deepcopy() const
{ 
    PglFrameBufferManager * copy = new PglFrameBufferManager(*this); 
    copy->__image = ImagePtr(new Image(*__image));
    return FrameBufferManagerPtr(copy);
}

