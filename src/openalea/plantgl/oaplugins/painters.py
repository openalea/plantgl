# -*- coding: utf-8 -*-

from openalea.plantgl.gui.qt import QtCore, QtGui
from openalea.oalab.plugins.controls.painters  import  AbstractPainter


class PainterColorList(AbstractPainter):

    def paint_data(self, data, painter, rectangle, option=None):
        painter.save()
        r = rectangle
        x = r.bottomLeft().x()
        y = r.topRight().y()
        ncolor = len(data)
        if ncolor:
            lx = r.width() / ncolor
            for name, color, diffuse in data:
                color = [diffuse * c for c in color]
                painter.fillRect(x, y, lx, r.height(), QtGui.QColor(*color))
                x += lx
        painter.restore()

class PainterMaterialList(AbstractPainter):

    def paint_data(self, data, painter, rectangle, option=None):
        painter.save()
        r = rectangle
        x = r.bottomLeft().x()
        y = r.topRight().y()
        ncolor = len(data)
        if ncolor:
            lx = r.width() / ncolor
            for mat in data:
                color = (mat.diffuseColor().red,mat.diffuseColor().green,mat.diffuseColor().blue)
                painter.fillRect(x, y, lx, r.height(), QtGui.QColor(*color))
                x += lx
        painter.restore()