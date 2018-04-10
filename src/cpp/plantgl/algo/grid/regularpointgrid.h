/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2009 - Cirad/Inria/Inra - Virtual Plant Team
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

#ifndef __regularpointgrid_h__
#define __regularpointgrid_h__

#include <vector>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/tool/errormsg.h>
#include <plantgl/tool/util_spatialarray.h>


PGL_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

template<class PointContainer>
class ContainerReferencePolicy {
public:
    ContainerReferencePolicy(const PointContainer& data) : __points(data) {}
protected:
    const PointContainer& __points;
};

template<class PointContainer>
class LocalContainerPolicy {
public:
    LocalContainerPolicy(const PointContainer& data) : __points(data) {}
protected:
    const PointContainer __points;
};


template<class VectorType>
bool is_point_in_cone(const VectorType& point, 
                      const VectorType& coneorigin, const VectorType& conedirection,
                      real_t coneradius,  real_t coneangle = GEOM_HALF_PI) 
{
    VectorType mconedirection = conedirection.normed(); 
    real_t cosconeangle = cos(coneangle / 2);
    VectorType pointtoconeorigin = point-coneorigin;
    real_t dist = pointtoconeorigin.normalize();
    if ((dist <= coneradius + GEOM_EPSILON) && (dot(pointtoconeorigin,mconedirection) > (cosconeangle - GEOM_EPSILON)))
        return true;
    return false;
}

/* ----------------------------------------------------------------------- */

template <class PointContainer,
        class ContainerPolicy = LocalContainerPolicy<PointContainer>,
        int NbDimension = TOOLS::Dimension<typename PointContainer::element_type>::Nb >
class PointGrid : public ContainerPolicy, public TOOLS::SpatialArrayN<std::vector<size_t>,typename PointContainer::element_type,NbDimension>
{
public:
    typedef TOOLS::SpatialArrayN<std::vector<size_t>,typename PointContainer::element_type,NbDimension> SpatialBase;
    typedef typename SpatialBase::Base Base;

    typedef PointContainer ContainerType;
    typedef typename PointContainer::element_type VectorType;
    typedef RCPtr<PointContainer> PointContainerPtr;

    typedef typename PointContainer::iterator PointIterator;
    typedef typename PointContainer::const_iterator PointConstIterator;

    typedef size_t PointIndex;
    typedef std::vector<PointIndex> PointIndexList;

    typedef typename SpatialBase::Index Index;
    typedef std::vector<Index> IndexList;

    typedef typename SpatialBase::CellId VoxelId;
    typedef typename SpatialBase::CellIdList VoxelIdList;

    typedef typename SpatialBase::iterator VoxelIterator;
    typedef typename SpatialBase::const_iterator VoxelConstIterator;

    typedef typename Base::const_partial_iterator const_partial_iterator;

    PointGrid(const VectorType& voxelsize,
              const VectorType& minpoint,
              const VectorType& maxpoint,
              const PointContainerPtr& data):
      ContainerPolicy(*data),
      SpatialBase(voxelsize,minpoint,maxpoint){
          registerData(data,0);
    }

    PointGrid(const VectorType& voxelsize,
              const PointContainerPtr& data):
      ContainerPolicy(*data),
      SpatialBase(voxelsize){
          /*for(size_t i = 0; i < NbDimension; ++i)
              assert(__voxelsize[i] > GEOM_EPSILON);*/
          std::pair<VectorType,VectorType> bounds = data->getBounds();
          SpatialBase::initialize(bounds.first,bounds.second,voxelsize);
          registerData(data,0);
    }

    PointGrid(const real_t& voxelsize,
              const PointContainerPtr& data):
      ContainerPolicy(*data),
        SpatialBase(){
          assert(voxelsize > GEOM_EPSILON);
          VectorType _voxelsize;
          for(size_t i = 0; i < NbDimension; ++i)
              _voxelsize[i] = voxelsize;
          std::pair<VectorType,VectorType> bounds = data->getBounds();
          SpatialBase::initialize(bounds.first,bounds.second,_voxelsize);
          registerData(data,0);
    }

