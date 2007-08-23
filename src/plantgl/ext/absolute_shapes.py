#!/usr/bin/env python
"""Absolute shapes implementation.

Absolute shapes (primary PlantGL4D:PlantGL for dummies) is an intermidiete
layer to connect miserable human factor to the intergalatic concepts of PlantGL.
The main idea is to help operating on PlantGL object in Cartesian 3D system. 
The most important is a AShape3D class which is a container for 3 basic transformation
which allow to express the most important object transformations.

:version: czw mar 22 14:36:00 CET 2007

:author:  szymon stoma, frederic boudon, david da silva
"""

import openalea.plantgl.all as pgl
import material

__docformat__="restructuredtext en"


ASHAPE3D_STANDARD_AXIS = pgl.Vector3.OZ
ASHAPE3D_STANDARD_ROLL = 0.
ASHAPE3D_STANDARD_POS = pgl.Vector3.ORIGIN
ASHAPE3D_STANDARD_MATERIAL = material.white
ASHAPE3D_STANDARD_SCALE = pgl.Vector3(1, 1, 1)

# save memory with primitive types related
ACYLINDER_PRIMITIVE = pgl.Cylinder()
ACYLINDER_PRIMITIVE.name = "ACylinderPrimitive"
ACYLINDER_STANDARD_RADIUS = 1.
ACYLINDER_STANDARD_HEIGHT=1.
ACYLINDER_STANDARD_AXIS = ASHAPE3D_STANDARD_AXIS

ASPHERE_PRIMITIVE = pgl.Sphere()
ASPHERE_PRIMITIVE.name = "ASpherePrimitive"
ASPHERE_STANDARD_RADIUS = 1.

AARROW_STANDARD_AXIS = ASHAPE3D_STANDARD_AXIS
AARROW_STANDARD_RADIUS =  1.
AARROW_STANDARD_HEIGHT= 1.
AARROW_SHAFT_PROPORTION = 0.7
AARROW_HEAD_PROPORTION = 1.5

ACONE_PRIMITIVE = pgl.Cone()
ACONE_PRIMITIVE.name = "AConePrimitive"


