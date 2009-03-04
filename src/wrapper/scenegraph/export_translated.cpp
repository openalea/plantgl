/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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
TOOLS_USING_NAMESPACE
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

