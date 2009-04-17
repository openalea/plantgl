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


template <class T, class VectorT, int NbDimension = Dimension<VectorT>::Nb>
class SpatialArrayN : public ArrayN<T,NbDimension>
{
public:
	typedef VectorT VectorType;
	typedef ArrayN<T,NbDimension> Base;
	typedef typename Base::Index Index;
	typedef typename Base::CellId CellId;

	SpatialArrayN(const VectorType& voxelsize,
				  const VectorType& minpoint, 
			      const VectorType& maxpoint):
		 ArrayN(gridSize(minpoint,maxpoint,voxelsize)),
	     __origin(minpoint),
		 __voxelsize(voxelsize)
		 { }

	SpatialArrayN(const VectorType& voxelsize = VectorType()):
		 ArrayN(),
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

	inline void initialize(const VectorType& minpoint, 
				   const VectorType& maxpoint,
				   const VectorType& voxelsize) 
	{
		__origin = minpoint;
		__voxelsize = voxelsize;
		Base::initialize(gridSize(minpoint,maxpoint,voxelsize));
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
