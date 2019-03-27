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



#include <plantgl/python/export_property.h>
#include <plantgl/algo/fitting/fit.h>
#include <plantgl/algo/base/discretizer.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
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

boost::python::object py_plane(Point3Array * points, const Index& subset = Index()){
    Vector3 center;
    Plane3 plane;
    bool res = Fit::plane(Point3ArrayPtr(points),center,plane,subset);
    if (!res) return object();
    else return make_tuple(center,plane);
}

/* ----------------------------------------------------------------------- */

void export_Fit()
{
  class_< Fit > ("Fit", init<>
     ( "Fit()" "fitting algorithms." ))
    .def(init<Point3ArrayPtr>("Fit(points)",args("points")))
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
    .def("plane",py_plane,(arg("points"),arg("subset")=Index()))
    .staticmethod("plane")
    .def("boundingCircle",py_boundingCircle,args("points"))
    .staticmethod("boundingCircle")
    .def("fitShapeFactor",Fit::fitShapeFactor,args("x","r","y","h"))
    .staticmethod("fitShapeFactor")
    ;


  def("fit",&fit,args("algo","src"));
  def("inertiaAxis",inertiaAxis,args("points"));
  def("inertiaAxis",inertiaAxis2d,args("points"));
}

/* ----------------------------------------------------------------------- */
