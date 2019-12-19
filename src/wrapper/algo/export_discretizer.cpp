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



#include <boost/python.hpp>

#include <plantgl/algo/base/discretizer.h>
#include <plantgl/algo/base/tesselator.h>
#include <plantgl/scenegraph/geometry/explicitmodel.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/python/exception.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python
/* ----------------------------------------------------------------------- */

ExplicitModelPtr d_getDiscretization( Discretizer* d )
{ return d->getDiscretization(); }

/* ----------------------------------------------------------------------- */

bool get_Dis_texCoord(Discretizer * obj){
  return obj->texCoordComputed();
}
void set_Dis_texCoord(Discretizer * obj, bool v){
  obj->computeTexCoord(v);
}

ExplicitModelPtr py_discretize( const GeometryPtr& obj) {
    if (!obj)throw PythonExc_ValueError("Cannot discretize empty object.");
    Discretizer d;
    if (!obj->apply(d))throw PythonExc_ValueError("Error in discretization.");
    else return d.getDiscretization();
}

/* ----------------------------------------------------------------------- */

void export_Discretizer()
{
  class_< Discretizer,bases< Action >,boost::noncopyable >
    ("Discretizer", init<>("Discretizer() -> Compute the objects discretization" ))
    .def("clear",&Discretizer::clear)
    .add_property("discretization",d_getDiscretization, "Return the last computed discretization.")
    .add_property("texCoord",get_Dis_texCoord,set_Dis_texCoord)
    .add_property("result",d_getDiscretization)
    ;

   def("discretize",&py_discretize);
}

/* ----------------------------------------------------------------------- */

TriangleSetPtr  t_getTriangulation ( Tesselator* t )
{ return t->getTriangulation(); }

TriangleSetPtr py_tesselate( const GeometryPtr& obj) {
    if (!obj)throw PythonExc_ValueError("Cannot tesselate empty object.");
    Tesselator t;
    if (!obj->apply(t))throw PythonExc_ValueError("Error in tesselation.");
    else return t.getTriangulation();
}

TriangleSetPtr py_triangulation( const GeometryPtr& obj) {
    if (!obj)throw PythonExc_ValueError("Cannot tesselate empty object.");
    Tesselator t;
    if (!obj->apply(t))throw PythonExc_ValueError("Error in tesselation.");
    else return t.getTriangulation();
}


/* ----------------------------------------------------------------------- */

void export_Tesselator()
{

  class_< Tesselator,bases< Discretizer >,boost::noncopyable >
    ("Tesselator", init<>("Tesselator() -> Compute tobjects triangulation. " ))
    .add_property("triangulation",t_getTriangulation,"Return the last computed triangulation.")
    .add_property("result",t_getTriangulation)
    ;
   def("tesselate",&py_tesselate);

   enum_<TriangulationMethod>("TriangulationMethod")
    .value("eStarTriangulation",eStarTriangulation)
    .value("eConvexTriangulation",eConvexTriangulation)
    .value("eGreeneTriangulation",eGreeneTriangulation)
    .value("eOptimalTriangulation", eOptimalTriangulation)
    .value("eYMonotonePartitioning", eYMonotonePartitioning)
      .export_values()
      ;

   def("polygonization",&polygonization,(bp::arg("contour"),bp::arg("method")=eConvexTriangulation));
   def("triangulation",&triangulation,(bp::arg("contour"),bp::arg("method")=eConvexTriangulation));
   def("is_simple_polygon",&is_simple_polygon,(bp::arg("contour")));

}

/* ----------------------------------------------------------------------- */

