/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include "shape.h"

#include <boost/python.hpp>

#include <scne_scene.h>
#include <scne_shape.h>
#include <scne_sceneobject.h>
#include <geom_geometry.h>
#include <appe_appearance.h>
#include <string>

#include "../util/export_refcountptr.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(GeomShape)
DEF_POINTEE(GeomShape3D)

void class_GeomShape3D()
{
  class_< GeomShape3D, GeomShape3DPtr, bases< SceneObject >, boost::noncopyable >("GeomShape3D", no_init);
}

void gs_setGeometry(GeomShape* shape, GeometryPtr geom )
{ shape->getGeometry()= geom; }

GeometryPtr gs_getGeometry(GeomShape* shape)
{ return shape->getGeometry(); }

void gs_setAppearance(GeomShape* shape, AppearancePtr app )
{ shape->getAppearance()= app; }

AppearancePtr gs_getAppearance(GeomShape* shape)
{ return shape->getAppearance(); }


struct sh_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(GeomShape const& sh)
	{
		return make_tuple(sh.getGeometry(),sh.getAppearance(),sh.getId());
	}
};

void class_GeomShape()
{
  class_< GeomShape,GeomShapePtr, bases< GeomShape3D > , boost::noncopyable >("Shape", init<>())
    .def( init< const RefCountPtr<Geometry> &, 
	          optional< const RefCountPtr<Appearance> &,
	                     long unsigned int > >("Shape( geometry, material, id )") )
    .add_property("appearance", gs_getAppearance, gs_setAppearance)
    .add_property("geometry", gs_getGeometry, gs_setGeometry)
    .def_readwrite("id", &GeomShape::id)
    .def("setComputedName", &GeomShape::setComputedName)
	.def_pickle(sh_pickle_suite());
    ;

  implicitly_convertible<GeomShapePtr, GeomShape3DPtr >();
  
}
