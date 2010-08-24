/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_tuple.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/algo/grid/regularpointgrid.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

 PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */


template<class PointGrid>
 object py_query_ball_point(PointGrid * grid, typename PointGrid::VectorType point, real_t radius) 
 { return make_list(grid->query_ball_point(point,radius))(); }

template<class PointGrid>
 object py_closest_point(PointGrid * grid, typename PointGrid::VectorType point) 
 { 
	typename PointGrid::PointIndex res;
	 if (grid->closest_point(point,res)) return object(res);
	 else return object();
 }

template<class PointGrid>
 object py_indexFromPoint(PointGrid * grid, typename PointGrid::VectorType v){
	 return make_pgl_tuple(grid->indexFromPoint(v))();
 }

template<class PointGrid>
typename PointGrid::VoxelId py_cellId(PointGrid * grid, object c){
	 return grid->cellId(extract_tuple<typename PointGrid::Index>(c));
 }

template<class PointGrid>
object  py_index(PointGrid * grid, typename PointGrid::VoxelId c){
	 return make_pgl_tuple(grid->index(c))();
 }

template<class PointGrid>
bool py_isValidIndex(PointGrid * grid, object c){
	return grid->isValidIndex(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelCenter(PointGrid * grid, object c){
	 return grid->getVoxelCenter(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelCenterFromId(PointGrid * grid,  typename PointGrid::VoxelId vid){
	 return grid->getVoxelCenter(vid);
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelLowerPoint(PointGrid * grid, object c){
	 return grid->getVoxelLowerPoint(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelLowerPointFromId(PointGrid * grid,  typename PointGrid::VoxelId vid){
	 return grid->getVoxelLowerPoint(vid);
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelUpperPoint(PointGrid * grid, object c){
	 return grid->getVoxelUpperPoint(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelUpperPointFromId(PointGrid * grid,  typename PointGrid::VoxelId vid){
	 return grid->getVoxelUpperPoint(vid);
}

template<class PointGrid>
void py_enablepoints(PointGrid * grid, object plist ){
	 return grid->enable_points(extract_vec<typename PointGrid::PointIndex>(plist));
}

template<class PointGrid>
void py_disablepoints(PointGrid * grid, object plist ){
	 return grid->disable_points(extract_vec<typename PointGrid::PointIndex>(plist));
}

template<class PointGrid>
object py_getCorners(PointGrid * grid){
	 return make_list(grid->getCorners())();
}

template<class PointGrid>
object py_getMaxIndexDistanceToBorder(PointGrid * grid, object c){
	 return make_pgl_tuple(grid->getMaxIndexDistanceToBorder(extract_tuple<typename PointGrid::Index>(c)));
}



template<class PointGrid>
class pointgrid_func : public boost::python::def_visitor<pointgrid_func<PointGrid> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
	c.def(init<real_t,typename PointGrid::PointContainerPtr>())
	 .def(init<typename PointGrid::VectorType,typename PointGrid::VectorType,typename PointGrid::VectorType,typename PointGrid::PointContainerPtr>())
	 .def("size",&PointGrid::size)
	 .def("indexFromPoint",&py_indexFromPoint<PointGrid>)
	 .def("cellIdFromPoint",&PointGrid::cellIdFromPoint)
	 .def("cellId",&py_cellId<PointGrid>)
	 .def("index",&py_index<PointGrid>)
	 .def("isValidId",&PointGrid::isValidId)
	 .def("isValidIndex",&py_isValidIndex<PointGrid>)
	 .def("getVoxelCenter",&py_getVoxelCenter<PointGrid>)
	 .def("getVoxelCenterFromId",&py_getVoxelCenterFromId<PointGrid>)
	 .def("getVoxelLowerPoint",&py_getVoxelLowerPoint<PointGrid>)
	 .def("getVoxelLowerPointFromId",&py_getVoxelLowerPointFromId<PointGrid>)
	 .def("getVoxelUpperPoint",&py_getVoxelUpperPoint<PointGrid>)
	 .def("getVoxelUpperPointFromId",&py_getVoxelUpperPointFromId<PointGrid>)
	 .def("query_ball_point",&py_query_ball_point<PointGrid>)
	 .def("closest_point",&py_closest_point<PointGrid>)
	 .def("enable_point",&PointGrid::enable_point)
	 .def("disable_point",&PointGrid::disable_point)
	 .def("is_point_enabled",&PointGrid::is_point_enabled)
	 .def("get_enabled_points",&PointGrid::get_enabled_points)
	 .def("enable_points",&py_enablepoints<PointGrid>)
	 .def("disable_points",&py_disablepoints<PointGrid>)
	 .def("nbFilledVoxels",&PointGrid::nbFilledVoxels)
	 .def("getGridSize",&PointGrid::getGridSize)
	 .def("getLowerCorner",&PointGrid::getLowerCorner)
	 .def("getUpperCorner",&PointGrid::getUpperCorner)
	 .def("getCorners",&py_getCorners<PointGrid>)
	 .def("getMaxIndexDistanceToBorder",&py_getMaxIndexDistanceToBorder<PointGrid>)
	 .def("getMaxDistanceToBorder",&PointGrid::getMaxDistanceToBorder)
	 
	 
         ;
    }
};


void export_PointGrid()
{

  class_< Point2Grid > ("Point2Grid", init<Vector2, Point2ArrayPtr>
     ( "Construct a regular grid from a set of 2D points." ))
	 .def(pointgrid_func<Point2Grid>())
    ;
  
  class_< Point3Grid > ("Point3Grid", init<Vector3, Point3ArrayPtr>
     ( "Construct a regular grid from a set of 3D points." ))
	 .def(pointgrid_func<Point3Grid>())
    ;
  class_< Point4Grid > ("Point4Grid", init<Vector4, Point4ArrayPtr>
     ( "Construct a regular grid from a set of 4D points." ))
	 .def(pointgrid_func<Point4Grid>())
    ;
  
}

/* ----------------------------------------------------------------------- */
