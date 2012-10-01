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

#ifndef __util_spatialarrayn_h__
#define __util_spatialarrayn_h__

#include <vector>
#include "rcobject.h"
#include "util_arrayn.h"
#include "errormsg.h"


TOOLS_BEGIN_NAMESPACE



/* ----------------------------------------------------------------------- */
template <class T, class VectorT, int NbDimension = Dimension<VectorT>::Nb, class ContainerType = VectorContainer<T> >
class SpatialArrayN : public ArrayN<T,NbDimension,ContainerType>
{
public:
	typedef VectorT VectorType;
	typedef ArrayN<T,NbDimension,ContainerType> Base;
	typedef typename Base::Index Index;
	typedef typename Base::CellId CellId;
	typedef typename Base::const_partial_iterator const_partial_iterator;

    typedef std::vector<CellId> CellIdList;


	SpatialArrayN(const VectorType& voxelsize,
				  const VectorType& minpoint, 
			      const VectorType& maxpoint):
		 Base(gridSize(minpoint,maxpoint,voxelsize)),
	     __origin(minpoint),
		 __voxelsize(voxelsize)
		 { }

	SpatialArrayN(const VectorType& voxelsize = VectorType()):
		 Base(),
	     __origin(),
		 __voxelsize(voxelsize)
		 { }

	Index indexFromPoint(const VectorType& point) const {
		Index index;
		VectorType m = point-__origin;
		for (size_t i = 0; i < NbDimension; ++i){
			index[i] = m[i]/__voxelsize[i];
		}
		return index;
	}

	size_t indexFromCoord(const VectorType& point, size_t dimension) const {
		return size_t((point[dimension] -__origin[dimension]) / __voxelsize[dimension]) ;
	}

	size_t indexFromCoord(real_t coord, size_t dimension) const {
		return size_t((coord -__origin[dimension]) / __voxelsize[dimension]) ;
	}

	inline CellId cellIdFromPoint(const VectorType& point) const {
		return Base::cellId(indexFromPoint(point));
	}

	VectorType getVoxelCenter(const Index& coord) const{
		VectorType res = __origin;
		for (size_t i = 0; i < NbDimension; ++i){
			res[i] += (coord[i]+0.5) * __voxelsize[i];
		}
		return res;
	}

	inline VectorType getVoxelCenter(const CellId& cid) const {
		return getVoxelCenter(Base::index(cid));
	}

	VectorType getVoxelLowerPoint(const Index& coord) const{
		VectorType res = __origin;
		for (size_t i = 0; i < NbDimension; ++i){
			res[i] += coord[i] * __voxelsize[i];
		}
		return res;
	}

	inline VectorType getVoxelLowerPoint(const CellId& cid) const {
		return getVoxelLowerPoint(Base::index(cid));
	}

	VectorType getVoxelUpperPoint(const Index& coord) const{
		VectorType res = __origin;
		for (size_t i = 0; i < NbDimension; ++i){
			res[i] += (coord[i]+1) * __voxelsize[i];
		}
		return res;
	}

	inline VectorType getVoxelUpperPoint(const CellId& cid) const {
		return getVoxelUpperPoint(Base::index(cid));
	}

	inline VectorType getOrigin() const { return __origin; }
	inline void setOrigin(const VectorType& v) const { __origin = v; }

	inline VectorType getVoxelSize() const { return __voxelsize; }
	inline void setVoxelSize(const VectorType& v) const { __voxelsize = v; }

	inline VectorType getGridSize() const { 
		VectorType res = __voxelsize;
		for (size_t i = 0; i < NbDimension; ++i)
			res[i] *= Base::dimensions()[i];
		return res;
	}

	inline void initialize(const VectorType& minpoint, 
				   const VectorType& maxpoint,
				   const VectorType& voxelsize) 
	{
		__origin = minpoint;
		__voxelsize = voxelsize;
		Base::initialize(gridSize(minpoint,maxpoint,voxelsize));
	}

	inline VectorType getLowerCorner() const {
		return __origin;
	}

	inline VectorType getUpperCorner() const {
		return __origin + getGridSize();
	}

	inline real_t getMaxDistanceToBorder(const VectorType& v) const { 
		VectorType minpoint = __origin;
		VectorType maxpoint = getUpperCorner();
		VectorType dist;
		for (size_t i = 0; i < NbDimension; ++i)
			dist[i] = std::max(fabs(v[i]-minpoint[i]),fabs(v[i]-maxpoint[i]));
		return norm(dist); 
	}

