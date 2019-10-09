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



#include <plantgl/algo/modelling/turtleparam.h>

#include <boost/python.hpp>
using namespace boost::python;
PGL_USING_NAMESPACE


#define EXPORT_PROP(PROPNAME,_CLASS) \
    def_readwrite(#PROPNAME, &_CLASS::PROPNAME)

    //add_property(#PROPNAME, make_getter(&_CLASS::PROPNAME), make_setter(&_CLASS::PROPNAME)) \

void export_TurtleParam()
{
  class_< TurtleParam >("TurtleParam", init<>("TurtleParam() -> Create default turtle parameters"))
    .def("isValid", &TurtleParam::isValid)
    .def("reset", &TurtleParam::reset)
    .def("dump", &TurtleParam::dump)
    .def("keepLastPoint", &TurtleParam::keepLastPoint)
    .def("removePoints", &TurtleParam::removePoints)
    .def("isPolygonOn", &TurtleParam::isPolygonOn)
    .def("isGeneralizedCylinderOn", &TurtleParam::isGeneralizedCylinderOn)
    .def("polygon", &TurtleParam::polygon)
    .def("generalizedCylinder", &TurtleParam::generalizedCylinder)
    .def("pushPosition", &TurtleParam::pushPosition)
    .def("transform", &TurtleParam::transform)
    .def("getOrientationMatrix", &TurtleParam::getOrientationMatrix)
    .def("getTransformationMatrix", &TurtleParam::getTransformationMatrix)
    .def("copy", &TurtleParam::copy, return_value_policy< manage_new_object >())
    .EXPORT_PROP(position,TurtleParam)
    .EXPORT_PROP(heading,TurtleParam)
    .EXPORT_PROP(up,TurtleParam)
    .EXPORT_PROP(left,TurtleParam)
    .EXPORT_PROP(scale,TurtleParam)
    .EXPORT_PROP(color,TurtleParam)
    .EXPORT_PROP(width,TurtleParam)
    .EXPORT_PROP(crossSection,TurtleParam)
    ;
}

