#include "lineicmodel.h"
#include "macro_refptr.h"
#include "polyline.h"
#include "beziercurve.h"
#include "nurbscurve.h"
#include "pointset.h"

#include <geom_lineicmodel.h>
#include <geom_curve.h>
#include <geom_disc.h>
#include <geom_sor.h>

#include <boost/python.hpp>

using namespace boost::python;

GEOM_USING_NAMESPACE

DEF_POINTEE( LineicModel )
DEF_POINTEE( PlanarModel )
DEF_POINTEE( Curve2D )
DEF_POINTEE( SOR2D )
DEF_POINTEE( Disc )

void class_LineicModel()
{
  class_<LineicModel,LineicModelPtr, bases<Primitive>, boost::noncopyable>( "LineicModel", no_init )
    .def( "getLength", &LineicModel::getLength )
    ;

  implicitly_convertible<LineicModelPtr, PrimitivePtr>();

  class_Polyline();
  class_BezierCurve();
  class_NurbsCurve();

}

void class_PlanarModel()
{
  class_<PlanarModel,PlanarModelPtr, bases<ParametricModel>, boost::noncopyable>( "PlanarModel", no_init )
    ;

  implicitly_convertible<PlanarModelPtr, ParametricModelPtr>();

  class_Curve2D();
  class_PointSet2D();
}

void class_Curve2D()
{
  class_<Curve2D,Curve2DPtr, bases<PlanarModel>, boost::noncopyable>( "Curve2D", no_init )
    ;

  implicitly_convertible<Curve2DPtr, PlanarModelPtr>();

  class_Polyline2D();
  class_BezierCurve2D();
  class_NurbsCurve2D();
  class_SOR2D();
  class_Disc();

}

void class_SOR2D()
{
  class_<SOR2D,SOR2DPtr, bases<PlanarModel>, boost::noncopyable>( "SOR2D", no_init )
    ;

  implicitly_convertible<SOR2DPtr, PlanarModelPtr>();

}

void class_Disc()
{
  class_<Disc,DiscPtr, bases<SOR2D>, boost::noncopyable>( "Disc", init< optional<real_t,uchar_t> >("Disc(radius, slices)") )
    ;

  implicitly_convertible<DiscPtr,SOR2DPtr>();

}