	inline Index getMaxIndexDistanceToBorder(const Index& v) const { 
		Index maxindex = Base::dimensions();
		Index res;
		for (size_t i = 0; i < NbDimension; ++i)
			res[i] = std::max(v[i],maxindex[i]-v[i]);
		return res; 
	}

	typename std::vector<VectorType> getCorners() const {
		VectorType minpoint = getLowerCorner();
		VectorType maxpoint = getUpperCorner();
		typename std::vector<VectorType> res;
		res.push_back(minpoint);
		for (size_t i = 0; i < NbDimension; ++i){
			typename std::vector<VectorType>::const_iterator beg = res.begin();
			typename std::vector<VectorType>::const_iterator end = res.end();
			for(typename std::vector<VectorType>::const_iterator it = beg; it != end; ++it){
				VectorType oppositepoint = *it;
				oppositepoint[i] = maxpoint[i];
				res.push_back(oppositepoint);
			}
		}
		return res;
	}

	typename std::vector<VectorType> getVoxelCorners(const Index& coord) const {
		VectorType minpoint = getVoxelLowerPoint(coord);
		VectorType maxpoint = getVoxelUpperPoint(coord);
		typename std::vector<VectorType> res;
		res.push_back(minpoint);
		for (size_t i = 0; i < NbDimension; ++i){
			typename std::vector<VectorType>::const_iterator beg = res.begin();
			typename std::vector<VectorType>::const_iterator end = res.end();
			for(typename std::vector<VectorType>::const_iterator it = beg; it != end; ++it){
				VectorType oppositepoint = *it;
				oppositepoint[i] = maxpoint[i];
				res.push_back(oppositepoint);
			}
		}
		return res;
	}

	typename std::vector<VectorType> getVoxelCorners(const CellId& cid) const {
        return getVoxelCorners(Base::index(cid));
    }


    CellIdList query_voxels_around_point(const VectorType& point, real_t radius, 
										real_t minradius = 0, bool filterEmpty = true) const {
        CellIdList res;
        Index centervxl = indexFromPoint(point);
        // discretize radius in term of voxel size
        Index radiusvoxelsize;
        for (size_t i = 0; i < NbDimension; ++i){
            radiusvoxelsize[i] = 1+(radius/getVoxelSize()[i]);
        }

        // find min and max voxel coordinates
        Index mincoord, maxcoord, dim;
        for (size_t i = 0; i < NbDimension; ++i){
            mincoord[i] = (centervxl[i] < radiusvoxelsize[i]?0:centervxl[i]-radiusvoxelsize[i]);
            maxcoord[i] = std::min<size_t>(Base::dimensions()[i]-1,centervxl[i]+radiusvoxelsize[i]);
            dim[i] = maxcoord[i] - mincoord[i];
        }

        // Index coord = mincoord;
		real_t normvoxelsize = norm(getVoxelSize());
        real_t r = radius + normvoxelsize;
		real_t minr = std::max<real_t>(0,minradius - normvoxelsize);
        const_partial_iterator itvoxel = this->getSubArray(mincoord,dim);
        while(!itvoxel.atEnd()){
            // Check whether coord is in ball
			real_t voxeldist = norm(getVoxelCenter(itvoxel.index())-point);
            if (voxeldist < r && voxeldist >= minr ){
                CellId vxlid = itvoxel.cellId();
                if(!filterEmpty || !ContainerType::is_empty(vxlid))
                    res.push_back(vxlid);
            }
            ++itvoxel;
        }
        return res;
    }

    CellIdList query_voxels_in_box(const Index& center, 
							   const Index& maxradius, 
							   const Index& minradius = Index(0), 
							   bool filterEmpty = true) const {
        CellIdList res;

        // find min and max voxel coordinates for minradius
        Index begminradius, endminradius;
        for (size_t i = 0; i < NbDimension; ++i){
            begminradius[i] = (center[i] < minradius[i]?0:center[i]-minradius[i]);
            endminradius[i] = std::min<size_t>(Base::dimensions()[i]-1,center[i]+minradius[i]);
        }

        // find min and max voxel coordinates for maxradius
        Index begmaxradius, endmaxradius, dim;
        for (size_t i = 0; i < NbDimension; ++i){
            begmaxradius[i] = (center[i] < maxradius[i]?0:center[i]-maxradius[i]);
            endmaxradius[i] = std::min<size_t>(Base::dimensions()[i]-1,center[i]+maxradius[i]);

			// if (begminradius[i] == minmaxradius[i]) minmaxradius[i] = maxminradius[i];
			// else if (maxminradius[i] == maxmaxradius[i]) maxmaxradius[i] = minminradius[i];

            dim[i] = endmaxradius[i] - begmaxradius[i];
        }


        // Index coord = mincoord;
        const_partial_iterator itvoxel = getSubArray(begmaxradius,dim);
        while(!itvoxel.atEnd()){
            CellId vxlid = itvoxel.cellId();
            // Check whether coord is not in min box
			bool toconsider = false;
			for (size_t i = 0; i < NbDimension; ++i){
				if (itvoxel.index()[i] <= begminradius[i] || itvoxel.index()[i] >= endminradius[i]) 
				{ toconsider = true; break; }
			}
            if ( toconsider ){
                if(!filterEmpty || !ContainerType::is_empty(vxlid))
                    res.push_back(vxlid);
            }
			++itvoxel;
        }
        return res;
    }

