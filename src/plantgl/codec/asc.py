


import openalea.plantgl.math as mt
import openalea.plantgl.scenegraph as sg
import openalea.plantgl.algo as alg

class AscCodec (sg.SceneCodec):
    """ todo 

    """

    def __init__(self):
        """
        todo
        """
        sg.SceneCodec.__init__(self,"ASC",sg.SceneCodec.Mode.ReadWrite)

    def formats(self):
        """ todo """
        return [ sg.SceneFormat("Asc Codec",["asc"],"The Ascii point file format") ]

    def read(self,fname):
        """ todo """
        pts = []
        col = []
        f = file(fname,"r")
        for line in f.readlines():
            values = line.split()
            pts.append(mt.Vector3(float(values[0]),float(values[1]),float(values[2])))
            if len(values) > 3:
                col.append(sg.Color4(int(values[3]),int(values[4]),int(values[5]),0))
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
        """ todo """
        print("Write "+fname)
        d = alg.Discretizer()
        f = file(fname,'w')
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
                        f.write(str(col.red)+' '+str(col.green)+' '+str(col.blue)+'\n')
        f.close()
    

codec = AscCodec()
sg.SceneFactory.get().registerCodec(codec)
