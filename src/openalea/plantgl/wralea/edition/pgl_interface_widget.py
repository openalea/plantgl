# -*- python -*-
#
#       OpenAlea.Visualea: OpenAlea graphical user interface
#
#       Copyright 2006-2024 INRIA - CIRAD - INRAE
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


from openalea.plantgl.gui.qt import QtCore, QtGui
from openalea.core.interface import * #IGNORE:W0614,W0401
from openalea.core.observer import lock_notify
from openalea.visualea.node_widget import NodeWidget
from openalea.plantgl.gui.curve2deditor import Curve2DEditorView #, Curve2DConstraint, FuncConstraint
from openalea.plantgl.gui.nurbspatcheditor import NurbsPatch3DEditorView
from openalea.plantgl.scenegraph import NurbsCurve2D
from .pgl_interface import ICurve2D, INurbsPatch
from pickle import loads

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


class Curve2DWidget(NodeWidget, Curve2DEditorView):
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
        Curve2DEditorView.__init__(self, parent)
        NodeWidget.__init__(self, node)
        
        self.notify(node, ('input_modified',))
        
        self.valueChanged.connect(self.onValueChanged)
        
        self.window().setWindowTitle(node.get_caption())

    @lock_notify      
    def onValueChanged(self):
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


class NurbsPatchWidget(NodeWidget, NurbsPatch3DEditorView):
    """
    NurbsPatchEditor widget
    """


    def __init__(self, node, parent):
        """
        """
        NurbsPatch3DEditorView.__init__(self, parent)
        NodeWidget.__init__(self, node)
        
        self.notify(node, ('input_modified',))
        
        #self.connect(self, QtCore.SIGNAL("valueChanged()"), 
        #             self.valueChanged)
        self.valueChanged.connect(self.onValueChanged)

        self.window().setWindowTitle(node.get_caption())

    @lock_notify      
    def onValueChanged(self):
        """ update value """
        patch = self.getNurbsObject()
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
                patch = self.node.get_input(0)
            except:
                patch= INurbsPatch.default()
            if not patch:
                patch = INurbsPatch.default()
            self.setNurbsObject(patch)
        
