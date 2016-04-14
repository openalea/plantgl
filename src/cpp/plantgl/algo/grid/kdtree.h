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
class ALGO_API AbstractKDTree : public TOOLS(RefCountObject)
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

#ifdef WITH_ANN

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