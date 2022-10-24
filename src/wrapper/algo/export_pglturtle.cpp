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



#include <plantgl/algo/modelling/pglturtle.h>

#include <boost/python.hpp>
#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_list.h>

using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE

boost::python::object getTurtleColorList(PglTurtle * turtle) {
    return make_list(turtle->getColorList())();
}

void setTurtleColorList(PglTurtle * turtle, boost::python::object applist) {
    return turtle->setColorList(extract_vec<AppearancePtr>(applist)());
}

boost::python::object getTurtleSurfaceList(PglTurtle * turtle) {
    return make_dict(turtle->getSurfaceList())();
}

void export_PglTurtle()
{
  class_< PglTurtle , boost::noncopyable, bases < Turtle > >("PglTurtle", init<optional<TurtleDrawerPtr,TurtleParam *>>("PglTurtle([TurtleDrawerPtr], [TurtleParam]) -> Create a Pgl Turtle"))
    .def("getScene",  &PglTurtle::getScene, return_value_policy<return_by_value>() )
    .def("partialView",  &PglTurtle::partialView, "Return the current turtle construction.")
    .def("clearColorList",    &PglTurtle::clearColorList )
    .def("clearSurfaceList",  &PglTurtle::clearSurfaceList )
    .def("defaultValue",      &PglTurtle::defaultValue )
    .def("removeColor",       &PglTurtle::removeColor )
    .def("setSurface",        &PglTurtle::setSurface )
    .def("removeSurface",     &PglTurtle::removeSurface )

    .def("appendMaterial",    (void(PglTurtle::*)(const AppearancePtr&))&PglTurtle::appendMaterial )
    .def("appendMaterial",    (void(PglTurtle::*)(const ImageTexturePtr&))&PglTurtle::appendMaterial )
    .def("insertMaterial",    (void(PglTurtle::*)(size_t,const AppearancePtr&))&PglTurtle::insertMaterial )
    .def("insertMaterial",    (void(PglTurtle::*)(size_t,const ImageTexturePtr&))&PglTurtle::insertMaterial )
    .def("setMaterial",       (void(PglTurtle::*)(size_t,const AppearancePtr&))&PglTurtle::setMaterial )
    .def("setMaterial",       (void(PglTurtle::*)(size_t,const ImageTexturePtr&))&PglTurtle::setMaterial )

    .def("getMaterial",       &PglTurtle::getMaterial )

    .def("appendColor",       (void(PglTurtle::*)( uint_t, uint_t, uint_t))
                              &PglTurtle::appendColor )
    .def("appendColor",       (void(PglTurtle::*)( float, float, float))
                              &PglTurtle::appendColor )
    .def("setColorAt",          (void(PglTurtle::*)( size_t, uint_t, uint_t, uint_t))
                              &PglTurtle::setColorAt )
    .def("setColorAt",          (void(PglTurtle::*)( size_t, float, float, float))
                              &PglTurtle::setColorAt )
    .def("setColorAt",          (void(PglTurtle::*)( size_t,const Color3&))
                              &PglTurtle::setColorAt )
    .def("appendColor",       (void(PglTurtle::*)( const Color3&))
                              &PglTurtle::appendColor )
    .def("getColorList",      &getTurtleColorList )
    .def("setColorList",&setTurtleColorList )
    .def("getSurfaceList",    &getTurtleSurfaceList )
    .def("pglShape",    (void(PglTurtle::*)( const GeometryPtr, real_t))&PglTurtle::pglShape, "Insert a custom plantgl primitive at the turtle position and orientation", (bp::arg("geometry"),bp::arg("scale")=1), return_self<>() )
    .def("pglShape",    (void(PglTurtle::*)( const ShapePtr, real_t))&PglTurtle::pglShape, "Insert a custom plantgl primitive at the turtle position and orientation", (bp::arg("geometry"),bp::arg("scale")=1), return_self<>() )
    .def("pglShape",    (void(PglTurtle::*)( const ScenePtr, real_t))&PglTurtle::pglShape, "Insert a custom plantgl primitive at the turtle position and orientation", (bp::arg("geometry"),bp::arg("scale")=1), return_self<>() )
    ;
}
