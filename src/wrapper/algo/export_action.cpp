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

#include "action.h"
#include "discretizer.h"
#include "bboxcomputer.h"
#include "printer.h"
#include "computer.h"
#include "bounding.h"
#include "fit.h"
#include "intersect.h"
#include "merge.h"

#include <boost/python.hpp>

#include "all_scene.h"
#include "all_appearance.h"
#include "all_geometry.h"

#include <string>

#include "macro_refptr.h"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define OVERLOADING_PROCESS( OBJECT )\
  bool (Action::*p_##OBJECT)( OBJECT *)= &Action::process;

#define DEF_PROCESS( OBJECT ) .def("process",p_##OBJECT, "Process object of type " #OBJECT, args("object"))

OVERLOADING_PROCESS(GeomShape)
OVERLOADING_PROCESS(GeomInline)
OVERLOADING_PROCESS(Material)
OVERLOADING_PROCESS(MonoSpectral)
OVERLOADING_PROCESS(MultiSpectral)
OVERLOADING_PROCESS(ImageTexture)
OVERLOADING_PROCESS(AmapSymbol)
OVERLOADING_PROCESS(AsymmetricHull)
OVERLOADING_PROCESS(AxisRotated)
OVERLOADING_PROCESS(BezierCurve)
OVERLOADING_PROCESS(BezierPatch)
OVERLOADING_PROCESS(Box)
OVERLOADING_PROCESS(Cone)
OVERLOADING_PROCESS(Cylinder)
OVERLOADING_PROCESS(ElevationGrid)
OVERLOADING_PROCESS(EulerRotated)
OVERLOADING_PROCESS(ExtrudedHull)
OVERLOADING_PROCESS(FaceSet)
OVERLOADING_PROCESS(Frustum)
OVERLOADING_PROCESS(Extrusion)
OVERLOADING_PROCESS(Group)
OVERLOADING_PROCESS(IFS)
OVERLOADING_PROCESS(NurbsCurve)
OVERLOADING_PROCESS(NurbsPatch)
OVERLOADING_PROCESS(Oriented)
OVERLOADING_PROCESS(Paraboloid)
OVERLOADING_PROCESS(PointSet)
OVERLOADING_PROCESS(GeomPolyline)
OVERLOADING_PROCESS(QuadSet)
OVERLOADING_PROCESS(Revolution)
OVERLOADING_PROCESS(Swung)
OVERLOADING_PROCESS(Scaled)
OVERLOADING_PROCESS(Sphere)
OVERLOADING_PROCESS(Tapered)
OVERLOADING_PROCESS(Translated)
OVERLOADING_PROCESS(TriangleSet)
OVERLOADING_PROCESS(BezierCurve2D)
OVERLOADING_PROCESS(Disc)
OVERLOADING_PROCESS(NurbsCurve2D)
OVERLOADING_PROCESS(PointSet2D)
OVERLOADING_PROCESS(GeomPolyline2D)

void class_Action()
{
  class_< Action, boost::noncopyable >("Action", no_init)
    .def("beginProcess",&Action::beginProcess, "Called at the begining of the processing of a scene")
    .def("endProcess",&Action::endProcess, "Called at the begining of the processing of a scene")
    DEF_PROCESS(GeomShape)
    DEF_PROCESS(GeomInline)
    DEF_PROCESS(Material)
    DEF_PROCESS(MonoSpectral)
    DEF_PROCESS(MultiSpectral)
    DEF_PROCESS(ImageTexture)
    DEF_PROCESS(AmapSymbol)
    DEF_PROCESS(AsymmetricHull)
    DEF_PROCESS(AxisRotated)
    DEF_PROCESS(BezierCurve)
    DEF_PROCESS(BezierPatch)
    DEF_PROCESS(Box)
    DEF_PROCESS(Cone)
    DEF_PROCESS(Cylinder)
    DEF_PROCESS(ElevationGrid)
    DEF_PROCESS(EulerRotated)
    DEF_PROCESS(ExtrudedHull)
    DEF_PROCESS(FaceSet)
    DEF_PROCESS(Frustum)
    DEF_PROCESS(Extrusion)
    DEF_PROCESS(Group)
    DEF_PROCESS(IFS)
    DEF_PROCESS(NurbsCurve)
    DEF_PROCESS(NurbsPatch)
    DEF_PROCESS(Oriented)
    DEF_PROCESS(Paraboloid)
    DEF_PROCESS(PointSet)
    DEF_PROCESS(GeomPolyline)
    DEF_PROCESS(QuadSet)
    DEF_PROCESS(Revolution)
    DEF_PROCESS(Swung)
    DEF_PROCESS(Scaled)
    DEF_PROCESS(Sphere)
    DEF_PROCESS(Tapered)
    DEF_PROCESS(Translated)
    DEF_PROCESS(TriangleSet)
    DEF_PROCESS(BezierCurve2D)
    DEF_PROCESS(Disc)
    DEF_PROCESS(NurbsCurve2D)
    DEF_PROCESS(PointSet2D)
    DEF_PROCESS(GeomPolyline2D)
    ;
}

void module_action()
{
  class_Action();
  
  // Discretizer
  class_Discretizer();
  class_Tesselator();

  // Printer
  module_printer();

  // Computer
  module_computer();
  module_bounding();

  class_Fit();
  class_RayIntersection();

  // Merge
  class_Merge();
}

