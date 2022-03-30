
# This file has been generated at Mon Aug 23 11:36:04 2010

from openalea.core import *


__name__ = 'vplants.plantgl.objects'

__editable__ = False
__alias__ = ['PlantGL.Objects']
__version__ = '0.0.2'
__description__ = 'PlantGL Object Generator nodes.'
__license__ = 'CECILL-V2'
__authors__ = 'F. Boudon, C. Pradal'
__url__ = 'http://openalea.gforge.inria.fr'
__institutes__ = 'INRIA/CIRAD'


__all__ = ['objectgenerator_TextNode', 'objectgenerator_BezierPatchNode', 'objectgenerator_MaterialNode', 'objectgenerator_Color3Node', 'objectgenerator_EulerRotatedNode', 'objectgenerator_BoxNode', 'objectgenerator_AsymmetricHullNode', 'objectgenerator_SceneNode', 'objectgenerator_Texture2DTransformationNode', 'objectgenerator_ExtrudedHullNode', 'objectgenerator_BezierCurve2DNode', 'objectgenerator_GroupNode', 'objectgenerator_QuadSetNode', 'objectgenerator_QuantisedFunctionNode', 'objectgenerator_AxisRotatedNode', 'objectgenerator_Color4Node', 'objectgenerator_ImageTextureNode', 'objectgenerator_PolylineNode', 'objectgenerator_TriangleSetNode', 'objectgenerator_ScaledNode', 'objectgenerator_Polyline2DNode', 'objectgenerator_NurbsCurveNode', 'objectgenerator_OrientedNode', 'objectgenerator_ExtrusionNode', 'objectgenerator_MonoSpectralNode', 'objectgenerator_CylinderNode', 'objectgenerator_ConeNode', 'objectgenerator_FrustumNode', 'objectgenerator_PointSetNode', 'objectgenerator_FaceSetNode', 'objectgenerator_RevolutionNode', 'objectgenerator_SwungNode', 'objectgenerator_Texture2DNode', 'objectgenerator_ParaboloidNode', 'objectgenerator_BezierCurveNode', 'objectgenerator_MultiSpectralNode', 'objectgenerator_NurbsPatchNode', 'objectgenerator_PointSet2DNode', 'objectgenerator_NurbsCurve2DNode', 'objectgenerator_DiscNode', 'objectgenerator_TaperedNode', 'objectgenerator_AmapSymbolNode', 'objectgenerator_ShapeNode', 'objectgenerator_TranslatedNode', 'objectgenerator_ElevationGridNode', 'objectgenerator_SphereNode', 'objectgenerator_IFSNode']



