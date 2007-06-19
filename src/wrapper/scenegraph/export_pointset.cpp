#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/geometry/mesh.h>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <string>
#include <sstream>

#include "../util/export_refcountptr.h"
#include "../util/extract_list.h"

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE( PointSet )

PointSetPtr gps_fromlist( boost::python::object l ) 
{ 
  return new PointSet(extract_pgllist<Point3Array>(l)());
}

Vector3 gps_getitem( PointSet* p, size_t pos )
{
  return p->getPointListAt( pos );
}

void gps_setitem( PointSet* p, size_t pos, Vector3* v )
{
  p->getPointListAt( pos ) = *v;
}

std::string gps_repr( PointSet* p )
{
  std::stringstream ss;
  if( p->getPointListSize() == 0 )
    {
      return "PointSet(Point3Array([]))";
    }
  Vector3 v = p->getPointListAt( 0 );
  ss << "PointSet(Point3Array([Vector3(" << v.x() << "," << v.y()
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

Point3ArrayPtr gps_getPointList( PointSet* pl )
{
  return pl->getPointList();
}

void export_PointSet()
{
  class_< PointSet, PointSetPtr, bases<ExplicitModel>, boost::noncopyable>( "PointSet", 
	  init<Point3ArrayPtr, optional<Color4ArrayPtr> >("PointSet(Point3Array pointList, Color4Array colorList = None)",args("pointList","colorList")) )
    .def( "__init__", make_constructor( gps_fromlist ) ) 
    .def( "copy", &PointSet::copy )
    .def( "__getitem__", gps_getitem /*, return_internal_reference<1>() */)
    .def( "__setitem__", gps_setitem )
    .def( "__repr__", gps_repr )
    .def( "getPointList", gps_getPointList )
    .def( "transform", &PointSet::transform )
    ;
  implicitly_convertible<PointSetPtr, ExplicitModelPtr>();
}

DEF_POINTEE( PointSet2D )

PointSet2DPtr gps2d_fromlist( boost::python::object l ) 
{ 
  return new PointSet2D(extract_pgllist<Point2Array>(l)());
}

Vector2 gps2d_getitem( PointSet2D* p, size_t pos )
{
  return p->getPointListAt( pos );
}

void gps2d_setitem( PointSet2D* p, size_t pos, Vector2* v )
{
  p->getPointListAt( pos ) = *v;
}

std::string gps2d_repr( PointSet2D* p )
{
  std::stringstream ss;
  if( p->getPointListSize() == 0 )
    {
      return "PointSet2D(Point2Array([]))";
    }
  Vector2 v = p->getPointListAt( 0 );
  ss << "PointSet2D(Point2Array([Vector2(" << v.x() << "," << v.y() <<  ")";
  size_t i;
  for( i = 1 ; i < p->getPointListSize() ; ++i )
    {
      v = p->getPointListAt( i );
      ss << ",Vector2(" << v.x() << "," << v.y() << ")";
    }
  ss << "]))";
  return ss.str();
}

Point2ArrayPtr gps2d_getPointList( PointSet2D* pl )
{
  return pl->getPointList();
}

void export_PointSet2D()
{
  class_< PointSet2D, PointSet2DPtr, bases<PlanarModel>, boost::noncopyable>( "PointSet2D", init<Point2ArrayPtr>() )
    .def( "__init__", make_constructor( gps_fromlist ) ) 
    .def( "copy", &PointSet2D::copy )
    .def( "__getitem__", gps2d_getitem /*, return_internal_reference<1>() */)
    .def( "__setitem__", gps2d_setitem )
    .def( "__repr__", gps2d_repr )
    .def( "getPointList", gps2d_getPointList )
//    .def( "transform", &PointSet2D::transform )
    ;
  implicitly_convertible<PointSet2DPtr, PlanarModelPtr>();
}

