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
 
#ifndef __export_scenegraph_h__
#define __export_scenegraph_h__

void export_arrays();
void export_arrays2();
void export_index();
void export_Color3();
void export_Color4();
void export_pointarrays();
void export_geometryarrays();

void export_Appearance();
void export_Material();
void export_Texture2D();
void export_ImageTexture();
void export_Texture2DTransformation();
void export_Spectrum();
void export_MonoSpectral();
void export_MultiSpectral();
void export_BezierCurve();
void export_BezierCurve2D();
void export_BoundingBox();
void export_Cylinder();
void export_Frustum();
void export_Paraboloid();
void export_ElevationGrid();
void export_Extrusion();
void export_Geometry();
void export_Group();
void export_Hull();
void export_AsymmetricHull();
void export_ExtrudedHull();
void export_LineicModel();
void export_PlanarModel();
void export_Curve2D();
void export_SOR2D();
void export_Disc();
void export_Material();
void export_ImageTexture();
void export_ExplicitModel();
void export_Mesh();
void export_NurbsCurve();
void export_NurbsCurve2D();
void export_Patch();
void export_BezierPatch();
void export_NurbsPatch();
void export_ParametricModel();
void export_PointSet();
void export_PointSet2D();
void export_Polyline();
void export_Polyline2D();
void export_Primitive();
void export_Box();
void export_SOR();
void export_Sphere();
void export_Cone();
void export_Revolution();
void export_SceneObject();
void export_Scene();
void export_Shape3D();
void export_Shape();
void export_Swung();
void export_Font();
void export_Text();
void export_Transformation();
void export_Taper();
void export_Scaling();
void export_Translation();
void export_OrthonormalBasis3D();
void export_AxisRotation();
void export_EulerRotation();
void export_BaseOrientation();
void export_Transform4();
void export_Transformed();
void export_Scaled();
void export_ScreenProjected();
void export_Translated();
void export_IFS();
void export_EulerRotated();
void export_AxisRotated();
void export_Oriented();
void export_Tapered();
void export_TriangleSet();
void export_QuadSet();
void export_FaceSet();
void export_AmapSymbol();

void export_SceneCodec();
void export_SceneFactory();
void export_DeepCopier();

void export_Function();
void export_Plane();
#endif