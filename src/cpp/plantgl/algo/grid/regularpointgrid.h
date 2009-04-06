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

#include <list>
#include <vector>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/tool/errormsg.h>


PGL_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

template<class T> class Dimension  {};

template<> class Dimension<TOOLS::Vector2> 
{ public: static const int Nb = 2; };

template<> class Dimension<TOOLS::Vector3> 
{ public: static const int Nb = 3; };

template<> class Dimension<TOOLS::Vector4> 
{ public: static const int Nb = 4; };

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


/* ----------------------------------------------------------------------- */

template <class PointContainer,
        class ContainerPolicy = LocalContainerPolicy<PointContainer>,
        int NbDimension = Dimension<typename PointContainer::element_type>::Nb >
class PointGrid : public ContainerPolicy 
{
public:
	typedef PointContainer ContainerType;
	typedef typename PointContainer::element_type VectorType;
	typedef RCPtr<PointContainer> PointContainerPtr;

	typedef typename PointContainer::iterator PointIterator;
	typedef typename PointContainer::const_iterator PointConstIterator;
	
	typedef size_t PointIndex;
	typedef size_t VoxelCoordinate;
	typedef std::vector<VoxelCoordinate> VoxelCoordinates;
	typedef size_t VoxelId;
	typedef std::vector<VoxelId> VoxelIdList;
	typedef std::vector<PointIndex> PointIndexList;

	typedef std::vector<PointIndexList> VoxelList;
	typedef typename VoxelList::iterator VoxelIterator;
	typedef typename VoxelList::const_iterator VoxelConstIterator;

	PointGrid(const VectorType& voxelsize,
			  const VectorType& minpoint, 
			  const VectorType& maxpoint,
			  const PointContainerPtr& data):
	  ContainerPolicy(*data),
	  __voxelsize(voxelsize){
		  initialize(minpoint,maxpoint);
		  registerData(data,0);
	}

	PointGrid(const VectorType& voxelsize,
			  const PointContainerPtr& data):
	  ContainerPolicy(*data),
	  __voxelsize(voxelsize){
		  for(size_t i = 0; i < NbDimension; ++i) 
			  assert(__voxelsize[i] > GEOM_EPSILON);
		  std::pair<VectorType,VectorType> bounds = data->getBounds();
		  initialize(bounds.first,bounds.second);
		  registerData(data,0);
	}

	PointGrid(const real_t& voxelsize,
			  const PointContainerPtr& data):
	  ContainerPolicy(*data),
	  __voxelsize(voxelsize){
		  for(size_t i = 1; i < NbDimension; ++i)
			  __voxelsize[i] = voxelsize;
		  assert(voxelsize > GEOM_EPSILON);
		  std::pair<VectorType,VectorType> bounds = data->getBounds();
		  initialize(bounds.first,bounds.second);
		  registerData(data,0);
	}
        const PointContainer& points() const { return ContainerPolicy::__points; }

	VoxelCoordinates getVoxelCoordFromPoint(const VectorType& point) const{
		VoxelCoordinates coord(NbDimension);
		VectorType m = point-__origin;
		for (size_t i = 0; i < NbDimension; ++i){
			coord[i] = m[i]/__voxelsize[i];
		}
		// assert(isValidCoord(coord));
		return coord;
	}

	VoxelId getVoxelIdFromCoord(const VoxelCoordinates& coord) const {
		size_t offset = 1;
		VoxelId v = 0;
		for (size_t i = 0; i < NbDimension; ++i){
			size_t j = NbDimension-1-i;
			v += coord[j] * offset;
			offset *= __dimension[j];
		}
		// assert(isValidId(v));
		return v;		
	}

	 VoxelCoordinates getVoxelCoordFromId(VoxelId coord) const {
		size_t offset[NbDimension];
		size_t offsetcount = 1;
		for (size_t i = 0; i < NbDimension; ++i){
			size_t j = NbDimension-1-i;
			offset[j] = offsetcount;
			offsetcount *= __dimension[j];
		}
		VoxelCoordinates v(NbDimension);
		for (size_t i = 0; i < NbDimension; ++i){
			v[i] = coord / offset[i];
			coord -=  v[i] * offset[i];
		}
		return v;		
	}

	inline VoxelId getVoxelIdFromPoint(const VectorType& point) const {
		return getVoxelIdFromCoord(getVoxelCoordFromPoint(point));
	}

	inline bool isValidId(const VoxelId& vid) const { return vid < __voxels.size(); }

	inline bool isValidCoord(const VoxelCoordinates& coord) const {
		for (size_t i = 0; i < NbDimension; ++i){
			if (coord[i] >= __dimension[i]) return false;
		}
		return true;
	}

	VectorType getVoxelCenter(const VoxelCoordinates& coord) const{
		VectorType res = __origin;
		for (size_t i = 0; i < NbDimension; ++i){
			res[i] += (coord[i]+0.5) * __voxelsize[i];
		}
		return res;
	}

	inline VectorType getVoxelCenter(const VoxelId& vid) const {
		return getVoxelCenter(getVoxelCoordFromId(vid));
	}

	inline  PointContainerPtr getVoxelPoints(const VoxelCoordinates& coord) const {
		return getVoxelPoints(getVoxelIdFromCoord(coord));
	}

	PointContainerPtr getVoxelPoints(const VoxelId& vid) const{
		size_t len = __voxels[vid].size();
		if (len == 0) return new PointContainer();
		PointContainerPtr pts(new PointContainer(len));
		PointIterator itpoints = pts->begin();
		for(PointIndexList::const_iterator itindex = __voxels[vid].begin(); 
			itindex != __voxels[vid].end(); ++itindex){
			*itpoints = points()[*itindex];
		}
		return pts;
	}

