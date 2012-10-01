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

#ifndef __annkdree_p__
#define __annkdree_p__

#include "../algo_config.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>

#ifdef WITH_ANN
#include <ANN/ANN.h>
#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef WITH_ANN

template<class VectorType>
inline void toANNPoint(const VectorType& v, ANNpoint& point)
{
    typedef typename VectorType::const_iterator CoordConstIterator;
    size_t coordid = 0;
    for(CoordConstIterator itC = v.begin(); itC != v.end(); ++itC, ++coordid)
        point[coordid] = *itC;
}

template<class PointArray>
inline ANNpointArray toANNPointArray(const RCPtr<PointArray>& points)
{
    // type definition
    typedef typename PointArray::const_iterator PointConstIterator;
    typedef typename PointArray::element_type VectorType;

    // allocate memory
    uint32_t nbPoints = points->size();
    int dim = VectorType::size();
    ANNpointArray pointdata = annAllocPts(nbPoints,dim);

    uint32_t pointid = 0;
    // Converting PointArray into ANNpointArray
    for(PointConstIterator itp = points->begin(); itp != points->end(); ++itp, ++pointid) {
        toANNPoint(*itp,pointdata[pointid]);
    }

    return pointdata;
}

inline IndexArrayPtr k_nearest_neighbors_of_kdtree_points(ANNkd_tree& kdtree, size_t k)
{
    size_t kk = k+(ANN_ALLOW_SELF_MATCH?1:0);

    ANNpointArray pointdata = kdtree.thePoints();
    size_t nbPoints = kdtree.nPoints();
    int dim = kdtree.theDim();

    // The resulting structure that will contains the K closest points of each point
    IndexArrayPtr res(new IndexArray(nbPoints,Index()));

    /// ANN data structure for query
    ANNidxArray nn_idx = new ANNidx[kk];
    ANNdistArray dists = new ANNdist[kk];

    /// For each point, ask k closest point to  kdtree
    for (size_t pointid = 0; pointid < nbPoints; ++pointid){
        kdtree.annkSearch(pointdata[pointid],kk,nn_idx,dists);
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

template<class PointArray>
IndexArrayPtr k_nearest_neighbors(RCPtr<PointArray> points, size_t k) 
{
   typedef typename PointArray::element_type VectorType;

   ANNpointArray pointdata = toANNPointArray(points);
   ANNkd_tree kdtree(pointdata,points->size(),VectorType::size());
   IndexArrayPtr result = k_nearest_neighbors_of_kdtree_points(kdtree,k);
   annDeallocPts(pointdata);
   return result;
}

template<class ContainerType>
class ANNKDTreeInternal  
{

    typedef ContainerType PointContainer;
    typedef typename PointContainer::element_type VectorType;
    typedef RCPtr<PointContainer> PointContainerPtr;

    
    ANNpointArray __pointdata;
    ANNkd_tree __kdtree;
    size_t __nbpoints;

    public:

        ANNKDTreeInternal(const PointContainerPtr points) : 
            __pointdata(toANNPointArray(points)), 
                        __kdtree(__pointdata,points->size(),VectorType::size()),
                        __nbpoints(points->size()){ }

        virtual ~ANNKDTreeInternal() { 
            if (__pointdata) annDeallocPts(__pointdata);
        }


        Index k_closest_points(const VectorType& point, size_t k, real_t maxdist = REAL_MAX) 
        {
            if (k > __nbpoints) k = __nbpoints;

            /// ANN data structure for query
            ANNidxArray nn_idx = new ANNidx[k];
            ANNdistArray dists = new ANNdist[k];
            ANNpoint queryPoint = annAllocPt(__kdtree.theDim());
            toANNPoint(point,queryPoint);

            size_t kres = k;
            if (maxdist != REAL_MAX)
                kres = std::min<size_t>(k,__kdtree.annkFRSearch(queryPoint,maxdist*maxdist,k,nn_idx,dists));
            else 
                __kdtree.annkSearch(queryPoint,k,nn_idx,dists);

            Index res;
            for(uint32_t i = 0; i < kres; ++i) res.push_back(nn_idx[i]);

            /// delete all allocated structures
            delete [] nn_idx;
            delete [] dists;
            annDeallocPt(queryPoint);

            return res;
        }

        inline IndexArrayPtr k_nearest_neighbors(size_t k) 
        {
            return k_nearest_neighbors_of_kdtree_points(__kdtree,k);
        }

        inline size_t size() { return __nbpoints; }
};



#endif

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif