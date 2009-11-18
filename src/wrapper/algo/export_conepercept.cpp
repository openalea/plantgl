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
#include <plantgl/algo/base/conepercept.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

template <class ConePerception>
object py_get_ConeAxis(ConePerception* self, const typename ConePerception::VectorType u, const real_t alpha)
{  						  
	 return make_list(self->get_ConeAxis(u, alpha))();
}

template <class ConePerception>
object py_query_AttractPoints(ConePerception* self, const typename ConePerception::VectorType start_point, 
							  const typename ConePerception::PointContainerPtr points, 
							  const typename ConePerception::VectorType u, const real_t di, const real_t alpha)						  
{ 
	ConePerception::PointsDict result = self->query_AttractPoints(start_point, points, u, di, alpha);
	return dict_converter<ConePerception::PointsDict,make_object<ConePerception::PointsDict::key_type>,
		                                            list_converter<ConePerception::PointsDict::
#if (defined(USING_UNORDERED_MAP)) || defined(WIN32_STL_EXTENSION)
          mapped_type
#else
          data_type
#endif
													>  >(result)();
}

void export_ConePerception()
{
	class_<ConePerception<Point3Array> >("ConePerception", init<>())
		.def("get_ConeAxis", &py_get_ConeAxis< ConePerception<Point3Array> >)
		.def("isInCone", &ConePerception<Point3Array>::isInCone)
		.def("query_AttractPoints", &py_query_AttractPoints< ConePerception<Point3Array> >)
	;
}