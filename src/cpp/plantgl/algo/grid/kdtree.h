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


/*! \file kdtree.h
    \brief Definition of KDTree.
*/



#ifndef __kdtree_h__
#define __kdtree_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class KDTree
    \brief a KDTree definition.
*/

/* ----------------------------------------------------------------------- */

template<class ContainerType>
class ALGO_API AbstractKDTree : public RefCountObject
{
public:
    typedef ContainerType PointContainer;
    typedef typename PointContainer::element_type VectorType;
    typedef RCPtr<PointContainer> PointContainerPtr;

    typedef typename PointContainer::iterator PointIterator;
    typedef typename PointContainer::const_iterator PointConstIterator;




    AbstractKDTree(const PointContainerPtr points) { }

    virtual ~AbstractKDTree() { }


    virtual Index k_closest_points(const VectorType& point, size_t k, real_t maxdist = REAL_MAX) = 0;

    virtual IndexArrayPtr k_nearest_neighbors(size_t k) = 0;

    virtual IndexArrayPtr r_nearest_neighbors(real_t radius) = 0;

    virtual size_t size() const = 0;

};

typedef AbstractKDTree<Point2Array>  AbstractKDTree2;
typedef AbstractKDTree<Point3Array>  AbstractKDTree3;
typedef AbstractKDTree<Point4Array>  AbstractKDTree4;

typedef RCPtr<AbstractKDTree2>       KDTree2Ptr;
typedef RCPtr<AbstractKDTree3>       KDTree3Ptr;
typedef RCPtr<AbstractKDTree4>       KDTree4Ptr;

#ifdef PGL_WITH_ANN

class ANNKDTree2Internal;
class ANNKDTree3Internal;
class ANNKDTree4Internal;

#define ANNKDTREEDECLARATION(basename) \
    class ALGO_API ANN##basename  : public Abstract##basename \
    { \
        public: \
        typedef Abstract##basename::PointContainer ContainerType; \
        typedef Abstract##basename::PointContainerPtr PointContainerPtr; \
        typedef Abstract##basename::VectorType VectorType; \
         \
        ANN##basename(const PointContainerPtr& points); \
        virtual ~ANN##basename(); \
        \
        virtual Index k_closest_points(const VectorType& pointclass, size_t k, real_t maxdist = REAL_MAX);  \
        virtual IndexArrayPtr k_nearest_neighbors(size_t k) ; \
        virtual IndexArrayPtr r_nearest_neighbors(real_t radius) ; \
        virtual size_t size() const; \
        protected: \
        ANN##basename##Internal * __internal; \
    };

ANNKDTREEDECLARATION(KDTree2)
ANNKDTREEDECLARATION(KDTree3)
ANNKDTREEDECLARATION(KDTree4)

typedef RCPtr<ANNKDTree2>       ANNKDTree2Ptr;
typedef RCPtr<ANNKDTree3>       ANNKDTree3Ptr;
typedef RCPtr<ANNKDTree4>       ANNKDTree4Ptr;


typedef ANNKDTree2 KDTree2 ;
typedef ANNKDTree3 KDTree3 ;
typedef ANNKDTree4 KDTree4 ;

#endif

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
