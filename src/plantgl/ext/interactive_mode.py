#!/usr/bin/env python
"""Interactive shapes implementation.

This module integretes the ``absolute_shapes`` with the interactive mode
editing. It allows to instantly follow the object changes in the moment. It creates
a direct link from visual object to the PlantGL Viewer (when a property
is changed for the object  the Viewer is updated instantly by default). It works almost perfectly
in the interactive mode. When we change the object we instantly see the change.

Of coz' it is preliminary proposition.

**Adding multiple objects**: sometimes it is required to add many object to the scene. To avoid
the update after each addition the ``set_instant_update_visualisation_policy()`` could be used.
It allow to substitute the default behaviour of Viewer update:

    >>> 
    import openalea.plantgl.all as pgl
    import openalea.plantgl.plantgl4d as pd
    pd.set_instant_update_visualisation_policy( policy = False )
    for i in range(500):
        l.append(pd.ASphere( pos=pgl.Vector3(i,0,0), radius=0.1 )
    pd.set_instant_update_visualisation_policy( policy = True )
    pd.instant_update_viewer( )

**Multi scenes**: currently I do not know how to do it in general way. Anyway i prepared the SCENES
with CURRENT_SCENE to be able to extend it in the future(?).

**Deleting an object**: with the current policy to *free* the memory of the object it is required to:
    1. set its ``visible`` propertry to ``False``
    2. delete all references to the object using ``del``
    
    >>> 
    import openalea.plantgl.all as pgl
    import openalea.plantgl.plantgl4d as pd
    s1 = pd.ASphere( pos=pgl.Vector3(i,0,0), radius=0.1 )
    s1.visible = False #: the object is removed from the current scene
    s1.visible = True #: the object is reappearing in the current scene
    s1.visible = False #: the object is removed from the current scene
    del s1 # the memory used by s1 is free!! no futher access possible.
    
:version: czw mar 22 14:36:00 CET 2007

:author:  szymon stoma
"""

import openalea.plantgl.all as pgl
import material
import absolute_shapes

__docformat__="restructuredtext en"


# scene related
SCENES = []
SCENES.append( pgl.Scene() ) 
CURRENT_SCENE = 0

#visualisation update policy
INSTANT_UPDATE_VISUALISATION_POLICY=True

ASHAPE3D_STANDARD_VISIBILITY = True


def change_scene( scene=None,scn_index=None ):
    """Changes current scene
    
    <Long description of the function functionality.>
    
    :parameters:
        arg1 : `T`
            <Description of `arg1` meaning>
    :rtype: `T`
    :return: <Description of ``return_object`` meaning>
    :raise Exception: <Description of situation raising `Exception`>
    """
    if not scn_index: SCENES[ CURRENT_SCENE ]=scene
    else:  SCENES[ scn_index ]=scene
    
def clear_scene( scn_index=None ):
    """Clears scene.
    
    <Long description of the function functionality.>
    
    :parameters:
        arg1 : `T`
            <Description of `arg1` meaning>
    :rtype: `T`
    :return: <Description of ``return_object`` meaning>
    :raise Exception: <Description of situation raising `Exception`>
    """
    if not scn_index: SCENES[ CURRENT_SCENE ].clear()
    else:  SCENES[ scn_index ].clear()
    
def get_scene( scn_index=None ):
    """Gets scene.
    
    <Long description of the function functionality.>
    
    :parameters:
        arg1 : `T`
            <Description of `arg1` meaning>
    :rtype: `T`
    :return: <Description of ``return_object`` meaning>
    :raise Exception: <Description of situation raising `Exception`>
    """
    if not scn_index: return SCENES[ CURRENT_SCENE ]
    else:  return SCENES[ scn_index ]

def display_viewer( ):
    """Helper function used to make the Viewer visible whenever sth was changed and the Viewer was closed. 
    """
    pgl.Viewer.display( SCENES[ CURRENT_SCENE ] )
    pgl.Viewer.start()

def update_viewer( ):
    """Updates the viewer window iff policy allows it. It should be run every time the scene object has changed.
    Policy is described by variable INSTANT_UPDATE_VISUALISATION_POLICY.
    """
    if INSTANT_UPDATE_VISUALISATION_POLICY:
        pgl.Viewer.update()

def instant_update_viewer( ):
    """Updates the viewer window without any condition.
    """
    pgl.Viewer.update()


