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



#include <plantgl/scenegraph/function/function.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#include <plantgl/python/export_list.h>
#include <plantgl/python/exception.h>


using namespace boost::python;

PGL_USING_NAMESPACE

DEF_POINTEE(QuantisedFunction)

object func_findX(QuantisedFunction * func, real_t y)
{
    bool found = false;
    real_t x = func->findX(y,found);
    if (!found) return object();
    else return object(x);
}

object func_findX2(QuantisedFunction * func, real_t y, real_t startingx)
{
    bool found = false;
    real_t x = func->findX(y,found,startingx);
    if (!found) return object();
    else return object(x);
}

real_t Func_getValue(QuantisedFunction * func, real_t x)
{
   if (func->getClamped() && (func->getFirstX() > x  || x > func->getLastX()))
      throw PythonExc_IndexError();
   else return func->getValue(x);
}

object Func_getSamples(QuantisedFunction * func)
{ return make_list(func->getSamples())(); }

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(isMonotonous_overloads, isMonotonous, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(isIncreasing_overloads, isIncreasing, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(isDecreasing_overloads, isDecreasing, 0, 1)

void export_Function()
{
  class_< QuantisedFunction, QuantisedFunctionPtr, boost::noncopyable >
    ("QuantisedFunction","A 2D quantised injective function (homomorphism) defined on the range [firstx,lastx].", init<const Curve2DPtr& , optional<uint_t> >
     (args("curve","sampling"),
     "QuantisedFunction(curve[,sampling,clamped]) : Quantised 2D function.\n"
     "If clamped parameter is set to False, if a x value is out of range, first or last value is returned.\n"
     "Otherwise an exception is raised."))
      .def(init<const Point2ArrayPtr& , optional<uint_t> >(args("points","sampling"),"Function(points [,sampling])"))
      // .def(init<const Point2ArrayPtr& , optional<uint_t> >())
      .def("__call__",&Func_getValue,args("x"))
      .def("getValue",&Func_getValue,args("x"))
      .def("findX",&func_findX,args("y"))
      .def("findX",&func_findX2,args("y","startingX"),"findX(y[,startingX]) : find the first x value such as f(x) = y.")
      .def("isMonotonous",&QuantisedFunction::isMonotonous,isMonotonous_overloads())
      .def("isIncreasing",&QuantisedFunction::isIncreasing,isIncreasing_overloads())
      .def("isDecreasing",&QuantisedFunction::isDecreasing,isDecreasing_overloads())
      .def("isValid",&QuantisedFunction::isValid)
      .def("inverse",&QuantisedFunction::inverse)
      .def("build",(bool(QuantisedFunction::*)(const Curve2DPtr&, uint_t))&QuantisedFunction::build,args("curve","sampling"))
      .def("build",(bool(QuantisedFunction::*)(const Curve2DPtr&))&QuantisedFunction::build)
      .add_property("sampling",&QuantisedFunction::getSampling)
      .add_static_property("DEFAULT_SAMPLING",make_getter(&QuantisedFunction::DEFAULT_SAMPLING))
      .add_property("firstx",&QuantisedFunction::getFirstX)
      .add_property("lastx",&QuantisedFunction::getLastX)
      .DEC_BT_NR_PROPERTY_WDV(clamped,QuantisedFunction,Clamped,bool,DEFAULT_CLAMPED)
      .def("_getSamples",&Func_getSamples)
      .def("checkQuantisableFunction",&QuantisedFunction::check)
      .staticmethod("checkQuantisableFunction");
    ;

  // implicitly_convertible< FontPtr, SceneObjectPtr >();
}
