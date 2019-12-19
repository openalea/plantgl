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



#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/exception.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/geometry/curve.h>
#include <plantgl/scenegraph/function/function.h>

#include <boost/python.hpp>

using namespace boost::python;

PGL_USING_NAMESPACE

DEF_POINTEE( LineicModel )
DEF_POINTEE( Curve2D )
DEF_POINTEE( Disc )

template<class CurveT, class VectorT>
object lm_findclosest(CurveT * lm, VectorT point)
{
    real_t u;
    VectorT res = lm->findClosest(point,&u);
    return make_tuple(res,u);
}

template<class VectorT>
object seg_findclosest(VectorT point, VectorT segA, VectorT segB)
{
    real_t u;
    real_t dist = closestPointToSegment(point,segA,segB,&u);
    return make_tuple(point,dist,u);
}

template <class T, class U, U (T::* func)(real_t) const >
U getCurveValue(const T * lm, real_t u){
   if (lm->getFirstKnot() - GEOM_EPSILON > u || lm->getLastKnot() + GEOM_EPSILON < u )
       throw PythonExc_IndexError();
   return (lm->*func)(u);
}

template <class T, class U, U (T::* func)(real_t) const >
U getCurveDerivativeValue(const T * lm, real_t u){
   if (lm->getFirstKnot() - GEOM_EPSILON > u || lm->getLastKnot() + GEOM_EPSILON < u
       || fabs(lm->getLastKnot() - lm->getFirstKnot()) < GEOM_EPSILON )
       throw PythonExc_IndexError();
   return (lm->*func)(u);
}

inline uchar_t py_get_lc_width(LineicModel * ln) { return ln->getWidth(); }

void export_LineicModel()
{
  class_<LineicModel,LineicModelPtr, bases<Primitive>, boost::noncopyable>( "LineicModel", "Abstract base class for all lineic model.", no_init )
    .add_property( "firstKnot", &LineicModel::getFirstKnot )
    .add_property( "lastKnot", &LineicModel::getLastKnot )
    .add_property( "stride", &LineicModel::getStride )
    // .def( "getStride", &LineicModel::getStride )
    .def( "getPointAt", &getCurveValue<LineicModel,Vector3,&LineicModel::getPointAt>, args("u") )
    .def( "getTangentAt", &getCurveDerivativeValue<LineicModel,Vector3,&LineicModel::getTangentAt>, args("u") )
    .def( "getNormalAt", &getCurveDerivativeValue<LineicModel,Vector3,&LineicModel::getNormalAt>, args("u") )
    .def( "findClosest", &lm_findclosest<LineicModel,Vector3>, args("point"), "findClosest(point) : return closestpoint, u" )
    .def( "getLength", (real_t (LineicModel::*)()const)&LineicModel::getLength )
    .def( "getLength", (real_t (LineicModel::*)(real_t)const)&LineicModel::getLength, args("begin") )
    .def( "getLength", (real_t (LineicModel::*)(real_t,real_t)const)&LineicModel::getLength, args("begin","end"), "getLength([begin,end]) : Return length of the curve from u = begin to u = end." )
    .def( "getArcLengthToUMapping", &LineicModel::getArcLengthToUMapping,"getArcLengthToUMapping() : Return a function that gives for each arc length the u parametrization of the curve." )
    .def( "getUToArcLengthMapping", &LineicModel::getUToArcLengthMapping,"getUToArcLengthMapping() : Return a function that gives for each u the arc length parametrization of the curve." )
    .DEC_BT_NR_PROPERTY_WDV(width,LineicModel,Width,uchar_t,DEFAULT_WIDTH)
    ;

  implicitly_convertible<LineicModelPtr, PrimitivePtr>();

  def("closestPointToSegment",&seg_findclosest<Vector3>, args("point","segA","segB"));
}



void export_Curve2D()
{
  class_<Curve2D,Curve2DPtr, bases<PlanarModel>, boost::noncopyable>( "Curve2D", "Abstract base class for objects of type of 2D parametric curve.", no_init )
    .add_property( "firstKnot", &Curve2D::getFirstKnot )
    .add_property( "lastKnot", &Curve2D::getLastKnot )
    .add_property( "stride", &Curve2D::getStride )
    .DEC_BT_NR_PROPERTY_WDV(width,Curve2D,Width,uchar_t,DEFAULT_WIDTH)
    // .def( "getStride", &Curve2D::getStride )
    .def( "getPointAt", &getCurveValue<Curve2D,Vector2,&Curve2D::getPointAt>, args("u") )
    .def( "getTangentAt", &getCurveDerivativeValue<Curve2D,Vector2,&Curve2D::getTangentAt>, args("u") )
    .def( "getNormalAt", &getCurveDerivativeValue<Curve2D,Vector2,&Curve2D::getNormalAt>, args("u") )
    .def( "getLength", (real_t (Curve2D::*)() const)&Curve2D::getLength )
    .def( "getLength", (real_t (Curve2D::*)(real_t) const)&Curve2D::getLength, args("begin") )
    .def( "getLength", (real_t (Curve2D::*)(real_t,real_t) const)&Curve2D::getLength, args("begin","end"), "getLength([begin,end]) : Return length of the curve from u = begin to u = end." )
    .def( "getArcLengthToUMapping", &Curve2D::getArcLengthToUMapping,"getArcLengthToUMapping() : Return a function that gives for each arc length the u parametrization of the curve." )
    .def( "getUToArcLengthMapping", &Curve2D::getUToArcLengthMapping,"getUToArcLengthMapping() : Return a function that gives for each u the arc length parametrization of the curve." )
    .def( "findClosest", &lm_findclosest<Curve2D,Vector2>, args("point"), "findClosest(point) : return closestpoint, u" )
    ;

  implicitly_convertible<Curve2DPtr, PlanarModelPtr>();

  def("closestPointToSegment",&seg_findclosest<Vector2>, args("point","segA","segB"),"Find the closest point from segment to a given point. Return closest point, distance, and u value of closest point on the segment");
}
