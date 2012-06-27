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

#include "export_grid.h"
#include <plantgl/algo/grid/regularpointgrid.h>

/* ----------------------------------------------------------------------- */


void export_PointGrid()
{

  class_< Point2Grid > ("Point2Grid", init<Vector2, Point2ArrayPtr>
     ( "Construct a regular grid from a set of 2D points.", args("voxelsize","points") ))
	 .def(pointgrid_func<Point2Grid>())
    ;
  
  class_< Point3Grid > ("Point3Grid", init<Vector3, Point3ArrayPtr>
     ( "Construct a regular grid from a set of 3D points.", args("voxelsize","points") ))
	 .def(pointgrid_func<Point3Grid>())
    ;
  class_< Point4Grid > ("Point4Grid", init<Vector4, Point4ArrayPtr>
     ( "Construct a regular grid from a set of 4D points.", args("voxelsize","points") ))
	 .def(pointgrid_func<Point4Grid>())
    ;
  
}

/* ----------------------------------------------------------------------- */
