from openalea.core.external import *
from openalea.plantgl import math as mt
from openalea.plantgl import scenegraph as sg
 

def toColor3(val):
	return sg.Color3(val[0],val[1],val[2])

def toVector3(val):
	return mt.Vector3(val[0],val[1],val[2])

class Material(Node):
        sg.Material.__doc__
	
	def __init__(self):
		
		Node.__init__(self)
		
		self.add_input( name='ambient', interface=IRGBColor, value=(160,160,160) )
		self.add_input( name='diffuse', interface=IFloat,  value=1 )
		self.add_input( name='emission', interface=IRGBColor, value=(160,160,160) )
		self.add_input( name='specular', interface=IRGBColor, value=(160,160,160) )
		self.add_input( name='shininess', interface=IFloat,  value=0 )
		self.add_input( name='transparency', interface=IFloat,  value=0 )
		self.add_output( name = "geometry", interface = None) 
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return ( sg.Material(ambient = toColor3(self.get_input( 'ambient' )),
					    diffuse = self.get_input( 'diffuse' ),
					    emission = toColor3(self.get_input( 'emission' )),
					    specular = toColor3(self.get_input( 'specular' )),
					    shininess = self.get_input( 'shininess' ),
					    transparency = self.get_input( 'transparency' )) ,)


class Shape(Node):
    sg.Material.__doc__

    def __init__(self):

        Node.__init__(self)
        self.add_input( name='geometry', interface=None)
        self.add_input( name='appearance', interface=None )
        self.add_input( name='id', interface=IInt(min=0), value=0 )
        self.add_output( name = "shape", interface = None) 

    def __call__(self, inputs):
        """ inputs is the list of input values """		
        geometry= self.get_input( 'geometry' )
        appearance= self.get_input( 'appearance' )
        id= self.get_input( 'id' )

        if (not geometry) or (not appearance):
            return

        return ( sg.Shape(geometry, appearance, id) ,)


class Scene(Node):
    sg.Scene.__doc__

    def __init__(self):

        Node.__init__(self)

        self.add_input( name = "objects", interface = None ) 
        self.add_output( name = "scene", interface = None) 

    def __call__(self, inputs):
        """ inputs is the list of input values """		
        scene= inputs[0]
        if scene:
            return (sg.Scene(scene),)
        else:
            return sg.Scene()


class AddToScene(Node):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		
		Node.__init__(self)
		
		self.add_input( name = "scene", interface = None) 
		self.add_input( name = "shape", interface = None) 
		self.add_output( name = "scene", interface = None) 
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		sc = self.get_input( 'scene' )
		sc.add(self.get_input( 'shape' ))
		return (sc,)


class SOR(Node):
	
	def __init__(self):
		Node.__init__(self)
		self.add_input( name='slices', interface=IInt, value=16 )
		self.add_output( name = "geometry", interface = None) 

class Cone(SOR):
        sg.Cone.__doc__
	
	def __init__(self):
		SOR.__init__(self)
		self.add_input( name='height', interface=IFloat, value=1 )
		self.add_input( name='radius', interface=IFloat, value=0.5 )
		self.add_input( name='solid', interface=IBool, value=False )
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Cone( height = self.get_input( 'height' ),
					    radius = self.get_input( 'radius' ),
					    solid = self.get_input( 'solid' ),
					    slices = self.get_input( 'slices' )),)

class Cylinder(Cone):
        sg.Cylinder.__doc__
	
	def __init__(self):
		Cone.__init__(self)
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Cylinder( height = self.get_input( 'height' ),
					    radius = self.get_input( 'radius' ),
					    solid = self.get_input( 'solid' ),
					    slices = self.get_input( 'slices' )),)

class Frustum(Cone):
        sg.Frustum.__doc__
	
	def __init__(self):
		Cone.__init__(self)
		self.add_input( name='taper', interface=IFloat, value=0.5 )
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Frustum( height = self.get_input( 'height' ),
					    taper = self.get_input( 'taper' ),
					    radius = self.get_input( 'radius' ),
					    solid = self.get_input( 'solid' ),
					    slices = self.get_input( 'slices' )),)

class Paraboloid(Cone):
        sg.Paraboloid.__doc__
	
	def __init__(self):
		Cone.__init__(self)
		self.add_input( name='shape', interface=IFloat, value=2.0 )
		self.add_input( name='stacks', interface=IFloat, value=8 )
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Paraboloid( height = self.get_input( 'height' ),
					    shape = self.get_input( 'shape' ),
					    radius = self.get_input( 'radius' ),
					    solid = self.get_input( 'solid' ),
					    slices = self.get_input( 'slices' ),
					    stacks = self.get_input( 'stacks' )),)

class Translated(Node):
    sg.Translated.__doc__

    def __init__(self):
        Node.__init__(self)
        self.add_input( name='x', interface=IFloat, value=0.)
        self.add_input( name='y', interface=IFloat, value=0.)
        self.add_input( name='z', interface=IFloat, value=0.)
        self.add_input( name='geometry', interface=None )
        self.add_output( name='translated', interface=None)

    def __call__(self, inputs):
        """ inputs is the list of input values """	
        t= mt.Vector3(*inputs[0:2])
        geometry = self.get_input( 'geometry' )
        return (sg.Translated( t, geometry),)

class EulerRotated(Node):
        sg.EulerRotated.__doc__

        def __init__(self):
            Node.__init__(self)
            self.add_input( name='azimuth', interface=IFloat, value=0. )
            self.add_input( name='elevation', interface=IFloat, value=0. )
            self.add_input( name='roll', interface=IFloat, value=0. )
            self.add_input( name='geometry', interface=None )
            self.add_output( name='geometry', interface=None)

        def __call__(self, inputs):
            """ inputs is the list of input values """	
            geometry = self.get_input( 'geometry' )

            if not geometry:
                return

            return (sg.EulerRotated( self.get_input( 'azimuth' ),
                                     self.get_input( 'elevation' ),
                                     self.get_input( 'roll' ), geometry),)

class Sphere(SOR):
    sg.Sphere.__doc__

    def __init__(self):
        SOR.__init__(self)
        self.add_input( name='radius', interface=IFloat, value=0.5 )
        self.add_input( name='stacks', interface=IInt, value=6 )

    def __call__(self, inputs):
        """ inputs is the list of input values """		
        return (sg.Sphere( radius = self.get_input( 'radius' ),
                           slices = self.get_input( 'slices' ),
                           stacks = self.get_input( 'stacks' ) ),)

