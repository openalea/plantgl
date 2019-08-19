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


 /*! \file util_intersection.h
    \brief Algorithm of intersection.
*/

#ifndef __util_intersection_h__
#define __util_intersection_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include "../algo_config.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/// Return whether segment [seg1, seg2] intersect [segb1, segb2]
bool ALGO_API intersectSegment(const Vector2& seg1, const Vector2& seg2 ,
              const Vector2& segb1,const Vector2& segb2);

/// Return whether segment [seg1, seg2] intersect [segb1, segb2]
bool ALGO_API intersectSegment(const Vector3& seg1, const Vector3& seg2 ,
              const Vector3& segb1,const Vector3& segb2);

/// Return whether segment [seg1, seg2] intersect the point pt1
bool ALGO_API intersectSegment(const Vector3& seg1, const Vector3& seg2 ,
              const Vector3& pt1);

/// Return whether segment define by [seg1,seg2] intersect triangle made by [triangle1,triangle2,triangle3].
bool ALGO_API intersectSegment(const Vector3& seg1, const Vector3& seg2 ,
           const Vector3& triangle1,const Vector3& triangle2, const Vector3& triangle3 );


/* \fn intersectSegment
  Return whether segment define by [seg1,seg2] intersect quad facet made by [quad1,quad2,quad3,quad4].
  \pre quad must be planar.
*/
bool ALGO_API intersectSegment(const Vector3& seg1, const Vector3& seg2 ,
           const Vector3& quad1,const Vector3& quad2, const Vector3& quad3 , const Vector3& quad4 );

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif // __util_intersection_h__
