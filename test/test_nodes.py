"""
:authors: Thomas cokelaer
"""
from openalea.core.alea import *
from PyQt4.QtGui import *

app = QApplication([])

pm = PackageManager()
pm.init(verbose=False)

from openalea.plantgl.all import *

def node(name, mytype=None):
    res = run(('vplants.plantgl.objects', name),{},pm=pm)
    if mytype == None:
        mytype = name
    exec('assert type(res[0]) == %s' % mytype)


def test_box():
    node('Box')
def test_sphere():
    node('Sphere')
def test_disc():
    node('Disc')
def test_cones():
    node('Cone')
def test_cylinder():
    node('Cylinder')
def test_colors():
    node('Color3')
    node('Color4')
def test_others():
    node('Material')
def test_others():
    node('Frustum')
def test_paraboloid():
    node('Paraboloid')
def test_faceset():
    node('FaceSet')
def test_scene():
    node('Scene')
def test_triangleset():
    node('TriangleSet')
def test_asymmetrichull():
    node('AsymmetricHull')
def test_quadset():
    node('QuadSet')
