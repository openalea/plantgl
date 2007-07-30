#include "../util/export_refcountptr.h"
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/geometry/curve.h>
#include <plantgl/scenegraph/geometry/disc.h>
#include <plantgl/scenegraph/geometry/sor.h>

#include <boost/python.hpp>

using namespace boost::python;

PGL_USING_NAMESPACE

DEF_POINTEE( LineicModel )
DEF_POINTEE( PlanarModel )
DEF_POINTEE( Curve2D )
DEF_POINTEE( SOR2D )
DEF_POINTEE( Disc )

void export_LineicModel()
{
  class_<LineicModel,LineicModelPtr, bases<Primitive>, boost::noncopyable>( "LineicModel", no_init )
    .add_property( "firstKnot", &LineicModel::getFirstKnot )
    .add_property( "lastKnot", &LineicModel::getLastKnot )
    .def( "getStride", &LineicModel::getStride )
    .def( "getPointAt", &LineicModel::getPointAt, args("u") )
    .def( "getTangentAt", &LineicModel::getTangentAt, args("u") )
    .def( "getNormalAt", &LineicModel::getNormalAt, args("u") )
    .def( "findClosest", &LineicModel::findClosest, args("point") )
    .def( "getLength", &LineicModel::getLength )
    ;

  implicitly_convertible<LineicModelPtr, PrimitivePtr>();

  def("closestPointToSegment",&closestPointToSegment, args("point","segA","segB"));
}

void export_PlanarModel()
{
  class_<PlanarModel,PlanarModelPtr, bases<ParametricModel>, boost::noncopyable>( "PlanarModel", no_init )
    ;

  implicitly_convertible<PlanarModelPtr, ParametricModelPtr>();

}

void export_Curve2D()
{
  class_<Curve2D,Curve2DPtr, bases<PlanarModel>, boost::noncopyable>( "Curve2D", no_init )
    .add_property( "firstKnot", &Curve2D::getFirstKnot )
    .add_property( "lastKnot", &Curve2D::getLastKnot )
    .def( "getStride", &Curve2D::getStride )
    .def( "getPointAt", &Curve2D::getPointAt, args("u") )
    .def( "getLength", &LineicModel::getLength )
    ;

  implicitly_convertible<Curve2DPtr, PlanarModelPtr>();

}

void export_SOR2D()
{
  class_<SOR2D,SOR2DPtr, bases<PlanarModel>, boost::noncopyable>( "SOR2D", no_init )
    ;

  implicitly_convertible<SOR2DPtr, PlanarModelPtr>();

}

void export_Disc()
{
  class_<Disc,DiscPtr, bases<SOR2D>, boost::noncopyable>( "Disc", init< optional<real_t,uchar_t> >("Disc(radius, slices)") )
    ;

  implicitly_convertible<DiscPtr,SOR2DPtr>();

}
