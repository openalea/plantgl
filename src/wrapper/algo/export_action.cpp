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
  
#include "../util/export_refcountptr.h"
#include <plantgl/pgl_scenegraph.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

#define OVERLOADING_PROCESS( OBJECT )\
  bool (Action::*p_##OBJECT)( OBJECT *)= &Action::process;

#define DEF_PROCESS( OBJECT ) .def("process",p_##OBJECT, "Process object of type " #OBJECT, args("object"))

/* ----------------------------------------------------------------------- */

OVERLOADING_PROCESS(Shape)
OVERLOADING_PROCESS(Inline)
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
OVERLOADING_PROCESS(Polyline)
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
OVERLOADING_PROCESS(Polyline2D)

/* ----------------------------------------------------------------------- */

void export_action()
{
  class_< Action, boost::noncopyable >("Action", no_init)
    .def("beginProcess",&Action::beginProcess, "Called at the begining of the processing of a scene")
    .def("endProcess",&Action::endProcess, "Called at the begining of the processing of a scene")
    DEF_PROCESS(Shape)
    DEF_PROCESS(Inline)
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
    DEF_PROCESS(Polyline)
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
    DEF_PROCESS(Polyline2D)
    ;
}

/* ----------------------------------------------------------------------- */


