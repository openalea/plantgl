/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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


/*! \file view_zbuffer.h
    \brief Definition of the viewer class ViewGLFrame.
*/



#ifndef __view_zbuffer_h__
#define __view_zbuffer_h__

#include <vector>

/* ----------------------------------------------------------------------- */

#include "Tools/config.h"
#include "Tools/util_vector.h"
#include "util_array2.h"

#include "appe_color.h"

struct GEOM_API RayIntersection {
public:
	RayIntersection (size_t _id,real_t _zmin,real_t _zmax):id(_id),zmin(_zmin),zmax(_zmax){}

	inline bool operator==(const RayIntersection & pIntersection) const
	{ 
		return pIntersection.id == id && 
			   pIntersection.zmin == zmin && 
			   pIntersection.zmax == zmax;
	}

	size_t id;
	real_t zmin;
	real_t zmax;
};

typedef std::vector<RayIntersection > RayIntersections;

class GEOM_API ViewRayBuffer : public GEOM(Array2)<RayIntersections>
{
public:
	ViewRayBuffer(size_t w, size_t h): GEOM(Array2)<RayIntersections>(w,h){}

	void setAt(size_t i, size_t j, void * buffer, size_t size,const TOOLS(Vector3)& position) ;
};

struct GEOM_API ZBufferUnit {
public:
	ZBufferUnit(TOOLS(Vector3) _pos = TOOLS(Vector3::ORIGIN), 
			    GEOM(Color4) _color = GEOM(Color4::BLACK),
				float _depth = 1.0):pos(_pos),color(_color),depth(_depth){}

	inline bool operator==(const ZBufferUnit & pValue) const
	{ return pValue.pos == pos && pValue.color == color && pValue.depth == depth; }

	TOOLS(Vector3) pos;
	GEOM(Color4) color;
	float depth;
};

class GEOM_API ViewZBuffer : public GEOM(Array2)<ZBufferUnit>
{
public:
	ViewZBuffer(size_t w, size_t h): GEOM(Array2)<ZBufferUnit>(w,h){}

	static ViewZBuffer* importglZBuffer();
};


#endif