    PointGrid(const PointContainerPtr& data,
              const real_t& voxelsizeratiofromglobal):
      ContainerPolicy(*data),
        SpatialBase(){
          assert(voxelsizeratiofromglobal > 1);
          std::pair<VectorType,VectorType> bounds = data->getBounds();
          VectorType _voxelsize;
          for(size_t i = 0; i < NbDimension; ++i)
              _voxelsize[i] = (bounds.second[i] - bounds.first[i]) / voxelsizeratiofromglobal;
          SpatialBase::initialize(bounds.first,bounds.second,_voxelsize);
          registerData(data,0);
    }

    const PointContainer& points() const { return ContainerPolicy::__points; }


    inline  PointContainerPtr getVoxelPoints(const Index& coord) const {
        return getVoxelPoints(this->cellId(coord));
    }

    PointContainerPtr getVoxelPoints(const VoxelId& vid) const{
        size_t len = Base::getAt(vid).size();
        if (len == 0) return new PointContainer();
        PointContainerPtr pts(new PointContainer(len));
        PointIterator itpoints = pts->begin();
        for(PointIndexList::const_iterator itindex = Base::getAt(vid).begin();
            itindex != Base::getAt(vid).end(); ++itindex){
            *itpoints = points()[*itindex];
        }
        return pts;
    }

    inline const PointIndexList& getVoxelPointIndices(const Index& coord) const{
        return Base::getAt(this->cellId(coord));
    }

    inline const PointIndexList& getVoxelPointIndices(const VoxelId& vid) const{
        return Base::getAt(vid);
    }


    PointIndexList query_ball_point(const VectorType& point, real_t radius) const{
        VoxelIdList voxels = this->query_voxels_around_point(point,radius);
        PointIndexList res;
        for(typename VoxelIdList::const_iterator itvoxel = voxels.begin(); itvoxel != voxels.end(); ++itvoxel){
            const PointIndexList& voxelpointlist = Base::getAt(*itvoxel);
            if(!voxelpointlist.empty()){
              for(typename PointIndexList::const_iterator itPointIndex = voxelpointlist.begin(); itPointIndex != voxelpointlist.end(); ++itPointIndex){
                // Check whether point i is in the ball
                if (!(norm(points().getAt(*itPointIndex)-point) > radius))
                    res.push_back(*itPointIndex);
              }
            }
        }
        return res;
    }

    PointIndexList query_points_in_cone(const VectorType& coneorigin, const VectorType& conedirection,
                                       real_t coneradius,  real_t coneangle = GEOM_HALF_PI) const{
        VectorType mdirection = conedirection.normed(); 
        VoxelIdList voxels = this->query_voxels_in_cone(coneorigin,mdirection,coneradius,coneangle);
        PointIndexList res;
        real_t cosconeangle = cos(coneangle / 2);
        for(typename VoxelIdList::const_iterator itvoxel = voxels.begin(); itvoxel != voxels.end(); ++itvoxel){
            const PointIndexList& voxelpointlist = Base::getAt(*itvoxel);
            if(!voxelpointlist.empty()){
              for(typename PointIndexList::const_iterator itPointIndex = voxelpointlist.begin(); itPointIndex != voxelpointlist.end(); ++itPointIndex){
                // Check whether point i is in the cone
                VectorType pointtoconeorigin = points().getAt(*itPointIndex)-coneorigin;
                real_t dist = pointtoconeorigin.normalize();
                if ((dist <= coneradius + GEOM_EPSILON) && (dot(pointtoconeorigin,mdirection) > (cosconeangle - GEOM_EPSILON)))
                    res.push_back(*itPointIndex);
              }
            }
        }
        return res;
    }

