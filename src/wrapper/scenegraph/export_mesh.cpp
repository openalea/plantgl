#include "mesh.h"
#include "triangleset.h"
#include "pointset.h"
#include "../util/export_property.h"

#include <boost/python.hpp>
#include <geom_mesh.h>
#include <geom_polyline.h>
#include <geom_transformed.h>
#include <geom_pointarray.h>

#include "../util/export_refcountptr.h"

using namespace boost::python;

PGL_USING_NAMESPACE

DEF_POINTEE( ExplicitModel )
DEF_POINTEE( Mesh )

SETGET(ExplicitModel,PointList,Point3ArrayPtr)
SETGET(ExplicitModel,ColorList,Color4ArrayPtr)

void class_ExplicitModel()
{
  class_<ExplicitModel, ExplicitModelPtr, bases<Primitive>, boost::noncopyable>( "ExplicitModel", no_init )
    .def( "transform", &ExplicitModel::transform )
	.DEC_SETGET(pointList,ExplicitModel,PointList,Point3Array)
	.DEC_SETGET_WD(colorList,ExplicitModel,ColorList,Color4Array)
    ;
  implicitly_convertible<ExplicitModelPtr, PrimitivePtr>();

  class_Mesh();
  class_PointSet();
}

SETGET(Mesh,Solid,           bool)
SETGET(Mesh,CCW,             bool)
SETGET(Mesh,NormalPerVertex, bool)
SETGET(Mesh,ColorPerVertex,  bool)
SETGET(Mesh,Skeleton,        GeomPolylinePtr)
SETGET(Mesh,NormalList,      Point3ArrayPtr)
SETGET(Mesh,TexCoordList,    Point2ArrayPtr)

void class_Mesh()
{
  class_<Mesh, MeshPtr, bases<ExplicitModel>, boost::noncopyable>( "Mesh", no_init )
	  .def("indexListSize",&Mesh::getIndexListSize)
	  .DEC_SETGET_WD(solid,           Mesh, Solid,           bool)
	  .DEC_SETGET_WD(ccw,             Mesh, CCW,             bool)
	  .DEC_SETGET_WD(normalPerVertex, Mesh, NormalPerVertex, bool)
	  .DEC_SETGET_WD(colorPerVertex,  Mesh, ColorPerVertex,  bool)
	  .DEC_SETGET_WD(skeleton,        Mesh, Skeleton,        GeomPolyline)
	  .DEC_SETGET_WD(normalList,      Mesh, NormalList,      Point3Array)
	  .DEC_SETGET_WD(texCoordList,    Mesh, TexCoordList,    Point2Array)
	  .def("computeNormalList",  (void (Mesh::*)())&Mesh::computeNormalList)

	  ;
  implicitly_convertible<MeshPtr, ExplicitModelPtr>();

  class_TriangleSet();
  class_QuadSet();
  class_FaceSet();
  class_AmapSymbol();
}
