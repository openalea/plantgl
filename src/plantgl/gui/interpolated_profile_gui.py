# -*- python -*-
#
#       OpenAlea.Visualea: OpenAlea graphical user interface
#
#       Copyright 2010 INRIA - CIRAD - INRA
#
#       File author(s): Daniel Barbeau <daniel.barbeau@sophia.inria.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
#
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
###############################################################################
#  ----------------------------------------------------------------------------
#
#       PlantGL: The Plant Graphic Library
#
#       Copyright 2006-2010 UMR CIRAD/INRIA/INRA DAP
#
#       File author(s): D. Barbeau et al.
#
#  ----------------------------------------------------------------------------
#
#                      GNU General Public Licence
#
#       This program is free software; you can redistribute it and/or
#       modify it under the terms of the GNU General Public License as
#       published by the Free Software Foundation; either version 2 of
#       the License, or (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public
#       License along with this program; see the file COPYING. If not,
#       write to the Free Software Foundation, Inc., 59
#       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
#  ----------------------------------------------------------------------------


from PyQGLViewer import QGLViewer, Vec, Camera, WorldConstraint
from openalea.plantgl.scenegraph import NurbsCurve, interpolated_profile
from openalea.plantgl.math import Vector4, Vector3
from openalea.plantgl.algo import GLRenderer, Discretizer
from OpenGL.GL import *
from math import cos, sin, sqrt, pi, atan, atan2
from openalea.vpltk.qt import QtCore, QtGui
from .interpolated_profile_gui_addons import UserSlicesAddOn, VisualCrossSectionsAddOn, GridAddOn



