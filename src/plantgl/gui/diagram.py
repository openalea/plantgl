def isiterable(obj): 
    try:
        iter(obj)
        return True
    except:
        return False

class DiagramPlotter:
    def __init__(self, xextent = None, yextent = None, position = (0.1,-0.3), size = (0.8,0.8),  xtick = None, ytick = None, withorigin = False ):
        self.position = position
        self.size = size
        self.xtick = xtick
        self.ytick = ytick
        self.ticklength = 0.02
        self.xextent = xextent
        self.yextent = yextent
        self.withorigin = withorigin
        self.curves = []
        self.points = []
        self.histo = []
  
    def add_curve(self, curvedata, color= (255,0,0), size = 2):
        if type(curvedata) == tuple:
          curvedata = zip(curvedata[0],curvedata[1])
        elif type(curvedata) == dict:
          curvedata = curvedata.items()
          curvedata.sort()
        assert len(curvedata) > 1
        self.curves.append((curvedata,color, size))
        return self

    def add_points(self,pointdata, color = (255,0,255), size = 6):
        if type(pointdata) == tuple:
          pointdata = zip(pointdata[0],pointdata[1])
        elif type(pointdata) == dict:
          pointdata = pointdata.items()
          pointdata.sort()
        assert len(pointdata) > 0
        self.points.append((pointdata,color, size))
        return self
    
    def add_histogram(self,values, color = (255,0,255)):
        if type(values) == tuple:
          values = zip(values[0],values[1])
        elif type(values) == dict:
          values = values.items()
          values.sort()
        assert len(values) > 0
        self.histo.append((values, color))
        return self
    
    def bbox(self):
        from math import ceil, floor
        data = []
        for curve, color, size in self.curves:
            data += curve
        for points, color, size in self.points:
            data += points
        for values, color in self.histo:
            data += values
        if len(data) > 1:
            if self.xextent:
                minx, maxx = self.xextent
            else :
                minx = floor(min([x for x,y in data]))
                maxx = ceil(max([x for x,y in data]))
            if self.yextent:
                miny, maxy = self.yextent
            else :
                miny = floor(min([y for x,y in data]))
                maxy = ceil(max([y for x,y in data]))
            if self.withorigin:
                maxx = max(0,maxx)
                minx = min(0,minx)
                maxy = max(0,maxy)
                miny = min(0,miny)
        return minx, maxx, miny, maxy
    
    def toScene(self):
        from math import ceil, floor, log
        from openalea.plantgl.scenegraph import Polyline2D, Polyline, PointSet, QuadSet, Text, Group, Translated, ScreenProjected, Material, Color3, Color4, Shape, Scene
        
        minx, maxx, miny, maxy = self.bbox()
        
        xext = maxx - minx
        yext = maxy - miny
        x2Dlength, y2Dlength = self.size

        projx = lambda x: self.position[0] + (x2Dlength*(x-minx)/xext)
        projy = lambda y: self.position[1] + (y2Dlength*(y-miny)/yext)
        projp = lambda p : (projx(p[0]),projy(p[1]))
        projp3 = lambda p : (projx(p[0]),projy(p[1]),0)
        proj = lambda x,y : (projx(x),projy(y))

        if self.xtick is None:
            nbdigit = round(log(xext,10))
            self.xtick = 10**(nbdigit-1)
        if self.ytick is None:
            nbdigit = round(log(yext,10))
            self.ytick = 10**(nbdigit-1)
        
        result = []
        def toMaterial(c) :
            if type(c) == Color4:
                return Material((c.red,c.green,c.blue),transparency=c.alpha)
            elif type(c) == Color3:
                return Material(c)
            else:
                m = Material((c[0],c[1],c[2]))
                if len(c) >= 4: m.transparency = c[3]
                return m
        def hasColorPerObject(c):
            if type(c) == Color3 or type(c) == Color4 : return False 
            return type(color[0]) != int

        for curve, color, size in self.curves:      
            curvedata = map(projp3, curve)
            colorperobject = hasColorPerObject(color)
            result.append(Shape(ScreenProjected(Polyline(curvedata, width=size, colorList = None if not colorperobject else color), False), toMaterial(color) if not colorperobject else Material((255,0,0))))

        for pointdata, color, size in self.points:      
            pointdata = map(projp3, pointdata)
            colorperobject = hasColorPerObject(color)
            result.append(Shape(ScreenProjected(PointSet(pointdata, width=size, colorList = None if not colorperobject else color), False), toMaterial(color) if not colorperobject else Material((255,0,0)) ))

        y0 = projy(0)
        for values, color in self.histo:      
            delta = abs(projx(values[0][0])-projx(values[1][0]))/2
            colorperobject = hasColorPerObject(color)
            for i,v in enumerate(values):
                vx, vy = projp(v)
                p1,p2,p3,p4 = (vx-delta,y0),(vx+delta,y0),(vx+delta,vy),(vx-delta,vy) 
                result.append(Shape(ScreenProjected(QuadSet([(x,y,0.0) for x,y in [p1,p2,p3,p4]], [range(4)]), False), toMaterial(color if not colorperobject else color[i]) ))
                result.append(Shape(ScreenProjected(Translated(0,0,-0.01,Group([Polyline2D([pi,pj]) for pi,pj in [(p1,p4),(p1,p2),(p2,p3),(p3,p4)] ])), False), toMaterial((0,0,0)) ))

        if miny <= 0 <= maxy:
            cxtick = ((minx // self.xtick) * self.xtick)
            if cxtick < minx : cxtick += self.xtick
            g = []
            while cxtick <= maxx:
                if cxtick != 0:
                    g.append(Polyline2D([(projx(cxtick),projy(0)-self.ticklength),(projx(cxtick),projy(0)+self.ticklength)]))
                cxtick += self.xtick
            result.append(Shape(ScreenProjected(Group(g+[Polyline2D([(proj(minx,0)),(proj(maxx,0))])]),False), toMaterial((0,255,0)) ))
          
        if minx <= 0 <= maxx:
            cytick = ((miny // self.ytick)*self.ytick)
            if cytick < miny : cytick += self.ytick
            g = []
            while cytick <= maxy:
                if cytick != 0:
                    g.append(Polyline2D([(projx(0)-self.ticklength,projy(cytick)),(projx(0)+self.ticklength,projy(cytick))]))
                cytick += self.ytick
            result.append(Shape(ScreenProjected(Group(g+[Polyline2D([(proj(0,miny)),(proj(0,maxy))])]),False), toMaterial((0,0,255)) ))

        xlabeldecal = 0.005
        ylabeldecal = 0.02
        xtagorient = -1 if maxy <= 0 else 1
        ytagorient = -1 if maxx <= 0 else 1

        result.append(Shape(Group([
            ScreenProjected(Translated(projx(0)- xlabeldecal/2, projy(0)- xtagorient * ( 2 * self.ticklength + xlabeldecal), 0, Text(str(minx))),False),
            ScreenProjected(Translated(projx(maxx)- xlabeldecal/2, projy(0)- xtagorient * (2 * self.ticklength + xlabeldecal), 0, Text(str(maxx))),False),
            ScreenProjected(Translated(projx(0) - ytagorient * (2 * self.ticklength + ylabeldecal), projy(miny) - ylabeldecal/2, 0, Text(str(miny))),False),
            ScreenProjected(Translated(projx(0) - ytagorient * (2 * self.ticklength + ylabeldecal), projy(maxy) - ylabeldecal/2, 0, Text(str(maxy))),False)
            ]), toMaterial((0,0,0))))

        return Scene(result)

    def plot(self):
        from openalea.plantgl.gui import Viewer
        Viewer.display(self.toScene())

    def toproduction(self):
        from openalea.lpy import nsproduce, PglShape
        nsproduce([PglShape(self.toScene())])

def plot(curvedata, xextend = None, yextend = None, position = (0.1,-0.3), size = (0.8,0.8), xtick = None, ytick = None, withorigin = True ):
    d = DiagramPlotter(xextend, yextend , position, size , xtick, ytick, withorigin)
    if curvedata : d.add_curve(curvedata)
    d.plot()
    