objectgenerator_TextNode = Factory(name='Text',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Text',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='TextNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_BezierPatchNode = Factory(name='BezierPatch',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a BezierPatch',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='BezierPatchNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_MaterialNode = Factory(name='Material',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Material',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='MaterialNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_Color3Node = Factory(name='Color3',
                authors='Thomas Cokelaer',
                description='',
                category='Unclassified',
                nodemodule='objectgenerator',
                nodeclass='Color3Node',
                inputs=[{'interface': ISequence, 'name': 'IN1', 'value': [0, 150, 0], 'desc': ''}],
                outputs=[{'interface': None, 'name': 'OUT1', 'desc': ''}],
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_EulerRotatedNode = Factory(name='EulerRotated',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a EulerRotated',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='EulerRotatedNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_BoxNode = Factory(name='Box',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Box',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='BoxNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_AsymmetricHullNode = Factory(name='AsymmetricHull',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a AsymmetricHull',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='AsymmetricHullNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_SceneNode = Factory(name='Scene',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Scene',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='SceneNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_Texture2DTransformationNode = Factory(name='Texture2DTransformation',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Texture2DTransformation',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='Texture2DTransformationNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ExtrudedHullNode = Factory(name='ExtrudedHull',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a ExtrudedHull',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ExtrudedHullNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_BezierCurve2DNode = Factory(name='BezierCurve2D',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a BezierCurve2D',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='BezierCurve2DNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_GroupNode = Factory(name='Group',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Group',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='GroupNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_QuadSetNode = Factory(name='QuadSet',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a QuadSet',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='QuadSetNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_QuantisedFunctionNode = Factory(name='QuantisedFunction',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a QuantisedFunction',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='QuantisedFunctionNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_AxisRotatedNode = Factory(name='AxisRotated',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a AxisRotated',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='AxisRotatedNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_Color4Node = Factory(name='Color4',
                authors='Thomas Cokelaer',
                description='',
                category='Unclassified',
                nodemodule='objectgenerator',
                nodeclass='Color4Node',
                inputs=[{'interface': ISequence, 'name': 'IN1', 'value': [150, 0, 0, 155], 'desc': ''}],
                outputs=[{'interface': None, 'name': 'OUT1', 'desc': ''}],
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ImageTextureNode = Factory(name='ImageTexture',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a ImageTexture',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ImageTextureNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_PolylineNode = Factory(name='Polyline',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Polyline',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='PolylineNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_TriangleSetNode = Factory(name='TriangleSet',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a TriangleSet',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='TriangleSetNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ScaledNode = Factory(name='Scaled',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Scaled',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ScaledNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_Polyline2DNode = Factory(name='Polyline2D',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Polyline2D',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='Polyline2DNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_NurbsCurveNode = Factory(name='NurbsCurve',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a NurbsCurve',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='NurbsCurveNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_OrientedNode = Factory(name='Oriented',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Oriented',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='OrientedNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ExtrusionNode = Factory(name='Extrusion',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Extrusion',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ExtrusionNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_MonoSpectralNode = Factory(name='MonoSpectral',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a MonoSpectral',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='MonoSpectralNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_CylinderNode = Factory(name='Cylinder',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Cylinder',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='CylinderNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ConeNode = Factory(name='Cone',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Cone',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ConeNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_FrustumNode = Factory(name='Frustum',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Frustum',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='FrustumNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_PointSetNode = Factory(name='PointSet',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a PointSet',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='PointSetNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_FaceSetNode = Factory(name='FaceSet',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a FaceSet',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='FaceSetNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_RevolutionNode = Factory(name='Revolution',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Revolution',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='RevolutionNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_SwungNode = Factory(name='Swung',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Swung',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='SwungNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_Texture2DNode = Factory(name='Texture2D',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Texture2D',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='Texture2DNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ParaboloidNode = Factory(name='Paraboloid',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Paraboloid',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ParaboloidNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_BezierCurveNode = Factory(name='BezierCurve',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a BezierCurve',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='BezierCurveNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_MultiSpectralNode = Factory(name='MultiSpectral',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a MultiSpectral',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='MultiSpectralNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_NurbsPatchNode = Factory(name='NurbsPatch',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a NurbsPatch',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='NurbsPatchNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_PointSet2DNode = Factory(name='PointSet2D',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a PointSet2D',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='PointSet2DNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_NurbsCurve2DNode = Factory(name='NurbsCurve2D',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a NurbsCurve2D',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='NurbsCurve2DNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_DiscNode = Factory(name='Disc',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Disc',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='DiscNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_TaperedNode = Factory(name='Tapered',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Tapered',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='TaperedNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_AmapSymbolNode = Factory(name='AmapSymbol',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a AmapSymbol',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='AmapSymbolNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ShapeNode = Factory(name='Shape',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Shape',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ShapeNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_TranslatedNode = Factory(name='Translated',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Translated',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='TranslatedNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_ElevationGridNode = Factory(name='ElevationGrid',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a ElevationGrid',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='ElevationGridNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_SphereNode = Factory(name='Sphere',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a Sphere',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='SphereNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




objectgenerator_IFSNode = Factory(name='IFS',
                authors='F. Boudon, C. Pradal (wralea authors)',
                description='Create a IFS',
                category='scene.PGL Object Generator',
                nodemodule='objectgenerator',
                nodeclass='IFSNode',
                inputs=None,
                outputs=None,
                widgetmodule=None,
                widgetclass=None,
               )




