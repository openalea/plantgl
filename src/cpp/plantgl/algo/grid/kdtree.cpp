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

/* ----------------------------------------------------------------------- */

#ifdef WITH_ANN
#include <ANN/ANN.h>
#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef WITH_ANN

template<class PointContainer>
class ANNKDTreeInternal  
{

    typedef PointContainer ContainerType;
    typedef typename PointContainer::element_type VectorType;
    typedef RCPtr<PointContainer> PointContainerPtr;

    typedef typename PointContainer::iterator PointIterator;
    typedef typename PointContainer::const_iterator PointConstIterator;
    
    ANNpointArray __pointdata;
    ANNkd_tree * __kdtree;

    public:

        ANNKDTreeInternal(const PointContainerPtr points) : 
            __pointdata(0), __kdtree(0) { build(points); }

        ~ANNKDTreeInternal() { 
            if (__pointdata) annDeallocPts(__pointdata);
            if (__kdtree) delete __kdtree;
        }


        Index k_closest_points(const VectorType& point, size_t k, real_t maxdist = REAL_MAX) const
        {
            /// ANN data structure for query
            ANNidxArray nn_idx = new ANNidx[k];
            ANNdistArray dists = new ANNdist[k];
            ANNpoint queryPoint = annAllocPt(__kdtree->theDim());

            size_t kres = k;
            if (maxdist != REAL_MAX)
                kres = __kdtree->annkFRSearch(queryPoint,maxdist*maxdist,k,nn_idx,dists);
            else 
                __kdtree->annkSearch(queryPoint,k,nn_idx,dists);

            Index res;
            for(uint32_t i = 0; i < kres; ++i) res.push_back(nn_idx[i]);

            /// delete all allocated structures
            delete [] nn_idx;
            delete [] dists;
            annDeallocPt(queryPoint);

            return res;
        }

        IndexArrayPtr k_nearest_neighbors(size_t k) const
        {
            size_t kk = k+(ANN_ALLOW_SELF_MATCH?1:0);

            size_t nbPoints = __kdtree->nPoints();
            int dim = __kdtree->theDim();

            // The resulting structure that will contains the K closest points of each point
            IndexArrayPtr res(new IndexArray(nbPoints,Index()));

            /// ANN data structure for query
            ANNidxArray nn_idx = new ANNidx[kk];
            ANNdistArray dists = new ANNdist[kk];

            /// For each point, ask k closest point to  kdtree
            for (size_t pointid = 0; pointid < nbPoints; ++pointid){
                __kdtree->annkSearch(__pointdata[pointid],kk,nn_idx,dists);
                Index& pointres = res->getAt(pointid);
                for(uint32_t i = 0; i < kk; ++i) 
                    if (nn_idx[i] != pointid)
                        pointres.push_back(nn_idx[i]);
            }

            /// delete all allocated structures
            delete [] nn_idx;
            delete [] dists;

            return res;
        }

protected:
        inline void toANNPoint(const VectorType& v, ANNpoint& point)
        {
            typedef typename VectorType::const_iterator CoordConstIterator;
            size_t coordid = 0;
            for(CoordConstIterator itC = v.begin(); itC != v.end(); ++itC, ++coordid)
                point[coordid] = *itC;
        }

        void build(const PointContainerPtr points)
        {
            uint32_t nbPoints = points->size();
            int dim = VectorType::size();
            __pointdata = annAllocPts(nbPoints,dim);
            uint32_t pointid = 0;

            // Converting PointArray into ANNpointArray
            for(PointConstIterator itp = points->begin(); itp != points->end(); ++itp, ++pointid) {
                toANNPoint(*itp,__pointdata[pointid]);
            }

            __kdtree = new ANNkd_tree(__pointdata,nbPoints,dim);
        }

};

// for instanciation

#define ANNKDTREEDECLARATIONCORE(basename,pointarraytype) \
    class ANN##basename##Internal : public ANNKDTreeInternal<pointarraytype> { public: ANN##basename##Internal(const RCPtr<pointarraytype>& points) : ANNKDTreeInternal<pointarraytype>(points) {} }; \
    \
    PGL::ANN##basename::ANN##basename(RCPtr<pointarraytype> const & points) : \
            basename(points), __internal(new ANN##basename##Internal(points)) { ; } \
    \
    PGL::ANN##basename::~ANN##basename() { delete __internal; } \
    \
    Index PGL::ANN##basename::k_closest_points(const VectorType& pointclass, size_t k, real_t maxdist ) const \
    { return __internal->k_closest_points(pointclass, k, maxdist) ; } \
    \
    IndexArrayPtr PGL::ANN##basename::k_nearest_neighbors(size_t k) const \
    { return __internal->k_nearest_neighbors(k); } \

ANNKDTREEDECLARATIONCORE(KDTree2,Point2Array)
ANNKDTREEDECLARATIONCORE(KDTree3,Point3Array)
ANNKDTREEDECLARATIONCORE(KDTree4,Point4Array)


#endif

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

