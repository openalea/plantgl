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



#include <plantgl/algo/projection/depthsortengine.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/python/export_list.h>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


boost::python::object py_idsurfaces(DepthSortEngine * ze){
    pgl_hash_map<uint32_t,real_t> res = ze->idsurfaces();
    boost::python::list bres;
    for(pgl_hash_map<uint32_t,real_t>::const_iterator _it = res.begin(); _it != res.end(); ++_it){
      bres.append(boost::python::make_tuple(_it->first,_it->second));
    }
    return bres;
}

boost::python::object py_agregate_idsurfaces(DepthSortEngine * engine) {
    pgl_hash_map<uint32_t,std::pair<real_t,std::vector<real_t> > > result = 
        engine->aggregateIdSurfaces();
    boost::python::dict bres;
    for(pgl_hash_map<uint32_t,std::pair<real_t,std::vector<real_t> > >::const_iterator _it = result.begin(); _it != result.end(); ++_it){
      bres[boost::python::object(_it->first)] = boost::python::make_tuple(boost::python::object(_it->second.first), make_list(_it->second.second)());
    }
    return bres;
}

void export_DepthSortEngine()
{
#ifdef PGL_WITH_CGAL

  class_< DepthSortEngine, bases<ProjectionEngine>, boost::noncopyable >
      ("DepthSortEngine", init<ProjectionEngine::eIdPolicy>("Construct a DepthSortEngine.",(bp::arg("idPolicy")=ProjectionEngine::eShapeIdBased)) )
      .def("processTriangle", &DepthSortEngine::processTriangle)
      .def("getResult", &DepthSortEngine::getResult, (bp::arg("format")=Color4::eARGB, bp::arg("cameraCoordinates")=true))
      .def("getProjectionResult", &DepthSortEngine::getProjectionResult, (bp::arg("format")=Color4::eARGB, bp::arg("cameraCoordinates")=true))
      .def("getSurfaceResult", &DepthSortEngine::getSurfaceResult, (bp::arg("cameraCoordinates")=true))
      .def("getSurfaceProjectionResult", &DepthSortEngine::getSurfaceProjectionResult, (bp::arg("cameraCoordinates")=true))
      .def("idsurfaces", &py_idsurfaces)
      .def("aggregateIdSurfaces", &py_agregate_idsurfaces)
      ;

#endif
}