class AShape3D( object ):
    """Basic shape used to wrap all <2 axis objects.
    
    Internally all the visual object are characterised by 3 transformation components:
    
        * scaling,
        * rotation,
        * translation.
        
    The order of applying these operations:  scaling, rotation, translation. This component is enough to express the position
    of many one-axis symetry objects. For more complex objects, the manual ingeration into component might be required
    (or different strategy should be developed).
    
    Important properties: 
        * pos : Vector3 convertable
            Property: pos of the object (look below),
        * axis : Vector3 convertable
            Property: main axis of the object, should be defined to describe the rotation. The Z of the primitive geometry
            would point to axis,
        * roll : Real
            Property: rotation of the object around main axis,
        * scale :  Vector3 convertable
            Property: used while resizing the object. While resizing the Z corresponds to main axis of the object,
        * material : pgl.Material
            Property: describes the appearance of the object,
        * geometry : pgl.Geometry
            describes the geometry of the object.
            
        The concept of pos property: pos attribute for cylinder, arrow, cone, pyramid, etc. corresponds to one end of the object,
        whereas for a box, sphere, ring, etc. it corresponds to the center of the object.
        
        
    """
    def __init__( self, pos=ASHAPE3D_STANDARD_POS,  axis=ASHAPE3D_STANDARD_AXIS, roll=ASHAPE3D_STANDARD_ROLL,
                 scale=ASHAPE3D_STANDARD_SCALE, material=ASHAPE3D_STANDARD_MATERIAL, geometry=None, **keys ):
        """Default constructor.
        
        Parameters:
            pos : Vector3 convertable
                pos of the object (look below),
            axis : Vector3 convertable
                main axis of the object, should be defined to describe the rotation. The Z of the primitive geometry
                would point to axis,
            roll : Real
                Property: rotation of the object around main axis,
            scale :  Vector3 convertable
                to use while resizing the object. While scaling the Z corresponds to main axis of the object,
            material : pgl.Material
                describes the appearance of the object,
            geometry : pgl.Geometry
                describes the geometry of the object.
        """
        if not geometry:
            raise Exception( "AShape3D: geometry not defined." )
        self.geometry = geometry
        #TODO check for custom rotation, scale, transformation objects (shared between shapes)
        self.scaled = pgl.Scaled( scale, self.geometry )

        # roll related        
        self._roll = roll #: to keep internal roll
        self.rolled = pgl.AxisRotated(pgl.Vector3.OZ, self._roll, self.scaled)

        # axis related (even the object which do not have intuitive axis need to have predefined axis
        self._axis = axis #: to keep internal axis vector
        rotation_axis = pgl.cross( pgl.Vector3.OZ, self._axis )
        rotation_angle = pgl.angle( self._axis, pgl.Vector3.OZ )
        self.rotated = pgl.AxisRotated( rotation_axis, rotation_angle, self.rolled )
        
        # position related
        self.translated = pgl.Translated( pos, self.rotated )
        
        # apperance related
        self.shape = pgl.Shape( self.translated, material )
            

    def _set_pos( self, pos ):
        """Property helper method.
        
        """
        if pos != self.translated.translation:
            self.translated.translation = pos
    def _get_pos( self, pos ):
        """Property helper method.
        """
        return self.translated.translation
    pos = property( fget=_get_pos, fset=_set_pos, doc="Position of the object is used to make default translate." )
    """Property describing the position of the object. Its' meaning depands on the type of the object.
    Pos attribute for cylinder, arrow, cone, pyramid, etc. corresponds to one end of the object,
    whereas for a box, sphere, ring, etc. it corresponds to the center of the object.
    """
    
    def _set_axis( self, axis ):
        """Property helper method.
        """
        if axis != self._axis:
            self._axis = axis
            rotation_axis = pgl.cross( pgl.Vector3.OZ, self._axis )
            rotation_angle = pgl.angle( self._axis, pgl.Vector3.OZ )
            self.rotated.axis = rotation_axis
            self.rotated.angle = rotation_angle
    def _get_axis( self ):
        """Property helper method.
        """
        return self._axis
    axis = property( fget=_get_axis, fset=_set_axis, doc="""Property describing the main
                    axis for the object. Its' meaning depands on the type of the object. It is important
                    to rotation operations. For some object is defined to reach  generalisation
                    (eg. sphere).""" )

    def _set_roll( self, roll ):
        """Property helper method.
        """
        if roll != self._roll:
            self._roll = roll
            self.rolled.angle = self._roll
    def _get_roll( self ):
        """Property helper method.
        """
        return self._roll
    roll = property( fget=_get_roll, fset=_set_roll, doc="""Property describing the main axis for
                    the object. Its' meaning depands on the type of the object. It is important
                    to rotation operations. For some object is defined to reach  generalisation (eg. sphere).
                    """ )
    

    
    def _set_scale( self, scale ):
        """Property helper method.
        """
        if scale != self.scaled.scale:
            self.scaled.scale = scale
    def _get_scale( self ):
        """Property helper method.
        """
        return self.scaled.scale
    scale = property( fget=_get_scale, fset=_set_scale, doc="""Property used  to make
                     default scale operations.""" )
    
    def _set_material( self, material=material.white):
        """Property helper method.
        """
        self.shape.appearance= material
    def _get_material( self ):
        """Property helper method.
        """
        return self.shape.appearance
    material = property( fget=_get_material, fset=_set_material, doc="""Material of the object
                        is used to add appearence properites. Exact copy from PlantGL material.""" )

class ASphere( AShape3D ):
    """ASphere implementation.
    
    ASphere individual properties:        
        * pos : Vector3 convertable
            Property: the point with equal distance from every point on the sphere surface
        * radius : Real
            Property: the distance between the center and every point on the sphere surface
        
    It also inherits properties from AShape3D object, hance look for possible properties in
    AShape3D object.
    """
    def __init__( self,  radius=ASPHERE_STANDARD_RADIUS, **keys ):
        """Default constructior.
        
        Parameters:
            pos : Vector3 convertable
                the point with equal distance from every point on the sphere surface
            radius : Real
                the distance between the center and every point on the sphere surface
        
        """
        keys.update({ "radius": radius })
        self._common_init( **keys )
        AShape3D.__init__( self, scale=pgl.Vector3(2*radius,2*radius,2*radius), geometry=ASPHERE_PRIMITIVE, **keys )
 
    def _common_init( self, **keys ):
        self._radius = keys[ "radius" ]
        
    def _set_radius( self, radius=ASPHERE_STANDARD_RADIUS):
        """Property helper method.
        """
        if radius != self._radius:
            self.scale = pgl.Vector3( 2*radius, 2*radius, 2*radius )
            self._radius = radius
    def _get_radius( self):
        """Property helper method.
        """
        return self._radius
    radius = property( fget=_get_radius, fset=_set_radius, doc="Radius of the sphere." )
    """Radius of the sphere."""
    
    # as fred remarked the axis has some meaning for sphere: assuming that the sphere
    # contains the appearence it is important when talking eg. about textures
    #def _set_axis( self, axis=1.):
    #    """Property helper method.
    #    """
    #    pass
    #def _get_axis( self):
    #    """Property helper method.
    #    """
    #    return self._axis
    #axis = property( fget=_get_axis, fset=_set_axis, doc="Axis of the sphere (dummy, no change on sphere)." )
    #"""Axis of the sphere (dummy, no change on sphere)."""
    
