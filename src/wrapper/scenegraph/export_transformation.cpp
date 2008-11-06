/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */

#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>
#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/transformation/axisrotated.h>
#include <plantgl/scenegraph/transformation/eulerrotated.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>

#include <plantgl/python/export_refcountptr.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;


DEF_POINTEE(Transformation)
DEF_POINTEE(Transformation2D)
DEF_POINTEE(Transformation3D)
DEF_POINTEE(Matrix3Transformation)
DEF_POINTEE(Matrix4Transformation)
DEF_POINTEE(OrthonormalBasis3D)
DEF_POINTEE(Deformation)

DEF_POINTEE(Taper)
DEF_POINTEE(Scaling)
DEF_POINTEE(Translation)
DEF_POINTEE(AxisRotation)
DEF_POINTEE(EulerRotation)
DEF_POINTEE(BaseOrientation)
DEF_POINTEE(Transform4)

/*
object tr2D(Transformation2D * t2, object arg){
	extract<Point2ArrayPtr> ex_p2(arg);
	if(ex_p2.check())return object(t2->transform(ex_p2()));
	else{
		extract<Point3ArrayPtr> ex_p3(arg);
		if(ex_p3.check())return object(t2->transform(ex_p3()));
		else {
			extract<Point2MatrixPtr> ex_p2m(arg);
			if(ex_p2m.check())return object(t2->transform(ex_p2m()));
			else{
				extract<Point3MatrixPtr> ex_p3m(arg);
				if(ex_p3m.check())return object(t2->transform(ex_p3m()));
				else{
					Vector2 a = extract<Vector2>(arg)();
					Point2ArrayPtr pa(new Point2Array(1,a));
					return object(t2->transform(pa)->getAt(0));
				}
			}
		}
	}
}

object tr3D(Transformation3D * t3, object arg){
	extract<Point3ArrayPtr> ex_p3(arg);
	if(ex_p3.check())return object(t3->transform(ex_p3()));
	else{
		extract<Point4ArrayPtr> ex_p4(arg);
		if(ex_p4.check())return object(t3->transform(ex_p4()));
		else {
			extract<Point3MatrixPtr> ex_p3m(arg);
			if(ex_p3m.check())return object(t3->transform(ex_p3m()));
			else{
				extract<Point4MatrixPtr> ex_p4m(arg);
				if(ex_p4m.check())return object(t3->transform(ex_p4m()));
				else{
					Vector3 a = extract<Vector3>(arg)();
					Point3ArrayPtr pa(new Point3Array(1,a));
					return object(t3->transform(pa)->getAt(0));
				}
			}
		}
	}
}*/

void export_Transformation()
{
  class_< Transformation, TransformationPtr, boost::noncopyable >("Transformation", no_init);

  class_< Transformation2D, Transformation2DPtr, bases< Transformation >, boost::noncopyable  >
    ("Transformation2D", no_init)
	.def("transform",(Point2ArrayPtr (Transformation2D::*)( const Point2ArrayPtr&) const)&Transformation2D::transform)
	// Cannot add transform on other type. boost.python will try to cast the object into a Point2Array anyway.
	.def("transformP3List",(Point3ArrayPtr (Transformation2D::*)( const Point3ArrayPtr&) const)&Transformation2D::transform)
	.def("transformP2Matrix",(Point2MatrixPtr (Transformation2D::*)( const Point2MatrixPtr&) const)&Transformation2D::transform)
	.def("transformP3Matrix",(Point3MatrixPtr (Transformation2D::*)( const Point3MatrixPtr&) const)&Transformation2D::transform)
	// .def("transform",&tr2D )
	;

  class_< Transformation3D, Transformation3DPtr, bases< Transformation >, boost::noncopyable  >
    ("Transformation3D", no_init)
	.def("transform",(Point3ArrayPtr (Transformation3D::*)( const Point3ArrayPtr&) const)&Transformation3D::transform)
	// Cannot add transform on other type. boost.python will try to cast the object into a Point2Array anyway.
	// .def("transform",&tr3D )
	.def("transformP4List",(Point4ArrayPtr (Transformation3D::*)( const Point4ArrayPtr&) const)&Transformation3D::transform)
	.def("transformP3Matrix",(Point3MatrixPtr (Transformation3D::*)( const Point3MatrixPtr&) const)&Transformation3D::transform)
	.def("transformP4Matrix",(Point4MatrixPtr (Transformation3D::*)( const Point4MatrixPtr&) const)&Transformation3D::transform)
	;

  class_< Matrix3Transformation, Matrix3TransformationPtr, bases< Transformation2D >, boost::noncopyable  >
    ("Matrix3Transformation", no_init);
  class_< Matrix4Transformation, Matrix4TransformationPtr, bases< Transformation3D >, boost::noncopyable  >
    ("Matrix4Transformation", no_init)
	.def("getMatrix",&Matrix4Transformation::getMatrix);
    ;
  class_< Deformation, DeformationPtr, bases< Transformation3D >, boost::noncopyable  >
    ("Deformation", no_init);

  implicitly_convertible<Transformation2DPtr, TransformationPtr >();
  implicitly_convertible<Transformation3DPtr, TransformationPtr >();
  implicitly_convertible<Matrix3TransformationPtr, Transformation2DPtr >();
  implicitly_convertible<Matrix4TransformationPtr, Transformation3DPtr >();
  implicitly_convertible<DeformationPtr, Transformation3DPtr >();

}

