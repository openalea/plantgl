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
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/tool/errormsg.h>


PGL_BEGIN_NAMESPACE

template <class PointContainer, int NbDimension>
class PointGrid {
public:
	typedef PointContainer ContainerType;
	typedef typename PointContainer::element_type VectorType;
	typedef RCPtr<PointContainer> PointContainerPtr;

	typedef typename PointContainer::iterator PointIterator;
	typedef typename PointContainer::const_iterator PointConstIterator;
	
	typedef size_t PointContainerIndex;
	typedef size_t VoxelCoordinate;
	typedef std::vector<VoxelCoordinate> VoxelCoordinates;
	typedef size_t VoxelId;
	typedef std::vector<VoxelId> VoxelIdList;
	typedef std::vector<PointContainerIndex> PointIndexList;

	typedef std::vector<PointIndexList> VoxelList;
	typedef typename VoxelList::iterator VoxelIterator;
	typedef typename VoxelList::const_iterator VoxelConstIterator;

	PointGrid(const VectorType& voxelsize,
			  const VectorType& minpoint, 
			  const VectorType& maxpoint,
			  const PointContainerPtr& data):
	  __voxelsize(voxelsize){
		  initialize(minpoint,maxpoint);
		  appendData(data);
	}

	PointGrid(const VectorType& voxelsize,
			  const PointContainerPtr& data):
	  __voxelsize(voxelsize){
		  for(size_t i = 0; i < NbDimension; ++i) 
			  assert(__voxelsize[i] > GEOM_EPSILON);
		  std::pair<VectorType,VectorType> bounds = data->getBounds();
		  initialize(bounds.first,bounds.second);
		  appendData(data);
	}

	PointGrid(const real_t& voxelsize,
			  const PointContainerPtr& data):
		  __voxelsize(voxelsize){
		  for(size_t i = 1; i < NbDimension; ++i)
			  __voxelsize[i] = voxelsize;
		  assert(voxelsize > GEOM_EPSILON);
		  std::pair<VectorType,VectorType> bounds = data->getBounds();
		  initialize(bounds.first,bounds.second);
		  appendData(data);
	}

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


	template<class Iterator>
	inline void appendData(Iterator beg, Iterator end){
		PointContainerIndex psize =  __points.getSize();
		for(Iterator it = beg; it != end; ++it){
			VoxelId vid = getVoxelIdFromPoint(*it);
			__voxels[vid].push_back(psize);
			psize++;
		}
		__points.insert(__points.getEnd(), beg, end);
	}

	inline  void appendData(const PointContainerPtr& data){
		appendData(data->getBegin(),data->getEnd());
	}

	inline  PointContainerPtr getVoxelPoints(const VoxelCoordinates& coord) const {
		return getVoxelPoints(getVoxelIdFromCoord(coord));
	}

	PointContainerPtr getVoxelPoints(const VoxelId& vid) const{
		size_t len = __voxels[vid].size();
		if (len == 0) return new PointContainer();
		PointContainerPtr pts(new PointContainer(len));
		PointIterator itpoints = pts->getBegin();
		for(PointIndexList::const_iterator itindex = __voxels[vid].begin(); 
			itindex != __voxels[vid].end(); ++itindex){
			*itpoints = __points[*itindex];
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
				if (!(norm(__points.getAt(*itPointIndex)-point) > radius))
					res.push_back(*itPointIndex);
			}
		}
		return res;
	}

protected:
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
	PointContainer __points;
};


typedef PointGrid<Point2Array,2> Point2Grid;
typedef PointGrid<Point3Array,3> Point3Grid;
typedef PointGrid<Point4Array,4> Point4Grid;

PGL_END_NAMESPACE
#endif