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

#include <boost/python.hpp>

#include <plantgl/tool/util_array.h>
#include <plantgl/tool/util_array2.h>

#include <plantgl/scenegraph/geometry/parametricmodel.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/geometry/patch.h>
#include <plantgl/scenegraph/geometry/bezierpatch.h>
#include <plantgl/scenegraph/geometry/nurbspatch.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Patch)
DEF_POINTEE(BezierPatch)
DEF_POINTEE(NurbsPatch)

void export_Patch()
{
  class_< Patch, PatchPtr, bases< ParametricModel >,boost::noncopyable >
    ("Patch","Abstract base class for patches.",no_init)
    .DEC_BT_NR_PROPERTY_WD(ccw,Patch,CCW,bool)
    ;

  implicitly_convertible< PatchPtr,ParametricModelPtr >();
}

void export_BezierPatch()
{
  class_< BezierPatch, BezierPatchPtr, bases< Patch >,boost::noncopyable >
    ("BezierPatch", 
    "BezierPatch describes rational and non rational Bezier surface.\n"
	"It is defined by two degrees n and m and a matrix of control Points Pi,j\n"
	"and using the parametric equation S(u,v) = Sum(i=0,n)Sum(j=0,m)(Bi,n(u)Bj,m(v)Pi,j) with u and v in [0,1]\n"
	"where Bi,n(u) and Bi,m(v) are the classical n and m-th degree Bernstein polynomials.",
	 init<Point4MatrixPtr, optional<uint_t,uint_t,bool> >
     ("BezierPatch(Point4Matrix ctrlPoints [,ustride,vstride,ccw])"))
    .DEC_BT_PROPERTY_WD(ustride,BezierPatch,UStride,uint_t)
    .DEC_BT_PROPERTY_WD(vstride,BezierPatch,VStride,uint_t)
    .add_static_property("DEFAULT_STRIDE",make_getter(&BezierPatch::DEFAULT_STRIDE))
    .DEC_PTR_PROPERTY(ctrlPointMatrix,BezierPatch,CtrlPointMatrix,Point4MatrixPtr)
    .def("getPointAt",&BezierPatch::getPointAt)
    ;

  implicitly_convertible< BezierPatchPtr,PatchPtr >();
}

void export_NurbsPatch()
{
  class_< NurbsPatch, NurbsPatchPtr, bases< BezierPatch >,boost::noncopyable >
    ("NurbsPatch", 
	 // "A NURBS Patch represented by 2 degrees, 2 knot vectors and a matrix of control Points.",
    "NurbsPatch describes rational and non rational Bezier surface.\n"
	"It is defined by two degrees n and m and a matrix of control Points Pi,j\n"
	"and using the parametric equation S(u,v) = Sum(i=0,n)Sum(j=0,m)(Ri,n(u)Rj,m(v)Pi,j) with u and v in [0,1]\n"
	"where Ri,n(u) and Ri,m(v) are classical n and m-th degree rational basis function.",
	 init<Point4MatrixPtr, optional< RealArrayPtr, RealArrayPtr, 
     uint_t,uint_t,uint_t,uint_t,bool> >
     ("NurbsPatch(Point4Matrix ctrlPoints, RealArray uKnotList,RealArray vKnotList [,uDeg, vDeg,ustride,vstride,ccw])"))
    .DEC_BT_NR_PROPERTY_WD(udegree,NurbsPatch,UDegree,uint_t)
    .DEC_BT_NR_PROPERTY_WD(vdegree,NurbsPatch,VDegree,uint_t)
    .DEC_PTR_PROPERTY_WD(uknotList,NurbsPatch,UKnotList,RealArrayPtr)
    .DEC_PTR_PROPERTY_WD(vknotList,NurbsPatch,VKnotList,RealArrayPtr)
    .def("setVKnotListToDefault",&NurbsPatch::setVKnotListToDefault)
    .def("setUKnotListToDefault",&NurbsPatch::setUKnotListToDefault)
    ;

  implicitly_convertible< NurbsPatchPtr,BezierPatchPtr >();

}


