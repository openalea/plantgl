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



#include <plantgl/python/export_refcountptr.h>
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


