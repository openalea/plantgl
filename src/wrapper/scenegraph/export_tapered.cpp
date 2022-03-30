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



#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/geometry/primitive.h>


#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(Tapered)


PrimitivePtr tr_primitive(Tapered * obj) { return obj->getPrimitive(); }
void tr_setprimitive(Tapered * obj, PrimitivePtr value) { obj->getPrimitive() = value; }

void export_Tapered()
{
  class_< Tapered, TaperedPtr, bases< Transformed > , boost::noncopyable >
    ("Tapered",
     "Tapered describes an object to which it has been applied a Taper deformation.\n"
     "A Taper deforms an object in order to be able to bound the object within a cone frustum of a specified base radius and top radius.\n"
     "For each point composing an object, a Taper scale the polar coordinates according the zcoordinate.\n"
     "The amplitude of the scale is given by the radii.\n",
     init< real_t,real_t,const PrimitivePtr & >
       ("Tapered(baseRadius, topRadius, primitive)",
       (bp::arg("baseRadius") = Tapered::DEFAULT_BASE_RADIUS,
        bp::arg("topRadius")  = Tapered::DEFAULT_TOP_RADIUS,
        bp::arg("primitive")  = PrimitivePtr()) ))
    .DEF_PGLBASE(Tapered)
    .DEC_BT_PROPERTY_WDV(topRadius,Tapered,TopRadius,real_t,DEFAULT_TOP_RADIUS)
    .DEC_BT_PROPERTY_WDV(baseRadius,Tapered,BaseRadius,real_t,DEFAULT_BASE_RADIUS)
    // .DEC_PTR_PROPERTY(primitive,Deformed,Primitive,PrimitivePtr)
    .add_property("primitive", &tr_primitive, &tr_setprimitive)
    ;

  implicitly_convertible< TaperedPtr, TransformedPtr >();
}



