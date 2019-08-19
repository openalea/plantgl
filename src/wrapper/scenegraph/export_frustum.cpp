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




#include <plantgl/scenegraph/geometry/frustum.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(Frustum)

void export_Frustum()
{
  class_< Frustum, FrustumPtr, bases< Cone > , boost::noncopyable >
    ("Frustum", "A frustum structure defined by a base radius, a height and a taper factor.\n Its base is centered at origin.",
       init< optional<const real_t&,const real_t&,const real_t&, bool,uchar_t> >
            ("Frustum([radius, height, taper, solid, slices])",
            (bp::arg("radius")=Frustum::DEFAULT_RADIUS,
            bp::arg("height")=Frustum::DEFAULT_HEIGHT,
            bp::arg("taper")=Frustum::DEFAULT_TAPER,
            bp::arg("solid")=Frustum::DEFAULT_SOLID,
            bp::arg("slices")=Frustum::DEFAULT_SLICES)))
   .DEF_PGLBASE(Frustum)
   .DEC_BT_PROPERTY_WDV(taper,Frustum,Taper,real_t,DEFAULT_TAPER)
      ;

  implicitly_convertible<FrustumPtr, ConePtr >();
}
