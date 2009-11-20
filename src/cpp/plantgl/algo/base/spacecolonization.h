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


#ifndef __spacecolonization_h__
#define __spacecolonization_h__

#include <plantgl/algo/grid/regularpointgrid.h>
#include <plantgl/algo/base/conepercept.h>


PGL_BEGIN_NAMESPACE


template <class PointContainer>
class SpaceColonization{
public:
	typedef typename PointContainer::element_type VectorType;
	typedef PointRefGrid<PointContainer> LocalPointGrid;
	typedef RCPtr<PointContainer> PointContainerPtr;
	typedef RCPtr<LocalPointGrid> LocalPointGridPtr;
	typedef typename PointContainer::const_iterator const_iterator;
	typedef typename LocalPointGrid::PointIndexList PointIndexList;
	typedef std::map<uint_t, std::vector<VectorType> > AxisDict;
	

/* ------------------------------------------------------------------------------------------------ */

	SpaceColonization(const PointContainerPtr points, const real_t radius)
	{
		grid = new LocalPointGrid(radius, points);
	}


/* ------------------------------------------------------------------------------------------------ */
	// Free memory
	void free() { /*delete grid;*/ }

/* ------------------------------------------------------------------------------------------------ */

	VectorType get_StartNode(real_t delta) 
	{
		VectorType bottom_point = *grid->points().getYMin();
		real_t zmax = bottom_point.y() + delta;
		VectorType start_node;
		uint_t len=0;
		for(const_iterator itsource = grid->points().begin(); 
			itsource != grid->points().end(); ++itsource)
		{
			VectorType v = *itsource;
			if(v.y() <= zmax){
				start_node += v;
				len++;
			}
		}
		start_node /= len;
		start_node.y() = bottom_point.y();
		return start_node;
	}

/* ------------------------------------------------------------------------------------------------ */

	PointContainerPtr query_TreeNeighbors(const VectorType point, const real_t di)
	{
		PointIndexList pointindices = grid->query_ball_point(point, di);

		if(pointindices.empty()) return NULL;  // the bud has no available space

		PointContainerPtr result(new PointContainer(pointindices.size()));
		typename PointContainer::iterator _itresult = result->begin();
		for(typename PointIndexList::const_iterator _itptindex = pointindices.begin();
			_itptindex != pointindices.end(); ++_itptindex, ++_itresult)
		{
			*_itresult = grid->points().getAt(*_itptindex);
		}

		return result;
	}

/* ------------------------------------------------------------------------------------------------ */
// In each iteration, marker within the occupancy zone of any bud are deleted

	void delete_Marker(const VectorType treenode, const real_t dk)
	{
		PointIndexList pointindices = grid->query_ball_point(treenode, dk);
		if (!pointindices.empty())
			grid->disable_points(pointindices);
	}

/* ------------------------------------------------------------------------------------------------ */

	bool is_NearestNeighbor(const PointContainerPtr neighbornodes, const VectorType treenode, const VectorType attpoint)
	{
		real_t distance = norm(attpoint - treenode);
		for(const_iterator _itsource = neighbornodes->begin();
			_itsource != neighbornodes->end(); ++_itsource)
		{
			if(norm(attpoint - *_itsource) < distance)
				return false;
		}
		return true;
	}

/* ------------------------------------------------------------------------------------------------ */

	VectorType compute_NewNode(std::vector<VectorType> attraction_point, const VectorType treenode, const real_t D)
	{
		VectorType vector_n = VectorType::ORIGIN;
		for(typename std::vector<VectorType>::iterator _itsource = attraction_point.begin();
			_itsource != attraction_point.end(); ++_itsource)
		{
			if(*_itsource != treenode)
				vector_n += (*_itsource - treenode)/norm(*_itsource - treenode);
		}

		if (norm(vector_n) < GEOM_EPSILON) return treenode;

		VectorType unitvector_n = vector_n/norm(vector_n);
		return treenode + (unitvector_n * D);
	}

/* ------------------------------------------------------------------------------------------------ */

	typedef std::map<VectorType, std::vector<VectorType> > NodeInfo;
	NodeInfo create_NewNode(const PointContainerPtr neighbornodes, const VectorType treenode,
						const VectorType directionV, const real_t D, const real_t di, const real_t dk,
						const real_t alpha)
	{
		delete_Marker(treenode, dk);
		PointContainerPtr query_point = query_TreeNeighbors(treenode, di);
		if (query_point == NULL) return nullSpace();
		 
		ConePerception<PointContainer> _coneperception;
		AxisDict axis = _coneperception.query_AttractPoints(treenode, query_point, directionV, di, alpha);

		NodeInfo result;		
		for (typename AxisDict::const_iterator iter=axis.begin(); iter != axis.end(); ++iter)
		{
			std::vector<VectorType> p, attraction_point;
			p = iter->second;
			if(p.size() > 2) // to protect a lot of twig
			{
				for(typename std::vector<VectorType>::const_iterator _it = p.begin(); _it != p.end(); ++_it)
				{
					if(is_NearestNeighbor(neighbornodes, treenode, *_it))
						attraction_point.push_back(*_it);
				}
				VectorType new_treenode = compute_NewNode(attraction_point, treenode, D);
				result[new_treenode] = attraction_point;
			}
		}

		return result;
	}

/* ------------------------------------------------------------------------------------------------ */

	NodeInfo nullSpace()
	{
		// no available space -> return {Vector3(-1,-1,-1): [Vector3(-1,-1,-1)]}
		
		return NodeInfo();
	}

/* ------------------------------------------------------------------------------------------------ */

	PointContainerPtr get_enabled_points() 
	{
		//PointContainerPtr enable_points(new PointContainer(grid->points().size()));

		//typename PointContainer::iterator _itresult = enable_points->begin();
		//for(size_t _itptindex = 0; _itptindex < grid->points().size(); ++_itptindex, ++_itresult)
		//{
		//	*_itresult = grid->points().getAt(_itptindex);
		//}

		return grid->get_enabled_points();
	}

/* ------------------------------------------------------------------------------------------------ */

	bool is_point_enabled(size_t pid) 
	{
		return grid->is_point_enabled(pid);
	}


/* ------------------------------------------------------------------------------------------------ */

protected:	
	LocalPointGridPtr grid;


}; // end SpaceColonization class

/* ------------------------------------------------------------------------------------------------ */


PGL_END_NAMESPACE

#endif
