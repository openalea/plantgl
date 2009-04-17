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

#include <plantgl/python/export_property.h>
#include <plantgl/algo/fitting/fit.h>
#include <plantgl/algo/base/discretizer.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

boost::python::list to_pylist(const std::vector<std::string>& l){
	boost::python::list res;
	for(std::vector<std::string>::const_iterator _it = l.begin(); _it != l.end(); _it++)
		res.append(boost::python::object(*_it));
	return res;
}

boost::python::list getClassNames(){
	return to_pylist(Fit::getClassNames());
}

boost::python::list getVolumeClassNames(){
	return to_pylist(Fit::getVolumeClassNames());
}

GeometryPtr fit(std::string algo,GeometryPtr src){
	Discretizer d;
	src->apply(d);
	Fit f;
	f.setPoints(d.getDiscretization()->getPointList());
	return f.use(algo);
}

boost::python::object inertiaAxis(Point3Array * points){
	Vector3 u,v,w,s;
	bool res = Fit::inertiaAxis(Point3ArrayPtr(points),u,v,w,s);
	if (!res) return object();
	else return make_tuple(u,v,w,s);
}

boost::python::object inertiaAxis2d(Point2Array * points){
	Vector2 u,v,s;
	bool res = Fit::inertiaAxis(Point2ArrayPtr(points),u,v,s);
	if (!res) return object();
	else return make_tuple(u,v,s);
}

boost::python::object py_boundingCircle(Point2Array * points){
	Vector2 center;
	real_t radius;
	bool res = Fit::boundingCircle(Point2ArrayPtr(points),center,radius);
	if (!res) return object();
	else return make_tuple(center,radius);
}

/* ----------------------------------------------------------------------- */

void export_Fit()
{
  class_< Fit > ("Fit", init<>
     ( "Fit()" "fitting algorithms." ))
    .def("use",&Fit::use)
    .def("__call__",&Fit::use)
	.add_property("points",&Fit::getPoints,&Fit::setPoints)
	.add_property("radius",&Fit::getRadius,&Fit::setRadius)
    .def("sphere",&Fit::sphere)
    .def("asphere",&Fit::asphere)
    .def("bsphere",&Fit::bsphere)
    .def("cylinder",&Fit::cylinder)
    .def("acylinder",&Fit::acylinder)
    .def("bcylinder",&Fit::bcylinder)
    .def("ellipsoid",&Fit::ellipsoid)
    .def("ellipsoid2",&Fit::ellipsoid2)
    .def("bellipsoid",&Fit::bellipsoid)
    .def("aellipsoid",&Fit::aellipsoid)
    .def("aalignedbox",&Fit::aalignedbox)
    .def("balignedbox",&Fit::balignedbox)
    .def("box",&Fit::box)
    .def("abox",&Fit::abox)
    .def("bbox",&Fit::bbox)
    // .def("frustum",&Fit::frustum)
    .def("extrudedHull",&Fit::extrudedHull)
    .def("asymmetricHull",&Fit::asymmetricHull)
    .def("convexHull",&Fit::convexHull)
    .def("convexPolyline",&Fit::convexPolyline)
	.staticmethod("convexPolyline")
    .def("nurbsCurve",(LineicModelPtr(*)(const Point3ArrayPtr &, int, int))&Fit::nurbsCurve,args("points","degree","nbctrlpoints"))
	.staticmethod("nurbsCurve")
    .def("getClassNames",&getClassNames)
	.staticmethod("getClassNames")
    .def("getVolumeClassNames",&getVolumeClassNames)
	.staticmethod("getVolumeClassNames")
    .def("available",&Fit::available)
	.staticmethod("available")
    .def("areRadiusNeeded",&Fit::areRadiusNeeded)
	.staticmethod("areRadiusNeeded")
	.def("inertiaAxis",inertiaAxis,args("points"))
	.def("inertiaAxis",inertiaAxis2d,args("points"))
    .staticmethod("inertiaAxis")
	.def("boundingCircle",py_boundingCircle,args("points"))
    .staticmethod("boundingCircle")
    ;
  
  
  def("fit",&fit,args("algo","src"));
  def("inertiaAxis",inertiaAxis,args("points"));
  def("inertiaAxis",inertiaAxis2d,args("points"));
}

/* ----------------------------------------------------------------------- */
