/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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


#include <plantgl/scenegraph/geometry/plane.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Plane3)


void export_Plane()
{
  class_< Plane3, Plane3Ptr, bases<RefCountObject>>
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

