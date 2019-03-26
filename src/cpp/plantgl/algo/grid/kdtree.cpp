/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2012 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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



/* ----------------------------------------------------------------------- */

#include "kdtree.h"
#include "annkdtree_p.h"

PGL_USING_NAMESPACE
/* ----------------------------------------------------------------------- */
#ifdef WITH_ANN

// for instanciation
#define ANNKDTREEDECLARATIONCORE(basename,pointarraytype) \
    PGL_BEGIN_NAMESPACE \
    class ANN##basename##Internal : public PGL(ANNKDTreeInternal)<pointarraytype> \
    { public: \
        ANN##basename##Internal(const RCPtr<pointarraytype>& points) : \
            PGL(ANNKDTreeInternal)<pointarraytype>(points) {} \
    }; \
    PGL_END_NAMESPACE \
    \
    PGL(ANN##basename)::ANN##basename(RCPtr<pointarraytype> const & points) : \
            Abstract##basename(points), __internal(new ANN##basename##Internal(points)) { ; } \
    \
    PGL(ANN##basename)::~ANN##basename() { delete __internal; } \
    \
    Index PGL(ANN##basename)::k_closest_points(const VectorType& pointclass, size_t k, real_t maxdist )  \
    { return __internal->k_closest_points(pointclass, k, maxdist) ; } \
    \
    IndexArrayPtr PGL(ANN##basename)::k_nearest_neighbors(size_t k)  \
    { return __internal->k_nearest_neighbors(k); } \
    \
    IndexArrayPtr PGL(ANN##basename)::r_nearest_neighbors(real_t radius)  \
    { return __internal->r_nearest_neighbors(radius); } \
    \
    size_t PGL(ANN##basename)::size()  const \
    { return __internal->size(); } \

        // virtual ~ANN##basename##Internal() {printf("delete internal 0\n"); } \

ANNKDTREEDECLARATIONCORE(KDTree2,Point2Array)
ANNKDTREEDECLARATIONCORE(KDTree3,Point3Array)
ANNKDTREEDECLARATIONCORE(KDTree4,Point4Array)

// PGL_END_NAMESPACE

#endif

/* ----------------------------------------------------------------------- */

