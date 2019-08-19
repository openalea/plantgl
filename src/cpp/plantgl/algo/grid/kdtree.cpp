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