class ACylinder( AShape3D ):
    """ACylinder implementation.
    
    ACylinders' individual properties:
        * pos : Vector3 convertable
             Property: the center of the lower basis,
        * axis : Vector3 convertable
            Property: the vector which is a symetry axis of the cylinder. If no height is given the height of the cylinder is the axis magnitude.
        * height : Real
            Property: the height of cylinder,
        * radius : Real
            Property: the radius of the lower basis

    It also inherits properties from AShape3D object, hance look for possible properties in
    AShape3D object.
    """
    
    def __init__( self,  radius=ACYLINDER_STANDARD_RADIUS, axis=ACYLINDER_STANDARD_AXIS, **keys ):
        """ Default constructor.
        
        Parameters:
            pos : Vector3 convertable
                the center of the lower basis,
            axis : Vector3 convertable
                the vector which is a symetry axis of the cylinder. If no height is given the height of the cylinder is the axis magnitude.
            height : Real
                the height of cylinder,
            radius : Real
                the radius of the lower basis

        """
        keys.update( {"radius": radius, "axis": axis} )
        self._common_init( **keys )
        AShape3D.__init__( self,  scale=pgl.Vector3(2*radius,2*radius,self._height), geometry=ACYLINDER_PRIMITIVE,  **keys )

    def _common_init( self, **keys ):
        """
        """
        if keys.has_key("height"):
            self._height = keys[ "height" ]
        else:
            self._height = pgl.norm( keys[ "axis" ] )
        self._radius = keys[ "radius" ]
        
    
    def _set_radius( self, radius=ACYLINDER_STANDARD_RADIUS):
        """Property helper method.
        """
        if radius != self._radius:
            self._radius = radius
            self.scale = pgl.Vector3( 2*self._radius, 2*self._radius, self._height )
    def _get_radius( self):
        """Property helper method.
        """
        return self._radius
    radius = property( fget=_get_radius, fset=_set_radius, doc="Radius of the cylinder." )
    """Radius of the cylinder."""

    def _set_height( self, height=ACYLINDER_STANDARD_HEIGHT):
        """Property helper method.
        """
        #TODO think about norm(self._axis) height relation. Shouldn't it be updated? 
        if height != self._height:
            self._height = height
            self.scale = pgl.Vector3( 2*self.radius, 2*self.radius, self._height )
    def _get_height( self):
        """Property helper method.
        """
        return self._height
    height = property( fget=_get_height, fset=_set_height, doc="Height of the cylinder." )
    """Height of the cylinder."""


class AArrow( ACylinder ):
    """AArrow implementation. AArrow is a shape build up from the cylinder and cone.
    
    AArrows' individual properties:
        * pos : Vector3 convertable
             Property: the center of the lower basis,
        * axis : Vector3 convertable
            Property: the vector which is a symetry axis of the arrow. If no height is given the height of the cylinder is the axis magnitude.
        * height : Real
            Property: the height of an arrow,
        * radius : Real
            Property: the radius of the lower basis

    It also inherits properties from AShape3D object, hance look for possible properties in
    AShape3D object.
    """
    
    def __init__( self,  radius=AARROW_STANDARD_RADIUS, axis=AARROW_STANDARD_AXIS, **keys ):
        """ Default constructor.
        
        Parameters:
            pos : Vector3 convertable
                the center of the lower basis,
            axis : Vector3 convertable
                the vector which is a symetry axis of an arrow. If no height is given the height of an arrow is the axis magnitude.
            height : Real
                the height of an arrow,
            radius : Real
                the radius of the lower basis

        """
        keys.update( {"radius": radius, "axis": axis} )
        self._common_init( **keys )
        AShape3D.__init__( self,  scale=pgl.Vector3(2*self.radius,2*self.radius,self.height),
                         geometry=pgl.Group([self.shaft,self.head]), **keys )

    def _common_init( self, **keys ):
        """
        """
        if keys.has_key("height"):
            self._height = keys[ "height" ]
        else:
            self._height = pgl.norm( keys[ "axis" ] )
        self._radius = keys[ "radius" ]
        
        self.shaft = pgl.Scaled(pgl.Vector3(1, 1, AARROW_SHAFT_PROPORTION), ACYLINDER_PRIMITIVE )
        self.head = pgl.Translated(pgl.Vector3(0, 0, AARROW_SHAFT_PROPORTION), pgl.Scaled(pgl.Vector3(
            AARROW_HEAD_PROPORTION,AARROW_HEAD_PROPORTION,1-AARROW_SHAFT_PROPORTION), ACONE_PRIMITIVE) )
        

    radius = property( fget=ACylinder._get_radius, fset=ACylinder._set_radius, doc="Radius of an arrow." )
    """Radius of an arrow."""

    height = property( fget=ACylinder._get_height, fset=ACylinder._set_height, doc="Height of an arrow." )
    """Height of an arrow."""


