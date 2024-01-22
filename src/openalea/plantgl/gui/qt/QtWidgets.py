"""
Provides widget classes and functions.

.. warning:: All PyQt4/PySide gui classes are exposed but when you use
    PyQt5, those classes are not available. Therefore, you should treat/use
    this package as if it was ``PyQt5.QtWidgets`` module.
"""
import os
from openalea.plantgl.gui.qt import QT_API, PYQT5_API, PYQT4_API, PYSIDE_API, PYSIDE2_API, PYSIDE6_API

if os.environ[QT_API] in PYSIDE6_API:
    from PySide6.QtWidgets import *
    from PySide6.QtGui import *
elif os.environ[QT_API] in PYQT5_API:
    from PyQt5.QtWidgets import *
    from PyQt5.QtGui import QFont
elif os.environ[QT_API] in PYQT4_API:
    from PyQt4.QtGui import *
    from PyQt4.QtGui import QFileDialog as OldFileDialog

    class QFileDialog(OldFileDialog):

        @staticmethod
        def getOpenFileName(parent=None, caption='', directory='',
                            filter='', selectedFilter='',
                            options=OldFileDialog.Options()):
            return OldFileDialog.getOpenFileNameAndFilter(
                parent, caption, directory, filter, selectedFilter,
                options)

        @staticmethod
        def getOpenFileNames(parent=None, caption='', directory='',
                             filter='', selectedFilter='',
                             options=OldFileDialog.Options()):
            return OldFileDialog.getOpenFileNamesAndFilter(
                parent, caption, directory, filter, selectedFilter,
                options)

        @staticmethod
        def getSaveFileName(parent=None, caption='', directory='',
                            filter='', selectedFilter='',
                            options=OldFileDialog.Options()):
            return OldFileDialog.getSaveFileNameAndFilter(
                parent, caption, directory, filter, selectedFilter,
                options)
elif os.environ[QT_API] in PYSIDE_API:
    from PySide.QtGui import *
elif os.environ[QT_API] in PYSIDE2_API:
    from PySide2.QtGui import *

class QGLTextRenderer:

    def __init__(self, qglwidget):
        self.qglwidget = qglwidget
        self.hasopenglwidget = isinstance(self.qglwidget, QOpenGLWidget)
        self._scalingfactor = self.qglwidget.window().devicePixelRatio()
        self.initialize()

    def __del__(self):
        self.finalize()
        pass

    def initialize(self):
        from openalea.plantgl.gui.qt.QtGui import  QPainter

        self.textList = []

        if self.hasopenglwidget:
            if not hasattr(self,'painter'):
                self.painter = QPainter(self.qglwidget)
                self.painter.setRenderHints(QPainter.Antialiasing | QPainter.TextAntialiasing)
            else:
                self.painter.begin(self.qglwidget)
            self.painter.beginNativePainting()

    def finalize(self):
        from openalea.plantgl.gui.qt.QtGui import  QPainter
        import OpenGL.GL as ogl

        if self.hasopenglwidget:
            self._drawText()
            self.painter.end()

    def _drawText(self):
        if self.painter.isActive():
            self.painter.endNativePainting()
            if len(self.textList) > 0:
                for fontColor, font, x, y, text in self.textList:
                    self.painter.setPen(fontColor)
                    self.painter.setFont(QFont(font))
                    self.painter.drawText(int(x/self._scalingfactor), int(y/self._scalingfactor), text)
                    pass
                self.textList.clear()
            self.painter.beginNativePainting()

    def renderText3D(self, x, y, z, text, font = QFont(), color = None):
        import OpenGL.GL as ogl
        import OpenGL.GLU as oglu

        viewport = ogl.glGetIntegerv(ogl.GL_VIEWPORT)
        modelMatrix = ogl.glGetDoublev(ogl.GL_MODELVIEW_MATRIX)
        projMatrix = ogl.glGetDoublev(ogl.GL_PROJECTION_MATRIX)
        winX, winY, winZ = oglu.gluProject(x,y,z,modelMatrix, projMatrix, viewport)
        self.renderText(winX, self.qglwidget.height()*self._scalingfactor-winY, text, font, color)


    def renderText(self, x, y, text, font = QFont(), color = None):
        from openalea.plantgl.gui.qt.QtGui import  QColor
        import OpenGL.GL as ogl

        # Retrieve last OpenGL color to use as a font xcolor
        if self.hasopenglwidget:
            if color is None:
                glColor = ogl.glGetDoublev(ogl.GL_CURRENT_COLOR)
                fontColor = QColor(int(glColor[0]*255), int(glColor[1]*255), int(glColor[2]*255), int(glColor[3]*255))
            else:
                fontColor = QColor(int(color[0]*255), int(color[1]*255), int(color[2]*255), int(color[3]*255))

            self.textList.append((fontColor,QFont(font),int(x), int(y), text))
            
        else:
            if not color is None: ogl.glColor4fv(color)
            ogl.glPolygonMode(ogl.GL_FRONT_AND_BACK,ogl.GL_FILL)
            self.qglwidget.renderText( int(x), int(y),text, font)