void export_Taper()
{
  class_< Taper, bases< Deformation > , TaperPtr, boost::noncopyable >
    ("Taper", init< const real_t&, const real_t&>() )
	.def("getBaseRadius",&Taper::getBaseRadius, return_value_policy<copy_const_reference>())
	.def("getTopRadius",&Taper::getTopRadius);

  implicitly_convertible<TaperPtr, DeformationPtr>();
}


void export_Scaling()
{
  class_< Scaling, bases< Matrix4Transformation > , ScalingPtr, boost::noncopyable >
    ("Scaling", init< const Vector3& >() );

  implicitly_convertible<ScalingPtr, Matrix4TransformationPtr>();
}

void export_Translation()
{
  class_< Translation, bases< Matrix4Transformation > , TranslationPtr, boost::noncopyable >
    ("Translation", init< const Vector3& >() );

  implicitly_convertible<TranslationPtr, Matrix4TransformationPtr>();
}


void export_OrthonormalBasis3D()
{
  class_< OrthonormalBasis3D, bases< Matrix4Transformation > , OrthonormalBasis3DPtr, boost::noncopyable >
    ("OrthonormalBasis3D", init< const Matrix3& >() )
	.def("getMatrix3",&OrthonormalBasis3D::getMatrix3)
	.def("getMatrix",&OrthonormalBasis3D::getMatrix);

  implicitly_convertible<OrthonormalBasis3DPtr, Matrix4TransformationPtr>();
}

void export_AxisRotation()
{
  class_< AxisRotation, bases< OrthonormalBasis3D > , AxisRotationPtr, boost::noncopyable >
    ("AxisRotation", init< const Vector3&, const real_t&>() );

  implicitly_convertible<AxisRotationPtr, OrthonormalBasis3DPtr>();
}

void export_EulerRotation()
{
  class_< EulerRotation, bases< OrthonormalBasis3D > , EulerRotationPtr, boost::noncopyable >
    ("EulerRotation", init< const real_t&, const real_t&, const real_t&>() );

  implicitly_convertible<EulerRotationPtr, OrthonormalBasis3DPtr>();
}

void export_BaseOrientation()
{
  class_< BaseOrientation, bases< OrthonormalBasis3D > , BaseOrientationPtr, boost::noncopyable >
    ("BaseOrientation", init< const Vector3&, const Vector3&>() );

  implicitly_convertible<EulerRotationPtr, OrthonormalBasis3DPtr>();
}


void translate( Transform4& t, const TOOLS(Vector3)& v ) { t.translate(v); }
void scale( Transform4& t, const TOOLS(Vector3)& v ) { t.scale(v); }
void rotate_t4( Transform4& t, const TOOLS(Matrix3)& v ) { t.rotate(v); }

Transform4Ptr t4_default()
{ return Transform4Ptr(new Transform4());}

std::string t4_str( Transform4* t )
{
  std::stringstream ss;
  ss << t->getMatrix();
  return ss.str();
}

std::string t4_repr( Transform4 * a )
{
  std::stringstream ss;
  Matrix4 m=a->getMatrix();
  real_t* data= m.getData();
  ss << "Transform4(Matrix4( ";
  std::copy( data, data+15, std::ostream_iterator< real_t >( ss, ", " ) );
  ss << *(data+15) << "])";
  return ss.str();
}

Point3ArrayPtr t4_transform_p3( Transform4 * t, const Point3ArrayPtr& points )
{
  return t->transform(points);
}

tuple t4_getTransformation(Transform4 * t)
{
	Vector3 scale,rotate,translate;
	t->getTransformation(scale,rotate,translate);
	return make_tuple(scale,rotate,translate);
}

void export_Transform4()
{
  class_< Transform4, Transform4Ptr, bases< Matrix4Transformation > , boost::noncopyable > 
    m2("Transform4", init<const Matrix4&>() );
  m2.def("__init__", make_constructor(t4_default));
  m2.def("isValid", &Transform4::isValid);
  m2.def("translate", translate,"Add a translation at the transformation");
  m2.def("scale", scale,"Add a scaling at the transformation");
  m2.def("rotate", rotate_t4,"Add a rotation at the transformation");
  m2.def("getTransformation", &t4_getTransformation,"Return scale, rotate and translate corresponding values.");
  m2.def("getVolume", &Transform4::getVolume,"compute the Matrix3 determinant of the transformation");
  m2.def("transform", t4_transform_p3, "transform a set of points");
  m2.def("__str__",t4_str);
  m2.def("__repr__",t4_repr);

  implicitly_convertible<Transform4Ptr, Matrix4TransformationPtr>();

}

