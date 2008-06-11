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

#include <boost/python.hpp>
#include <plantgl/scenegraph/geometry/geometry.h>
#include "../util/export_refcountptr.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Geometry)

const GeometryPtr deepcopy(Geometry * s){
    return GeometryPtr::Cast(s->copy());
}

void export_Geometry()
{

   class_< Geometry,GeometryPtr, bases< SceneObject >, boost::noncopyable > 
      ("Geometry","Abstract base class for all geometrical structure.", no_init)
     .def("isACurve",&Geometry::isACurve)
     .def("isASurface",&Geometry::isASurface)
     .def("isAVolume",&Geometry::isAVolume)
     .def("isExplicit",&Geometry::isExplicit)
     .def("deepcopy", &deepcopy)
;
   
   implicitly_convertible<GeometryPtr, SceneObjectPtr >();
   
}