    bool closest_point(const VectorType& point, PointIndex& result, real_t maxdist = REAL_MAX) const{
        Index centervxl = this->indexFromPoint(point);
        real_t radius = maxdist;
        Index maxindexdist = SpatialBase::getMaxIndexDistanceToBorder(centervxl);
        if (maxdist < REAL_MAX){
            for (size_t i = 0; i < NbDimension; ++i){
                real_t maxinddist = std::max<real_t>(1,maxdist / SpatialBase::__voxelsize[i]);
                if (real_t(maxindexdist[i]) > maxinddist)
                    maxindexdist[i] =  size_t(maxinddist);
            }
        }
        size_t maxiter = *maxindexdist.getMax();
        size_t iter = 0;

        while (radius == maxdist && iter < maxiter){
            // iter throught box layers of voxels
            VoxelIdList voxelids = this->query_voxels_in_box(centervxl,Index(iter),Index(iter));
            for(typename VoxelIdList::const_iterator itVoxel = voxelids.begin(); 
                itVoxel != voxelids.end(); ++itVoxel){
                // iter throught points
                const PointIndexList& voxelpointlist = getVoxelPointIndices(*itVoxel);
                for(typename PointIndexList::const_iterator itPointIndex = voxelpointlist.begin(); 
                    itPointIndex != voxelpointlist.end(); ++itPointIndex){
                    // Find closest point in voxel
                    real_t dist = norm(points().getAt(*itPointIndex)-point);
                    if (dist < radius){
                        radius = dist;
                        result = *itPointIndex;
                    }
                }
            }
            if (radius < maxdist){
                VectorType borderdist = SpatialBase::getVoxelSize()/2 - abs(point-SpatialBase::getVoxelCenter(centervxl));
                real_t initialvoxelenclosedballradius = *(borderdist.getMin());
                // check what is the enclosed ball by the box and if point is inside
                real_t enclosedballradius = (*SpatialBase::getVoxelSize().getMin()*iter)+initialvoxelenclosedballradius;
                if (radius > enclosedballradius){
                    // other points not in the box but in the sphere can be closer
                    VoxelIdList voxels = this->query_voxels_around_point(point,radius,enclosedballradius);
                    for(typename VoxelIdList::const_iterator itvoxel = voxels.begin(); 
                        itvoxel != voxels.end(); ++itvoxel){
                        const PointIndexList& voxelpointlist = Base::getAt(*itvoxel);
                        if(!voxelpointlist.empty()){
                            for(typename PointIndexList::const_iterator itPointIndex = 
                                voxelpointlist.begin(); itPointIndex != voxelpointlist.end(); 
                                ++itPointIndex){
                                // Find closest point in voxel
                                real_t dist = norm(points().getAt(*itPointIndex)-point);
                                if (dist < radius){
                                    radius = dist;
                                    result = *itPointIndex;
                                }
                            }
                        }
                    }
                }
            }
            iter += 1;
        }
        return radius < maxdist;
    }


    bool disable_point(PointIndex pid) {
        VectorType point = points().getAt(pid);
        PointIndexList& voxelpointlist = this->getAt(this->cellIdFromPoint(point));
        typename PointIndexList::iterator itPointIndex = std::find(voxelpointlist.begin(),voxelpointlist.end(),pid);
        if (itPointIndex != voxelpointlist.end()) { voxelpointlist.erase(itPointIndex); return true; }
        return false;
    }

    bool enable_point(PointIndex pid) {
        VectorType point = points().getAt(pid);
        PointIndexList& voxelpointlist = this->getAt(this->cellIdFromPoint(point));
        typename PointIndexList::const_iterator itPointIndex = std::find(voxelpointlist.begin(),voxelpointlist.end(),pid);
        if (itPointIndex == voxelpointlist.end()) { voxelpointlist.push_back(pid); return true; }
        return false;
    }

    bool is_point_enabled(PointIndex pid) const {
        VectorType point = points().getAt(pid);
        const PointIndexList& voxelpointlist = this->getAt(this->cellIdFromPoint(point));
        typename PointIndexList::const_iterator itPointIndex = std::find(voxelpointlist.begin(),voxelpointlist.end(),pid);
        if (itPointIndex == voxelpointlist.end()) { return false; }
        return true;
    }

    inline void disable_points(const PointIndexList& pids) 
    { disable_points(pids.begin(), pids.end()); }

    template<class ConstIterator>
    void disable_points(ConstIterator begin, ConstIterator end) {
            for(ConstIterator itPointIndex = begin; itPointIndex != end; ++itPointIndex){
                    disable_point(*itPointIndex);
            }
    }

    inline void enable_points(const PointIndexList& pids) 
    {  enable_points(pids.begin(), pids.end()); }

    template<class ConstIterator>
    void enable_points(ConstIterator begin, ConstIterator end) {
            for(ConstIterator itPointIndex = begin; itPointIndex != end; ++itPointIndex){
                    enable_point(*itPointIndex);
            }
    }

    PointContainerPtr get_enabled_points() const {
		PointContainerPtr result(new PointContainer());
        for(PointIndex itPointIndex = 0; itPointIndex < points().size(); ++itPointIndex){
				if(is_point_enabled(itPointIndex)){
					result->push_back(points()[itPointIndex]);
				}
        }
		return result; 
	}

