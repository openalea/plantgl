"""
:authors: Thomas cokelaer
"""

from openalea.core.alea import *
from openalea.vpltk.qt import QtGui

app = QtGui.QApplication([])

pm = PackageManager()
pm.init(verbose=False)



def test_demo_quickstart():
    res = run(('vplants.plantgl.demos', 'quickstart'),{},pm=pm)
    assert res == []

def test_demo_hull():
    res = run(('vplants.plantgl.demos', 'hull'),{},pm=pm)
    assert res == []

def test_demo_texture():
    res = run(('vplants.plantgl.demos', 'texture'),{},pm=pm)
    assert res == []

def test_demo_translation_and_scene():
    res = run(('vplants.plantgl.demos', 'translation and scene'),{},pm=pm)
    assert res == []

def test_demo_quadset():
    res = run(('vplants.plantgl.demos', 'quadset'),{},pm=pm)
    assert res == []
