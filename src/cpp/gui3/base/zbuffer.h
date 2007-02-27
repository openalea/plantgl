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
    \brief Definition of the viewer class ViewGLFrame3.
*/



#ifndef __view_zbuffer_h__
#define __view_zbuffer_h__

#include <vector>

/* ----------------------------------------------------------------------- */

#include "../gui3_config.h"
#include <math/util_vector.h>
#include <tool/util_array2.h>
#include <scenegraph/appearance/color.h>

struct VIEW3_API RayHit3 {
public:
	RayHit3 (size_t _id,real_t _zmin,real_t _zmax):id(_id),zmin(_zmin),zmax(_zmax){}

	inline bool operator==(const RayHit3& phit) const
	{  return phit.id == id &&  phit.zmin == zmin && phit.zmax == zmax; }

	size_t id;
	real_t zmin;
	real_t zmax;
};

typedef std::vector<RayHit3 > RayHitList3;

class VIEW3_API ViewRayBuffer3 : public TOOLS(Array2)<RayHitList3>
{
public:
	ViewRayBuffer3(size_t w, size_t h): TOOLS(Array2)<RayHitList3>(w,h){}

	void setAt(size_t i, size_t j, void * buffer, size_t size,const TOOLS(Vector3)& position) ;
};

struct VIEW3_API RayPointHit3 {
public:
	RayPointHit3 (size_t _id,
				 const TOOLS(Vector3)& _zmin,
				 const TOOLS(Vector3)& _zmax):
		id(_id),zmin(_zmin),zmax(_zmax){}

	inline bool operator==(const RayPointHit3& phit) const
	{  return phit.id == id &&  phit.zmin == zmin && phit.zmax == zmax; }

	size_t id;
	TOOLS(Vector3) zmin;
	TOOLS(Vector3) zmax;
};

typedef std::vector<RayPointHit3 > RayPointHitList3;

class VIEW3_API ViewRayPointHitBuffer3 : public TOOLS(Array2)<RayPointHitList3>
{
public:
	ViewRayPointHitBuffer3(size_t w, size_t h): TOOLS(Array2)<RayPointHitList3>(w,h){}

	/** \brief Add pBuff to \e self.
      \pre
      - \e pBuff and self must have the same size. */
    ViewRayPointHitBuffer3& operator+=(const ViewRayPointHitBuffer3& pBuff);

	/** \brief Add pBuff to \e self.
      \pre
      - \e pBuff and self must have the same size. */
    ViewRayPointHitBuffer3 operator+(const ViewRayPointHitBuffer3& pBuff)const;
};

struct VIEW3_API ZBufferUnit3 {
public:
	ZBufferUnit3(TOOLS(Vector3) _pos = TOOLS(Vector3::ORIGIN), 
			    PGL(Color4) _color = PGL(Color4::BLACK),
				float _depth = 1.0):pos(_pos),color(_color),depth(_depth){}

	inline bool operator==(const ZBufferUnit3 & pValue) const
	{ return pValue.pos == pos && pValue.color == color && pValue.depth == depth; }

	TOOLS(Vector3) pos;
	PGL(Color4) color;
	float depth;
};

class VIEW3_API ViewZBuffer3 : public TOOLS(Array2)<ZBufferUnit3>
{
public:
	ViewZBuffer3(size_t w, size_t h): TOOLS(Array2)<ZBufferUnit3>(w,h){}

	/// import depth and color buffer
	static ViewZBuffer3* importglZBuffer(bool alldepth = true);

	/// import depth buffer
	static ViewZBuffer3* importglDepthBuffer(bool alldepth = true);
};


#endif

