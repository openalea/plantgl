#include "polyline.h"

#include <geom_polyline.h>
#include <geom_pointarray.h>
#include <geom_mesh.h>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <string>
#include <sstream>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "exception.hh"

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE( GeomPolyline )

GeomPolylinePtr gpl_fromlist( boost::python::object l ) 
{ 
  Point3Array * array = new Point3Array();
  object iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );
  while( 1 )
  {
	    object obj;
		try { 
          obj = iter_obj.attr( "next" )();
		} catch( error_already_set ) { PyErr_Clear(); break; }
        Vector3 val = boost::python::extract<Vector3>( obj );
        array->pushBack( val );
  }
  return GeomPolylinePtr(new GeomPolyline(array));
}

Vector3 gpl_getitem( GeomPolyline* p, size_t pos )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	return p->getPointListAt( pos );
  else throw PythonExc_IndexError();
}

void gpl_setitem( GeomPolyline* p, size_t pos, Vector3* v )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	p->getPointListAt( pos ) = *v;
  else throw PythonExc_IndexError();
}

size_t gpl_size( GeomPolyline* pl )
{
  return (pl->getPointList()?pl->getPointList()->getSize():0);
}

std::string gpl_repr( GeomPolyline* p )
{
  std::stringstream ss;
  if( p->getPointListSize() == 0 )
    {
      return "Polyline(Point3Array([]))";
    }
  Vector3 v = p->getPointListAt( 0 );
  ss << "Polyline(Point3Array([Vector3(" << v.x() << "," << v.y()
     << "," << v.z() << ")";
  size_t i;
  for( i = 1 ; i < p->getPointListSize() ; ++i )
    {
      v = p->getPointListAt( i );
      ss << ",Vector3(" << v.x() << "," << v.y() << "," << v.z() << ")";
    }
  ss << "]))";
  return ss.str();
}


void class_Polyline()
{
  class_<GeomPolyline, GeomPolylinePtr, bases<ExplicitModel, LineicModel>, boost::noncopyable>( "Polyline", 
	  init<Point3ArrayPtr, optional<Color4ArrayPtr> >("Polyline(Point3Array pointList, Color4Array colorList = None)",args("pointList","colorList")) )
    .def( "__init__", make_constructor( gpl_fromlist ) ) 
    .def( "copy", &GeomPolyline::copy )
    .def( "__getitem__", gpl_getitem /*, return_internal_reference<1>() */)
    .def( "__setitem__", gpl_setitem )
    .def( "__len__", gpl_size )
    .def( "__repr__", gpl_repr )
    ;
  implicitly_convertible<GeomPolylinePtr, ExplicitModelPtr>();
  implicitly_convertible<GeomPolylinePtr, LineicModelPtr>();
}

DEF_POINTEE( GeomPolyline2D )

GeomPolyline2DPtr gpl2_fromlist( boost::python::object l ) 
{ 
  Point2Array * array = new Point2Array();
  object iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );
  while( 1 )
  {
	   object obj;
		try { 
          obj = iter_obj.attr( "next" )();
		}
		catch( error_already_set ) { PyErr_Clear(); break; }
        Vector2 val = boost::python::extract<Vector2>( obj );
        array->pushBack( val );
  }
  return GeomPolyline2DPtr(new GeomPolyline2D(array));
}

Vector2 gpl2_getitem( GeomPolyline2D* p, size_t pos )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	return p->getPointListAt( pos );
  else throw PythonExc_IndexError();
}

void gpl2_setitem( GeomPolyline2D* p, size_t pos, Vector2* v )
{
  if (p->getPointList() && pos < p->getPointList()->getSize())
	p->getPointListAt( pos ) = *v;
  else throw PythonExc_IndexError();
}


size_t gpl2_size( GeomPolyline2D* pl )
{
  return (pl->getPointList()?pl->getPointList()->getSize():0);
}

std::string gpl2_repr( GeomPolyline2D* p )
{
  std::stringstream ss;
  if( p->getPointListSize() == 0 )
    {
      return "Polyline2D(Point2Array([]))";
    }
  Vector2 v = p->getPointListAt( 0 );
  ss << "Polyline2D(Point2Array([Vector2(" << v.x() << "," << v.y() << ")";
  size_t i;
  for( i = 1 ; i < p->getPointListSize() ; ++i )
    {
      v = p->getPointListAt( i );
      ss << ",Vector2(" << v.x() << "," << v.y() << ")";
    }
  ss << "]))";
  return ss.str();
}

SETGET(GeomPolyline2D,PointList,Point2ArrayPtr);


void class_Polyline2D()
{
  class_<GeomPolyline2D, GeomPolyline2DPtr, bases<Curve2D>, boost::noncopyable>( "Polyline2D", init<Point2ArrayPtr>() )
    .def( "__init__", make_constructor( gpl2_fromlist ) ) 
    .def( "copy", &GeomPolyline2D::copy )
    .def( "__getitem__", gpl2_getitem /*, return_internal_reference<1>()*/ )
    .def( "__setitem__", gpl2_setitem )
    .def( "__len__", gpl2_size )
    .def( "__repr__", gpl2_repr )
	.DEC_SETGET(pointList,GeomPolyline2D,PointList,Point2ArrayPtr)
    ;
  implicitly_convertible<GeomPolyline2DPtr, Curve2DPtr>();
}