class ProfileViewer (QGLViewer):

    _posCurveColorF = [0.0,1.0,0.0]

    positionChanged = QtCore.Signal(int)
    userSectionHighlighted = QtCore.Signal(object, object)

    def __init__(self,parent, profile=None):
        QGLViewer.__init__(self,parent)
        self.__profile  = None
        self.__position = 0
        self.__positionCurve = None
        self.increment = 0.01
        self._axisScaleTolerance = abs(pi/8.) # (radians)

        #states
        self.__oldY = None
        self.__scalingDirection = None

        #ranges
        self.__param_min = 0.0
        self.__param_max = 1.0

        # --rendering components --
        self.factor = [1., 1., 1.] #x, y, z scene scaling factors.
        self.discretizer = Discretizer()
        self.renderer    = GLRenderer(self.discretizer)
        self.renderer.renderingMode = GLRenderer.Dynamic

        # -- addons --
        self._addons        = []
        self.visualSections = VisualCrossSectionsAddOn(self)
        self.grids          = GridAddOn(self)
        self.userSlices     = UserSlicesAddOn(self)


    def init(self):
        self.camera().setUpVector(Vec(0,1,0))
        self.camera().setType(Camera.ORTHOGRAPHIC)
        self.camConstraint = WorldConstraint()
        self.camera().frame().setConstraint(self.camConstraint)
        self.camera().setZClippingCoefficient(4) #arbitrary.
        self.showEntireScene()

    def set_axis_scale_tolerance(self, tolerance):
        self._axisScaleTolerance = abs(tolerance)

    def set_profile(self, profile):
        if profile is not None:
            profile.add_update_callback(self.__profileChanged)
            self.__profile = profile
            # -- recompute stuff --
            self.__param_min,self.__param_max=profile.get_param_range()
            self.increment = (self.__param_max - self.__param_min)/100
            for addon in self._addons:
                addon._init_from_profile(profile)
            self.set_position(self.range()[0])
        else:
            self.__clearCaches()
            self.__profile = None

    def profile(self):
        return self.__profile

    def range(self):
        return self.__param_min, self.__param_max

    def position(self):
        return self.__position

    def set_position(self, position):
        # -- clamp position --
        min, max = self.range()
        if position > max: position = max
        if position < min: position = min
        self.__position=position
        self.update_displayed_geometry()
        self.positionChanged.emit(position)
        if position not in self.__profile:
            position = self.round_within_increment(position)
        if position in self.__profile:
            self.userSectionHighlighted.emit(position, self.__profile[position])

    def round_within_increment(self, pos):
        keys = list(map(self.unnormalised_parameter, iter(self.__profile.keys())))
        for k in keys:
            if abs(k-pos) <= self.increment:
                return k
        return pos


    #########################
    # Normalisation Helpers #
    #########################
    def normalised_parameter(self, par, **kwargs):
        if self.__profile:
            return self.__profile.normalised_parameter(par, **kwargs)

    def normalised_abscissa(self, par, **kwargs):
        if self.__profile:
            return self.__profile.normalised_abscissa(par, **kwargs)

    def unnormalised_parameter(self, par, **kwargs):
        if self.__profile:
            return self.__profile.unnormalised_parameter(par, **kwargs)

    def unnormalised_abscissa(self, par, **kwargs):
        if self.__profile:
            return self.__profile.unnormalised_abscissa(par, **kwargs)

    #################
    # Painting code #
    #################
    def draw(self):
        self.setBackgroundColor(QtGui.QColor(0,0,0))
        glDisable(GL_LIGHTING)
        glEnable(GL_LINE_SMOOTH)
        glEnable(GL_BLEND)
        glEnable(GL_ALPHA_TEST)
        glAlphaFunc ( GL_GREATER, 0.1 )
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST)

        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        mat = 16*[0]
        mat[0] = self.factor[0]
        mat[5] = self.factor[1]
        mat[10] = self.factor[2]
        mat[15] = 1.
        glMultMatrixd(mat)

        if self.__profile is not None:
            self.discretizer.clear()
            if self.__positionCurve:
                for addon in self._addons:
                    addon._draw(self.renderer)
                glColor4f(0.0,1.0,0.0,1.0)
                glDisable(GL_DEPTH_TEST)
                self.__positionCurve.apply(self.renderer)
                glEnable(GL_DEPTH_TEST)
        glPopMatrix()

    ###############################
    # Qt Event Handlers overloads #
    ###############################
    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.MidButton:
            self.__oldX = event.x()
            self.__oldY = event.y()
        else:
            QGLViewer.mousePressEvent(self, event)

    def mouseReleaseEvent(self, event):
        self.__oldX = None
        self.__oldY = None
        self.__scalingDirection = None
        QGLViewer.mouseReleaseEvent(self, event)

    def mouseMoveEvent(self, event):
        if self.__oldY is not None:
            pos = self.__position

            #some values that are used a bit everywhere
            newX, newY = event.x(), event.y()
            xDiff = float(newX-self.__oldX)
            yDiff = float(newY-self.__oldY)
            cartDist = sqrt(yDiff**2+xDiff**2)

            mod = event.modifiers()
            if mod != QtCore.Qt.ControlModifier:
                delta =  newY - self.__oldY
                if delta > 0:
                    pos += self.increment
                elif delta < 0:
                    pos -= self.increment
                self.__oldY = newY

                self.set_position(pos)
            elif self.__scalingDirection is None:
                if cartDist < 50 : #pixels
                    return

                #compute direction coefficient of the mouse pointer to the screen center
                if xDiff==0.0:
                    return
                pA = atan2(yDiff,xDiff)
                tol = self._axisScaleTolerance

                #compute projected direction coefficients of axes
                camera = self.camera()
                origin = camera.getProjectedCoordinatesOf(Vec(0.,0.,0.))
                xProj  = camera.getProjectedCoordinatesOf(Vec(1.,0.,0.))
                yProj  = camera.getProjectedCoordinatesOf(Vec(0.,1.,0.))
                zProj  = camera.getProjectedCoordinatesOf(Vec(0.,0.,1.))

                xVec = (xProj[0] - origin[0]),(xProj[1] - origin[1])
                yVec = (yProj[0] - origin[0]),(yProj[1] - origin[1])
                zVec = (zProj[0] - origin[0]),(zProj[1] - origin[1])

                #angles
                if xVec[0]==0.0 or yVec[0]==0.0 or zVec[0]==0.0:
                    return

                xA = atan(xVec[1]/xVec[0])
                yA = atan(yVec[1]/yVec[0])
                zA = atan(zVec[1]/zVec[0])

                dire = min( ( abs(xA%pi-pA%pi),0,pA,xVec),
                            ( abs(yA%pi-pA%pi),1,pA,yVec),
                            ( abs(zA%pi-pA%pi),2,pA,zVec) )
                self.__scalingDirection = dire if (dire[0] <= tol) else None

            elif self.__scalingDirection is not None:
                angD, dire, angle, vec = self.__scalingDirection

                #cartDist is used to obtain a significant difference
                #or else computations are screwed.
                if xDiff != 0.0 and yDiff != 0 and cartDist > 5 :
                    pA = atan2(yDiff,xDiff)
                    if pA < 0: pA += 2*pi
                    if angle < 0: angle +=2*pi
                    angleDiv = int(angle/pi*2)
                    mouseDiv = int(pA/pi*2)
                    positive = angleDiv == mouseDiv

                    f = self.factor[dire]
                    f = f*1.02 if positive else f*0.98
                    self.factor[dire] = f

                    self.__oldY = newY
                    self.__oldX = newX
                    self.update()

        else:
            QGLViewer.mouseMoveEvent(self, event)

    def keyPressEvent(self, event):
        key = event.key()
        if key in [QtCore.Qt.Key_Return,QtCore.Qt.Key_Return]:
            self.__profile.create_cross_section(self.__position)
        elif key == QtCore.Qt.Key_Delete:
            position = self.round_within_increment(self.__position)
            if position in self.__profile:
                del self.__profile[position]

    ##############################
    # Internal State Maintenance #
    ##############################
    def update_displayed_geometry(self):
        if self.__profile:
            # -- 'add-ons' --
            for addon in self._addons:
                addon._compute(self.__profile)
            self.__positionCurve = self.__profile(self.__position)
            self.update()

    def __clearCaches(self):
        for addon in self._addons:
            addon.clear_caches()

    def __profileChanged(self):
        # -- cleanup caches --
        self.__clearCaches()
        # -- recompute stuff --
        self.__param_min,self.__param_max=self.__profile.get_param_range()
        self.increment = (self.__param_max - self.__param_min)/100
        for addon in self._addons:
            addon._interpolation_changed()
        self.update_displayed_geometry()