def set_instant_update_visualisation_policy( policy=True ):
    """ Sets the update visualisation policy. Check for semantics in the description of PlantGL4D.
    
    Parameters:
        policy : Bool
            True iff Viewer should be refreshed every time the scen is changed. False elsewhere.
    """
    global INSTANT_UPDATE_VISUALISATION_POLICY
    INSTANT_UPDATE_VISUALISATION_POLICY = policy


class AIShape3D( absolute_shapes.AShape3D ):
    """Basic shape used to wrap all <2 axis objects with binding to Viewer.
    
    Important properties: 
        * visible : Bool
            Property: True iff object is visible (look below),
        
        The concept of visible property: to temporary make the object invisible in the current scene set the visible to False.
        If you want to wipe the object from the memory: make it invisible  and then use del to remove reference to the object.

        eg. The concept of pos property: pos attribute for cylinder, arrow, cone, pyramid, etc. corresponds to one end of the object,
        whereas for a box, sphere, ring, etc. it corresponds to the center of the object. 
        
    """
    def __init__( self, visible=ASHAPE3D_STANDARD_VISIBILITY, **keys ):
        """Default constructor (look at AShape3D).
        
        Parameters:
            visible : Bool
                True iff object is visible (look below),
        """
        absolute_shapes.AShape3D.__init__( self, **keys )
        # visible related
        self._visible = False
        self.visible = visible
    
    def _set_visible( self, visible ):
        """Property helper method.
        """
        global SCENES
        if self._visible != visible:
            if self._visible == False:
                SCENES[ CURRENT_SCENE ] += self.shape
                if not pgl.Viewer.isRunning():
                    display_viewer()
            else:
                SCENES[ CURRENT_SCENE ].remove( self.shape )              
                #TODO think about refreshing the viewer
            update_viewer()
            self._visible = visible
    def _get_visible( self ):
        """Property helper method.
        """
        return self._visible        
    visible = property( fget=_get_visible, fset=_set_visible, doc="True iff shape is currently displayed." )
    """Property describing whether the object is currently visible in the current Scene. If object is not present
    in current scene nothing is happanning.
    """

    def _set_pos( self, pos ):
        """Property helper method.
        
        """
        absolute_shapes.AShape3D._set_pos( self, pos )
        if self.visible:
            update_viewer()            
    pos = property( fget=absolute_shapes.AShape3D._get_pos, fset=_set_pos, doc="Position of the object is used to make default translate." )
    """Property describing the position of the object. Its' meaning depands on the type of the object.
    Pos attribute for cylinder, arrow, cone, pyramid, etc. corresponds to one end of the object,
    whereas for a box, sphere, ring, etc. it corresponds to the center of the object.
    """
    
    def _set_axis( self, axis ):
        """Property helper method.
        """
        if axis != self._axis:
            absolute_shapes.AShape3D._set_axis( self, axis )
            if self.visible:
                update_viewer()            
    axis = property( fget=absolute_shapes.AShape3D._get_axis, fset=_set_axis, doc="Axis of the object is used to make default translate rotate operations." )
    """Property describing the main axis for the object. Its' meaning depands on the type of the object. It is important
    to rotation operations. For some object is defined to reach  generalisation (eg. sphere).
    """

    def _set_roll( self, roll ):
        """Property helper method.
        """
        if roll != self._roll:
            absolute_shapes.AShape3D._set_roll( self, roll )
            if self.visible:
                update_viewer()            
    roll = property( fget=absolute_shapes.AShape3D._get_roll, fset=_set_roll, doc="Axis of the object is used to make default translate rotate operations." )
    """Property describing the main axis for the object. Its' meaning depands on the type of the object. It is important
    to rotation operations. For some object is defined to reach  generalisation (eg. sphere).
    """

    
    def _set_scale( self, scale ):
        """Property helper method.
        """
        if scale != self.scaled.scale:
            absolute_shapes.AShape3D._set_scale( self, scale )
            if self.visible:
                update_viewer()
    scale = property( fget=absolute_shapes.AShape3D._get_scale, fset=_set_scale, doc="Scale of the object is used to make default scale operations." )
    
    def _set_material( self, material=material.white):
        """Property helper method.
        """
        absolute_shapes.AShape3D._set_material( self, material )
        if self.visible:
            update_viewer()            
    material = property( fget=absolute_shapes.AShape3D._get_material, fset=_set_material, doc="Material of the object is used to add appearence properites." )
    """Property describing the appearence of the object. Exact copy from PlantGL material.
    """


