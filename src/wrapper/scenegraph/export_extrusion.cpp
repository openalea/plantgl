#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/profile.h>
#include <plantgl/scenegraph/geometry/curve.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

#include <boost/python.hpp>
#include <string>
#include <sstream>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

DEF_POINTEE( Extrusion )

SETGET(Extrusion,Axis,LineicModelPtr);
SETGET(Extrusion,CrossSection,Curve2DPtr);

SETGET(Extrusion,Solid,bool);
SETGET(Extrusion,CCW,bool);

#define SETGET2(_CLASS,PROP,TYPE)\
TYPE get_##_CLASS##_##PROP(_CLASS * obj){ \
  return obj->getProfileTransformation()->get##PROP(); \
} \
void set_##_CLASS##_##PROP(_CLASS * obj, TYPE v){ \
  obj->getProfileTransformation()->get##PROP() = v; \
} \

SETGET2(Extrusion,Scale,Point2ArrayPtr);
SETGET2(Extrusion,Orientation,RealArrayPtr);
SETGET2(Extrusion,KnotList,RealArrayPtr);


void export_Extrusion()
{
  class_<Extrusion, ExtrusionPtr, bases<ParametricModel>, boost::noncopyable>
	( "Extrusion", init<LineicModelPtr,Curve2DPtr,optional<Point2ArrayPtr> >
	 ( "Extrusion (LineicModel, Curve2D,[Point2Array([])])"))
    .def( "copy", &Extrusion::copy )
	.DEC_SETGET(axis,Extrusion,Axis,LineicModelPtr)
	.DEC_SETGET(crossSection,Extrusion,CrossSection,Curve2DPtr)
	.DEC_SETGET(scale,Extrusion,Scale,Point2ArrayPtr)
	.DEC_SETGET(orientation,Extrusion,Orientation,RealArrayPtr)
	.DEC_SETGET_WD(solid,Extrusion,Solid,bool)
	.DEC_SETGET_WD(ccw, Extrusion,CCW,bool)
    ;
  implicitly_convertible<ExtrusionPtr, ParametricModelPtr>();
}


