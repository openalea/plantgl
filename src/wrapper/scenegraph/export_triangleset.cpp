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

#include <plantgl/scenegraph/geometry/triangleset.h>
#include "export_mesh.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;
#define bp boost::python

DEF_POINTEE( TriangleSet )


void export_TriangleSet()
{
    class_<TriangleSet, TriangleSetPtr, bases<Mesh>, boost::noncopyable>
	( "TriangleSet", 
	  "A TriangleSet describes a surface formed by a set of connected triangles, i.e. three sided polygons.\n"
	  " Triangles are specified using set of tuples of 3 indices (Index3) pointing to a list of points.",no_init)
	  .def(mesh_func<TriangleSet>())
	  ;

  implicitly_convertible<TriangleSetPtr, MeshPtr>();
}

