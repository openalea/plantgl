class PglColorMap:
    def __init__(self, minvalue = 0, maxvalue = 1, name = 'jet'):

        import matplotlib.pyplot as plt
        from matplotlib.colors import Normalize


        self.pltcolormap = plt.get_cmap(name)
        self.normalizer = Normalize(minvalue, maxvalue)

    def __call__(self, value):
        return self.__topglcolor__(self.normalizer(value))
    
    def __topglcolor__(self, normalizedindex):
        import _pglsg as sg
        color = self.pltcolormap(normalizedindex)
        color = list(color)
        color[3] = 1. - color[3]
        return sg.Color4([int(255 * c) for c in color])

    def pglrepr(self, length = 0.5, width = 0.1, position = (-0.8, 0.8)):
        import _pglsg as sg
        
        ptlist = [position,(position[0]+width,position[1])]
        indexlist = []
        colorlist = []
        nbcolors = self.pltcolormap.N
        dl = length / float(nbcolors)
        for colid in xrange(nbcolors):
            dy = -(colid+1)*dl
            ptlist.append((position[0],position[1]+dy))
            ptlist.append((position[0]+width,position[1]+dy))
            indexlist.append((2*colid,2*colid+1,2*colid+3,2*colid+2))
            colorlist.append(self.__topglcolor__(1.-colid / float(nbcolors)))
        dc = 1
        def sc2txt(coord) :
            return 50*(coord+1)
        return sg.Scene([sg.Shape(sg.ScreenProjected(sg.QuadSet([(px,py,0) for px,py in ptlist], indexlist, colorList=colorlist, colorPerVertex = False),keepAspectRatio=False), sg.Material((255,255,255))),
                         sg.Shape(sg.Text(str(self.normalizer.vmax), (sc2txt(position[0]), sc2txt(position[1]+0.01) , 0 ), True ), sg.Material((0,0,0))),
                         sg.Shape(sg.Text(str(self.normalizer.vmin), (sc2txt(position[0]), sc2txt(position[1]-length-0.05) , 0 ), True ), sg.Material((0,0,0)))])        


class PglMaterialMap (PglColorMap):
    def __init__(self, minvalue = 0, maxvalue = 1, name = 'jet', ambientlevel = 0.5):
        PglColorMap.__init__(self, minvalue, maxvalue, name)
        assert 0 < ambientlevel <= 1
        self.ambientlevel = ambientlevel

    def __call__(self, value):
        import _pglsg as sg

        color = self.pltcolormap(self.normalizer(value))
        return sg.Material([int(255 * c * self.ambientlevel) for c in color[:3]], diffuse = 1. / self.ambientlevel, transparency=  1. - color[3])

