#include "../util/export_refcountptr.h"
#include "../util/exception.h"

#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/geometry/curve.h>
#include <plantgl/scenegraph/geometry/disc.h>
#include <plantgl/scenegraph/geometry/sor.h>

#include <boost/python.hpp>

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE( LineicModel )
DEF_POINTEE( PlanarModel )
DEF_POINTEE( Curve2D )
DEF_POINTEE( SOR2D )
DEF_POINTEE( Disc )

object lm_findclosest(LineicModel * lm, Vector3 point)
{
    real_t u;
    Vector3 res = lm->findClosest(point,&u);
    return make_tuple(res,u);
}

object seg_findclosest(Vector3 point, Vector3 segA, Vector3 segB)
{
    real_t u;
    real_t dist = closestPointToSegment(point,segA,segB,&u);
    return make_tuple(point,dist,u);
}

template <class T, class U, U (T::* func)(real_t) const >
U getCurveValue(const T * lm, real_t u){
   if (lm->getFirstKnot() - GEOM_EPSILON > u || lm->getLastKnot() + GEOM_EPSILON < u )
       throw PythonExc_IndexError();
   return (lm->*func)(u);
}

void export_LineicModel()
{
  class_<LineicModel,LineicModelPtr, bases<Primitive>, boost::noncopyable>( "LineicModel", no_init )
    .add_property( "firstKnot", &LineicModel::getFirstKnot )
    .add_property( "lastKnot", &LineicModel::getLastKnot )
    .def( "getStride", &LineicModel::getStride )
    .def( "getPointAt", &getCurveValue<LineicModel,Vector3,&LineicModel::getPointAt>, args("u") )
    .def( "getTangentAt", &getCurveValue<LineicModel,Vector3,&LineicModel::getTangentAt>, args("u") )
    .def( "getNormalAt", &getCurveValue<LineicModel,Vector3,&LineicModel::getNormalAt>, args("u") )
    .def( "findClosest", &lm_findclosest, args("point") )
    .def( "getLength", (real_t (LineicModel::*)())&LineicModel::getLength )
    .def( "getLength", (real_t (LineicModel::*)(real_t))&LineicModel::getLength, args("begin") )
    .def( "getLength", (real_t (LineicModel::*)(real_t,real_t))&LineicModel::getLength, args("begin","end"), "getLength([begin,end]) : Return length of the curve from u = begin to u = end." )
    ;

  implicitly_convertible<LineicModelPtr, PrimitivePtr>();

  def("closestPointToSegment",&seg_findclosest, args("point","segA","segB"));
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
    .def( "getPointAt", &getCurveValue<Curve2D,Vector2,&Curve2D::getPointAt>, args("u") )
    .def( "getLength", (real_t (Curve2D::*)())&Curve2D::getLength )
    .def( "getLength", (real_t (Curve2D::*)(real_t))&Curve2D::getLength, args("begin") )
    .def( "getLength", (real_t (Curve2D::*)(real_t,real_t))&Curve2D::getLength, args("begin","end"), "getLength([begin,end]) : Return length of the curve from u = begin to u = end." )
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
