from openalea.plantgl.all import *
from math import floor, pi

def order_points(p0,p1,dim):
    if p0[dim] < p1[dim] : return (p0,p1)
    else : return (p1,p0)
    
def polygon_in_grid(grid,pts, maxdim = None):
    nbdim = maxdim if not maxdim is None else grid.nbDimensions()
    origin = grid.getOrigin()
    npts = [pi - origin for pi in pts]
    voxelsize = grid.getVoxelSize()
    
    cdim = 0
    partialindex = [0 for i in xrange(nbdim)]

    def process_unigrid(unigrid, cdim):
        result = []
        
        
        for j in xrange(1,len(unigrid)):
            ci = unigrid[j-1]
            cj = unigrid[j]
            pia = list(ci[1])
            pia.reverse()
            pib = ci[2]
            pja = cj[1]
            pjb = ci[3]
            polygon = pia+pib+pja+pjb
            if cdim == nbdim:
                result.append((ci[0],[p+origin for p in polygon]))
            else:
                result += process_polygon(polygon, cdim, ci[0])
        if len(unigrid[-1][2]) > 0 or len(unigrid[-1][3]) > 0 :
            ci = unigrid[-1]
            pia = list(ci[1])
            pia.reverse()
            pib = ci[2]
            pic = ci[3]
            polygon = pia + pib + pic
            if cdim == nbdim:
                result.append((ci[0],[p+origin for p in polygon]))
            else:
                result += process_polygon(polygon, cdim, ci[0])

        return result
        
    def process_polygon(pts, cdim,partialindex):
        vsize = voxelsize[cdim]
        cindex = lambda pt : int(floor(pt[cdim] / vsize)) 
        ptList = list(pts)
        ptList.reverse()
        indices = map(cindex,ptList)
        minindex = min(indices)
        maxindex = max(indices)
        # order list of point to start from the one with minimum coord in cdim
        pos = indices.index(minindex)        
        if pos > 0:
            ptList = ptList[pos:]+ptList[:pos]
            
        def update_gindex(pindex,cdim,value):
            pindex = list(pindex)
            pindex[cdim] = value
            return pindex
        
        if maxindex == minindex:
            unigrid = [(update_gindex(partialindex,cdim,minindex),[],pts,[])]
        if maxindex > minindex:
            nbIntervals = maxindex-minindex+1
            unigrid = [(update_gindex(partialindex,cdim,minindex+i),[],[],[]) for i in xrange(nbIntervals)]
            edges =[(ptList[i],ptList[i+1]) for i in xrange(len(ptList)-1)]+[(ptList[-1],ptList[0])]
            eid = 0
            forward = True
            for pi, pj in edges:
                
                pii, pjj = order_points(pi,pj,cdim)
                iindex = cindex(pii)
                jindex = cindex(pjj)
                # We add intersection points
                if iindex < jindex:
                    delta = pjj - pii
                    for idx in xrange(iindex+1,jindex+1):            
                        alpha = (idx*vsize - pii[cdim])/(pjj[cdim] - pii[cdim])
                        if 0 < alpha < 1:
                            pni = pii + alpha * delta
                            unigrid[idx-minindex][1].append(pni)
                
                # We add first point of each edge in the structure
                pindex = cindex(pj)
                ploc = 1
                if pj[cdim] - pindex * vsize > 0:
                    ploc = (2 if forward else 3)
                unigrid[pindex-minindex][ploc].append(pj)
                if jindex == maxindex :
                    forward = False
                eid += 1
                
        return process_unigrid(intersections,cdim+1)
            
    return process_polygon(npts,cdim,partialindex)

    
def scene_in_grid(sc, grid):    
    d = Discretizer()
    
    groups = dict()
    for sh in sc:
        d = Discretizer()
        sh.apply(d)
        tr = d.result
        if tr:
            for idx in tr.indexList:
                polygons = polygon_in_grid(grid,[tr.pointList[i] for i in idx])
                for gidx,pol in polygons:
                    groups[tuple(gidx)] = groups.get(tuple(gidx),[])+[(sh.id,FaceSet(pol,[range(len(pol))]))]
    return groups
    
def test():
    class FakeGrid():
        def __init__(self): pass
        def nbDimensions(self) : return 3
        def getOrigin(self): return Vector3(0,0,0)
        def getVoxelSize(self): return Vector3(0.1,0.1,0.1)
        
    pts = [Vector3(0.5,0.5,0.5),Vector3(1.2,1.1,0.2),Vector3(1.0,0.7,0.5)]
    #pts = [Vector3(0.5,0.5,0.5),Vector3(0.95,0.2,0.5),Vector3(1.4,0.5,0.5),Vector3(0.96,0.7,0.5)]
    # pts = [Vector3(0.95,0.2,0.5),Vector3(1.4,0.5,0.5),Vector3(0.96,0.7,0.5),Vector3(0.5,0.5,0.5)]
    polygons = polygon_in_grid(FakeGrid(),pts)
    s = Scene()
    for idx,pol in polygons:
        if len(pol) <= 2: print('Wrong polygon :', pol)
        else : s.add(Shape(Oriented((0,0,1),(0,1,0),FaceSet(pol,[range(len(pol))])),Material((255,0,0))))
    Viewer.display(s)
    Viewer.add(Shape(Oriented((0,0,1),(0,1,0),Translated((0,0,1),FaceSet(pts,[range(len(pts))]))),Material((0,255,0))))

if __name__ == '__main__':
    test()