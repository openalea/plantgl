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

