#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/mesh.h>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <string>
#include <sstream>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "../util/extract_list.h"
#include "../util/exception.h"

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

template<class T>
object py_poly_plit(T * polyline, real_t u){
    std::pair<RCPtr<T>,RCPtr<T> > res = polyline->split(u);
    return make_tuple(res.first,res.second);
}

DEF_POINTEE( Polyline )

/*
PolylinePtr gpl_fromlist( boost::python::object l ) 
{ 
  return PolylinePtr(new Polyline(extract_pgllist<Point3Array>(l).toRCPtr()));
}*/

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


void export_Polyline()
{
  class_<Polyline, PolylinePtr, bases<ExplicitModel, LineicModel>, boost::noncopyable>( "Polyline", 
	  init<Point3ArrayPtr, optional<Color4ArrayPtr> >("Polyline(Point3Array pointList, Color4Array colorList = None)",args("pointList","colorList")) )
    // .def( "__init__", make_constructor( gpl_fromlist ) ) 
    .def( "copy", &Polyline::copy )
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

/*
Polyline2DPtr gpl2_fromlist( boost::python::object l ) 
{ 
  return Polyline2DPtr(new Polyline2D(extract_pgllist<Point2Array>(l)()));
}*/

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

void export_Polyline2D()
{
  class_<Polyline2D, Polyline2DPtr, bases<Curve2D>, boost::noncopyable>( "Polyline2D", init<Point2ArrayPtr>() )
    // .def( "__init__", make_constructor( gpl2_fromlist ) ) 
    .def( "copy", &Polyline2D::copy )
    .def( "__getitem__", gpl2_getitem /*, return_internal_reference<1>()*/ )
    .def( "__setitem__", gpl2_setitem )
    .def( "__len__", gpl2_size )
    .def( "__repr__", gpl2_repr )
    .def( "getTangentAt", &Polyline2D::getTangentAt, args("u") )
    .def( "getNormalAt", &Polyline2D::getNormalAt, args("u") )
    .def( "split", &py_poly_plit<Polyline2D>, arg("u") )
	.DEC_PTR_PROPERTY(pointList,Polyline2D,PointList,Point2ArrayPtr)
    ;
  implicitly_convertible<Polyline2DPtr, Curve2DPtr>();
}

