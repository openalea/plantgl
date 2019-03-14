/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 2000-2007 UMR CIRAD/INRIA/INRA DAP
 *
 *       File author(s): J. Chopard et al.
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

#ifndef __util_deformation_h__
#define __util_deformation_h__

/*! \file util_deformation.h
    \brief File that contains some deformation utility.
*/

#include "util_vector.h"
#include "linearalgebra.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \brief compute the engineering strain of a given transformation
*/

inline Matrix2 strain(const Matrix2& transformation) {
    return (transpose(transformation)*transformation-Matrix2::IDENTITY)/2.;
}

inline Matrix2 strain(const Vector2& i1,
               const Vector2& j1,
               const Vector2& i2,
               const Vector2& j2) {
    return strain(Matrix2::linearTransformation(i1,j1,i2,j2));
}

/**
    \brief compute the stress associated to a strain using Hook's law
*/
Matrix2 stress (const Matrix2& strain, const Matrix3& material) {
    Vector3 strain_vec(strain(0,0),strain(1,1),strain(0,1));
    Vector3 stress_vec = material * strain_vec;
    return Matrix2(stress_vec.x(),stress_vec.z(),stress_vec.z(),stress_vec.y());
}

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

//__util_deformation_h__
#endif
