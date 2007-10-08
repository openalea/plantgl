from openalea.core.external import *
from openalea.plantgl.gui import *
from openalea.plantgl.scenegraph import *


class Plot3D(Node):
    """
    Display PlantGL Viewer
    """

    def __init__(self):

        Node.__init__(self)
        self.add_input( name='object', interface=None )

    def __call__(self, inputs):
        """ inputs is the list of input values """
        obj = inputs[0]
        if isinstance( obj, Shape ):
            sc= Scene()
            sc.add(obj)
            obj= sc

        Viewer.display(obj)
