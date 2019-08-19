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

