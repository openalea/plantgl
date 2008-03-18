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

#ifndef __linear_algebra_h__
#define __linear_algebra_h__

/*! \file linearalgebra.h
    \brief File that contains some math utility.
*/

#include "util_math.h"
#include "util_matrix.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \brief compute singular value decomposition of a symmetric Matrix2
	\pre matrix is symmetric
*/

Matrix2 svd (const Matrix2& matrix, real_t& lambda1, real_t& lambda2 ) {
	real_t tr = trace(matrix);
	real_t delta = sqrt(tr*tr-4*det(matrix));
	lambda1=(tr-delta)/2.;
	lambda2=(tr+delta)/2.;

    if (fabs(delta) < GEOM_EPSILON) {
        return Matrix2::IDENTITY;
    }
    else {
		real_t diff=(matrix(0,0)-matrix(1,1))/(2.*delta);
		real_t ctheta=sqrt(0.5-diff);
	    real_t stheta=sqrt(0.5+diff);
		if (matrix(0,1) < 0 )
			stheta*=-1;
        return Matrix2(ctheta,-stheta,stheta,ctheta); 
    }
}

/*
def space_restriction_matrix (vec1, vec2) :
	"""
	construct the projection matrix of a Vector3
	on the space defined by vec1 and vec2
	"""
	b1=vec1/norm(vec1)
	normal=b1^vec2
	b2=normal^b1
	b2.normalize()
	return array([tuple(b1),tuple(b2)])
    */


/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

//__linear_algebra_h__
#endif 
