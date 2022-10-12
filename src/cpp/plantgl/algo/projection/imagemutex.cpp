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

#include "imagemutex.h"

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

ImageMutex::ImageMutex(uint16_t imageWidth, uint16_t imageHeight):
    RefCountObject(), 
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __mutexarray(imageWidth, imageHeight)
{
    // printf("ImageMutex\n");
}

ImageMutex::~ImageMutex() 
{
}
    
void ImageMutex::lock(uint_t x, uint_t y)
{
    // printf("lock %i %i\n",x,y);
    __mutexarray.getAt(x,y).lock();
}

void ImageMutex::unlock(uint_t x, uint_t y)
{
    __mutexarray.getAt(x,y).unlock();
}

bool ImageMutex::tryLock(uint_t x, uint_t y)
{
    return __mutexarray.getAt(x,y).tryLock();
}

