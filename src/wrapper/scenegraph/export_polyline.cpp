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
TOOLS_USING_NAMESPACE

template<class T>
object py_poly_plit(T * polyline, real_t u){
    std::pair<RCPtr<T>,RCPtr<T> > res = polyline->split(u);
	return boost::python::make_tuple(res.first,res.second);
}

DEF_POINTEE( Polyline )


Vector3 gpl_getitem( Polyline* p, size_t pos )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	return p->getPointListAt( pos );
  else throw PythonExc_IndexError();
}

void gpl_setitem( Polyline* p, size_t pos, Vector3* v )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	p->getPointListAt( pos ) = *v;
  else throw PythonExc_IndexError();
}

size_t gpl_size( Polyline* pl )
{
  return (pl->getPointList()?pl->getPointList()->getSize():0);
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
    .DEF_PGLBASE(Polyline)
    .def( "__getitem__", gpl_getitem /*, return_internal_reference<1>() */)
    .def( "__setitem__", gpl_setitem )
    .def( "__len__", gpl_size )
    .def( "__repr__", gpl_repr )
    .def( "split", &py_poly_plit<Polyline>, arg("u") )
    ;
  implicitly_convertible<PolylinePtr, ExplicitModelPtr>();
  implicitly_convertible<PolylinePtr, LineicModelPtr>();
}

DEF_POINTEE( Polyline2D )

Vector2 gpl2_getitem( Polyline2D* p, size_t pos )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	return p->getPointListAt( pos );
  else throw PythonExc_IndexError();
}

void gpl2_setitem( Polyline2D* p, size_t pos, Vector2* v )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	p->getPointListAt( pos ) = *v;
  else throw PythonExc_IndexError();
}


size_t gpl2_size( Polyline2D* pl )
{
  return (pl->getPointList()?pl->getPointList()->getSize():0);
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
	  init<Point2ArrayPtr>() )
    .DEF_PGLBASE(Polyline2D)
    .def( "__getitem__", gpl2_getitem )
    .def( "__setitem__", gpl2_setitem )
    .def( "__len__", gpl2_size )
    .def( "__repr__", gpl2_repr )
    .def( "getTangentAt", &Polyline2D::getTangentAt, args("u") )
    .def( "getNormalAt", &Polyline2D::getNormalAt, args("u") )
    .def( "split", &py_poly_plit<Polyline2D>, arg("u") )
	.DEC_PTR_PROPERTY(pointList,Polyline2D,PointList,Point2ArrayPtr)
	.def("Circle",&Polyline2D::Circle,(bp::arg("radius")=Disc::DEFAULT_RADIUS,bp::arg("slices")=SOR::DEFAULT_SLICES))
    .staticmethod("Circle")
    ;
  implicitly_convertible<Polyline2DPtr, Curve2DPtr>();
}

