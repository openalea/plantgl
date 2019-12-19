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

#include <plantgl/algo/base/surfcomputer.h>
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

/* ----------------------------------------------------------------------- */

real_t surf_geom(Geometry * obj){
    Discretizer d;
    SurfComputer sf(d);
    obj->apply(sf);
    return sf.getSurface();
}

real_t surf_sh(Shape * obj){
    Discretizer d;
    SurfComputer sf(d);
    obj->apply(sf);
    return sf.getSurface();
}

RealArrayPtr surfaces(Index3Array * triangles, Point3Array * points){
    RealArrayPtr result(new RealArray(triangles->size(),0));
    RealArray::iterator itres = result->begin();
    for(Index3Array::const_iterator it = triangles->begin(); it != triangles->end(); ++it, ++itres){
        *itres = surface(points->getAt(it->getAt(0)),points->getAt(it->getAt(1)),points->getAt(it->getAt(2)));
    }
    return result;
}

/* ----------------------------------------------------------------------- */

void export_SurfComputer()
{
  class_< SurfComputer, bases<Action>, boost::noncopyable >
    ("SurfComputer", init<Discretizer&>("SurfComputer() -> compute the object surface"))
    .def("clear",&SurfComputer::clear)
    .def("process", (bool (SurfComputer::*)(const ScenePtr))&SurfComputer::process)
    .add_property("surface", &SurfComputer::getSurface, "Return the surface of the shape")
    .add_property("result",  &SurfComputer::getSurface)
    ;

  def("surface",(real_t(*)(const ScenePtr))&sceneSurface,"Compute surface of a scene");
  def("surface",&surf_geom,"Compute surface of a geometry");
  def("surface",&surf_sh,"Compute surface of a shape");
  def("surface",(real_t(*)(const Vector2&,const Vector2&,const Vector2&))&surface,"Compute surface of a 2D triangle");
  def("surface",(real_t(*)(const Vector3&,const Vector3&,const Vector3&))&surface,"Compute surface of a triangle");
  def("surfaces",&surfaces,"Compute the surfaces of a set of triangles");
}
