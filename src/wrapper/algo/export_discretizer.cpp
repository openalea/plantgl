/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

#include <algo/base/discretizer.h>
#include <algo/base/tesselator.h>
#include <scenegraph/geometry/explicitmodel.h>
#include <scenegraph/geometry/triangleset.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

/* ----------------------------------------------------------------------- */

ExplicitModelPtr d_getDiscretization( Discretizer* d )
{ return d->getDiscretization(); }

/* ----------------------------------------------------------------------- */

bool get_Dis_texCoord(Discretizer * obj){ 
  return obj->texCoordComputed(); 
} 
void set_Dis_texCoord(Discretizer * obj, bool v){ 
  obj->computeTexCoord(v); 
} 

/* ----------------------------------------------------------------------- */

void export_Discretizer()
{
  class_< Discretizer,bases< Action >,boost::noncopyable >
    ("Discretizer", init<>("Discretizer() -> Compute the objects discretization" ))
    .def("clear",&Discretizer::clear)
    .add_property("discretization",d_getDiscretization, "Return the last computed discretization.")
	.add_property("texCoord",get_Dis_texCoord,set_Dis_texCoord)
    .add_property("result",d_getDiscretization)
    ;
}

/* ----------------------------------------------------------------------- */

TriangleSetPtr 	t_getTriangulation ( Tesselator* t )
{ return t->getTriangulation(); }

/* ----------------------------------------------------------------------- */

void export_Tesselator()
{
  class_< Tesselator,bases< Discretizer >,boost::noncopyable >
    ("Tesselator", init<>("Tesselator() -> Compute tobjects triangulation. " ))
    .add_property("triangulation",t_getTriangulation,"Return the last computed triangulation.")
    .add_property("result",t_getTriangulation)
    ;

}

/* ----------------------------------------------------------------------- */

