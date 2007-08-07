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

void export_Extrusion()
{
  class_<Extrusion, ExtrusionPtr, bases<ParametricModel>, boost::noncopyable>
	( "Extrusion", init<LineicModelPtr,Curve2DPtr,optional<Point2ArrayPtr> >
	 ( "Extrusion (LineicModel, Curve2D,[Point2Array([])])"))
    .def( "copy", &Extrusion::copy )
	.DEC_PTR_PROPERTY(axis,Extrusion,Axis,LineicModelPtr)
	.DEC_PTR_PROPERTY(crossSection,Extrusion,CrossSection,Curve2DPtr)
	.DEC_BT_NR_PROPERTY_WD(solid,Extrusion,Solid,bool)
	.DEC_BT_NR_PROPERTY_WD(ccw, Extrusion,CCW,bool)

	.DEC_PTR_PROPERTY(scale,Extrusion,Scale,Point2ArrayPtr)
	.DEC_PTR_PROPERTY(orientation,Extrusion,Orientation,RealArrayPtr)
	.DEC_PTR_NR_PROPERTY(knotList,Extrusion,KnotList,RealArrayPtr)
    ;
  implicitly_convertible<ExtrusionPtr, ParametricModelPtr>();
}