class ACenterPolygon( AShape3D ):
    """ACenterPolygon implementation. ACenterPolygon is a wrapper for a FaceSet. Currently no
    useful operations to scale, rotate, roll, translate were redefined so it is more container for
    PlantGL FaceSet object. The default ones are still defined but they are not very usfull. The polygon
    is presented as a set of triangles. One vertex of each triangle is a baricenter of all points positions.
    the two others vertices are taken from ordered list of points obtained from init.
    
    ACenterPolygons' individual properties:
        * pos : Vector3 convertable
             Property: the shared translation T of each point creating a surface,
        * axis : Vector3 convertable
            Property:  the shared rotation axis A of each point around. Zero element for the rotation axis is OZ.
        * roll : Vector3 convertable
            Property:  the shared "roll angle" R of each point creating a surface around axis,
        * points : Vector3 convertable tuple
            Property: the tuple of points positions,

    ACenterPolygons' individual properties:        
        * update_k_point : index
            Updates the point in the polygon
        * get_center : Vector3
            Returns the center of ACenterPolygon

    It also inherits properties from AShape3D object, hance look for possible properties in
    AShape3D object.
    """
    
    def __init__( self,  **keys ):
        """ Default constructor.
        
        Parameters:
            pos : Vector3 convertable
                the shared translation T of each point creating a surface,
            axis : Vector3 convertable
                the shared rotation axis A of each point around. Zero element for the rotation axis is OZ.
            roll : Vector3 convertable
                the shared "roll angle" R of each point creating a surface around axis,
            points : Vector3 convertable tuple
                the list of *ordered* points positions,

        """
        self._common_init( **keys )
        AShape3D.__init__( self,  
                         geometry=pgl.FaceSet( self._points, self._indexes ), axis=axis, **keys )

    def _common_init( self, **keys ):
        """
        """
        if len( keys[ "points" ] ) < 3:
            raise Exception("ACenterPolygon: Too few points in..")
       
        self._indexes = []
        self._points = pgl.Point3Array( [pgl.Vector3()]+keys[ "points" ] )
        for i in xrange( 1, len(keys[ "points" ] ) ):
            self._points[ 0 ] += self._points[ i ]
            self._indexes.append( pgl.Index( [0,i, i+1] ) )
        self._points[ 0 ] += self._points[ len( keys[ "points" ] )  ]   
        self._indexes.append( pgl.Index( [0,len(keys[ "points" ] ), 1] ))
        self._points[ 0 ] = self._points[ 0 ]/len( keys[ "points" ] )
        self._indexes = pgl.IndexArray( self._indexes )
        
    

class ATriangle( AShape3D ):
    """ATriangle implementation. ATriangle is a wrapper for a FaceSet. Currently no
    useful operations to scale, rotate, roll, translate were redefined so it is more container for
    PlantGL FaceSet object. The default ones are still defined but they are not very usfull. 
    
    ATriangles' individual properties:
        * pos : Vector3 convertable
             Property: the shared translation T of each point creating a surface,
        * axis : Vector3 convertable
            Property:  the shared rotation axis A of each point around. Zero element for the rotation axis is OZ.
        * roll : Vector3 convertable
            Property:  the shared "roll angle" R of each point creating a surface around axis,
        * points : Vector3 convertable tuple
            Property: the tuple of points positions,

    ATriangles' individual properties:        
        * update_k_point : index
            Updates the point in the polygon
        * get_center : Vector3
            Returns the center of ACenterPolygon

    It also inherits properties from AShape3D object, hance look for possible properties in
    AShape3D object.
    """
    
    def __init__( self,  **keys ):
        """ Default constructor.
        
        Parameters:
            pos : Vector3 convertable
                the shared translation T of each point creating a surface,
            axis : Vector3 convertable
                the shared rotation axis A of each point around. Zero element for the rotation axis is OZ.
            roll : Vector3 convertable
                the shared "roll angle" R of each point creating a surface around axis,
            points : Vector3 convertable tuple
                the list of *ordered* points positions,

        """
        self._common_init( **keys )
        AShape3D.__init__( self,  
                         geometry=pgl.TriangleSet( self._points, self._indexes ), axis=axis, **keys )

    def _common_init( self, **keys ):
        """
        """
        if len( keys[ "points" ] ) != 3:
            raise Exception("ATriangle: triangle must be described by 3 points..")
       
        self._indexes = []
        self._points = pgl.Point3Array( keys[ "points" ] )
        for i in xrange( 1, len(keys[ "points" ] ) ):
            self._indexes.append( pgl.Index3( 0,1,2 ) )
        self._indexes = pgl.Index3Array( self._indexes )
            