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
void export_PyPrinter();

// reader export
void export_PglReader();

/* ----------------------------------------------------------------------- */
// gl export
void export_GLRenderer();
void export_GLSkelRenderer();
void export_GLBBoxRenderer();
void export_GLCtrlPointRenderer();

void export_ProjectionCamera();
void export_ProjectionEngine();
void export_ZBufferEngine();
void export_DepthSortEngine();
void export_ProjectionRenderer();

/* ----------------------------------------------------------------------- */
// Turtle export
void export_TurtleParam();
void export_Turtle();
void export_PglTurtle();

/* ----------------------------------------------------------------------- */
// Modeling
void export_SpaceColonization();

/* ----------------------------------------------------------------------- */
// RayCasting export
void export_SegIntersection();
void export_Ray();
void export_RayIntersection();
void export_Intersection();

/* ----------------------------------------------------------------------- */
// Grid export
void export_Mvs();
void export_Octree();
void export_PointGrid();
void export_KDtree();
void export_PyGrid();
void export_PlaneClip();

/* ----------------------------------------------------------------------- */
// CurveManipulation export
void export_CurveManipulation();

/* ----------------------------------------------------------------------- */
// Skeleton export
void export_Skeleton();

/* ----------------------------------------------------------------------- */
// Point manip export
void export_PointManip();
void export_Triangulation3D();

// Dijkstra shortest path
void export_Dijkstra();

// random points algorithm
void export_randompoints();

#endif
