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

#include <plantgl/scenegraph/geometry/nurbscurve.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/scenegraph/geometry/mesh.h>
#include <plantgl/algo/fitting/fit.h>

#include <boost/python.hpp>
#include <string>
#include <sstream>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE( NurbsCurve )
DEF_POINTEE( NurbsCurve2D )



std::string nc_repr( NurbsCurve* p )
{
  std::stringstream ss;
  Point4ArrayPtr ctrl= p->getCtrlPoints();
  RealArrayPtr knot= p->getKnotList();
  uint32_t d= p->getDegree();
  uint32_t stride= p->getStride();
  uint32_t n= ctrl->getSize();
  uint32_t nk= knot->getSize();
  if( n == 0 )
    {
      ss << "NurbsCurve(Point4Array([]),RealArray([])," << stride << ")";
      return ss.str();
    }

  Vector4 v = ctrl->getAt( 0 );
  ss << "NurbsCurve(Point4Array([Vector4(" << v.x() << ", " << v.y()
     << ", " << v.z() << ", " << v.w() << ")";
  size_t i;
  for( i = 1 ; i < n ; ++i )
  {
      v = ctrl->getAt( i );
      ss << ", Vector4(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")";
  }
  ss << "),RealArray([";
  if (knot && !knot->isEmpty()) {
    ss << knot->getAt(0);
	for( i = 1 ; i < nk ; ++i )
	{
      ss << ", " << knot->getAt( i );
	}
  }
  ss << "])," << d << ',' << stride <<")";
  return ss.str();
}


object nurbs_fit1(Point3ArrayPtr pts, int degree, int nbctrlpoints){
	LineicModelPtr res = Fit::nurbsCurve(pts,degree,nbctrlpoints);
	if (!res)return object();
	else {
		return object(res);
	}
}

object nurbs_fit2(Point3ArrayPtr pts){
	Fit p(pts);
	LineicModelPtr res = p.nurbsCurve();
	if (!res)return object();
	else {
		return object(res);
	}
}

object nurbs_fit3(Polyline * pts, int degree, int nbctrlpoints){
	return nurbs_fit1(pts->getPointList(),degree,nbctrlpoints);
}


object nurbs_fit4(Polyline * pts){
	return nurbs_fit2(pts->getPointList());
}

void export_NurbsCurve()
{
  class_<NurbsCurve, NurbsCurvePtr, bases<BezierCurve>, boost::noncopyable>
    ( "NurbsCurve", init<Point4ArrayPtr, optional< RealArrayPtr, uint32_t, uint32_t > >(args("ctrlPointList","knotList","degree","strides")) )
     .DEC_BT_NR_PROPERTY_WD(degree,NurbsCurve,Degree,uint32_t)
     .def("setKnotListToDefault",&NurbsCurve::setKnotListToDefault)
     .def( "__repr__", nc_repr )
     .def( "fit", nurbs_fit1, args("points","degree","nbctrlpoints"), "fit(points [, int degree, int nbctrlpoints])" )
     .def( "fit", nurbs_fit2, args("points") )
     .def( "fit", nurbs_fit3, args("points","degree","nbctrlpoints") )
     .def( "fit", nurbs_fit4, args("points") )
	 .staticmethod("fit")
     .def( "getDerivativeAt", &NurbsCurve::getDerivativeAt, args("u","d") )
     .def( "getDerivativesAt", &NurbsCurve::getDerivativesAt, args("u") )
     .def( "findSpan", &PGL::findSpan, args("u","degree","knotList") )
	 .staticmethod("findSpan")
     .def( "basisFunctions", &basisFunctions, args("span","u","degree","knotList") )
	 .staticmethod("basisFunctions")
     .def( "derivatesBasisFunctions", &derivatesBasisFunctions, args("n","u","span","degree","knotList") )
	 .staticmethod("derivatesBasisFunctions")
    ;

  implicitly_convertible<NurbsCurvePtr, BezierCurvePtr>();
}


std::string nc2_repr( NurbsCurve2D* p )
{
  std::stringstream ss;
  Point3ArrayPtr ctrl= p->getCtrlPoints();
  RealArrayPtr knot= p->getKnotList();
  uint32_t d= p->getDegree();
  uint32_t stride= p->getStride();
  uint32_t n= ctrl->getSize();
  uint32_t nk= knot->getSize();
  if( n == 0 )
    {
      ss << "NurbsCurve2D(Point3Array([]),RealArray([])," << stride << ")";
      return ss.str();
    }

  Vector3 v = ctrl->getAt( 0 );
  ss << "NurbsCurve2D(Point3Array([Vector3(" << v.x() << ", " << v.y()
     << ", " << v.z()  << ")";
  size_t i;
  for( i = 1 ; i < n ; ++i )
  {
      v = ctrl->getAt( i );
      ss << ", Vector3(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
  }
  ss << "),RealArray([";
  if (knot && !knot->isEmpty()) {
    ss << knot->getAt(0);
	for( i = 1 ; i < nk ; ++i )
	{
      ss << ", " << knot->getAt( i );
	}
  }
  ss << "])," << d << ',' << stride <<")";
  return ss.str();
}

