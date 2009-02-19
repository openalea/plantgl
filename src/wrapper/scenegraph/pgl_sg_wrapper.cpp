/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): F. Boudon
 *
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
#include "export_scenegraph.h"
#include <plantgl/python/exception_core.h>
#include <plantgl/scenegraph/pgl_version.h>
#include <plantgl/scenegraph/scene/factory.h>
#include <iostream>

using namespace boost::python;

void module_sg()
{
	// check lib version.
	PGL_LIB_VERSION_CHECK
			
    define_stl_exceptions();

    export_arrays();
    export_arrays2();
    export_index();
    export_Color3();
    export_Color4();
    export_pointarrays();

    export_SceneObject();

	export_BoundingBox();

	export_Scene();
    export_Shape3D();
    export_Shape();

	export_Appearance();
    export_Material();
    export_ImageTexture();
    export_Spectrum();
    export_MonoSpectral();
    export_MultiSpectral();

    export_Geometry();
    export_Primitive();

    export_ExplicitModel();
    export_LineicModel();

    export_Mesh();
    export_Hull();
	export_Patch();
    export_SOR();
    export_PlanarModel();

    export_Curve2D();
    export_BezierCurve2D();
    export_NurbsCurve2D();

    export_geometryarrays();

    export_BezierCurve();
    export_NurbsCurve();

    export_ElevationGrid();
    export_Extrusion();

    export_AsymmetricHull();
	export_ExtrudedHull();
    export_Swung();

    export_SOR2D();
    export_Disc();

    export_BezierPatch();
    export_NurbsPatch();

	export_PointSet();
    export_PointSet2D();
    export_Polyline();
    export_Polyline2D();

	export_Box();
    export_Cone();
    export_Sphere();
    export_Revolution();
	export_Cylinder();
    export_Frustum();
    export_Paraboloid();

    export_Font();
    export_Text();

    export_TriangleSet();
    export_QuadSet();
    export_FaceSet();
    export_AmapSymbol();

    export_Group();
    export_Transformed();

	export_Transformation();
    export_Taper();
    export_Scaling();
    export_Translation();
    export_OrthonormalBasis3D();
    export_AxisRotation();
    export_EulerRotation();
    export_BaseOrientation();
    export_Transform4();

    export_Scaled();
    export_Translated();
    export_IFS();
    export_EulerRotated();
    export_AxisRotated();
    export_Oriented();
    export_Tapered();

    export_SceneCodec();
    export_SceneFactory();

    export_Function();

	scope().attr("PGL_VERSION_STR") = getPGLVersionString();
	scope().attr("PGL_VERSION") = PGL_VERSION;
	scope().attr("PGL_SVNREVISION") = getPGLSvnRevision();
	def("getPGLVersion",&getPGLVersion);

};

void finalize_sg() 
{
	Py_AtExit( &PGL::SceneFactory::finalize );
}

#ifdef PGL_DEBUG
BOOST_PYTHON_MODULE(_pglsg_d)
#else
BOOST_PYTHON_MODULE(_pglsg)
#endif
{
  module_sg();
  finalize_sg();
};

