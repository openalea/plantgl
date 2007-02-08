#include "../util/export_refcountptr.h"
#include <scenegraph/geometry/lineicmodel.h>
#include <scenegraph/geometry/curve.h>
#include <scenegraph/geometry/disc.h>
#include <scenegraph/geometry/sor.h>

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
    .def( "getLength", &LineicModel::getLength )
    ;

  implicitly_convertible<LineicModelPtr, PrimitivePtr>();

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
