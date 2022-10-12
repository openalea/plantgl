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



#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/profile.h>
#include <plantgl/scenegraph/geometry/curve.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE

using namespace boost::python;
#define bp boost::python

DEF_POINTEE( Extrusion )

void export_Extrusion()
{
  class_<Extrusion, ExtrusionPtr, bases<ParametricModel>, boost::noncopyable>
    ( "Extrusion", "An Extrusion represented by an axis, a cross section and a tranformation function.",
      init<LineicModelPtr,Curve2DPtr,optional<Point2ArrayPtr, RealArrayPtr, RealArrayPtr,bool,bool,Vector3> >
     ( "Extrusion (LineicModel axis, Curve2D crossSection,[Point2Array scale, RealArray orientation, RealArray knotList, bool solid, bool ccw, Vector3 initialVector])",
     (bp::arg("axis"),bp::arg("crossSection"),
      bp::arg("scaleList")=Extrusion::DEFAULT_SCALE_LIST,
      bp::arg("orientationList")=Extrusion::DEFAULT_ORIENTATION_LIST,
      bp::arg("knotList")=RealArrayPtr(),
      bp::arg("solid")=Extrusion::DEFAULT_SOLID,
      bp::arg("ccw")=Extrusion::DEFAULT_CCW,
      bp::arg("initialNormal")= Extrusion::DEFAULT_INITIAL_NORMAL)))
    .DEF_PGLBASE(Extrusion)
    .DEC_PTR_PROPERTY(axis,Extrusion,Axis,LineicModelPtr)
    .DEC_PTR_PROPERTY(crossSection,Extrusion,CrossSection,Curve2DPtr)
    .DEC_BT_NR_PROPERTY_WDV(solid,Extrusion,Solid,bool,DEFAULT_SOLID)
    .DEC_BT_NR_PROPERTY_WDV(ccw, Extrusion,CCW,bool,DEFAULT_CCW)
    .DEC_BT_PROPERTY_WDV(initialNormal, Extrusion,InitialNormal,Vector3,DEFAULT_INITIAL_NORMAL)

    .DEC_PTR_PROPERTY_WDV(scaleList,Extrusion,Scale,Point2ArrayPtr,DEFAULT_SCALE_LIST)
    .DEC_PTR_PROPERTY_WDV(orientationList,Extrusion,Orientation,RealArrayPtr,DEFAULT_ORIENTATION_LIST)
    .DEC_PTR_NR_PROPERTY(knotList,Extrusion,KnotList,RealArrayPtr)
    .def("getInitialNormalValue",&Extrusion::getInitialNormalValue)
    .def("getFrameAt",&Extrusion::getFrameAt)
    .def("getNextFrameAt",&Extrusion::getNextFrameAt)
    .def("getPointAt",(Vector3 (Extrusion::*)(real_t,real_t) const)&Extrusion::getPointAt)
    .def("getPointAt",(Vector3 (Extrusion::*)(real_t,real_t,const Matrix3&) const)&Extrusion::getPointAt)
    .def("getUTangentAt",(Vector3 (Extrusion::*)(real_t,real_t) const)&Extrusion::getUTangentAt)
    .def("getUTangentAt",(Vector3 (Extrusion::*)(real_t,real_t,const Matrix3&) const)&Extrusion::getUTangentAt)
    .def("getVTangentAt",(Vector3 (Extrusion::*)(real_t,real_t) const)&Extrusion::getVTangentAt)
    .def("getVTangentAt",(Vector3 (Extrusion::*)(real_t,real_t,const Matrix3&) const)&Extrusion::getVTangentAt)
    .def("getNormalAt",(Vector3 (Extrusion::*)(real_t,real_t) const)&Extrusion::getNormalAt)
    .def("getNormalAt",(Vector3 (Extrusion::*)(real_t,real_t,const Matrix3&) const)&Extrusion::getNormalAt)
    .def("getSecondDerivativeUUAt",(Vector3 (Extrusion::*)(real_t,real_t) const)&Extrusion::getSecondDerivativeUUAt)
    .def("getSecondDerivativeUUAt",(Vector3 (Extrusion::*)(real_t,real_t,const Matrix3&) const)&Extrusion::getSecondDerivativeUUAt)
    .def("getSecondDerivativeVVAt",(Vector3 (Extrusion::*)(real_t,real_t) const)&Extrusion::getSecondDerivativeVVAt)
    .def("getSecondDerivativeVVAt",(Vector3 (Extrusion::*)(real_t,real_t,const Matrix3&) const)&Extrusion::getSecondDerivativeVVAt)
    .def("getSecondDerivativeUVAt",(Vector3 (Extrusion::*)(real_t,real_t) const)&Extrusion::getSecondDerivativeUVAt)
    .def("getSecondDerivativeUVAt",(Vector3 (Extrusion::*)(real_t,real_t,const Matrix3&) const)&Extrusion::getSecondDerivativeUVAt)
    ;
  implicitly_convertible<ExtrusionPtr, ParametricModelPtr>();
}


