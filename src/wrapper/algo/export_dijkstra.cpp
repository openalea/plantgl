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




#include <plantgl/algo/base/dijkstra.h>
#include <boost/python.hpp>
// #include <plantgl/python/boost_python.h>
// #include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
using namespace boost::python;
#define bpy boost::python

using namespace std;

struct PyDistance {
        boost::python::object __pyfunc;
        real_t operator()(uint32_t a, uint32_t b) const { return extract<real_t>(__pyfunc(a,b))(); }
        PyDistance(boost::python::object evaluator) : __pyfunc(evaluator) { }
};


object py_dijkstra_shortest_paths(const IndexArrayPtr& connections,
                                   uint32_t root,
                                   boost::python::object distevaluator)
{
    PyDistance mydist( distevaluator );
    std::pair<Uint32Array1Ptr,RealArrayPtr> result = dijkstra_shortest_paths(connections,root,mydist);
    return boost::python::make_tuple(result.first,result.second);
}

object py_dijkstra_shortest_paths_in_a_range(const IndexArrayPtr& connections,
                                             uint32_t root,
                                             boost::python::object distevaluator,
                                             real_t maxdist = REAL_MAX,
                                             uint32_t maxnbelements = 0)
{
    if (maxnbelements == 0) { maxnbelements = UINT32_MAX; }
    PyDistance mydist( distevaluator );
    DijkstraNodeList result = dijkstra_shortest_paths_in_a_range(connections,root,mydist,maxdist,maxnbelements);
    boost::python::list pyresult;
    for(DijkstraNodeList::const_iterator itres = result.begin(); itres != result.end(); ++itres)
        pyresult.append(boost::python::make_tuple(itres->id,itres->parent,itres->distance));
    return pyresult;
}

void export_Dijkstra()
{

    def("dijkstra_shortest_paths", &py_dijkstra_shortest_paths,args("connections","root","edgeweigthevaluator"),
        "Return the parent and distance to the root for each node."
        "connections is an array that should contains at the ith place all nodes connected to the ith node."
        "edgeweigthevaluator should be a function that takes as argument the ids of two nodes and return the weigth of the edge between these 2 nodes.");
    def("dijkstra_shortest_paths_in_a_range", &py_dijkstra_shortest_paths_in_a_range,(bpy::arg("connections"),bpy::arg("root"),bpy::arg("edgeweigthevaluator"),bpy::arg("maxdist")=REAL_MAX,bpy::arg("maxnbelements")=0),
        "Return list of id, parent and distance to the root for node with distance < maxdist. "
        "connections is an array that should contains at the ith place all nodes connected to the ith node."
        "edgeweigthevaluator should be a function that takes as argument the ids of two nodes and return the weigth of the edge between these 2 nodes."
        "if maxnbelements is set to 0, no constraints is taken.");
}


