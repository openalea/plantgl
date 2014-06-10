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

#include <plantgl/algo/base/curvemanipulation.h>
#include <plantgl/python/extract_list.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

std::vector<Polyline2DPtr> convert(boost::python::object obj){
	std::vector<Polyline2DPtr> res;
	boost::python::extract<Polyline2DPtr> pol_e(obj);
	if(pol_e.check()) res.push_back(pol_e());
	else {
		return extract_vec<Polyline2DPtr>(obj)();
	}
}

Point2ArrayPtr py_CurveIntersection_compute(boost::python::object arg){
	return CurveIntersection::compute(convert(arg));
}

bool py_CurveIntersection_check(boost::python::object arg){
	return CurveIntersection::check(convert(arg));
}


IndexArrayPtr py_determine_faces_from_edges(const Point2ArrayPtr& pts, boost::python::object edges)
{
  std::vector< std::pair<uint32_t,uint32_t> > cedges = extract_vec_pair< uint32_t,uint32_t >(edges)();
  return determine_faces_from_edges(pts, cedges);

}

IndexArrayPtr py_determine_faceedges_from_edges(const Point2ArrayPtr& pts, boost::python::object edges)
{
  std::vector< std::pair<uint32_t,uint32_t> > cedges = extract_vec_pair< uint32_t,uint32_t >(edges)();
  return determine_faceedges_from_edges(pts, cedges);

}


void export_CurveManipulation()
{
  class_< Overlay > ("Overlay", no_init)
    .def("process",&Overlay::process, "compute the overlay between 2 closed polylines. Requires CGAL extension.")
	.staticmethod("process")
    ;
 
  class_< CurveIntersection > ("CurveIntersection", no_init)
    .def("compute",&py_CurveIntersection_compute, "compute the interesection between polylines. Requires CGAL extension.")
	.staticmethod("compute")
    .def("check",&py_CurveIntersection_check, "check whether an interesection between polylines exists. Requires CGAL extension.")
	.staticmethod("check")
    ;
 
  def("determine_faces_from_edges",&py_determine_faces_from_edges,args("points","edges"),"Return list of pids that form faces");
  def("determine_faceedges_from_edges",&py_determine_faceedges_from_edges,args("points","edges"),"Return list of edge ids that form faces");
}

/* ----------------------------------------------------------------------- */
