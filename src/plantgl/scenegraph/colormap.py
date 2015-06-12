class PglColorMap:
    def __init__(self, minvalue = 0, maxvalue = 1, name = 'jet'):

        import matplotlib.pyplot as plt
        from matplotlib.colors import Normalize


        self.pltcolormap = plt.get_cmap(name)
        self.normalizer = Normalize(minvalue, maxvalue)

    def __call__(self, value):
        import _pglsg as sg

        color = self.pltcolormap(self.normalizer(value))
        color = list(color)
        color[3] = 1. - color[3]
        return sg.Color4([int(255 * c) for c in color])

class PglMaterialMap (PglColorMap):
    def __init__(self, minvalue = 0, maxvalue = 1, name = 'jet', ambientlevel = 0.5):
        PglColorMap.__init__(self, minvalue, maxvalue, name)
        assert 0 < ambientlevel <= 1
        self.ambientlevel = ambientlevel

    def __call__(self, value):
        import _pglsg as sg

        color = self.pltcolormap(self.normalizer(value))
        return sg.Material([int(255 * c * self.ambientlevel) for c in color[:3]], diffuse = 1. / self.ambientlevel, transparency=  1. - color[3])
