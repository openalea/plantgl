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



#include <plantgl/scenegraph/transformation/oriented.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python


DEF_POINTEE(Oriented)

void export_Oriented()
{
  class_< Oriented, OrientedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("Oriented",
     "Oriented describes an object to a change of coordinate specified by an orthonormal basis has been applied.\n"
     "The basis is expressed by the matrix:\n"
     "|px sx tx|\n"
     "|py sy ty|\n"
     "|pz sz tz|\n"
     "where (px, py, pz) denotes the primary direction, (sx,, sy, sz) the secondary direction and (tx,, ty, tz) the ternary direction, which is given by: t = p ^ s.",
     init< const Vector3&,const Vector3&,const GeometryPtr& >
       ("Oriented(primary,secondary,geometry) with primary,secondary: Vector3" ,
       (bp::arg("primary")   = Oriented::DEFAULT_PRIMARY,
        bp::arg("secondary") = Oriented::DEFAULT_SECONDARY,
        bp::arg("geometry")  = GeometryPtr())   ) )
    .DEF_PGLBASE(Oriented)
    .DEC_CT_PROPERTY_WDV(primary,Oriented,Primary,Vector3,DEFAULT_PRIMARY)
    .DEC_CT_PROPERTY_WDV(secondary,Oriented,Secondary,Vector3,DEFAULT_SECONDARY)
    ;

  implicitly_convertible< OrientedPtr, OrthoTransformedPtr >();
}


