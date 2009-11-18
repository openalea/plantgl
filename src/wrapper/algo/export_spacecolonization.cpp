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
#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/algo/base/spacecolonization.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

template <class SpaceColonization>
object py_create_newnode(SpaceColonization* self, const typename SpaceColonization::PointContainerPtr neighbornodes, 
						  const typename SpaceColonization::VectorType treenode,
						  const typename SpaceColonization::VectorType directionV,
						  const real_t D, const real_t di, const real_t dk, const real_t alpha)
{
     //SpaceColonization::NodeInfo info = self->create_NewNode(neighbornodes, treenode, directionV, D, di, dk, alpha);
	 //return make_tuple(make_tuple(info.first.first,info.first.second), make_list(info.second)());
	SpaceColonization::NodeInfo result = self->create_NewNode(neighbornodes, treenode, directionV, D, di, dk, alpha);
	return dict_converter<SpaceColonization::NodeInfo,make_object<SpaceColonization::NodeInfo::key_type>,
		                                            list_converter<SpaceColonization::NodeInfo::
#if (defined(USING_UNORDERED_MAP)) || defined(WIN32_STL_EXTENSION)
          mapped_type
#else
          data_type
#endif
													>  >(result)();
 }



void export_SpaceColonization()
{
	class_<SpaceColonization<Point3Array> >("SpaceColonization", init<Point3ArrayPtr, real_t>())
		.def("get_StartNode", &SpaceColonization<Point3Array>::get_StartNode)
		.def("query_TreeNeighbors", &SpaceColonization<Point3Array>::query_TreeNeighbors) 
		.def("create_NewNode", &py_create_newnode< SpaceColonization<Point3Array> >)
		.def("get_enabled_points", &SpaceColonization<Point3Array>::get_enabled_points)
		.def("free", &SpaceColonization<Point3Array>::free) 
	;
}