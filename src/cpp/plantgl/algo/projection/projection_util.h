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

/*! \file projection_util.h
    \brief Definition of Rendering Engine based on ZBuffer.
*/



#ifndef __ProjectionUtil_h__
#define __ProjectionUtil_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include "../algo_config.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


inline real_t edgeFunction(const TOOLS(Vector3) &a, const TOOLS(Vector3) &b, const TOOLS(Vector3) &c, bool ccw = true)
{ 
    if (!ccw)
        return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x()); 
    else 
        return (a.x() - b.x()) * (c.y() - a.y()) - (a.y() - b.y()) * (c.x() - a.x()); 
}

inline real_t edgeFunction(const TOOLS(Vector3) &a, const TOOLS(Vector3) &b, const TOOLS(Vector2) &c, bool ccw = true)
{ 
    if (!ccw)
        return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x()); 
    else 
        return (a.x() - b.x()) * (c.y() - a.y()) - (a.y() - b.y()) * (c.x() - a.x()); 
}

/* ----------------------------------------------------------------------- */


PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

