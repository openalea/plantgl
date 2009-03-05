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

#include <plantgl/scenegraph/geometry/quadset.h>
#include "export_mesh.h"

DEF_POINTEE( QuadSet )

void export_QuadSet()
{

  class_<QuadSet, QuadSetPtr, bases<Mesh>, boost::noncopyable> ( "QuadSet", 
	  "A QuadSet describes a surface formed by a set of connected quadrilaterals, i.e. four sided polygons.\n"
	  " Quads are specified using set of tuples of 4 indices (Index4) pointing to a list of points.",no_init)
	  .def(mesh_func<QuadSet>())
	  .DEF_PGLBASE(QuadSet)
	  ;
     implicitly_convertible<QuadSetPtr, MeshPtr>();
}



