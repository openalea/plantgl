/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): C. Preuksakarn et al.
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

#ifndef __conepercept_h__
#define __conepercept_h__

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_matrix.h>

TOOLS_USING(Matrix3)

PGL_BEGIN_NAMESPACE

template <class PointContainer>
class ConePerception{
public:
	typedef typename PointContainer::element_type VectorType;
	typedef RCPtr<PointContainer> PointContainerPtr;
	typedef typename PointContainer::const_iterator const_iterator;
	typedef std::map<uint_t, std::vector<VectorType> > PointsDict;

	ConePerception(){}

/* ----------------------------------------------------------------------- */

	std::vector<VectorType> get_ConeAxis(const VectorType u, const real_t alpha)
	{
		VectorType v = cross(u, VectorType::OX);
		if (v == VectorType::ORIGIN)
			v = cross(u, VectorType::OY);

		std::vector<VectorType> _uvector;
		_uvector.push_back(u);

		real_t _polar = alpha;
		while(_polar < M_PI/2)
		{
			VectorType vu = Matrix3::axisRotation(v,_polar) * u;
			real_t _azimuthal = 0;
			while(_azimuthal < 2*M_PI)
			{
				VectorType vv = Matrix3::axisRotation(u,_azimuthal) * vu;
				_uvector.push_back(vv);
				_azimuthal += alpha;
			}
			_polar += alpha;
		}

		return _uvector;
	}

/* ----------------------------------------------------------------------- */

	bool isInCone(const VectorType start_point, const VectorType point, const VectorType axis, 
				  const real_t di, const real_t alpha)
	{
		VectorType v = direction(point - start_point);
		if(norm(point - start_point) > di || angle(axis, v) > alpha/2)
			return false;

		return true;
	}

/* ----------------------------------------------------------------------- */

	real_t distancePointtoVector(const VectorType start_point, const VectorType p, const VectorType axis)
	{
		return norm(cross(p-start_point, axis));
	}

/* ----------------------------------------------------------------------- */

	int_t getNeighborAxis(std::vector<VectorType> coneAxisList,  VectorType start_point, VectorType point,
							real_t di, real_t alpha)
	{
		real_t distance = 100000000;
		int_t idx = -1;
			
		for(int_t i=0; i < coneAxisList.size(); i++)
		{
			if(isInCone(start_point, point, coneAxisList[i], di, alpha))
			{
				real_t d = distancePointtoVector(start_point, point, coneAxisList[i]);
				if (d < distance)
				{
					idx = i;
					distance = d;
				}
			}
		}

		return idx;
	}

/* ----------------------------------------------------------------------- */

	void assign_Attractor(PointsDict& pd, VectorType point, int_t idx)
	{
		// because we can't directly insert a new attractor point into the list.
		std::vector<VectorType> _attractPointsList;
		_attractPointsList.push_back(point);

		if (pd.count(idx)>0)
		{
			std::vector<VectorType> plst = pd.find(idx)->second;   // obtained list of points
			for(typename std::vector<VectorType>::iterator _itp = plst.begin();
				_itp != plst.end(); ++_itp)
			{
				_attractPointsList.push_back(*_itp);  // assign it back
			}
		}
				 
		pd[idx] = _attractPointsList;
	}

/* ----------------------------------------------------------------------- */

	PointsDict query_AttractPoints(const VectorType start_point, 
							const PointContainerPtr points, const VectorType u, const real_t di, const real_t alpha)
	{
		std::vector<VectorType> _coneAxisList = get_ConeAxis(u, alpha);
		PointsDict result;

		for(const_iterator _itsource = points->begin(); _itsource != points->end(); ++_itsource)	
		{
			// compute the nearest axis of each point
			int_t idx = getNeighborAxis(_coneAxisList, start_point, *_itsource, di, alpha);

			if (idx != -1)
				assign_Attractor(result, *_itsource, idx);	
		}

		return result;  // return dictionary : Key is axis, Value is list of points ... {1: [Vector3(3,0,1), Vector3(4,2,1)], 2: [Vector3(1,2,3), Vector3(3,3,3), Vector3(1,2,1), Vector3(2,4,2)], 4: [Vector3(-0.5,-1,0)]}
	}

}; // end ConePerception class

/* ----------------------------------------------------------------------- */


PGL_END_NAMESPACE

#endif
