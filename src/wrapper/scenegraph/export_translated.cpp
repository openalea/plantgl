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



#include <plantgl/scenegraph/transformation/translated.h>

#include <plantgl/math/util_vector.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <boost/python/make_constructor.hpp>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python


DEF_POINTEE(Translated)


TranslatedPtr tr_from_val(real_t x, real_t y, real_t z, const GeometryPtr& geom)
{
    return TranslatedPtr(new Translated(Vector3(x,y,z),geom));
}

void export_Translated()
{
  class_< Translated, TranslatedPtr, bases< MatrixTransformed > , boost::noncopyable >
    ("Translated",
    "A Translated describes an object to which a translation of a specified vector is applied.\n"
    "The translation is given by the homogeneous matrix:\n"
    "|1 0 0 tx|\n"
    "|0 1 0 ty|\n"
    "|0 0 1 tz|\n"
    "|0 0 0 1 |\n"
    "where (tx, ty, tz) denotes the translation vector.\n",
     init< const Vector3&, const GeometryPtr& >
     ("Translated(translation,geometry)",
       (bp::arg("translation")=Translated::DEFAULT_TRANSLATION,
        bp::arg("geometry")=GeometryPtr())) )
    .def( "__init__", make_constructor( tr_from_val ) )
    .DEF_PGLBASE(Translated)
    .DEC_CT_PROPERTY_WDV(translation,Translated,Translation,Vector3,DEFAULT_TRANSLATION)
    ;

  implicitly_convertible< TranslatedPtr, MatrixTransformedPtr >();

}

