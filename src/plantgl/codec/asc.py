import openalea.plantgl.math as mt
import openalea.plantgl.scenegraph as sg
import openalea.plantgl.algo as alg

def rgb2intensity(c) : return c.getRGBAverage()

class AscCodec (sg.SceneCodec):
    """ Ascii Point File Format 

    """

    def __init__(self):
        """
        Initialisation of the codec info
        """
        sg.SceneCodec.__init__(self,"ASC",sg.SceneCodec.Mode.ReadWrite)

    def formats(self):
        """ return formats """
        return [ sg.SceneFormat("Asc Codec",["asc","pts"],"The Ascii point file format") ]

    def read(self,fname):
        """ read an ascii point file """
        import warnings
        pts = []
        col = []
        isptsfile = ('.pts' in fname)
        f = file(fname,"r")
        if isptsfile:
            f.readline()
        for i,line in enumerate(f.readlines()):
            values = line.split()
            try:
                pts.append(mt.Vector3(float(values[0]),float(values[1]),float(values[2])))
                if len(values) > 3:
                    if not isptsfile : col.append(sg.Color4(int(values[3]),int(values[4]),int(values[5]),0))
                    else : col.append(sg.Color4(int(values[4]),int(values[5]),int(values[6]),0))
            except Exception,e:
                if isptsfile and len(values) == 1:
                    warnings.warn("Skip line "+str(i+isptsfile)+" in file '"+fname+"'.")
                else:
                    warnings.warn("Error in file '"+fname+"' at line "+str(i+isptsfile))
                    raise e
        f.close()
        pts = sg.Point3Array(pts)
        center = pts.getCenter()
        if len(col) == len(pts) :
            pointset = sg.PointSet(pts,col)
        else:
            pointset = sg.PointSet(pts)
        pointset = sg.Translated(-center,pointset)
        sc = sg.Scene()
        sc += pointset
        return sc

    def write(self,fname,scene):
        """ write an ascii point file """
        print("Write "+fname)
        d = alg.Discretizer()
        f = file(fname,'w')
        isptsfile = ('.pts' in fname)        
        for i in scene:
            if i.apply(d):
                p = d.discretization
                if isinstance(p,sg.PointSet) :
                    hasColor = not p.colorList is  None and len(p.colorList) > 0
                    col = i.appearance.ambient
                    for i,pt in enumerate(p.pointList):
                        f.write(str(pt.x)+' '+str(pt.y)+' '+str(pt.z)+' ')                        
                        if hasColor:                          
                            col = p.colorList[i]
                        if isptsfile:
                            f.write(str(rgb2intensity(col)))
                        f.write(str(col.red)+' '+str(col.green)+' '+str(col.blue)+'\n')
        f.close()
    

codec = AscCodec()
sg.SceneFactory.get().registerCodec(codec)