    CellIdList query_voxels_in_cone(const VectorType& coneorigin, const VectorType& conedirection,
                                  real_t radius,  real_t coneangle = GEOM_HALF_PI, bool filterEmpty = true
                                  ) const {
        CellIdList res;
        Index centervxl = indexFromPoint(coneorigin);
        VectorType mdirection = conedirection.normed();

        // examine all voxels which are in bbox of the sphere around point with radius radius

        // discretize radius in term of voxel size
        Index radiusvoxelsize;
        for (size_t i = 0; i < NbDimension; ++i){
            radiusvoxelsize[i] = 1+(radius/getVoxelSize()[i]);
        }

        // find min and max voxel coordinates
        Index mincoord, maxcoord, dim;
        for (size_t i = 0; i < NbDimension; ++i){
            mincoord[i] = (centervxl[i] < radiusvoxelsize[i]?0:centervxl[i]-radiusvoxelsize[i]);
            maxcoord[i] = std::min<size_t>(Base::dimensions()[i]-1,centervxl[i]+radiusvoxelsize[i]);
            dim[i] = maxcoord[i] - mincoord[i];
        }

		real_t normvoxelsize = norm(getVoxelSize());
        real_t r = radius + normvoxelsize;
        const_partial_iterator itvoxel = this->getSubArray(mincoord,dim);

        real_t halfconeangle = coneangle / 2;
        real_t cosconeangle = cos(halfconeangle);
        real_t coslargeconeangle = cos(std::min<real_t>(halfconeangle+ GEOM_HALF_PI/2,GEOM_PI));
        while(!itvoxel.atEnd()){
            // Check whether coord is in ball
            VectorType voxelcentertoconeorigin = getVoxelCenter(itvoxel.index())-coneorigin;
			real_t voxeldist = voxelcentertoconeorigin.normalize();
            if (voxeldist < r ){
                CellId vxlid = itvoxel.cellId();
                 if(!filterEmpty || !ContainerType::is_empty(vxlid)){
                    // Check first for center point of the voxel respect the angle condition
                    real_t a = dot(voxelcentertoconeorigin,mdirection);
                    if (a > cosconeangle) res.push_back(vxlid);
                    // if the angle is not too big, we check if one of the corner is inside the cone
                    else if(a > coslargeconeangle) {
                        std::vector<VectorType> corners = getVoxelCorners(vxlid);
                        for(typename std::vector<VectorType>::const_iterator itcorner = corners.begin(); itcorner != corners.end(); ++itcorner){
                            real_t b = dot(direction(*itcorner - coneorigin),mdirection);
                            if (b > cosconeangle){
                                res.push_back(vxlid);
                                break;
                            }
                        }
                    }
                 }
            }
            ++itvoxel;
        }
        return res;
    }


protected:
	static Index gridSize(const VectorType& minpoint, 
				   const VectorType& maxpoint,
				   const VectorType& voxelsize)
	{
		for (size_t i = 0; i < NbDimension; ++i){
			if(voxelsize[i] < GEOM_EPSILON)
				pglError("Bad voxelSize value for dimension %i : %f",i,voxelsize[i]);
			if(minpoint[i] > maxpoint[i])
				pglError("Bad min and max point value for dimension %i : %f - %f",i,minpoint[i],maxpoint[i]);
		}
		VectorType m = maxpoint-minpoint;
		Index dimension;
		for (size_t i = 0; i < NbDimension; ++i){
			dimension[i] = (m[i]/voxelsize[i])+1;
		}
		return dimension;
	}


	VectorType __origin;
	VectorType __voxelsize;
};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE
#endif
