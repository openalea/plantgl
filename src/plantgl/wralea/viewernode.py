from openalea.core.external import *
from openalea.plantgl.gui import *
 
class ViewerStart(Node):
	"""
	Start PlantGL Viewer
	"""
	
	def __init__(self):
		
		Node.__init__(self)
		
		self.add_output( name = "Y", interface = None) 
	 
	def __call__(self, inputs):
		""" inputs is the list of input values """
		Viewer.threaded = False
		Viewer.start()
		return (True,)

class Plot3D(Node):
	"""
	Display PlantGL Viewer
	"""
	
	def __init__(self):
		
		Node.__init__(self)
		self.add_input( name='object', interface=None )
	 
	def __call__(self, inputs):
		""" inputs is the list of input values """
		sc = self.get_input_by_key( 'object' )
		Viewer.display(sc)
