from openalea.plantgl.gui.qt import QtCore
from PyQGLViewer import *
from openalea.plantgl.all import *


from openalea.plantgl.gui.qt.QtCore import QObject,pyqtSignal

class Pos3Setter:
    def __init__(self,ctrlpointset,index):
        self.ctrlpointset = ctrlpointset
        self.index = index
    def __call__(self,pos):
        self.ctrlpointset[self.index] = pos

class Pos4Setter:
    def __init__(self,ctrlpointset,index):
        self.ctrlpointset = ctrlpointset
        self.index = index
    def __call__(self,pos):
        self.ctrlpointset[self.index] = Vector4(pos,1)
        

class CtrlPoint(ManipulatedFrame):
    """the class CtrlPoint  represent the control points of a NurbsPatch by PlantGL Spheres. 
        The user can grab them and move them dynamically. 
        Change of value is automatically propagate to the initial structure using the position_setter function """   

    translated = pyqtSignal("PyQt_PyObject","PyQt_PyObject")
    valueChanged = pyqtSignal(int) 

    def __init__(self, position, position_setter, color = (30,30,250),id = 0):
        """Constructor 
           :param position: initial position of the control point
           :param position_setter: makes it possible to propagate position change to initial structure
        """
        ManipulatedFrame.__init__(self)
        self.id = id
        
        # current position and method to propagate displacement on original structure
        self.setPosition(Vec(position[0],position[1],position[2]))
        self.position_setter = position_setter
        
        # point selection or focus
        self.selected = False
        self.hasFocus = False
        self.color = color
        
        # previous position to compute displacement
        self.previousPosition=self.position()
        
        # Constraint the direction of translation
        self.constraint = WorldConstraint()
        self.constraint.setRotationConstraintType(AxisPlaneConstraint.FORBIDDEN)
        self.setConstraint(self.constraint)
        
        # SIGNAL to propagate displacement
        self.modified.connect(self.__propagate_position_change__) 
        self.manipulated.connect(self.__emit_translation__) 
        
        # manipulated call back
        self.callback = None
        
    def setCallBack(self,callback):
        self.callback = callback
        
    def __push_position__(self):
        """ push current position status as previous position """
        self.previousPosition=self.position()

    def displacement(self):
        """ get displacement applied to the point """ 
        delta = self.position() - self.previousPosition 
        self.__push_position__()
        return delta
    
    def __propagate_position_change__(self):
        """ apply position change to initial structure """
        self.position_setter(self.position())
    
    def __emit_translation__(self):
        """ emit a SIGNAL when a translation has been applied to the point """
        pos = self.position()
        tr = pos - self.previousPosition
        self.translated.emit(self,tr) 
        self.valueChanged.emit(self.id) 
        if self.callback:
            self.callback(self.id)
     
    def apply_translation(self,tr):
        """ apply a displacement. USefull when point is part of selection and a displacement  occur on another point """
        self.setPosition(self.position()+tr)
        self.__push_position__()
        self.__propagate_position_change__()
    
    def representation(self, pointsize = 0.05) :
        """ Compute representation of the control points as PlantGL Spheres"""
        p=self.position()
        #we change the color of the point if it is selected or has focus
        if self.hasFocus:    m = Material((250,30,30),1) # red
        elif self.selected : m = Material((250,250,30),1) #yellow
        else:                m = Material(self.color,1)
        #the point is represented by a PlantGl Sphere
        primitive = pointsize  if isinstance(pointsize,Geometry)  else Sphere(pointsize)
        return Shape(Translated(Vector3(p.x,p.y,p.z),primitive),m,self.id)
    
    def getx(self):
        """ Get the x coordinate """
        return self.position().x
    def gety(self):
        """ Get the y coordinate """
        return self.position().y
    def getz(self):
        """ Get the z coordinate """
        return self.position().z
    def setx(self,x):
        """ Set the x coordinate """
        p = self.position()
        p.x = x
        self.setPosition(p)
    
    x  = property(getx,""" The x coordinate property """)
    y  = property(gety,""" The y coordinate property """)
    z  = property(getz,""" The z coordinate property """)

###########################################

