from PyQt4.QtCore import *
from PyQt4.QtGui import *
from openalea.plantgl.all import Scene, Viewer, Shape, Sphere, Material, Translated, Vector3, Box
from openalea.plantgl.scenegraph import *
from openalea.plantgl.math import Vector3,Matrix4,scaling


app = QApplication([])

def create_default_objects():
    yield AmapSymbol('../share/plantgl/database/amapsymbols/nentn105.smb')
    yield AsymmetricHull()
    ds = Sphere()
    yield AxisRotated((1,0,0),0,ds)
    bc = BezierCurve([(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)])
    yield bc
    yield BezierCurve2D([(0,0,1),(.5,1,1),(1.,1,1),(1.5,0,1)])
    yield Box()
    yield Cone()
    yield Cylinder()
    yield Disc()
    yield ElevationGrid([[0,0],[0,0]])
    yield EulerRotated(0,0,0,ds)
    circle = Polyline2D.Circle(1,20)
    yield ExtrudedHull(circle,circle)
    yield Extrusion(bc,circle)
    yield FaceSet([(0,0,0),(1,0,0),(1,1,0),(0,1,0)],[range(4)])
    yield Frustum()
    yield Group(ds,Translated((1,0,0),ds))
    yield IFS(0,[Transform4(Matrix4.translation((0,0,0)))],ds)
    yield IFS(0,[Transform4(Matrix4.translation((0,0,0)))],ds)
    yield NurbsCurve([(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)])
    yield NurbsCurve2D([(0,0,1),(.5,1,1),(1.,1,1),(1.5,0,1)])
    yield NurbsPatch([[(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)],[(0,0,1,1),(.5,1,1,1),(1.,1,1,1),(1.5,0,1,1)],[(0,0,3,1),(.5,1,3,1),(1.,1,3,1),(1.5,0,3,1)]],2,2)

def test_create_default_objects():
    Viewer.start()
    s = Scene()
    for i,geom in enumerate(create_default_objects()):
        assert geom.isValid()
        s += Shape(Translated((0,i*2,0),geom))
    Viewer.display(s)
    
if __name__ == '__main__':
    test_create_default_objects()
        
