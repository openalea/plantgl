from openalea.plantgl.all import *


methods = [eStarTriangulation, eConvexTriangulation, eGreeneTriangulation, eOptimalTriangulation, eYMonotonePartitioning]

def test_concave(display = False):
    s = Scene('concavecontour.bgeom')
    pts2 = s[0].geometry.pointList
    for m in methods:
        tr = polygonization(pts2,m)
        if display:
            Viewer.display(FaceSet([Vector3(i,0) for i in pts2],tr))
            Viewer.dialog.question('Polygonization', 'Method : '+str(m))

def test_triangulation_concave(display = False):
    s = Scene('concavecontour.bgeom')
    pts2 = s[0].geometry.pointList
    for m in methods:
        tr = triangulation(pts2,m)
        if display:
            Viewer.display(TriangleSet([Vector3(i,0) for i in pts2],tr))
            Viewer.dialog.question('Triangulation', 'Method : '+str(m))

def test_concave_selfintersecting(display = False):
    s = Scene('concavecontourselfintersect.bgeom')
    pts2 = s[0].geometry.pointList
    for m in methods:
        tr = polygonization(pts2,m)
        if display:
            Viewer.display(FaceSet([Vector3(i,0) for i in pts2],tr))
            Viewer.dialog.question('Polygonization', 'Method : '+str(m))

def test_concave2(display = False):
    s = Scene('concavecontour.bgeom')
    pts2 = s[0].geometry.pointList    
    tr = Skeleton.getDelaunayConstrained2DTriangulation(Polyline2D(pts2))
    if display:
        Viewer.display(tr)
        Viewer.dialog.question('Triangulation', 'Method : Delaunay')

if __name__ == '__main__':
    test_concave(True)
    test_concave_selfintersecting(True)
    test_concave2(True)