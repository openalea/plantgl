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
#include "export_action.h"
#include <plantgl/python/exception_core.h>

/* ----------------------------------------------------------------------- */

void module_algo()
{
  define_stl_exceptions();

    // util class export
    export_Sequencer();

	// abstract action class export
    export_action();

	// basic action export
    export_Discretizer();
    export_Tesselator();
    export_BBoxComputer();
    export_VolComputer();
    export_SurfComputer();
    export_AmapTranslator();
    export_MatrixComputer();

	// custom algo
    export_Merge();
    export_Fit();

	// abstract printer export
    export_StrPrinter();
    export_FilePrinter();

	// printer export
    export_PglPrinter();
    export_PglBinaryPrinter();
    export_PovPrinter();
    export_VrmlPrinter();
    export_VgstarPrinter();
    export_PyPrinter();

    // reader export
    export_PglReader();

    // gl export
    export_GLRenderer();
    export_GLSkelRenderer();
    export_GLBBoxRenderer();
    export_GLCtrlPointRenderer();

    // Turtle export
    export_TurtleParam();
    export_Turtle();
    export_PglTurtle();

    // Modeling
    export_SpaceColonization();

    // RayCasting export
    export_SegIntersection();
    export_Ray();
    export_RayIntersection();
    export_Intersection();

    // Grid export
    export_Mvs();
    export_Octree();
    export_PointGrid();
    export_KDtree();
    export_PyGrid();
    export_PlaneClip();

    // CurveManipulation export
    export_CurveManipulation();

    // Skeleton export
    export_Skeleton();

    // Point manip
    export_PointManip();
    export_Triangulation3D();

    // Dijkstra shortest path
    export_Dijkstra();
};

/* ----------------------------------------------------------------------- */

#ifdef PGL_DEBUG
BOOST_PYTHON_MODULE(_pglalgo_d)
#else
BOOST_PYTHON_MODULE(_pglalgo)
#endif
{
  module_algo();
};

