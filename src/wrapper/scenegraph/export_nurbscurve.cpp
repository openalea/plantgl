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



#include <plantgl/scenegraph/geometry/nurbscurve.h>
#include <plantgl/scenegraph/geometry/interpol.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/algo/fitting/fit.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

#include <boost/python.hpp>
#include <string>
#include <sstream>


using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE

DEF_POINTEE( NurbsCurve )
DEF_POINTEE( NurbsCurve2D )



std::string nc_repr( NurbsCurve* p )
{
  std::stringstream ss;
  ss << "NurbsCurve(";
  ss << extract<std::string>(str(object(p->getCtrlPoints())))();
  if (!p->isDegreeToDefault())
      ss << ", degree = " << p->getDegree();
  if (!p->isKnotListToDefault())
      ss << ", knotList = " << extract<std::string>(str(object(p->getKnotList())))();
  if (!p->isStrideToDefault())
      ss << ", stride = " << p->getStride();
  if (!p->isWidthToDefault())
      ss << ", width = " << (int)p->getWidth();
  ss << ")";
  return ss.str();
}


object nurbs_fit1(Point3ArrayPtr pts, int degree, int nbctrlpoints){
    LineicModelPtr res = Fit::nurbsCurve(pts,degree,nbctrlpoints);
    if (!res)return object();
    else {
        return object(res);
    }
}

NurbsCurvePtr nurbs_interpol(Point3ArrayPtr pts, const RealArrayPtr&  parametrization, int degree, bool closed = false){
    Interpol itp (pts, parametrization, degree, closed);
    return itp.get3DCurve();
}

void export_NurbsCurve()
{
  class_<NurbsCurve, NurbsCurvePtr, bases<BezierCurve>, boost::noncopyable>
    ( "NurbsCurve",
      "NURBS Curve describes rational and non rational B-Spline curves.\n"
      "It is defined from a list of control points Pi, a knots vector and a degree p."
      "It uses the parametric equation C(u) = Sum(i=0,n)(Ri,p(u)Pi with u in [a,b]"
      "where the Ri,p(u) are p-th degree rational basis functions defined on the knot vector.",
      init<Point4ArrayPtr, optional< uint_t, RealArrayPtr, uint_t, uchar_t > >(
      "NurbsCurve(ctrlPointList[,degree,knotList,stride])",(
          bp::arg("ctrlPointList"),
          bp::arg("degree")  = NurbsCurve::DEFAULT_NURBS_DEGREE,
          bp::arg("knotList")= TOOLS(RealArrayPtr()) ,
          bp::arg("stride") = NurbsCurve::DEFAULT_STRIDE,
          bp::arg("width") =  NurbsCurve::DEFAULT_WIDTH)) )
     .def(init<Point4ArrayPtr,  RealArrayPtr, optional< uint_t, uint_t, uchar_t > >())
     .DEF_PGLBASE(NurbsCurve)
     .DEC_BT_NR_PROPERTY_WDV(degree,NurbsCurve,Degree,uint_t,DEFAULT_NURBS_DEGREE)
     .DEC_PTR_PROPERTY_WD(knotList,NurbsCurve,KnotList,RealArrayPtr)
     .def("setKnotListToDefault",&NurbsCurve::setKnotListToDefault)
     .def( "__repr__", nc_repr )
     .def( "fit", nurbs_fit1, "fit(points [, int degree, int nbctrlpoints])", (bp::arg("points"),bp::arg("degree")=3,bp::arg("nbctrlpoints")=4) )
     .staticmethod("fit")
     .def( "interpol", nurbs_interpol, "interpol(points [, parametrization, int degree, bool closed])", (bp::arg("points"),bp::arg("parametrization"),bp::arg("degree")=3,bp::arg("closed")=false) )
     .staticmethod("interpol")
     .def( "getDerivativeAt", &NurbsCurve::getDerivativeAt, args("u","d") )
     .def( "getDerivativesAt", &NurbsCurve::getDerivativesAt, args("u") )
     .def( "findSpan", &findSpan, args("u","degree","knotList"),
           "int findSpan(float u,  int degree,  [float] knotList)."
           "Determine the knot Span index at a given u for degree and on the knot vector knotList."
           "See the Nurbs Book : A2.1 p68" )
     .staticmethod("findSpan")
     .def( "basisFunctions", &basisFunctions, args("span","u","degree","knotList"),
        "[float] basisFunctions(int span, float u, int  degree, [float] knotList)."
        "Compute the Basis Functions values at a given u for degree and on the knot vector knotList."
        "See Algo 2.2 From The Nurbs Book p70.")
     .staticmethod("basisFunctions")
     .def( "derivatesBasisFunctions", &derivatesBasisFunctions, args("n","u","span","degree","knotList"),
        "[float] derivatesBasisFunctions(int span, float u, int  _degree, [float] _knotList)."
        "Compute the n-th Derivative Basis Functions values at a given u for degree and on the knot vector knotList."
        "See Algo 2.2 From The Nurbs Book p70." )
     .staticmethod("derivatesBasisFunctions")
     .def("defaultKnotList",&NurbsCurve::defaultKnotList,args("nbCtrlPoints","degree"),"Compute a default knot list for a curve of n control points and a degree d")
     .staticmethod("defaultKnotList")
     .def("defaultKnotListTest",&NurbsCurve::defaultKnotListTest,args("knots","nbCtrlPoints","degree"),"Test whether knots corresponds to a default knot list for a curve of n control points and a degree d")
     .staticmethod("defaultKnotListTest")
    ;

  implicitly_convertible<NurbsCurvePtr, BezierCurvePtr>();
}


