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

#include "../gui_config.h"
#include <math/util_vector.h>
#include <tool/util_array2.h>
#include <scenegraph/appearance/color.h>

struct VIEW_API RayHit {
public:
	RayHit (size_t _id,real_t _zmin,real_t _zmax):id(_id),zmin(_zmin),zmax(_zmax){}

	inline bool operator==(const RayHit& phit) const
	{  return phit.id == id &&  phit.zmin == zmin && phit.zmax == zmax; }

	size_t id;
	real_t zmin;
	real_t zmax;
};

typedef std::vector<RayHit > RayHitList;

class VIEW_API ViewRayBuffer : public TOOLS(Array2)<RayHitList>
{
public:
	ViewRayBuffer(size_t w, size_t h): TOOLS(Array2)<RayHitList>(w,h){}

	void setAt(size_t i, size_t j, void * buffer, size_t size,const TOOLS(Vector3)& position) ;
};

struct VIEW_API RayPointHit {
public:
	RayPointHit (size_t _id,
				 const TOOLS(Vector3)& _zmin,
				 const TOOLS(Vector3)& _zmax):
		id(_id),zmin(_zmin),zmax(_zmax){}

	inline bool operator==(const RayPointHit& phit) const
	{  return phit.id == id &&  phit.zmin == zmin && phit.zmax == zmax; }

	size_t id;
	TOOLS(Vector3) zmin;
	TOOLS(Vector3) zmax;
};

typedef std::vector<RayPointHit > RayPointHitList;

class VIEW_API ViewRayPointHitBuffer : public TOOLS(Array2)<RayPointHitList>
{
public:
	ViewRayPointHitBuffer(size_t w, size_t h): TOOLS(Array2)<RayPointHitList>(w,h){}

	/** \brief Add pBuff to \e self.
      \pre
      - \e pBuff and self must have the same size. */
    ViewRayPointHitBuffer& operator+=(const ViewRayPointHitBuffer& pBuff);

	/** \brief Add pBuff to \e self.
      \pre
      - \e pBuff and self must have the same size. */
    ViewRayPointHitBuffer operator+(const ViewRayPointHitBuffer& pBuff)const;
};

struct VIEW_API ZBufferUnit {
public:
	ZBufferUnit(TOOLS(Vector3) _pos = TOOLS(Vector3::ORIGIN), 
			    PGL(Color4) _color = PGL(Color4::BLACK),
				float _depth = 1.0):pos(_pos),color(_color),depth(_depth){}

	inline bool operator==(const ZBufferUnit & pValue) const
	{ return pValue.pos == pos && pValue.color == color && pValue.depth == depth; }

	TOOLS(Vector3) pos;
	PGL(Color4) color;
	float depth;
};

class VIEW_API ViewZBuffer : public TOOLS(Array2)<ZBufferUnit>
{
public:
	ViewZBuffer(size_t w, size_t h): TOOLS(Array2)<ZBufferUnit>(w,h){}

	/// import depth and color buffer
	static ViewZBuffer* importglZBuffer(bool alldepth = true);

	/// import depth buffer
	static ViewZBuffer* importglDepthBuffer(bool alldepth = true);
};


#endif

