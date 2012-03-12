# -*- python -*-
#
#       OpenAlea.Visualea: OpenAlea graphical user interface
#
#       Copyright 2006-2008 INRIA - CIRAD - INRA  
#
#       File author(s): Samuel Dufour-Kowalski <samuel.dufour@sophia.inria.fr>
#                       Christophe Pradal <christophe.prada@cirad.fr>
#
#       Distributed under the CeCILL v2 License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL_V2-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
################################################################################
"""Catalog of InterfaceWidgets"""

__license__ = "CeCILL V2"
__revision__ = " $Id: gui_catalog.py 2244 2010-02-08 17:10:17Z cokelaer $"


from PyQt4 import QtCore, QtGui
from openalea.core.interface import * #IGNORE:W0614,W0401
from openalea.core.observer import lock_notify
from openalea.visualea.node_widget import NodeWidget
from openalea.plantgl.gui.curve2deditor import Curve2DEditor, Curve2DConstraint, FuncConstraint
from openalea.plantgl.gui.nurbspatcheditor import NurbsPatchEditor
from openalea.plantgl.scenegraph import NurbsCurve2D
from pgl_interface import ICurve2D, INurbsPatch
from cPickle import loads

'''
class ICurve2DWidget(IInterfaceWidget, Curve2DEditor):
    """
    Curve2D Interface widget
    """
    print 'ICurve2DWidget'
    # Corresponding Interface & Metaclass
    __interface__ = ICurve2D
    try:
        toto = make_metaclass()
        __metaclass__ = toto
    except TypeError:
        pass

    def __init__(self, node, parent, parameter_str, interface):
        """
        @param parameter_str : the parameter key the widget is associated to
        @param interface : instance of interface object
        """
        Curve2DEditor.__init__(self, parent, FuncConstraint() if interface.func_constraint else Curve2DConstraint())
        IInterfaceWidget.__init__(self, node, parent, parameter_str, interface)
        
        self.notify(node, None)
        
        self.connect(self, QtCore.SIGNAL("valueChanged()"), \
                     self.valueChanged)

    @lock_notify      
    def valueChanged(self):
        """ update value """
        from copy import deepcopy
        self.node.set_input(self.param_str, deepcopy(self.getCurve()))

    def notify(self, sender, event):
        """ Notification sent by node """
        try:
            crv = self.node.get_input(self.param_str)
            if crv is None:
                crv = ICurve2D.default()
        except:
            crv = ICurve2D.default()
            
        self.setCurve(crv)
'''


class Curve2DWidget(NodeWidget, Curve2DEditor):
    """
    Curve2D  widget
    """
    default_crv = ICurve2D.default()

    @staticmethod
    def isDefault(crv):
        return list(crv.ctrlPointList) == list(Curve2DWidget.default_crv.ctrlPointList)

    def __init__(self, node, parent):
        """
        """
        Curve2DEditor.__init__(self, parent)
        NodeWidget.__init__(self, node)
        
        self.notify(node, ('input_modified',))
        
        self.connect(self, QtCore.SIGNAL("valueChanged()"), \
                     self.valueChanged)
        
        self.window().setWindowTitle(node.get_caption())

    @lock_notify      
    def valueChanged(self):
        """ update value """
        crv = self.getCurve()
        if crv and not self.isDefault(crv):
            self.node.set_input(0, crv)

    def notify(self, sender, event):
        """ Notification sent by node """
        if event[0] == 'caption_modified':
            self.window().setWindowTitle(event[1])
            
        if event[0] == 'input_modified':
            state = self.node.get_input_state(0)
            try:
                notconnected = bool(state != "connected")
                self.setEnabled(notconnected)
            except:
                pass
            
            crv = self.node.get_input(0)
            if not crv:
                pts = self.node.get_input(1)
                if pts:
                    crv = NurbsCurve2D(loads(pts))
            if not crv:
                crv = ICurve2D.default()
            if not crv: 
                crv = self.newDefaultCurve()

            self.setCurve(crv)
        

#####################################################################
# NurbsPatch Editor
'''
class INurbsPatchWidget(IInterfaceWidget, NurbsPatchEditor):
    """
    NurbsPatch Interface widget
    """

    # Corresponding Interface & Metaclass
    __interface__ = INurbsPatch
    __metaclass__ = make_metaclass()

    def __init__(self, node, parent, parameter_str, interface):
        """
        @param parameter_str : the parameter key the widget is associated to
        @param interface : instance of interface object
        """
        NurbsPatchEditor.__init__(self, parent)
        IInterfaceWidget.__init__(self, node, parent, parameter_str, interface)
        
        self.notify(node, None)
        
        self.connect(self, QtCore.SIGNAL("valueChanged()"), \
                     self.valueChanged)

    @lock_notify      
    def valueChanged(self):
        """ update value """
        from copy import deepcopy
        self.node.set_input(self.param_str, deepcopy(self.getNurbsPatch()))

    def notify(self, sender, event):
        """ Notification sent by node """
        try:
            crv = self.node.get_input(self.param_str)
            if crv is None:
                crv = INurbsPatch.default()
        except:
            crv = INurbsPatch.default()
            
        self.setNurbsPatch(crv)
'''


class NurbsPatchWidget(NodeWidget, NurbsPatchEditor):
    """
    NurbsPatchEditor widget
    """


    def __init__(self, node, parent):
        """
        """
        NurbsPatchEditor.__init__(self, parent)
        NodeWidget.__init__(self, node)
        
        self.notify(node, ('input_modified',))
        
        self.connect(self, QtCore.SIGNAL("valueChanged()"), \
                     self.valueChanged)
        
        self.window().setWindowTitle(node.get_caption())

    @lock_notify      
    def valueChanged(self):
        """ update value """
        patch = self.getNurbsPatch()
        self.node.set_input(0, patch)

    def notify(self, sender, event):
        """ Notification sent by node """
        if event[0] == 'caption_modified':
            self.window().setWindowTitle(event[1])
            
        if event[0] == 'input_modified':
            state = self.node.get_input_state(0)
            try:
                notconnected = bool(state != "connected")
                self.setEnabled(notconnected)
            except:
                pass
            
            try:
                crv = self.node.get_input(0)
            except:
                crv = INurbsPatch.default()
            if not crv:
                crv = INurbsPatch.default()
            self.setNurbsPatch(crv)
        