std::string nc2_repr( NurbsCurve2D* p )
{
  std::stringstream ss;
  ss << "NurbsCurve2D(";
  ss << extract<std::string>(str(object(p->getCtrlPoints())))();
  if (!p->isDegreeToDefault())
      ss << ", degree = " << p->getDegree();
  if (!p->isKnotListToDefault())
      ss << ", knotList = " << extract<std::string>(str(object(p->getKnotList())))();
  if (!p->isStrideToDefault())
      ss << ", stride = " << p->getStride();
  if (!p->isWidthToDefault())
      ss << ", width = " << (int)p->getWidth();
  ss << ")";
  return ss.str();
}

object convert_lc(const LineicModelPtr& res)
{
    if (!res)return object();
    else {
        NurbsCurvePtr c = dynamic_pointer_cast<NurbsCurve>(res);
        if(c){
            Point3ArrayPtr cpts(new Point3Array(c->getCtrlPointList()->size()));
            Point3Array::iterator _it = cpts->begin();
            for (Point4Array::const_iterator _it2 = c->getCtrlPointList()->begin();
                _it2 != c->getCtrlPointList()->end(); _it2++){
                    *_it = Vector3(_it2->x(),_it2->y(),_it2->w());
                    _it++;
                }
                NurbsCurve2DPtr c2(new NurbsCurve2D(cpts,c->getKnotList(),c->getDegree(),c->getStride()));
                return object(c2);
        }
        else {
            BezierCurvePtr bc = dynamic_pointer_cast<BezierCurve>(res);
            if (bc){
                Point3ArrayPtr cpts(new Point3Array(bc->getCtrlPointList()->size()));
                Point3Array::iterator _it = cpts->begin();
                for (Point4Array::const_iterator _it2 = bc->getCtrlPointList()->begin();
                    _it2 != bc->getCtrlPointList()->end(); _it2++){
                        *_it = Vector3(_it2->x(),_it2->y(),_it2->w());
                        _it++;
                    }
                    BezierCurve2DPtr bc2(new BezierCurve2D(cpts,bc->getStride()));
                    return object(bc2);
            }
            else {
                PolylinePtr pc = dynamic_pointer_cast<Polyline>(res);
                if(pc){
                    Point2ArrayPtr cpts(new Point2Array(pc->getPointList()->size()));
                    Point2Array::iterator _it = cpts->begin();
                    for (Point3Array::const_iterator _it2 = pc->getPointList()->begin();
                        _it2 != pc->getPointList()->end(); _it2++){
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

NurbsCurve2DPtr nurbs2_interpol(Point2ArrayPtr pts, const RealArrayPtr&  parametrization, int degree, bool closed = false){
    
    Interpol itp (Point3ArrayPtr(new Point3Array(pts,0)), parametrization, degree,closed);
    return itp.get2DCurve();
}

void export_NurbsCurve2D()
{
   class_<NurbsCurve2D, NurbsCurve2DPtr, bases<BezierCurve2D>, boost::noncopyable>
     ( "NurbsCurve2D", "A 2D NURBS Curve represented by an array of control Points, a knots list and a degree. See NurbsCurve.",
       init<Point3ArrayPtr,  optional<uint_t, RealArrayPtr, uint_t, uchar_t > >(
       "NurbsCurve2D(ctrlPointList[,degree,knotList,stride])",
         ( bp::arg("ctrlPointList"),
          bp::arg("degree")  = NurbsCurve::DEFAULT_NURBS_DEGREE,
          bp::arg("knotList")= TOOLS(RealArrayPtr()) ,
          bp::arg("stride") = NurbsCurve::DEFAULT_STRIDE,
          bp::arg("width") = NurbsCurve2D::DEFAULT_WIDTH)) )
     .def(init<Point3ArrayPtr, RealArrayPtr, optional<  uint_t, uint_t, uchar_t > >(args("ctrlPointList","knotList","degree","stride","width")) )
     .DEF_PGLBASE(NurbsCurve2D)
     .def( "__repr__", nc2_repr )
     .def( "fit", nurbs2_fit1, args("points") )
     .def( "fit", nurbs2_fit2, args("points","degree","nbctrlpoints"), "fit(points [, int degree, int nbctrlpoints])" )
     .def( "fit", nurbs2_fit3, args("points") )
     .def( "fit", nurbs2_fit4, args("points","degree","nbctrlpoints") )
     .staticmethod("fit")
     .def( "interpol", nurbs2_interpol, "interpol(points, parametrization [, int degree, bool closed])", (bp::arg("points"),bp::arg("parametrization"),bp::arg("degree")=3,bp::arg("closed")=false) )
     .staticmethod("interpol")
     .DEC_BT_NR_PROPERTY_WD(degree,NurbsCurve2D,Degree,uint_t)
     .DEC_PTR_PROPERTY_WD(knotList,NurbsCurve2D,KnotList,RealArrayPtr)
     .def("setKnotListToDefault",&NurbsCurve2D::setKnotListToDefault)
    ;

   implicitly_convertible< NurbsCurve2DPtr, BezierCurve2DPtr >();
}