	inline const PointIndexList& getVoxelPointIndices(const VoxelCoordinates& coord) const{
		return __voxels[getVoxelIdFromCoord(coord)];
	}

	inline const PointIndexList& getVoxelPointIndices(const VoxelId& vid) const{
		return __voxels[vid];
	}

	VoxelIdList get_voxel_around_point(const VectorType& point, real_t radius, bool filterEmpty = true) const {
		VoxelIdList res;
		VoxelCoordinates centervxl = getVoxelCoordFromPoint(point);
		// discretize radius in term of voxel size
		VoxelCoordinates radiusvoxelsize(NbDimension);
		for (size_t i = 0; i < NbDimension; ++i){
			radiusvoxelsize[i] = 1+(radius/__voxelsize[i]);
		}

		// find min and max voxel coordinates
		VoxelCoordinates mincoord = centervxl;
		VoxelCoordinates maxcoord = centervxl;
		for (size_t i = 0; i < NbDimension; ++i){
			mincoord[i] = std::max<int>(0,mincoord[i]-radiusvoxelsize[i]);
			maxcoord[i] = std::min<int>(__dimension[i],maxcoord[i]+radiusvoxelsize[i]);
		}

		VoxelCoordinates coord = mincoord;
		real_t r = radius + norm(__voxelsize);
		bool stop = false;
		while(!stop){			
			// Check whether coord is in ball
			if (!(norm(getVoxelCenter(coord)-point) > r) ){
				VoxelId vxlid = getVoxelIdFromCoord(coord);
				if(!filterEmpty || !__voxels[vxlid].empty())
					res.push_back(vxlid);
			}
			// increment coord
			for (size_t i = 0; i < NbDimension; ++i){
				coord[i]+=1;
				if(coord[i] >= maxcoord[i]) {
					if (i == (NbDimension-1)) stop = true;
					else coord[i] = mincoord[i];
				}
				else break;
			}
		}
		return res;
	}

	PointIndexList query_ball_point(const VectorType& point, real_t radius) const{
		VoxelIdList voxels = get_voxel_around_point(point,radius);
		PointIndexList res;
		for(VoxelIdList::const_iterator itvoxel = voxels.begin(); itvoxel != voxels.end(); ++itvoxel){
			const PointIndexList& voxelpointlist = __voxels[*itvoxel];
			for(typename PointIndexList::const_iterator itPointIndex = voxelpointlist.begin(); itPointIndex != voxelpointlist.end(); ++itPointIndex){
			    // Check whether point i is in the ball
				if (!(norm(points().getAt(*itPointIndex)-point) > radius))
					res.push_back(*itPointIndex);
			}
		}
		return res;
	}

	bool disable_point(PointIndex pid) {
		VectorType point = points().getAt(pid);
		PointIndexList& voxelpointlist = __voxels[getVoxelIdFromPoint(point)];
		typename PointIndexList::iterator itPointIndex = std::find(voxelpointlist.begin(),voxelpointlist.end(),pid);
		if (itPointIndex != voxelpointlist.end()) { voxelpointlist.erase(itPointIndex); return true; }
		return false;
	}

	bool enable_point(PointIndex pid) {
		VectorType point = points().getAt(pid);
		PointIndexList& voxelpointlist = __voxels[getVoxelIdFromPoint(point)];
		typename PointIndexList::iterator itPointIndex = std::find(voxelpointlist.begin(),voxelpointlist.end(),pid);
		if (itPointIndex == voxelpointlist.end()) { voxelpointlist.push_back(pid); return true; }
		return false;
	}

	void disable_points(const PointIndexList& pids) {
			for(typename PointIndexList::const_iterator itPointIndex = pids.begin(); 
				itPointIndex != pids.end(); ++itPointIndex){
					disable_point(*itPointIndex);
			}
	}

	void enable_points(const PointIndexList& pids) {
			for(typename PointIndexList::const_iterator itPointIndex = pids.begin(); 
				itPointIndex != pids.end(); ++itPointIndex){
					enable_point(*itPointIndex);
			}
	}

protected:
	template<class Iterator>
	inline void registerData(Iterator beg, Iterator end, PointIndex startingindex){
		for(Iterator it = beg; it != end; ++it){
			VoxelId vid = getVoxelIdFromPoint(*it);
			__voxels[vid].push_back(startingindex);
			startingindex++;
		}
	}

	inline  void registerData(const PointContainerPtr& data, PointIndex startingindex){
		registerData(data->begin(),data->end(),startingindex);
	}

	void initialize(const VectorType& minpoint, 
					VectorType maxpoint)
	{
		for (size_t i = 0; i < NbDimension; ++i)
			if(__voxelsize[i] < GEOM_EPSILON)
				pglError("Bad voxelSize value for dimension %i : %f",i,__voxelsize[i]);
		for (size_t i = 0; i < NbDimension; ++i){
			__origin[i] = std::min(minpoint[i],maxpoint[i]);
			maxpoint[i] = std::max(minpoint[i],maxpoint[i]);
		}
		VectorType m = maxpoint-__origin;
		size_t totdim=1;
		for (size_t i = 0; i < NbDimension; ++i){
			__dimension[i] = (m[i]/__voxelsize[i])+1;
			totdim *= __dimension[i];
		}
		__voxels = VoxelList(totdim);
	}

	VectorType __origin;
	VectorType __voxelsize;
	size_t  __dimension[NbDimension];
	VoxelList __voxels;
};

template <class PointContainer, int NbDimension = Dimension<typename PointContainer::element_type>::Nb >
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

typedef PointRefGrid<Point2Array> Point2RefGrid;
typedef PointRefGrid<Point3Array> Point3RefGrid;
typedef PointRefGrid<Point4Array> Point4RefGrid;


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE
#endif
