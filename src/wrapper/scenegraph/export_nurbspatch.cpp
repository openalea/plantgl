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





#include <plantgl/scenegraph/geometry/nurbspatch.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/tool/util_array.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(NurbsPatch)

std::string np_repr( NurbsPatch* p )
{
  std::stringstream ss;
  ss << "NurbsPatch(";
  ss << extract<std::string>(str(object(p->getCtrlPoints())))();
  if (!p->isUDegreeToDefault())
      ss << ", udegree = " << p->getUDegree();
  if (!p->isVDegreeToDefault())
      ss << ", vdegree = " << p->getVDegree();
  if (!p->isUKnotListToDefault())
      ss << ", uknotList = " << extract<std::string>(str(object(p->getUKnotList())))();
  if (!p->isVKnotListToDefault())
      ss << ", vknotList = " << extract<std::string>(str(object(p->getVKnotList())))();
  if (!p->isUStrideToDefault())
      ss << ", ustride = " << p->getUStride();
  if (!p->isVStrideToDefault())
      ss << ", vstride = " << p->getVStride();
  if (!p->isCCWToDefault())
      ss << ", ccw = " << (p->getCCW()?"True":"False");
  ss << ")";
  return ss.str();
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
     init<const Point4MatrixPtr&, boost::python::optional< RealArrayPtr, RealArrayPtr, uint_t,uint_t,uint_t,uint_t,bool> >
     ("NurbsPatch(Point4Matrix ctrlPointMatrix, RealArray uKnotList,RealArray vKnotList [,uDeg, vDeg,ustride,vstride,ccw])"))
     .def(init<const Point4MatrixPtr&, uint_t, boost::python::optional< uint_t,RealArrayPtr, RealArrayPtr, uint_t,uint_t,bool> >
         ("NurbsPatch(Point4Matrix ctrlPointMatrix, udegree, vdegree,"
          "uknotList, vknotList [,ustride,vstride,ccw])",
          (bp::arg("ctrlPointMatrix"),
           bp::arg("udegree") = NurbsPatch::DEFAULT_NURBS_DEGREE,
           bp::arg("vdegree") = NurbsPatch::DEFAULT_NURBS_DEGREE,
           bp::arg("uknotList") = TOOLS(RealArrayPtr()),
           bp::arg("vknotList") = TOOLS(RealArrayPtr()),
           bp::arg("ustride") = BezierPatch::DEFAULT_STRIDE,
           bp::arg("vstride") = BezierPatch::DEFAULT_STRIDE,
           bp::arg("ccw") = Patch::DEFAULT_CCW)))
    .DEF_PGLBASE(NurbsPatch)
     .def( "__repr__", np_repr )
    .DEC_BT_NR_PROPERTY_WD(udegree,NurbsPatch,UDegree,uint_t)
    .DEC_BT_NR_PROPERTY_WD(vdegree,NurbsPatch,VDegree,uint_t)
    .DEC_PTR_PROPERTY_WD(uknotList,NurbsPatch,UKnotList,RealArrayPtr)
    .DEC_PTR_PROPERTY_WD(vknotList,NurbsPatch,VKnotList,RealArrayPtr)
    .add_property( "firstUKnot", &NurbsPatch::getFirstUKnot )
    .add_property( "lastUKnot", &NurbsPatch::getLastUKnot )
    .add_property( "firstVKnot", &NurbsPatch::getFirstVKnot )
    .add_property( "lastVKnot", &NurbsPatch::getLastVKnot )
    .def("setVKnotListToDefault",&NurbsPatch::setVKnotListToDefault)
    .def("setUKnotListToDefault",&NurbsPatch::setUKnotListToDefault)
    .def("getUTangentAt",&NurbsPatch::getUTangentAt,bp::args("u","v"))
    .def("getVTangentAt",&NurbsPatch::getVTangentAt,bp::args("u","v"))
    .def("getNormalAt",&NurbsPatch::getNormalAt,bp::args("u","v"))
    .def("deriveAt",&NurbsPatch::deriveAt,bp::args("u","v","d","uspan","vspan"))
    .def("getDerivativeAt",&NurbsPatch::getDerivativeAt,bp::args("u","v","du","dv"),"Return the derivative at u and v. du and dv specify how many time you want to derive with respect to u and v.")
    .def("getDerivativesAt",&NurbsPatch::getDerivativesAt,bp::args("u","v"))
    ;

  implicitly_convertible< NurbsPatchPtr,BezierPatchPtr >();

}