##############################################################################
# -------------------------------------------------------------------------- #
##############################################################################
class SenderWidget(QtCore.QObject):
    """A helper class that reemits signals from its inner widget
    with self as first argument, to identify the origin of a signal."""
    valueChanged = QtCore.Signal(QtCore.QObject, object, object)
    def __init__(self, widget, typ, parent):
        QtCore.QObject.__init__(self, parent)
        self.inner = widget
        self.type = typ
        if hasattr(widget, "valueChanged"):
            widget.valueChanged.connect(self.__on_value_changed)
            self.setValue = lambda x, y: x.inner.setValue(y)
        elif hasattr(widget, "stateChanged"):
            widget.stateChanged.connect(self.__on_value_changed)
            self.setValue = lambda x, y: x.inner.setCheckState(QtCore.Qt.Checked if y else QtCore.Qt.Unchecked)
        elif hasattr(widget, "textChanged"):
            widget.textChanged.connect(self.__on_value_changed)
            self.setValue = lambda x, y: x.inner.setText(str(y))
    def __on_value_changed(self, value):
            self.valueChanged.emit(self, value, self.type)


import weakref
class AddOnControlWidget(QtGui.QScrollArea):
    """ A widget that creates a control panel for the addons of a
    ProfileExplorer. """


    def __init__(self, explorer, parent=None):
        QtGui.QScrollArea.__init__(self, parent)

        # -- reference to the profile explorer
        self.__exp = weakref.proxy(explorer)
        self.setSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Maximum)
        # -- Layout --
        self.__subWidget = QtGui.QWidget() # A QScrollArea needs an inner widget to scroll.
        self.__layout = QtGui.QBoxLayout(QtGui.QBoxLayout.TopToBottom)
        # -- internals --
        self.__widmap = {} #map a widget instance to an addon path "addon_name.attribute_name" and the opposite way too.
        self.__addOns = {} #map an addon name to an addon instance.
        self.build_gui()        #browse the addon list and build the GUI.
        self.__subWidget.setLayout(self.__layout)
        self.setWidget(self.__subWidget)

    def build_gui(self):
        """ Build the GUI from the list of addons """
        for addon in self.__exp._addons:
            name = addon.name()
            self.__addOns[name] = addon

            # -- addon gui layout --
            box = QtGui.QGroupBox(name, self.__subWidget)
            subLayout = QtGui.QGridLayout()

            varDict  = addon.variable_dict()
            varNames = sorted(iter(varDict.keys()), lambda x, y: -1 if x=="enabled" else cmp(x,y))
            for key in varNames:
                t, v = varDict[key]
                w = self.make_widget(t, v, self)
                if not w:
                    continue
                row = subLayout.rowCount()
                subLayout.addWidget(QtGui.QLabel(key), row, 0)
                subLayout.addWidget(w.inner, row, 1 )
                path = name+"."+key
                self.__widmap[w] = path
                self.__widmap[path] = w
                addon.add_callback(key, self.__on_addon_changed)

            box.setLayout(subLayout)
            self.__layout.addWidget(box)

    def init(self):
        """ Read values from addons and fill the gui with them """
        for addon in self.__exp._addons:
            name = addon.name()
            varDict  = addon.variable_dict()
            for key, (t, v) in varDict.items():
                path = name+"."+key
                widget = self.__widmap[path]
                widget.setValue(widget, v)

    def make_widget(self, typ, value, parent):
        """ Take this value and type and make me a nice widget please.
        The widget is wrapped in a SenderWidget and can be accessed through
        the widget.inner attribute."""
        w = None
        if typ == bool:
            w = QtGui.QCheckBox(parent)
            w.setCheckState( QtCore.Qt.Checked if value else QtCore.Qt.Unchecked )
        elif typ == tuple:
            w = QtGui.QLineEdit(str(value), parent)
        elif typ == int:
            w = QtGui.QSpinBox(parent)
            w.setRange(-10000, 10000)
            w.setSingleStep(1)
        elif typ == float:
            w = QtGui.QDoubleSpinBox(parent)
            w.setRange(-10000.0, 10000.0)
            w.setSingleStep(0.01)

        if w is not None:
            wrapper = SenderWidget(w, typ, self)
            wrapper.valueChanged.connect(self.__on_widget_changed)
            return wrapper

    def __on_widget_changed(self, sender, value, typ):
        """ Slot to react to changes coming from inner widgets. The actualise
        the addons. """
        path = self.__widmap[sender]
        addon, key = path.split(".")
        addon = self.__addOns[addon]
        if typ == tuple:
            try:
                value = eval(str(value))
            except:
                pass
        addon.set(key, value)
        setattr(addon, key, value)

    def __on_addon_changed(self, path, value):
        """ Method to react to changes coming from the addons to update the
        widgets"""
        widget = self.__widmap[path]
        widget.setValue(widget, value)


