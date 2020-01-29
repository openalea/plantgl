import openalea.plantgl.math as mt
import openalea.plantgl.scenegraph as sg
# import openalea.plantgl.algo as alg

def gtsread(fname,glcoord = False):
    f = file(fname,'r')
    sizes = [int(i) for i in f.readline().split(' ')]
    pts = [None for i in range(sizes[0])]
    for itpoint in range(sizes[0]):
        point = [float(i) for i in f.readline().split(' ')]
        if glcoord:
            pts[itpoint] = (point[2],point[0],point[1])        
        else:
            pts[itpoint] = (point[0],point[1],point[2])        
    edges = [None for i in range(sizes[1])]
    for itedge in range(sizes[1]):
        edges[itedge] = [int(i) for i in f.readline().split(' ')]
    indices = [None for i in range(sizes[2])]
    a,b,c,d = 0,0,0,0
    for ittriangle in range(sizes[2]):
        edgeindex = [int(i) for i in f.readline().split(' ')]
        if (edges[edgeindex[0]-1][0] == edges[edgeindex[1]-1][0]):
            a+=1
            index = [edges[edgeindex[0]-1][0],edges[edgeindex[2]-1][0],edges[edgeindex[2]-1][1]]
        else :
            if (edges[edgeindex[0]-1][0] == edges[edgeindex[1]-1][1]):
                b += 1
                index = [edges[edgeindex[0]-1][0],edges[edgeindex[0]-1][1],edges[edgeindex[1]-1][0]]
            else:
                if (edges[edgeindex[0]-1][1] == edges[edgeindex[1]-1][0]):
                    c += 1
                    index = [edges[edgeindex[0]-1][0],edges[edgeindex[0]-1][1],edges[edgeindex[1]-1][1]]
                elif (edges[edgeindex[0]-1][1] == edges[edgeindex[1]-1][1]):
                    d += 1
                    index = [edges[edgeindex[2]-1][0],edges[edgeindex[2]-1][1],edges[edgeindex[0]-1][1]]
        indices[ittriangle] = (index[0]-1,index[1]-1,index[2]-1)
    return sg.TriangleSet(pts,indices)

class GtsCodec (sg.SceneCodec):
    def __init__(self):
        sg.SceneCodec.__init__(self,"GTS",sg.SceneCodec.Mode.Read)
    def formats(self):
        return [ sg.SceneFormat("Gts Codec",["gts"],"The Gnu Triangulated Surface library file format") ]
    def read(self,fname):
        t = gtsread(fname)
        return sg.Scene([sg.Shape(t)])        
    def write(self,fname,scene):
        pass
codec = GtsCodec()
sg.SceneFactory.get().registerCodec(codec)
    
