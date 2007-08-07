
#include <boost/python.hpp>
#include <plantgl/scenegraph/geometry/mesh.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include "../util/export_property.h"
#include "../util/export_refcountptr.h"

using namespace boost::python;

PGL_USING_NAMESPACE

DEF_POINTEE( ExplicitModel )
DEF_POINTEE( Mesh )

void export_ExplicitModel()
{
  class_<ExplicitModel, ExplicitModelPtr, bases<Primitive>, boost::noncopyable>( "ExplicitModel", no_init )
    .def( "transform", &ExplicitModel::transform )
	.DEC_PTR_PROPERTY(pointList,ExplicitModel,PointList,Point3ArrayPtr)
	.DEC_PTR_PROPERTY_WD(colorList,ExplicitModel,ColorList,Color4ArrayPtr)
    ;
  implicitly_convertible<ExplicitModelPtr, PrimitivePtr>();

}

void export_Mesh()
{
  class_<Mesh, MeshPtr, bases<ExplicitModel>, boost::noncopyable>( "Mesh", no_init )
	  .def("indexListSize",&Mesh::getIndexListSize)
	  .DEC_BT_PROPERTY_WD(solid,            Mesh, Solid,           bool)
	  .DEC_BT_PROPERTY_WD(ccw,              Mesh, CCW,             bool)
	  .DEC_BT_PROPERTY_WD(normalPerVertex,  Mesh, NormalPerVertex, bool)
	  .DEC_BT_PROPERTY_WD(colorPerVertex,   Mesh, ColorPerVertex,  bool)
	  .DEC_PTR_PROPERTY_WD(skeleton,        Mesh, Skeleton,        PolylinePtr)
	  .DEC_PTR_PROPERTY_WD(normalList,      Mesh, NormalList,      Point3ArrayPtr)
	  .DEC_PTR_PROPERTY_WD(texCoordList,    Mesh, TexCoordList,    Point2ArrayPtr)
	  .def("computeNormalList",  (void (Mesh::*)())&Mesh::computeNormalList)

	  ;
  implicitly_convertible<MeshPtr, ExplicitModelPtr>();

}