##############################################################################
# -------------------------------------------------------------------------- #
##############################################################################
from openalea.plantgl.gui.curve2deditor import Curve2DEditor
class CurvePanel( QtGui.QScrollArea ):

    class SimpleCurve2DView(Curve2DEditor):
        clicked = QtCore.Signal(object, object)
        deleteRequested = QtCore.Signal(object, object)
        def __init__(self, pos, curve, parent):
            Curve2DEditor.__init__(self, parent)
            self.pos = pos
            self.accessorType[NurbsCurve]=ProfileEditor.NurbsAccessor
            self.setFixedSize(100, 90)
            self.setEnabled(True)
            self.setAutoFillBackground(True)
            self.bkgdColor = self.backgroundColor()
            if curve:
                self.setCurve(curve)
        def highlight(self, val):
            if val:
                self.defaultColor = QtGui.QColor(100,200,100)
            else:
                self.defaultColor = self.bkgdColor
            self.update()
        def drawGrid(self):
            return
        def mousePressEvent(self,event):
            self.clicked.emit(self.pos, self.getCurve())
        def mouseDoubleClickEvent(self,event):
            QGLViewer.mouseDoubleClickEvent(self,event)
        def mouseReleaseEvent(self,event):
            QGLViewer.mouseReleaseEvent(self,event)
        def contextMenuEvent(self, event):
            menu = QtGui.QMenu(self)
            deleteAction = menu.addAction("Delete section")
            deleteAction.triggered.connect(self.__on_delete_request)
            menu.popup(event.globalPos())
        def __on_delete_request(self):
            self.deleteRequested.emit(self.pos, self.getCurve())


    curveEditRequested   = QtCore.Signal(object, object)
    curveDeleteRequested = QtCore.Signal(object, object)
    curveMoveRequested   = QtCore.Signal(object, object)

    def __init__(self, orientation=QtGui.QBoxLayout.LeftToRight, parent=None):
        QtGui.QScrollArea.__init__(self, parent)
        self.orientation = orientation
        self.setSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        self.__curveViews  = []
        self.__curveLabels = []
        self.__refreshTimout = QtCore.QTimer()
        self.__refreshTimout.setInterval(500)
        self.__refreshTimout.timeout.connect(self.__set_curves)
        self.__crvs = None

    def set_curves(self, crvs):
        self.__crvs = crvs
        if self.__refreshTimout.isActive():
            self.__refreshTimout.stop()
        self.__refreshTimout.start()

    def get_curve(self, id):
        w= self.widget().layout().itemAt(id).widget()
        return w.getCurve()

    def __set_curves(self):
        self.__refreshTimout.stop()
        crvs = self.__crvs
        if len(crvs)==len(self.__curveViews): #refresh the existing views
            for i in range(len(crvs)):
                pos, c = crvs[i]
                v= self.__curveViews[i]
                v.setCurve( c )
                v.pos = pos
                self.__curveLabels[i].setValue( pos )
        else: #recreate them
            del self.__curveViews[:]
            del self.__curveLabels[:]
            scrolledWidget = QtGui.QWidget(self)
            layout      = QtGui.QBoxLayout(self.orientation)
            scrolledWidget.setLayout(layout)

            # -- used to lock the bounding cross section line edits --
            minPos = min(crvs, key=lambda x:x[0])[0]
            maxPos = max(crvs, key=lambda x:x[0])[0]

            for pos, c in crvs:
                frame = QtGui.QFrame(scrolledWidget)
                frame.setFrameShape(QtGui.QFrame.Panel)
                frame.setFrameShadow(QtGui.QFrame.Raised)
                subLay = QtGui.QBoxLayout(QtGui.QBoxLayout.TopToBottom)
                frame.setLayout(subLay)

                w = CurvePanel.SimpleCurve2DView(pos, c, frame)
                w.clicked.connect(self.curveEditRequested)
                w.deleteRequested.connect(self.curveDeleteRequested)

                label = QtGui.QDoubleSpinBox()
                label.setRange(-100000., 100000.)
                label.setValue(pos)
                label.setAlignment( QtCore.Qt.AlignHCenter )
                sender = SenderWidget(label, float, self)
                if pos in [minPos, maxPos]:
                    label.setEnabled(False)
                else:
                    sender.valueChanged.connect(self.__on_label_changed)

                subLay.addWidget(w, QtCore.Qt.AlignHCenter)
                subLay.addWidget(label, QtCore.Qt.AlignHCenter)
                layout.addWidget(frame)
                self.__curveViews.append(w)
                self.__curveLabels.append(label)
            self.setWidget(scrolledWidget)

    def highlight_curve_at_position(self, pos, crv):
        for cv in self.__curveViews:
            if cv.pos == pos:
                cv.highlight(True)
            else:
                cv.highlight(False)

    #################
    # Private stuff #
    #################
    def __on_label_changed(self, sender, value, typ):
        try:
            i = self.__curveLabels.index(sender.inner)
        except:
            return
        oldPosition = self.__curveViews[i].pos
        newPosition = value
        self.curveMoveRequested.emit(oldPosition, newPosition)


