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




#include <plantgl/scenegraph/geometry/cone.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Cone)


void export_Cone()
{
  class_< Cone, ConePtr, bases< SOR > , boost::noncopyable >
    ("Cone", "A cone structure defined by a radius and a height. Its base is centered at origin.",
    init< optional<const real_t&,const real_t&, bool,uchar_t > >
               ("Cone(radius, height [, solid, slices])",
               (bp::arg("radius")=Cone::DEFAULT_RADIUS,
                bp::arg("height")=Cone::DEFAULT_HEIGHT,
                bp::arg("solid") =Cone::DEFAULT_SOLID,
                bp::arg("slices")=Cone::DEFAULT_SLICES)
                )
    )
    .DEF_PGLBASE(Cone)
  .DEC_BT_PROPERTY_WDV(radius,Cone,Radius,real_t,DEFAULT_RADIUS)
  .DEC_BT_PROPERTY_WDV(height,Cone,Height,real_t,DEFAULT_HEIGHT)
  .DEC_BT_NR_PROPERTY_WDV(solid,Cone,Solid,bool,DEFAULT_SOLID);

  implicitly_convertible<ConePtr, SORPtr >();
}


