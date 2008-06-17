/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 2000-2007 UMR CIRAD/INRIA/INRA DAP 
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
 
#ifndef __export_action_h__
#define __export_action_h__

/* ----------------------------------------------------------------------- */
// util class export
void export_Sequencer();

// abstract action class export
void export_action();

// basic action export
void export_Discretizer();
void export_Tesselator();
void export_BBoxComputer();
void export_VolComputer();
void export_SurfComputer();
void export_AmapTranslator();
void export_MatrixComputer();

// custom algo
void export_Merge();
void export_Fit();

/* ----------------------------------------------------------------------- */
// abstract printer export
void export_StrPrinter();
void export_FilePrinter();

// printer export
void export_PglPrinter();
void export_PglBinaryPrinter();
void export_PovPrinter();
void export_VrmlPrinter();
void export_VgstarPrinter();

/* ----------------------------------------------------------------------- */
// gl export
void export_GLRenderer();
void export_GLSkelRenderer();
void export_GLBBoxRenderer();
void export_GLCtrlPointRenderer();

/* ----------------------------------------------------------------------- */
// Turtle export
void export_TurtleParam();
void export_Turtle();
void export_PglTurtle();

/* ----------------------------------------------------------------------- */
// RayCasting export
void export_SegIntersection();
void export_Ray();
void export_RayIntersection();

/* ----------------------------------------------------------------------- */
// Grid export
void export_Mvs();
void export_Octree();

/* ----------------------------------------------------------------------- */
// Overlay export
void export_Overlay();
#endif
