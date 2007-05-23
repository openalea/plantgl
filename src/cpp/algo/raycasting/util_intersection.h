/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) 
 *
 *       $Source$

 *       $Id: util_intersection.h 1045 2002-10-28 21:30:46Z fboudon $
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

 /*! \file util_intersection.h
    \brief Algorithm of intersection.
*/

#ifndef __util_intersection_h__
#define __util_intersection_h__

/* ----------------------------------------------------------------------- */

#include <math/util_vector.h>
#include "../algo_config.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/// Return whether segment [seg1, seg2] intersect [segb1, segb2]
bool ALGO_API intersectSegment(const TOOLS(Vector2)& seg1, const TOOLS(Vector2)& seg2 , 
		      const TOOLS(Vector2)& segb1,const TOOLS(Vector2)& segb2);

/// Return whether segment [seg1, seg2] intersect [segb1, segb2]
bool ALGO_API intersectSegment(const TOOLS(Vector3)& seg1, const TOOLS(Vector3)& seg2 , 
		      const TOOLS(Vector3)& segb1,const TOOLS(Vector3)& segb2);
 
/// Return whether segment [seg1, seg2] intersect the point pt1
bool ALGO_API intersectSegment(const TOOLS(Vector3)& seg1, const TOOLS(Vector3)& seg2 , 
		      const TOOLS(Vector3)& pt1);
 
/// Return whether segment define by [seg1,seg2] intersect triangle made by [triangle1,triangle2,triangle3].
bool ALGO_API intersectSegment(const TOOLS(Vector3)& seg1, const TOOLS(Vector3)& seg2 , 
	       const TOOLS(Vector3)& triangle1,const TOOLS(Vector3)& triangle2, const TOOLS(Vector3)& triangle3 );


/* \fn intersectSegment
  Return whether segment define by [seg1,seg2] intersect quad facet made by [quad1,quad2,quad3,quad4]. 
  \pre quad must be planar.
*/
bool ALGO_API intersectSegment(const TOOLS(Vector3)& seg1, const TOOLS(Vector3)& seg2 , 
	       const TOOLS(Vector3)& quad1,const TOOLS(Vector3)& quad2, const TOOLS(Vector3)& quad3 , const TOOLS(Vector3)& quad4 );

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif // __util_intersection_h__
