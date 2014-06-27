
from openalea.oalab.plugins.control import ControlWidgetSelectorPlugin

class PluginColorListWidget(ControlWidgetSelectorPlugin):
    controls = ['IColorList']
    name = 'ColorListWidget'
    edit_shape = ['large']
    paint = True

    @classmethod
    def load(cls):
        from openalea.plantgl.oaplugins.controls import ColorListWidget
        return ColorListWidget


class PluginCurve2DWidget(ControlWidgetSelectorPlugin):
    controls = ['ICurve2D']
    name = 'Curve2DWidget'
    edit_shape = ['large']
    paint = True

    @classmethod
    def load(cls):
        from openalea.plantgl.oaplugins.controls import Curve2DWidget
        return Curve2DWidget

class PluginQuantisedFunctionWidget(ControlWidgetSelectorPlugin):
    controls = ['IQuantisedFunction']
    name = 'QuantisedFunctionWidget'
    edit_shape = ['large']
    paint = True

    @classmethod
    def load(cls):
        from openalea.plantgl.oaplugins.controls import QuantisedFunctionWidget
        return QuantisedFunctionWidget

class PluginPatchWidget(ControlWidgetSelectorPlugin):
    controls = ['IPatch']
    name = 'NurbsPatchWidget'
    edit_shape = ['large']
    paint = True

    @classmethod
    def load(cls):
        from openalea.plantgl.oaplugins.controls import NurbsPatchWidget
        return NurbsPatchWidget


