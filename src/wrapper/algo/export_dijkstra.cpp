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

#include <plantgl/python/boost_python.h>

#include <plantgl/algo/base/dijkstra.h>
// #include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

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
    std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)> result = dijkstra_shortest_paths(connections,root,mydist);
    return make_tuple(result.first,result.second);
}

object py_dijkstra_shortest_paths_in_a_range(const IndexArrayPtr& connections, 
                                             uint32_t root, 
                                             boost::python::object distevaluator,
                                             real_t maxdist = REAL_MAX,
                                             uint32_t maxnbelements = UINT32_MAX)
{
    PyDistance mydist( distevaluator );
    NodeList result = dijkstra_shortest_paths_in_a_range(connections,root,mydist,maxdist);
    boost::python::list pyresult;
    for(NodeList::const_iterator itres = result.begin(); itres != result.end(); ++itres)
        pyresult.append(make_tuple(itres->id,itres->parent,itres->distance));
    return pyresult;
}

void export_Dijkstra()
{

	def("dijkstra_shortest_paths", &py_dijkstra_shortest_paths,args("connections","root","edgeweigthevaluator"),
        "Return the parent and distance to the root for each node."
        "connections is an array that should contains at the ith place all nodes connected to the ith node."
        "edgeweigthevaluator should be a function that takes as argument the ids of two nodes and return the weigth of the edge between these 2 nodes.");
	def("dijkstra_shortest_paths_in_a_range", &py_dijkstra_shortest_paths_in_a_range,(bp::arg("connections"),bp::arg("root"),bp::arg("edgeweigthevaluator"),bp::arg("maxdist")=REAL_MAX,bp::arg("maxnbelements")=UINT32_MAX),
        "Return list of id, parent and distance to the root for node with distance < maxdist. "
        "connections is an array that should contains at the ith place all nodes connected to the ith node."
        "edgeweigthevaluator should be a function that takes as argument the ids of two nodes and return the weigth of the edge between these 2 nodes.");
}


