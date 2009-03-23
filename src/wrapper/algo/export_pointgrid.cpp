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


 object py_query_ball_point(Point3Grid * grid, Point3Grid::VectorType point, real_t radius) 
 { return make_list(grid->query_ball_point(point,radius))(); }

 object py_getVoxelCoordFromPoint(Point3Grid * grid, Point3Grid::VectorType v){
	 return make_list(grid->getVoxelCoordFromPoint(v))();
 }

 Point3Grid::VoxelId py_getVoxelIdFromCoord(Point3Grid * grid, object c){
	 return grid->getVoxelIdFromCoord(extract_vec<Point3Grid::VoxelCoordinate>(c));
 }

 object  py_getVoxelCoordFromId(Point3Grid * grid, Point3Grid::VoxelId c){
	 return make_list(grid->getVoxelCoordFromId(c))();
 }

bool py_isValidCoord(Point3Grid * grid, object c){
	 return grid->isValidCoord(extract_vec<Point3Grid::VoxelCoordinate>(c));
}

Point3Grid::VectorType py_getVoxelCenter(Point3Grid * grid, object c){
	 return grid->getVoxelCenter(extract_vec<Point3Grid::VoxelCoordinate>(c));
}

Point3Grid::VectorType py_getVoxelCenterFromId(Point3Grid * grid,  Point3Grid::VoxelId vid){
	 return grid->getVoxelCenter(vid);
}



void export_PointGrid()
{

  class_< Point3Grid > ("Point3Grid", init<Vector3, Point3ArrayPtr>
     ( "Construct a regular grid from a set of 3D points." ))
	 .def(init<real_t,Point3ArrayPtr>())
	 .def("getVoxelCoordFromPoint",&py_getVoxelCoordFromPoint)
	 .def("getVoxelIdFromPoint",&Point3Grid::getVoxelIdFromPoint)
	 .def("getVoxelIdFromCoord",&py_getVoxelIdFromCoord)
	 .def("getVoxelCoordFromId",&py_getVoxelCoordFromId)
	 .def("isValidId",&Point3Grid::isValidId)
	 .def("isValidCoord",&py_isValidCoord)
	 .def("getVoxelCenter",&py_getVoxelCenter)
	 .def("getVoxelCenterFromId",&py_getVoxelCenterFromId)
	 .def("query_ball_point",&py_query_ball_point)

    ;
  
}

/* ----------------------------------------------------------------------- */