    PointContainerPtr get_disabled_points() const {
		PointContainerPtr result(new PointContainer());
        for(PointIndex itPointIndex = 0; itPointIndex < points().size(); ++itPointIndex){
				if(!is_point_enabled(itPointIndex)){
					result->push_back(points()[itPointIndex]);
				}
        }
		return result; 
	}

    PointIndexList get_enabled_point_indices() const {
		PointIndexList result;
        for(PointIndex itPointIndex = 0; itPointIndex < points().size(); ++itPointIndex){
				if(is_point_enabled(itPointIndex)){
					result.push_back(itPointIndex);
				}
        }
		return result; 
	}

    PointIndexList get_disabled_point_indices() const {
		PointIndexList result;
        for(PointIndex itPointIndex = 0; itPointIndex < points().size(); ++itPointIndex){
				if(!is_point_enabled(itPointIndex)){
					result.push_back(itPointIndex);
				}
        }
		return result; 
	}


    template<class Array>
    Array filter_disabled(const Array& pointlist) const {
        Array result;
        for(typename Array::const_iterator itp = pointlist.begin(); itp != pointlist.end(); ++itp){
            if(is_point_enabled(*itp)) result.push_back(*itp);
        }
        return result; 
    }

    template<class Array>
    Array filter_enabled(const Array& pointlist) const {
        Array result;
        for(typename Array::const_iterator itp = pointlist.begin(); itp != pointlist.end(); ++itp){
            if(!is_point_enabled(*itp)) result.push_back(*itp);
        }
        return result; 
    }

    size_t nbFilledVoxels() const {
        size_t count = 0;
        for(typename Base::const_iterator it = Base::begin(); it != Base::end(); ++it)
            if(!it->empty())++count;
        return count;
    }

protected:
    template<class Iterator>
    inline void registerData(Iterator beg, Iterator end, PointIndex startingindex){
        for(Iterator it = beg; it != end; ++it){
            VoxelId vid = this->cellIdFromPoint(*it);
            this->getAt(vid).push_back(startingindex);
            startingindex++;
        }
    }

    inline  void registerData(const PointContainerPtr& data, PointIndex startingindex){
        registerData(data->begin(),data->end(),startingindex);
    }

	/*
	inline void print_index(const std::string& before, 
					   const Index& index,
					   const std::string& after = "\n") const{
		printf("%s (",before.c_str());
		for (size_t i = 0; i < NbDimension-1; ++i){
			printf("%i,",index[i]);
		}
		printf("%i) %s",index[NbDimension-1],after.c_str());
	}*/
		

};

template <class PointContainer, int NbDimension = TOOLS::Dimension<typename PointContainer::element_type>::Nb >
class PointRefGrid : public PointGrid<PointContainer,ContainerReferencePolicy<PointContainer>,  NbDimension>
{
public:
    typedef typename PointContainer::element_type VectorType;
    typedef RCPtr<PointContainer> PointContainerPtr;
    typedef PointGrid<PointContainer,ContainerReferencePolicy<PointContainer>, NbDimension> ParentGridType;

    PointRefGrid(const VectorType& voxelsize,
              const VectorType& minpoint,
              const VectorType& maxpoint,
              const PointContainerPtr& data):
        ParentGridType(voxelsize, minpoint, maxpoint, data)
    {
    }

    PointRefGrid(const VectorType& voxelsize,
              const PointContainerPtr& data):
        ParentGridType(voxelsize,data)
    {
    }

    PointRefGrid(const real_t& voxelsize,
              const PointContainerPtr& data):
        ParentGridType(voxelsize, data)
    {
    }
};

/* ----------------------------------------------------------------------- */

typedef PointGrid<Point2Array> Point2Grid;
typedef PointGrid<Point3Array> Point3Grid;
typedef PointGrid<Point4Array> Point4Grid;
typedef RCPtr<Point2Grid> Point2GridPtr;
typedef RCPtr<Point3Grid> Point3GridPtr;
typedef RCPtr<Point4Grid> Point4GridPtr;

typedef PointRefGrid<Point2Array> Point2RefGrid;
typedef PointRefGrid<Point3Array> Point3RefGrid;
typedef PointRefGrid<Point4Array> Point4RefGrid;

typedef RCPtr<Point2RefGrid> Point2RefGridPtr;
typedef RCPtr<Point3RefGrid> Point3RefGridPtr;
typedef RCPtr<Point4RefGrid> Point4RefGridPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE
#endif
