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



#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/python/exception.h>
#include "export_sceneobject.h"

#include <string>
#include <sstream>

using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE

template<class T>
object py_poly_split(T * polyline, real_t u){
    std::pair<RCPtr<T>,RCPtr<T> > res = polyline->split(u);
    return boost::python::make_tuple(res.first,res.second);
}

DEF_POINTEE( Polyline )


Vector3 gpl_getitem( Polyline* p, size_t pos )
{
  if (p->getPointList() && pos < p->getPointList()->size())
    return p->getPointListAt( pos );
  else throw PythonExc_IndexError();
}

void gpl_setitem( Polyline* p, size_t pos, Vector3* v )
{
  if (p->getPointList() && pos < p->getPointList()->size())
    p->getPointListAt( pos ) = *v;
  else throw PythonExc_IndexError();
}

size_t gpl_size( Polyline* pl )
{
  return (pl->getPointList()?pl->getPointList()->size():0);
}

std::string gpl_repr( Polyline* p )
{
  std::stringstream ss;
  ss << "Polyline(" << extract<std::string>(boost::python::str(boost::python::object(p->getPointList())))();
  if (p->getColorList())
    ss << "," << extract<std::string>(boost::python::str(boost::python::object(p->getColorList())))() ;
  ss << ")";
  return ss.str();
}


void export_Polyline()
{
  class_<Polyline, PolylinePtr, bases<ExplicitModel, LineicModel>, boost::noncopyable>( "Polyline",
      "A Polyline describes a curve formed by connected segments.\n"
      "It is defined by a set of points. A color can be associated to each point for visualisation.",
      init<Point3ArrayPtr, optional<Color4ArrayPtr> >("Polyline(Point3Array pointList, Color4Array colorList = None)",args("pointList","colorList")) )
    .def(init<Point3ArrayPtr, Color4ArrayPtr, uchar_t >("Polyline(Point3Array pointList, Color4Array colorList = None, width = 1)",(bp::arg("pointList"),bp::arg("colorList"),bp::arg("width")=Polyline::DEFAULT_WIDTH)))
    .def(init<Point3ArrayPtr, uchar_t >("Polyline(Point3Array pointList, width = 1)",(bp::arg("pointList"),bp::arg("width")=Polyline::DEFAULT_WIDTH)))
    .DEF_PGLBASE(Polyline)
    .def( "__getitem__", gpl_getitem /*, return_internal_reference<1>() */)
    .def( "__setitem__", gpl_setitem )
    .def( "__len__", gpl_size )
    .def( "__repr__", gpl_repr )
    .def( "split", &py_poly_split<Polyline>, arg("u") )
    ;
  implicitly_convertible<PolylinePtr, ExplicitModelPtr>();
  implicitly_convertible<PolylinePtr, LineicModelPtr>();
}

DEF_POINTEE( Polyline2D )

Vector2 gpl2_getitem( Polyline2D* p, size_t pos )
{
  if (p->getPointList() && pos < p->getPointList()->size())
    return p->getPointListAt( pos );
  else throw PythonExc_IndexError();
}

void gpl2_setitem( Polyline2D* p, size_t pos, Vector2* v )
{
  if (p->getPointList() && pos < p->getPointList()->size())
    p->getPointListAt( pos ) = *v;
  else throw PythonExc_IndexError();
}


size_t gpl2_size( Polyline2D* pl )
{
  return (pl->getPointList()?pl->getPointList()->size():0);
}

std::string gpl2_repr( Polyline2D* p )
{
  std::stringstream ss;
  ss << "Polyline2D(" << extract<std::string>(boost::python::str(boost::python::object(p->getPointList())))() << ")";
  return ss.str();
}

void export_Polyline2D()
{
  class_<Polyline2D, Polyline2DPtr, bases<Curve2D>, boost::noncopyable>(
      "Polyline2D",
      "A Polyline2D describes a curve formed by connected 2D segments.\n"
      "It is defined by a set of 2D points. See Polyline.",
      init<Point2ArrayPtr,optional<uchar_t> >("Polyline2D(pointList[,width])",(bp::arg("pointList"),bp::arg("width") = Polyline2D::DEFAULT_WIDTH)) )
    .DEF_PGLBASE(Polyline2D)
    .def( "__getitem__", gpl2_getitem )
    .def( "__setitem__", gpl2_setitem )
    .def( "__len__", gpl2_size )
    .def( "__repr__", gpl2_repr )
    .def( "getTangentAt", &Polyline2D::getTangentAt, args("u") )
    .def( "getNormalAt", &Polyline2D::getNormalAt, args("u") )
    .def( "split", &py_poly_split<Polyline2D>, arg("u") )
    .DEC_PTR_PROPERTY(pointList,Polyline2D,PointList,Point2ArrayPtr)
    .def("Circle",&Polyline2D::Circle,(bp::arg("radius")=Disc::DEFAULT_RADIUS,bp::arg("slices")=SOR::DEFAULT_SLICES))
    .staticmethod("Circle")
    .def("ArcOfCircle",&Polyline2D::ArcOfCircle,(bp::arg("radius")=Disc::DEFAULT_RADIUS,
                                                 bp::arg("starting_angle")=0,
                                                 bp::arg("angle_range")=GEOM_PI,
                                                 bp::arg("slices")=SOR::DEFAULT_SLICES))
    .staticmethod("ArcOfCircle")
    .def("SuperEllipsis",&Polyline2D::SuperEllipsis,(bp::arg("radius")=Disc::DEFAULT_RADIUS,
                                                     bp::arg("height")=Disc::DEFAULT_RADIUS,
                                                     bp::arg("degree")=2,
                                                     bp::arg("starting_angle")=0,
                                                     bp::arg("angle_range")=GEOM_TWO_PI,
                                                     bp::arg("slices")=SOR::DEFAULT_SLICES))
    .staticmethod("SuperEllipsis")
    ;
  implicitly_convertible<Polyline2DPtr, Curve2DPtr>();
}

