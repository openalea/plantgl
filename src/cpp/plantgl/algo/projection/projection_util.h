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


inline real_t edgeFunction(const Vector3 &a, const Vector3 &b, const Vector3 &c, bool ccw = true)
{ 
    if (ccw)
        return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x()); 
    else 
        return (a.x() - b.x()) * (c.y() - a.y()) - (a.y() - b.y()) * (c.x() - a.x()); 
}

inline real_t edgeFunction(const Vector3 &a, const Vector3 &b, const Vector2 &c, bool ccw = true)
{ 
    if (ccw)
        return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x()); 
    else 
        return (a.x() - b.x()) * (c.y() - a.y()) - (a.y() - b.y()) * (c.x() - a.x()); 
}

/* ----------------------------------------------------------------------- */


PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