class SelectionAxis (ManipulatedFrame):
    """ The selection Axis class. 
        We consider here that x, y and z axis are represented. 
        These axis are represented by plantGL polylines and spheres 
        and must be grabbable for constrained displacements
    """
    O,X,Y,Z = (Vec(0,0,0), Vec(1,0,0), Vec(0,1,0),  Vec(0,0,1)) # predefined axis direction
    AxisLength = 0.2  # length of axis representation

    translated = pyqtSignal('PyQt_PyObject')
    
    def __init__(self, parent, direction= Vec(), material =Material(), selectionMaterial = Material((0,0,0),1)):
        """Constructor 
        
        :param parent : the reference frame on which it is defined. In practice, it is a SelectionManipulator.
        :param direction: the direction of the axis.
        :param material: color of the representation of this axis
        :param selectionMaterial: color of the representation of this axis when it is selected
        """
        ManipulatedFrame.__init__(self)
        self.setReferenceFrame(parent)
        
        # selection
        self.hasFocus = False
        
        # direction and position
        self.direction = direction
        self.initpos = direction*SelectionAxis.AxisLength
        self.setTranslation(self.initpos)
        
        # display option
        self.material = material
        self.selectionMaterial = selectionMaterial
        
        #displacement computation
        self.previousPosition = Vec()
        
        # Constraint the direction of translation
        self.constraint = WorldConstraint()
        self.constraint.setRotationConstraintType(AxisPlaneConstraint.FORBIDDEN) 
        self.constraint.setTranslationConstraint(AxisPlaneConstraint.AXIS,self.direction)
        self.setConstraint(self.constraint)
        
        self.manipulated.connect(self.__emit_translation__) 
    
   
    def representation(self, pointsize = 0.05):
        """ Compute representation of the SelectionAxis as line and sphere. """
        p=self.position()
        p2 = p - self.direction * SelectionAxis.AxisLength
        if self.hasFocus : material = self.selectionMaterial
        else: material = self.material
        sh  =[Shape(Polyline([Vector3(*p2),Vector3(*p)]),material)]
        sh +=[Shape(Translated(Vector3(*p),Sphere(pointsize)),material)]
        return sh

    def __emit_translation__(self):
        """ emit a SIGNAL when a translation occur on this axis """
        tr = self.translation()
        self.setTranslation(self.initpos)
        self.translated.emit(tr-self.initpos) 
    
#############################################################

class SelectionManipulator (ManipulatedFrame):
    
    valueChanged = pyqtSignal()

    def __init__(self):
        ManipulatedFrame.__init__(self)
        # set of selected point
        self.selection = []
        # axis for axial displacement
        self.axis=[ SelectionAxis(self,SelectionAxis.X,material=Material((250,30,30),1)), 
                    SelectionAxis(self,SelectionAxis.Y,material=Material((30,250,30),1)), 
                    SelectionAxis(self,SelectionAxis.Z,material=Material((250,250,250),1))]
        
        # previous position to compute displacement
        self.previousPosition = Vec()        
        self.manipulated.connect(self.propagatePointTranslation) 
        for axis in self.axis:
            axis.translated.connect(self.propagateAxisTranslation) 
    
    def __push_position__(self):
        """ push current position status as previous position """
        self.previousPosition=self.position()

    def displacement(self):
        """ Gives current displacement of the frame """
        delta = self.position() - self.previousPosition 
        self.__push_position__()
        return delta
        
    def propagateAxisTranslation(self,displacement):
        """ Apply the displacement of the axis on the set of selected control points """
        self.setPosition(self.position()+displacement)
        self.__push_position__()
        for point in self.selection:
            point.apply_translation(displacement)
        self.valueChanged.emit() 

    
    def propagatePointTranslation(self):
        """ Apply the displacement of self on the set of selected control points """
        displacement = self.displacement()
        for point in self.selection:
            point.apply_translation(displacement)
        self.valueChanged.emit() 
    
    def clear(self):
        """ remove all control points """
        self.manipulated.disconnect(self.propagatePointTranslation)
        for ctrlpoint in self.selection:
            ctrlpoint.selected = False
            ctrlpoint.translated.disconnect(self.propagatePointTranslation)
        self.selection = None
            
    def add(self,ctrlpoint):
        """ add a ctrl point to the selection """
        self.selection.append(ctrlpoint)
        ctrlpoint.selected = True
        ctrlpoint.translated.connect(self.propagatePointTranslation) 
        self.__update__()
    
    def remove(self,ctrlpoint):
        """ remove a ctrl point to the selection """
        ctrlpoint.selected = False
        ctrlpoint.translated.disconnect(self.propagatePointTranslation) 
        del self.selection[self.selection.index(ctrlpoint)]
        self.__update__()
        
    def toogleSelection(self,ctrlpoint):
        """ add a ctrl point to the selection or remove it if already in it"""
        if ctrlpoint in self.selection:
            self.remove(ctrlpoint)
        else:
            self.add(ctrlpoint)
    
    def __update__(self):
        """ update the central point position when selection has changed """
        self.centralPoint = sum([i.position() for i in self.selection],Vec()) / len(self.selection)
        self.setPosition(self.centralPoint)
        self.__push_position__()
    
    def representation(self, pointsize = 0.05):
        """ define the representation of this manipulator """
        shapes = []
        shapes+=self.axis[0].representation(pointsize)
        shapes+=self.axis[1].representation(pointsize)
        shapes+=self.axis[2].representation(pointsize)    
        return shapes
    
    def empty(self):
        """ Tell whether the selection set is empty """
        return len(self.selection) == 0
    
    def checkIfAxisGrabsMouse(self,event,viewer):
        """ check whether an axis has been selected """
        for axis in self.axis:
            axis.checkIfGrabsMouse(event.x(),event.y(),viewer.camera())
            if axis.grabsMouse():
                viewer.setManipulatedFrame(axis)
                axis.hasFocus = True
                return True
        return False
    
    def clearFocus(self):
        """ clear focus """
        for axis in self.axis:
            axis.hasFocus = False
