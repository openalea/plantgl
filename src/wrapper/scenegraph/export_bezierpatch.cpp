/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
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
     init<const Point4MatrixPtr&, boost::python::optional<uint_t,uint_t,bool> >
     ("BezierPatch(Point4Matrix ctrlPointMatrix [,ustride,vstride,ccw])",
     (bp::arg("ctrlPointMatrix"),
      bp::arg("ustride")=BezierPatch::DEFAULT_STRIDE,
      bp::arg("vstride")=BezierPatch::DEFAULT_STRIDE,
      bp::arg("ccw")=Patch::DEFAULT_CCW)))
    .DEF_PGLBASE(BezierPatch)
    .DEC_BT_PROPERTY_WD(ustride,BezierPatch,UStride,uint_t)
    .DEC_BT_PROPERTY_WD(vstride,BezierPatch,VStride,uint_t)
    .add_property("udegree",&BezierPatch::getUDegree)
    .add_property("vdegree",&BezierPatch::getVDegree)
    .add_static_property("DEFAULT_STRIDE",make_getter(&BezierPatch::DEFAULT_STRIDE))
    .DEC_PTR_PROPERTY(ctrlPointMatrix,BezierPatch,CtrlPointMatrix,Point4MatrixPtr)
    .def("getPointAt",&BezierPatch::getPointAt)
    .def("getIsoUSectionAt",&BezierPatch::getIsoUSectionAt,args("u"),"Compute a section line of the patch corresponding to a constant u value.")
    .def("getIsoVSectionAt",&BezierPatch::getIsoVSectionAt,args("v"),"Compute a section line of the patch corresponding to a constant v value.")
    ;

  implicitly_convertible< BezierPatchPtr,PatchPtr >();
}

