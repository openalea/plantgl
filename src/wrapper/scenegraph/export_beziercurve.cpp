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

#include <plantgl/scenegraph/geometry/beziercurve.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/mesh.h>
#include <plantgl/math/util_polymath.h>

#include <boost/python.hpp>
#include <sstream>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "../util/export_list.h"

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE( BezierCurve )
DEF_POINTEE( BezierCurve2D )

std::string gbc_repr( BezierCurve* p )
{
  std::stringstream ss;
  Point4ArrayPtr ctrl= p->getCtrlPoints();
  uint32_t stride= p->getStride();
  uint32_t n= ctrl->getSize();
  if( n == 0 )
    {
      ss << "BezierCurve(Point4Array([])," << stride << ")";
      return ss.str();
    }

  Vector4 v = ctrl->getAt( 0 );
  ss << "BezierCurve(Point4Array([Vector4(" << v.x() << ", " << v.y()
     << ", " << v.z() << ", " << v.w() << ")";
  size_t i;
  for( i = 1 ; i < n ; ++i )
    {
      v = ctrl->getAt( i );
      ss << ", Vector4(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")";
    }
  ss << "])," << stride <<")";
  return ss.str();
}

object bernstein_factors(uint32_t n, real_t u){
    return make_list<std::vector<real_t> >(all_bernstein(n,u))();
}

void export_BezierCurve()
{
  class_<BezierCurve, BezierCurvePtr, bases<ParametricModel, LineicModel>, boost::noncopyable>
    ( "BezierCurve", init<Point4ArrayPtr, optional< uint32_t > >() )
    .def( "copy", &BezierCurve::copy )
    .def( "__repr__", gbc_repr )
    .DEC_BT_NR_PROPERTY_WDV(stride,BezierCurve,Stride,uint32_t,DEFAULT_STRIDE)
    .DEC_PTR_PROPERTY(ctrlPointList,BezierCurve,CtrlPointList,Point4ArrayPtr)
    .def("bernstein_factors",&bernstein_factors,args("n","u"))
    .staticmethod("bernstein_factors")
    .def("bernstein",&bernstein,args("i","n","u"))
    .staticmethod("bernstein")

    ;

  implicitly_convertible<BezierCurvePtr, ParametricModelPtr>();
  implicitly_convertible<BezierCurvePtr, LineicModelPtr>();
}

std::string gbc2_repr( BezierCurve2D* p )
{
  std::stringstream ss;
  Point3ArrayPtr ctrl= p->getCtrlPoints();
  uint32_t stride= p->getStride();
  uint32_t n= ctrl->getSize();
  if( n == 0 )
    {
      ss << "BezierCurve2D(Point3Array([])," << stride << ")";
      return ss.str();
    }

  Vector3 v = ctrl->getAt( 0 );
  ss << "BezierCurve2D(Point3Array([Vector3(" << v.x() << ", " << v.y()
     << ", " << v.z() <<  ")";
  size_t i;
  for( i = 1 ; i < n ; ++i )
    {
      v = ctrl->getAt( i );
      ss << ", Vector3(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    }
  ss << "])," << stride <<")";
  return ss.str();
}

void export_BezierCurve2D()
{
   class_<BezierCurve2D, BezierCurve2DPtr, bases<Curve2D>, boost::noncopyable>
    ( "BezierCurve2D", init<Point3ArrayPtr, optional< uint32_t > >() )
    .def( "copy", &BezierCurve2D::copy )
    .def( "__repr__", gbc2_repr )
    .DEC_BT_NR_PROPERTY_WD(stride,BezierCurve2D,Stride,uint32_t)
    .DEC_PTR_PROPERTY(ctrlPointList,BezierCurve2D,CtrlPointList,Point3ArrayPtr)
    ;

  implicitly_convertible<BezierCurve2DPtr, Curve2DPtr>();
}

