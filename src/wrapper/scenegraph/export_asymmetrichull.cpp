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



#include <plantgl/scenegraph/geometry/asymmetrichull.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(AsymmetricHull)

void export_AsymmetricHull()
{
  class_< AsymmetricHull, AsymmetricHullPtr, bases< Hull >,boost::noncopyable >
    ("AsymmetricHull",
    "An Asymmetric Hull describes an anvelop defined by 6 morphological points.\n"
    "This is an implementation of the asymmetric crowns introduced by [Koop,89] and [Cescatti,97]."
    "The two first morphological points are the bottom and top points of the hull."
    "The four other points are used to defined the peripheral line of the hull (P1,P2,P3,P4)."
    "The two first points are located along the x -axis (P1,P2) and the two other along the y-axis (P3,P4)."
    "As such Pi points are described with only two dimensions, i.e. a radius and a height using corresponding parameters"
    "Finally, the shape coefficients are versatile indices which describe the curvature of the hull above and below the peripheral line.",
     init< boost::python::optional < const real_t&, const real_t&, const real_t&,
     const real_t&, const real_t&, const real_t&,
     const real_t&, const real_t&, const Vector3&, const Vector3&, const real_t&, const real_t&,
     uchar_t, uchar_t > >
     ( "AsymmetricHull([negXRadius,posXRadius,negYRadius,posYRadius,"
            "negXHeight,posXHeight,negYHeight,posYHeight,"
            "bottom, top, bottomShape, topShape, slices, stacks])",
            (bp::arg("negXRadius")=AsymmetricHull::DEFAULT_NEG_X_RADIUS,
             bp::arg("posXRadius")=AsymmetricHull::DEFAULT_POS_X_RADIUS,
             bp::arg("negYRadius")=AsymmetricHull::DEFAULT_NEG_Y_RADIUS,
             bp::arg("posYRadius")=AsymmetricHull::DEFAULT_POS_Y_RADIUS,
             bp::arg("negXHeight")=AsymmetricHull::DEFAULT_NEG_X_HEIGHT,
             bp::arg("posXHeight")=AsymmetricHull::DEFAULT_POS_X_HEIGHT,
             bp::arg("negYHeight")=AsymmetricHull::DEFAULT_NEG_Y_HEIGHT,
             bp::arg("posYHeight")=AsymmetricHull::DEFAULT_POS_Y_HEIGHT,
             bp::arg("bottom")=AsymmetricHull::DEFAULT_BOTTOM,
             bp::arg("top")=AsymmetricHull::DEFAULT_TOP,
             bp::arg("bottomShape")=AsymmetricHull::DEFAULT_BOTTOM_SHAPE,
             bp::arg("topShape")=AsymmetricHull::DEFAULT_TOP_SHAPE,
             bp::arg("slices")=AsymmetricHull::DEFAULT_SLICES,
             bp::arg("stacks")=AsymmetricHull::DEFAULT_STACKS )) )
        .DEC_BT_PROPERTY_WDV(negXRadius,AsymmetricHull,NegXRadius,real_t,DEFAULT_NEG_X_RADIUS)
        .DEC_BT_PROPERTY_WDV(posXRadius,AsymmetricHull,PosXRadius,real_t,DEFAULT_POS_X_RADIUS)
        .DEC_BT_PROPERTY_WDV(negYRadius,AsymmetricHull,NegYRadius,real_t,DEFAULT_NEG_Y_RADIUS)
        .DEC_BT_PROPERTY_WDV(posYRadius,AsymmetricHull,PosYRadius,real_t,DEFAULT_POS_Y_RADIUS)
        .DEC_BT_PROPERTY_WDV(negXHeight,AsymmetricHull,NegXHeight,real_t,DEFAULT_NEG_X_HEIGHT)
        .DEC_BT_PROPERTY_WDV(posXHeight,AsymmetricHull,PosXHeight,real_t,DEFAULT_POS_X_HEIGHT)
        .DEC_BT_PROPERTY_WDV(negYHeight,AsymmetricHull,NegYHeight,real_t,DEFAULT_NEG_Y_HEIGHT)
        .DEC_BT_PROPERTY_WDV(posYHeight,AsymmetricHull,PosYHeight,real_t,DEFAULT_POS_Y_HEIGHT)
        .DEC_CT_PROPERTY_WDV(bottom,AsymmetricHull,Bottom,Vector3,DEFAULT_BOTTOM)
        .DEC_CT_PROPERTY_WDV(top, AsymmetricHull,Top,Vector3,DEFAULT_TOP)
        .DEC_BT_PROPERTY_WDV(bottomShape,AsymmetricHull,BottomShape,real_t,DEFAULT_BOTTOM_SHAPE)
        .DEC_BT_PROPERTY_WDV(topShape,AsymmetricHull,TopShape,real_t,DEFAULT_TOP_SHAPE)
        .DEC_BT_PROPERTY_WDV(slices,AsymmetricHull,Slices,uchar_t,DEFAULT_SLICES)
        .DEC_BT_PROPERTY_WDV(stacks,AsymmetricHull,Stacks,uchar_t,DEFAULT_STACKS)
        .DEF_PGLBASE(AsymmetricHull)

    ;

  implicitly_convertible<AsymmetricHullPtr,HullPtr >();
}



