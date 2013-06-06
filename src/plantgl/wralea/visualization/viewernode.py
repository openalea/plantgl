from openalea.core.external import *
from openalea.plantgl.gui import *
from openalea.plantgl.scenegraph import *

class PlantGLPlotter(object):
    def plot(self,scene):
        Viewer.display(scene)
    def selection(self):
        return Viewer.selection
    def waitSelection(self,txt):
        return Viewer.waitSelection(txt)
    def save(self,fname,format):
        Viewer.frameGL.saveImage(fname,format)

plotter = PlantGLPlotter()

def registerPlotter(viewer):
    global plotter
    plotter = viewer

def get_plotter():
    return plotter

def Plot3D(obj):
    if isinstance( obj, Shape ):
        sc= Scene()
        sc.add(obj)
        obj= sc
    get_plotter().plot(obj)
    return obj,


#X class Plot3D(Node):
#X     """
#X     Display in PlantGL Viewer
#X     """
#X 
#X     def __init__(self):
#X 
#X         Node.__init__(self)
#X         self.add_input( name='object', interface=None )
#X         self.add_output( name='object', interface=None )
#X 
#X     def __call__(self, inputs):
#X         """ inputs is the list of input values """
#X         obj = inputs[0]
#X         if isinstance( obj, Shape ):
#X             sc= Scene()
#X             sc.add(obj)
#X             obj= sc
#X         get_plotter().plot(obj)
#X         return obj
#X 
class AddToPlot3D(Node):
    """
    Add to current display in PlantGL Viewer
    """

    def __init__(self):

        Node.__init__(self)
        self.add_input( name='object', interface=None )
        self.add_output( name='object', interface=None )

    def __call__(self, inputs):
        """ inputs is the list of input values """
        obj = inputs[0]
        if isinstance( obj, Shape ):
            sc= Scene()
            sc.add(obj)
            obj= sc

        get_plotter().plot(obj)
        return inputs[0]
