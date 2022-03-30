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



#ifndef __export_scenegraph_h__
#define __export_scenegraph_h__

void export_arrays();
void export_arrays2();
void export_index();
void export_Color3();
void export_Color4();
void export_pointarrays();
void export_geometryarrays();
void export_Image();


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

void export_Progress();

#endif
