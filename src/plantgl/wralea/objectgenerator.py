from openalea.core.external import *
from vplants.plantgl import math as mt
from vplants.plantgl import scenegraph as sg
 

def toColor3(val):
	return sg.Color3(val[0],val[1],val[2])

def toVector3(val):
	return mt.Vector3(val[0],val[1],val[2])

class Material(Node):
	"""
	PGL Oject Generator
	"""
	
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
		return ( sg.Material(ambient = toColor3(self.get_input_by_key( 'ambient' )),
					    diffuse = self.get_input_by_key( 'diffuse' ),
					    emission = toColor3(self.get_input_by_key( 'emission' )),
					    specular = toColor3(self.get_input_by_key( 'specular' )),
					    shininess = self.get_input_by_key( 'shininess' ),
					    transparency = self.get_input_by_key( 'transparency' )) ,)


class Shape(Node):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		
		Node.__init__(self)
		
		self.add_input( name='geometry', interface=None)
		self.add_input( name='appearance', interface=None )
		self.add_input( name='id', interface=IInt(min=0), value=0 )
		self.add_output( name = "shape", interface = None) 
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Shape(self.get_input_by_key( 'geometry' ),
					    self.get_input_by_key( 'appearance' ),
					    self.get_input_by_key( 'id' )),)


class Scene(Node):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		
		Node.__init__(self)
		
		self.add_output( name = "scene", interface = None) 
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Scene(),)


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
		sc = self.get_input_by_key( 'scene' )
		sc.add(self.get_input_by_key( 'shape' ))
		return (sc,)


class SOR(Node):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		Node.__init__(self)
		self.add_input( name='slices', interface=IInt, value=16 )
		self.add_output( name = "geometry", interface = None) 

class Cone(SOR):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		SOR.__init__(self)
		self.add_input( name='height', interface=IFloat, value=1 )
		self.add_input( name='radius', interface=IFloat, value=0.5 )
		self.add_input( name='solid', interface=IBool, value=False )
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Cone( height = self.get_input_by_key( 'height' ),
					    radius = self.get_input_by_key( 'radius' ),
					    solid = self.get_input_by_key( 'solid' ),
					    slices = self.get_input_by_key( 'slices' )),)

class Cylinder(Cone):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		Cone.__init__(self)
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Cylinder( height = self.get_input_by_key( 'height' ),
					    radius = self.get_input_by_key( 'radius' ),
					    solid = self.get_input_by_key( 'solid' ),
					    slices = self.get_input_by_key( 'slices' )),)

class Frustum(Cone):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		Cone.__init__(self)
		self.add_input( name='taper', interface=IFloat, value=0.5 )
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Frustum( height = self.get_input_by_key( 'height' ),
					    taper = self.get_input_by_key( 'taper' ),
					    radius = self.get_input_by_key( 'radius' ),
					    solid = self.get_input_by_key( 'solid' ),
					    slices = self.get_input_by_key( 'slices' )),)

class Paraboloid(Cone):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		Cone.__init__(self)
		self.add_input( name='shape', interface=IFloat, value=2.0 )
		self.add_input( name='stacks', interface=IFloat, value=8 )
	
	def __call__(self, inputs):
		""" inputs is the list of input values """		
		return (sg.Paraboloid( height = self.get_input_by_key( 'height' ),
					    shape = self.get_input_by_key( 'shape' ),
					    radius = self.get_input_by_key( 'radius' ),
					    solid = self.get_input_by_key( 'solid' ),
					    slices = self.get_input_by_key( 'slices' ),
					    stacks = self.get_input_by_key( 'stacks' )),)

class Translated(Node):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		Node.__init__(self)
		self.add_input( name='translation', interface=ITuple3, value=(0,0,0) )
		self.add_input( name='geometry', interface=None )
		self.add_output( name='geometry', interface=None)
	
	def __call__(self, inputs):
		""" inputs is the list of input values """	
		geometry = 	self.get_input_by_key( 'geometry' )
		return (sg.Translated( toVector3(self.get_input_by_key( 'translation' )), geometry),)

class EulerRotated(Node):
	"""
	PGL Oject Generator
	"""
	
	def __init__(self):
		Node.__init__(self)
		self.add_input( name='azimuth', interface=IFloat, value=0. )
		self.add_input( name='elevation', interface=IFloat, value=0. )
		self.add_input( name='roll', interface=IFloat, value=0. )
		self.add_input( name='geometry', interface=None )
		self.add_output( name='geometry', interface=None)
	
	def __call__(self, inputs):
		""" inputs is the list of input values """	
		geometry = 	self.get_input_by_key( 'geometry' )
		return (sg.EulerRotated( self.get_input_by_key( 'azimuth' ),
								 self.get_input_by_key( 'elevation' ),
								 self.get_input_by_key( 'roll' ), geometry),)
