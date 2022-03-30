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
    export_WireComputer();

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

    export_ProjectionCamera();
    export_ProjectionEngine();
    export_ZBufferEngine();
    export_DepthSortEngine();
    export_ProjectionRenderer();

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

    // random points algo
    export_randompoints();
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

