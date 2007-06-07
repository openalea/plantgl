from openalea.core.external import *
from openalea import plantgl as plantgl
from openalea.plantgl import math, scenegraph, gui
from openalea.plantgl.scenegraph import *
import random

def load( fn ):
    return list(scenegraph.Scene(fn))

def sample( x_size= 20,y_size= 20, nb= 20 ):
    """ return a list of transformation """ 
    import random
    l=random.sample( xrange(x_size*y_size), nb )
    return ( math.Vector3(t%y_size,t/x_size, 0.) for t in l )

def regular_sample( nx= 4, ny= 5, space_x= 5, space_y= 4 ):
  return (math.Vector3( x*space_x, (x%2)*space_x/2.+y*space_y, 0) for x in range(nx) for y in range(ny))
  
def multiply( positions, group ):
    " return a scene by translated each group by a given position "
    # compute the bbox
    bbox= BoundingBox(group[0])
    for shape in group:
        bbox+= BoundingBox(shape)
    size= max(bbox.getSize()[0], bbox.getSize()[1])

    scene= Scene()
    z_axis= math.Vector3(0,0,1)
    for pos in positions:
        angle= random.uniform(0,random.TWOPI)
        [scene.add( Shape( Translated( pos*size, 
                                       AxisRotated( z_axis, 
                                                    angle, 
                                                    s.geometry ) ), 
                           s.appearance) ) for s in group ] 
    return scene

class ImportScene(Node):
    def __init__(self):
        Node.__init__(self)
        self.add_input( name = 'filename', interface = IFileStr )
        self.add_output( name = 'shapes', interface = None )

    def __call__( self, inputs ):
        fn = self.get_input("filename")
        if fn and fn.endswith('geom'):
            return (list( Scene(fn) ),)
        else:
            return (None,)

class RandomSample(Node):
    def __init__(self):
        Node.__init__(self)
        self.add_input( name = 'x_size', interface = IInt, value= 10 )
        self.add_input( name = 'y_size', interface = IInt, value= 10 )
        self.add_input( name = 'nb_element', 
                        interface = IInt, 
                        value= 10 )
        self.add_output( name = 'transformations', 
                         interface = ISequence )

    def __call__( self, inputs ):
        x_size= self.get_input( 'x_size' )
        y_size= self.get_input( 'y_size' )
        nb_tree= self.get_input( 'nb_element' )
        return (sample(x_size, y_size, nb_tree),)

class RegularSample(Node):
    def __init__(self):
        Node.__init__(self)
        self.add_input( name = 'nb_rows', interface = IInt, value= 5 )
        self.add_input( name = 'nb_cols', interface = IInt, value= 5 )
        self.add_input( name = 'row_size',
                        interface = IInt, 
                        value= 5 )
        self.add_input( name = 'col_size',
                        interface = IInt, 
                        value= 5 )
        self.add_output( name = 'transformations', 
                         interface = ISequence )

    def __call__( self, inputs ):
        nx= self.get_input( 'nb_rows' )
        ny= self.get_input( 'nb_cols' )
        size_x= self.get_input( 'row_size' )
        size_y= self.get_input( 'col_size' )
        return (regular_sample(nx,ny,size_x, size_y),)

class Planter(Node):
    def __init__(self):
        Node.__init__(self)
        self.add_input( name = 'transformations', 
                        interface = ISequence )
        self.add_input( name = 'shapes', 
                        interface = None )

        self.add_output( name = 'scene', 
                         interface = None )

    def __call__( self, inputs ):
        transfos= self.get_input( 'transformations' )
        shapes= self.get_input( 'shapes' )
        if not (shapes and transfos):
            return ( None, )
        if not hasattr(shapes, "__iter__"):
            shapes= [shapes]
        return ( multiply( transfos, shapes ), )

