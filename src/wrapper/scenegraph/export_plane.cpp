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




#include <plantgl/scenegraph/geometry/plane.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Plane3)


void export_Plane()
{
  class_< Plane3, Plane3Ptr, bases<RefCountObject> >
    ("Plane3", "A 3D plane",
    init<optional<const Vector3&,real_t> > ("Plane3(normal, d)",(bp::arg("normal")=Vector3::OZ, bp::arg("d")=0)))
  .def(init<const Vector3&, const Vector3& > ("Plane3(normal, origin)",(bp::arg("normal")=Vector3::OZ, bp::arg("origin")=Vector3::ORIGIN)))
  .def(init<real_t, real_t, real_t>  ("Plane3(alpha, beta, d)",(bp::arg("alpha")=0, bp::arg("beta")=0,bp::arg("d")=0)))
  .def(init<real_t, real_t, real_t, real_t> ("Plane3(a, b, c, d)",(bp::arg("a")=0, bp::arg("b")=0, bp::arg("c")=1,bp::arg("d")=0)))
  .DEC_BT_PROPERTY(normal,Plane3,Normal,Vector3)
  .DEC_BT_PROPERTY(d,Plane3,D,real_t)
  .add_property("a",&Plane3::a)
  .add_property("b",&Plane3::b)
  .add_property("c",&Plane3::c)
  .def("getDistance",&Plane3::getDistance)
  .def("base1",&Plane3::base1)
  .def("base2",&Plane3::base2)
  ;


  implicitly_convertible<Plane3Ptr, RefCountObjectPtr >();
}

