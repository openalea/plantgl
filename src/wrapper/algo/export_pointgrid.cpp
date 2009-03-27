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
 object py_getVoxelCoordFromPoint(PointGrid * grid, typename PointGrid::VectorType v){
	 return make_list(grid->getVoxelCoordFromPoint(v))();
 }

template<class PointGrid>
typename PointGrid::VoxelId py_getVoxelIdFromCoord(PointGrid * grid, object c){
	 return grid->getVoxelIdFromCoord(extract_vec<typename PointGrid::VoxelCoordinate>(c));
 }

template<class PointGrid>
object  py_getVoxelCoordFromId(PointGrid * grid, typename PointGrid::VoxelId c){
	 return make_list(grid->getVoxelCoordFromId(c))();
 }

template<class PointGrid>
bool py_isValidCoord(PointGrid * grid, object c){
	 return grid->isValidCoord(extract_vec<typename PointGrid::VoxelCoordinate>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelCenter(PointGrid * grid, object c){
	 return grid->getVoxelCenter(extract_vec<typename PointGrid::VoxelCoordinate>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelCenterFromId(PointGrid * grid,  typename PointGrid::VoxelId vid){
	 return grid->getVoxelCenter(vid);
}

template<class PointGrid>
class pointgrid_func : public boost::python::def_visitor<pointgrid_func<PointGrid> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
		c.def(init<real_t,typename PointGrid::PointContainerPtr>())
	 .def("getVoxelCoordFromPoint",&py_getVoxelCoordFromPoint<PointGrid>)
	 .def("getVoxelIdFromPoint",&PointGrid::getVoxelIdFromPoint)
	 .def("getVoxelIdFromCoord",&py_getVoxelIdFromCoord<PointGrid>)
	 .def("getVoxelCoordFromId",&py_getVoxelCoordFromId<PointGrid>)
	 .def("isValidId",&PointGrid::isValidId)
	 .def("isValidCoord",&py_isValidCoord<PointGrid>)
	 .def("getVoxelCenter",&py_getVoxelCenter<PointGrid>)
	 .def("getVoxelCenterFromId",&py_getVoxelCenterFromId<PointGrid>)
	 .def("query_ball_point",&py_query_ball_point<PointGrid>)
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