##############################################################################
# -------------------------------------------------------------------------- #
##############################################################################
from openalea.plantgl.gui.curve2deditor import Nurbs2DAccessor
class ProfileEditor(QtGui.QSplitter):

    # -----------------------------------------------------------------------
    class NurbsAccessor (Nurbs2DAccessor):
        """Direct manipulation of the InterpolatedProfile"""
        def __init__(self,curve):
            Nurbs2DAccessor.__init__(self,curve)
            self.profile = None
            self.position = None
        def checkType(self,curve):
            assert type(curve) == NurbsCurve
        def insertPoint(self,index,npoint):
            if self.profile is not None:
                pt, u = self.curve.findClosest(Vector3(*npoint))
                self.profile.create_control_point(u)
        def setPoint(self,index,npoint):
            crv = self.curve
            pi = crv.ctrlPointList[index]
            crv.ctrlPointList[index] = Vector4(npoint[0],
                                               npoint[1],
                                               pi[2] ,1)
            if self.profile is not None and \
                   crv is not None and \
                   self.position in self.profile:
                self.profile[self.position] = interpolated_profile.CrossSection(*crv.ctrlPointList)
        def bounds(self):
            minp,maxp = self.curve.ctrlPointList.getBounds()
            vmin, vmax = self.verticalBounds()
            return (minp.project()[0],vmin), (maxp.project()[0], vmax)
        def verticalBounds(self):
            minp,maxp = min(self.curve.ctrlPointList, key=lambda x: x[1]), \
                        max(self.curve.ctrlPointList, key=lambda x: x[1])
            return minp[1], maxp[1]
        def findClosest(self,p):
            ctrlpolyline = interpolated_profile.sg.Polyline([Vector3(i[0], i[1], 0) \
                                              for i in self.curve.ctrlPointList])
            lp,u = ctrlpolyline.findClosest(Vector3(p,1))
            return u
    # -----------------------------------------------------------------------

    def __init__(self, parent=None, editingCentral=False):
        QtGui.QSplitter.__init__(self,QtCore.Qt.Vertical, parent)
        self.__splitter = QtGui.QSplitter(QtCore.Qt.Horizontal, self)

        self.__profileExplorer = ProfileViewer(None)
        self.__curveOverview   = CurvePanel ()
        self.__curveEdit      = Curve2DEditor(self.__splitter)
        self.__curveEdit.position = 0.0 #additional attribute needed to refresh the edited curve.

        self.__addonConf       = AddOnControlWidget(self.__profileExplorer, self)

        # -- curve panel conf --
        self.__curveEdit.sphere = interpolated_profile.sg.Sphere(radius=0.002)
        self.__curveEdit.accessorType[NurbsCurve]=ProfileEditor.NurbsAccessor

        # -- profile explorer conf --
        self.__profileExplorer.factor = [1,1,2]
        self.__profileExplorer.userSlices.enabled = True
        self.__profileExplorer.visualSections.enabled = True
        self.__profileExplorer.visualSections.baseAlpha = 0.5
        self.__profileExplorer.grids.enabled = True
        self.__profileExplorer.grids.xzGrid = True
        self.__profileExplorer.grids.zScale = True

        # -- signal binding --
        self.__curveOverview.curveEditRequested.connect(self.__on_curve_edit_request)
        self.__curveOverview.curveDeleteRequested.connect(self.__on_curve_delete_request)
        self.__curveOverview.curveMoveRequested.connect(self.__on_curve_move_request)
        self.__profileExplorer.positionChanged.connect(self.__on_position_changed)
        self.__profileExplorer.userSectionHighlighted.connect(self.__on_user_section_highlight)
        self.__profileExplorer.userSectionHighlighted.connect(self.__curveOverview.highlight_curve_at_position)
        # !!! Do not bind this one as valueChanged can be anything
        # !!! and that our custom curvePanel.curveAccessor directly
        # !!! edits the interpolated profile.
        # self.__curveEdit.connect(self.__curveEdit, QtCore.SIGNAL("valueChanged()"), self.__on_curve_edited)

        # -- layout --
        l = QtGui.QBoxLayout(QtGui.QBoxLayout.TopToBottom)
        l.addWidget(self.__profileExplorer if not editingCentral else self.__curveEdit)
        l.addWidget(self.__curveOverview)
        l.setContentsMargins(0,0,0,0)
        explorerAndCurveOverview = QtGui.QWidget(self.__splitter)
        explorerAndCurveOverview.setLayout(l)
        self.__splitter.addWidget(self.__addonConf)
        self.__splitter.addWidget(explorerAndCurveOverview)
        self.__splitter.addWidget(self.__curveEdit if not editingCentral else self.__profileExplorer)
        self.__splitter.setSizes([0, explorerAndCurveOverview.width(), 0])
        self.addWidget(self.__splitter)

        # -- latest position of profile explorer
        # -- cursor that matched a user defined curve.
        self.__editedPosition = None

    def set_profile(self, prof):
        prof.add_update_callback(self.__on_interpolation_changed)
        self.__profileExplorer.set_profile(prof)
        self.__push_user_sections_to_overview()
        self.__on_position_changed(self.__profileExplorer.position(), self)
        self.__addonConf.init()

    def get_profile(self):
        return self.__profileExplorer.profile()

    def __on_interpolation_changed(self):
        self.__push_user_sections_to_overview()
        # -- refresh curve panel with new curve
        interpol = self.get_profile()
        pos = self.__curveEdit.position
        if pos in interpol:
            self.__push_section_to_curve_panel(interpol[pos])

    def __push_user_sections_to_overview(self):
        prof = self.get_profile()
        l = prof.as_sorted_tuples()
        real = [(prof.unnormalised_parameter(k),NurbsCurve(v)) for k,v in l]
        self.__curveOverview.set_curves(real)

    def __push_section_to_curve_panel(self, section):
        section = NurbsCurve(section) if isinstance(section, list) else section
        self.__curveEdit.setCurve(section)
        self.__curveEdit.curveAccessor.profile = self.get_profile()
        if self.__editedPosition is not None:
            self.__curveEdit.position = self.__editedPosition
            self.__curveEdit.curveAccessor.position = self.__editedPosition

    def __on_position_changed(self, position, sender=None ):
        pass

    def __on_user_section_highlight(self, position, crv ):
        self.__editedPosition = position
        self.__push_section_to_curve_panel( crv )

    def __on_curve_edit_request(self, position, curve):
        # -- following call makes the profileExplorer
        # -- send userSectionHighlighted.
        self.__profileExplorer.set_position(position)

    def __on_curve_delete_request(self, position, curve):
        profile = self.get_profile()
        if position not in profile:
            position = self.__profileExplorer.round_within_increment(position)
        if position in profile:
            del profile[position]

    def __on_curve_move_request(self, oldPos, newPos):
        profile = self.get_profile()
        profile[oldPos] = newPos



if __name__ == '__main__':
    import openalea.plantgl.scenegraph.interpolated_profile as interpolated_profile
    crsSect1 = interpolated_profile.CrossSection((0., 0.), (0.5, 0.), (0.6, 0.1), (1., 0.))
    crsSect2 = interpolated_profile.CrossSection((0., 2.), (0.5, 1.), (0.8, 0.3), (1., 0.))
    crsSect3 = interpolated_profile.CrossSection((0., 0.), (0.5, 0.), (0.7, 0.8), (1., 0.))

    tc = interpolated_profile.InterpolatedProfile(interpolator=interpolated_profile.CSplineMethod)
    tc.set_param_range(-180.0, 180.0)
    tc.add_cross_sections(-180, crsSect1,
                             0, crsSect2,
                           180, crsSect3)

    qapp = QtGui.QApplication([])
    w = ProfileEditor(editingCentral=False)
    w.set_profile(tc)
    w.show()
    qapp.exec_()

