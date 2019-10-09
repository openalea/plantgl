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



#include <plantgl/scenegraph/transformation/eulerrotated.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(EulerRotated)


void export_EulerRotated()
{
  class_< EulerRotated, EulerRotatedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("EulerRotated",
    "The EulerRotated describes an object to which it has been applied a composition of 3 rotations.\n"
    "Rotations angles are the roll around the x-axis, the elevation around the rotated y-axis\n"
    "and azimuth around the rotated z -axis. The equivalent rotation is given by the matrix:\n"
    " | ca*ce   ca*se*sr - sa*cr    ca*se*cr + sa*sr |\n"
    " | sa*ce   ca*cr + sa*se*sr    sa*se*cr - ca*sr |\n"
    " | -se     ce*sr               ce*cr            |\n"
    "where cr = cos(roll), sr = sin(roll), ce = cos(elevation), se = sin(elevation), ca = cos(azimuth) and sa = sin(azimuth).\n",
    init< const real_t&,const real_t&,const real_t&, const GeometryPtr& >
       ( "EulerRotated(azimuth,elevation,roll,geometry)",
       (bp::arg("azimuth")=EulerRotated::DEFAULT_AZIMUTH,
         bp::arg("elevation")=EulerRotated::DEFAULT_ELEVATION,
         bp::arg("roll")=EulerRotated::DEFAULT_ROLL,
         bp::arg("geometry")=GeometryPtr()) ))
    .DEF_PGLBASE(EulerRotated)
    .DEC_ANGLE_PROPERTY_WDV(azimuth,EulerRotated,Azimuth,DEFAULT_AZIMUTH)
    .DEC_ANGLE_PROPERTY_WDV(elevation,EulerRotated,Elevation,DEFAULT_ELEVATION)
    .DEC_ANGLE_PROPERTY_WDV(roll,EulerRotated,Roll,DEFAULT_ROLL)
    ;

  implicitly_convertible< EulerRotatedPtr, OrthoTransformedPtr >();
}