class AISphere( AIShape3D, absolute_shapes.ASphere ):
    """AISphere implementation with binding to Viewer.
            
    It also inherits properties from AIShape3D and ASphere object, hance look for possible properties in
    AShape3D, ASphere objects.
    """
    def __init__( self,  radius=absolute_shapes.ASPHERE_STANDARD_RADIUS, **keys ):
        """Default constructior.        
        """
        keys.update( {"radius":radius} ) 
        self._common_init( **keys )
        AIShape3D.__init__( self, scale=pgl.Vector3(2*self.radius,2*self.radius,2*self.radius), geometry=absolute_shapes.ASPHERE_PRIMITIVE, **keys )
        #self._radius = radius
        
    
    
class AICylinder( absolute_shapes.ACylinder, AIShape3D ):
    """AICylinder implementation with binding to Viewer.
    
    It also inherits properties from AIShape3D, ACylinder object, hance look for possible properties in
    AShape3D, ACylinder objects.
    """
    def __init__( self,  radius=absolute_shapes.ACYLINDER_STANDARD_RADIUS, axis=absolute_shapes.ACYLINDER_STANDARD_AXIS, **keys ):
        """ Default constructor.
        """
        #if keys.has_key("height"):
        #    self._height = keys[ "height" ]
        #else:
        #    self._height = pgl.norm( axis )
        #self._radius = radius
        keys.update( {"radius": radius, "axis": axis} )
        absolute_shapes.ACylinder._common_init( self, **keys  )
        AIShape3D.__init__( self,  scale=pgl.Vector3(2*self.radius,2*self.radius,self.height), geometry=absolute_shapes.ACYLINDER_PRIMITIVE,  **keys )

class AIArrow( absolute_shapes.AArrow, AIShape3D ):
    """AIArrow implementation with binding to Viewer. AArrow is a shape build up from the cylinder and cone.
    
    It also inherits properties from AShape3D, AArrow object, hance look for possible properties in
    AShape3D, AArrow objects.
    """
    def __init__( self,   radius=absolute_shapes.AARROW_STANDARD_RADIUS, axis=absolute_shapes.AARROW_STANDARD_AXIS, **keys ):
        """ Default constructor.
        """
        #if keys.has_key("height"):
        #    self._height = keys[ "height" ]
        #else:
        #    self._height = pgl.norm( axis )
        #self._radius = radius
        #
        #self.shaft = pgl.Scaled(pgl.Vector3(1, 1, AARROW_SHAFT_PROPORTION), ACYLINDER_PRIMITIVE )
        #self.head = pgl.Translated(pgl.Vector3(0, 0, AARROW_SHAFT_PROPORTION), pgl.Scaled(pgl.Vector3(
        #    AARROW_HEAD_PROPORTION,AARROW_HEAD_PROPORTION,1-AARROW_SHAFT_PROPORTION), ACONE_PRIMITIVE) )
        keys.update( {"radius": radius, "axis": axis} )
        self._common_init( **keys )
        AIShape3D.__init__( self,  scale=pgl.Vector3( 2*self.radius, 2*self.radius, self.height),
                         geometry=pgl.Group([self.shaft,self.head]),  **keys )


class AICenterPolygon( AIShape3D, absolute_shapes.ACenterPolygon ):
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
        absolute_shapes.ACenterPolygon._common_init( self, **keys )
        AIShape3D.__init__( self,  
                         geometry=pgl.FaceSet( self._points, self._indexes ),  **keys )
    
class AITriangle( AIShape3D, absolute_shapes.ATriangle ):
    """AITriangle implementation. AITriangle is a wrapper for a FaceSet. Currently no
    useful operations to scale, rotate, roll, translate were redefined so it is more container for
    PlantGL FaceSet object. The default ones are still defined but they are not very usfull.
    
    AITriangle' individual properties:
        * pos : Vector3 convertable
             Property: the shared translation T of each point creating a surface,
        * axis : Vector3 convertable
            Property:  the shared rotation axis A of each point around. Zero element for the rotation axis is OZ.
        * roll : Vector3 convertable
            Property:  the shared "roll angle" R of each point creating a surface around axis,
        * points : Vector3 convertable tuple
            Property: the tuple of points positions,

    AITriangle' individual properties:        
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
        absolute_shapes.ATriangle._common_init( self, **keys )
        AIShape3D.__init__( self,  
                         geometry=pgl.FaceSet( self._points, self._indexes ),  **keys )
    