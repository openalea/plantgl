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


#include <plantgl/scenegraph/geometry/bezierpatch.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/geometry/lineicmodel.h>


#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(BezierPatch)


void export_BezierPatch()
{
  class_< BezierPatch, BezierPatchPtr, bases< Patch >,boost::noncopyable >
    ("BezierPatch", 
    "BezierPatch describes rational and non rational Bezier surface.\n"
	"It is defined by two degrees n and m and a matrix of control Points Pi,j\n"
	"and using the parametric equation S(u,v) = Sum(i=0,n)Sum(j=0,m)(Bi,n(u)Bj,m(v)Pi,j) with u and v in [0,1]\n"
	"where Bi,n(u) and Bi,m(v) are the classical n and m-th degree Bernstein polynomials.",
	 init<const Point4MatrixPtr&, optional<uint_t,uint_t,bool> >
     ("BezierPatch(Point4Matrix ctrlPointList [,ustride,vstride,ccw])",
	 (bp::arg("ctrlPointList"),
	  bp::arg("ustride")=BezierPatch::DEFAULT_STRIDE,
	  bp::arg("vstride")=BezierPatch::DEFAULT_STRIDE,
	  bp::arg("ccw")=Patch::DEFAULT_CCW)))
	.DEF_PGLBASE(BezierPatch)
    .DEC_BT_PROPERTY_WD(ustride,BezierPatch,UStride,uint_t)
    .DEC_BT_PROPERTY_WD(vstride,BezierPatch,VStride,uint_t)
    .add_property("udegree",&BezierPatch::getUDegree)
    .add_property("vdegree",&BezierPatch::getVDegree)
#ifdef STATIC_PROPERTY
    .add_static_property("DEFAULT_STRIDE",make_getter(&BezierPatch::DEFAULT_STRIDE))
#else
    .add_property("DEFAULT_STRIDE",make_getter(&BezierPatch::DEFAULT_STRIDE))
#endif
    .DEC_PTR_PROPERTY(ctrlPointMatrix,BezierPatch,CtrlPointMatrix,Point4MatrixPtr)
    .def("getPointAt",&BezierPatch::getPointAt)
    .def("getUSection",&BezierPatch::getUSection,args("u"),"Compute a section line of the patch corresponding to a constant u value.")
    .def("getVSection",&BezierPatch::getVSection,args("v"),"Compute a section line of the patch corresponding to a constant v value.")
    ;

  implicitly_convertible< BezierPatchPtr,PatchPtr >();
}