object convert_lc(const LineicModelPtr& res)
{
	if (!res)return object();
	else {
		NurbsCurvePtr c;		
		if(c.cast(res)){
			Point3ArrayPtr cpts(new Point3Array(c->getCtrlPointList()->getSize()));
			Point3Array::iterator _it = cpts->getBegin();
			for (Point4Array::const_iterator _it2 = c->getCtrlPointList()->getBegin();
				_it2 != c->getCtrlPointList()->getEnd(); _it2++){
					*_it = Vector3(_it2->x(),_it2->y(),_it2->w());
					_it++;
				}
				NurbsCurve2DPtr c2(new NurbsCurve2D(cpts,c->getKnotList(),c->getDegree(),c->getStride()));
				return object(c2);
		}
		else {
			BezierCurvePtr bc = BezierCurvePtr::Cast(res);
			if (bc){
				Point3ArrayPtr cpts(new Point3Array(bc->getCtrlPointList()->getSize()));
				Point3Array::iterator _it = cpts->getBegin();
				for (Point4Array::const_iterator _it2 = bc->getCtrlPointList()->getBegin();
					_it2 != bc->getCtrlPointList()->getEnd(); _it2++){
						*_it = Vector3(_it2->x(),_it2->y(),_it2->w());
						_it++;
					}
					BezierCurve2DPtr bc2(new BezierCurve2D(cpts,bc->getStride()));
					return object(bc2);
			}
			else {
				PolylinePtr pc;
				if(pc.cast(res)){
					Point2ArrayPtr cpts(new Point2Array(pc->getPointList()->getSize()));
					Point2Array::iterator _it = cpts->getBegin();
					for (Point3Array::const_iterator _it2 = pc->getPointList()->getBegin();
						_it2 != pc->getPointList()->getEnd(); _it2++){
							*_it = Vector2(_it2->x(),_it2->y());
							_it++;
						}
						Polyline2DPtr pc2(new Polyline2D(cpts));
						return object(pc2);

				}
				else return object();
			}
		}
	}
}
object nurbs2_fit1(Point2ArrayPtr pts){
	Fit p(Point3ArrayPtr(new Point3Array(*pts,0)));
	LineicModelPtr res = p.nurbsCurve();
	return convert_lc(res);
}

object nurbs2_fit2(Point2ArrayPtr pts,int degree, int nbCtrlPoint){
	LineicModelPtr res = Fit::nurbsCurve(Point3ArrayPtr(new Point3Array(*pts,1)),degree,nbCtrlPoint);
	return convert_lc(res);
}

object nurbs2_fit3(Polyline2D * pts){
	return nurbs2_fit1(pts->getPointList());
}

object nurbs2_fit4(Polyline2D * pts,int degree, int nbCtrlPoint){
	return nurbs2_fit2(pts->getPointList(),degree,nbCtrlPoint);
}

void export_NurbsCurve2D()
{
   class_<NurbsCurve2D, NurbsCurve2DPtr, bases<BezierCurve2D>, boost::noncopyable>
     ( "NurbsCurve2D", init<Point3ArrayPtr,  optional<RealArrayPtr, uint32_t, uint32_t > >( args("ctrlPointList","knotList","degree","strides") ) )
      .def( "__repr__", nc2_repr )
     .def( "fit", nurbs2_fit1, args("points") )
     .def( "fit", nurbs2_fit2, args("points","degree","nbctrlpoints"), "fit(points [, int degree, int nbctrlpoints])" )
     .def( "fit", nurbs2_fit3, args("points") )
     .def( "fit", nurbs2_fit4, args("points","degree","nbctrlpoints") )
	 .staticmethod("fit")
     .DEC_BT_NR_PROPERTY_WD(degree,NurbsCurve2D,Degree,uint32_t)
     .DEC_PTR_PROPERTY_WD(knotList,NurbsCurve2D,KnotList,RealArrayPtr)
     .def("setKnotListToDefault",&NurbsCurve2D::setKnotListToDefault)
    ;

   implicitly_convertible< NurbsCurve2DPtr, BezierCurve2DPtr >();
}

