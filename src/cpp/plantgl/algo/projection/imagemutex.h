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



#ifndef __ImageMutex_h__
#define __ImageMutex_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/util_array2.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_mutex.h>
#include "../algo_config.h"
#include <list>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ImageMutex
    \brief An utility to lock access to pixel of an image.
*/

/* ----------------------------------------------------------------------- */
typedef TOOLS(Array2<PglMutex>) MutexArray2;
typedef RCPtr<MutexArray2> MutexArray2Ptr;

class ImageMutex : public TOOLS(RefCountObject) {
public :
    ImageMutex(uint16_t imageWidth, uint16_t imageHeight);
    virtual ~ImageMutex() ;
    
    void lock(uint_t x, uint_t y);
    void unlock(uint_t x, uint_t y);
    bool tryLock(uint_t x, uint_t y);


    inline uint16_t width() const  { return __imageWidth; }
    inline uint16_t height() const { return __imageHeight; }

protected:
    
    uint16_t __imageWidth;
    uint16_t __imageHeight;
    MutexArray2 __mutexarray;

};

/* ----------------------------------------------------------------------- */

typedef RCPtr<ImageMutex> ImageMutexPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


