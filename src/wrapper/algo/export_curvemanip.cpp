/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
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
    return res;
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
