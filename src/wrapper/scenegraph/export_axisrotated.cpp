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



#include <plantgl/scenegraph/transformation/axisrotated.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python


DEF_POINTEE(AxisRotated)

void export_AxisRotated()
{
  class_< AxisRotated, AxisRotatedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("AxisRotated",
     "The AxisRotated describes an object to which it has been applied a rotation of a specified angle about a specified axis.\n"
     "The rotation is given by the matrix: |(1-c)x2 + c    (1-c)xy - sz (1-c)xy + sy|\n"
     "                                     |(1-c)xy + sz   (1-c)y2 + c  (1-c)yz - sx|\n"
     "                                     |(1-c)xz - sy   (1-c)yz + sx (1-c)z2 + c |\n"
     "where s = sin(angle), c = cos(angle), x,y and z the coordinates of the axis.",
     init< const Vector3&, const real_t&, const GeometryPtr& >
     ("AxisRotated( Vector3 axis, radian angle, geometry)",
     (bp::arg("axis") = AxisRotated::DEFAULT_AXIS,
      bp::arg("angle") = AxisRotated::DEFAULT_ANGLE,
      bp::arg("geometry") = GeometryPtr())))
    .DEC_CT_PROPERTY_WDV(axis,AxisRotated,Axis,Vector3,DEFAULT_AXIS)
    .DEC_ANGLE_NR_PROPERTY_WDV(angle,AxisRotated,Angle,DEFAULT_ANGLE)
    .DEF_PGLBASE(AxisRotated)
    ;

  implicitly_convertible< AxisRotatedPtr, OrthoTransformedPtr >();
}



