
from openalea.oalab.plugins.control import ControlWidgetPlugin

class PluginColorListWidget(ControlWidgetPlugin):
    controls = ['IColorList']
    name = 'ColorListWidget'
    edit_shape = ['large']
    paint = True

    @classmethod
    def load(cls):
        from openalea.plantgl.plugins.controls import ColorListWidget
        return ColorListWidget


class PluginCurve2DWidget(ControlWidgetPlugin):
    controls = ['ICurve2D']
    name = 'Curve2DWidget'
    edit_shape = ['large']
    paint = True

    @classmethod
    def load(cls):
        from openalea.plantgl.plugins.controls import Curve2DWidget
        return Curve2DWidget

