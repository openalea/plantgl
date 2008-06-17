from openalea.plantgl.all import *

def overlay_with_cirle(p2i, visual = False):
    p1 = Polyline2D.Circle(2,32)
    p2 = Polyline2D([ Vector2(i.x,i.y) for i in discretize(p2i).pointList])
    res =  Overlay.process(p1,p2)    
    if visual:
        visres = res
        b = Scene([Shape(p1,Material((255,0,0),1)),Shape(p2i,Material((0,255,0),1))])
        Viewer.display(b)
        Viewer.dialog.question('given curves','given curves','Ok')
        if not visres is None:
            if not isinstance(visres,Group):
              visres = Group([visres])
            for i in visres:
                Viewer.display(b)
                Viewer.add(Shape(i,Material((0,0,255),1)))
                Viewer.dialog.question('results','one of the resulting overlay curves','Ok')
    return res

def test_two_circle(visual = False):
    if not Overlay.supportCGAL(): return
    res = overlay_with_cirle(Translated((-2.5,1,0),Polyline2D.Circle(1,32)),visual)
    assert not res is None and "overlay between 2 circles fail. Gives None."
    assert isinstance(res,Polyline2D) and "overlay between 2 circles fail. Gives multiple curves."

def test_circle_beziercurve(visual = False):
    if not Overlay.supportCGAL(): return
    res = overlay_with_cirle(BezierCurve2D([(1.2,0,1),(1,1.5,1),(-3,2,1),(-6,0,1),(-3,-2,1),(1,-1.5,1),(1.2,0,1)]),visual)
    assert not res is None and "overlay between circle and bezier curve fail. Gives None."
    assert isinstance(res,Polyline2D) and "overlay between circle and bezier curve  fail. Gives multiple curves."

def test_circle_beziercurve_two_bumps(visual = False):
    if not Overlay.supportCGAL(): return
    res = overlay_with_cirle(BezierCurve2D([(-3,0,1),(1,1.5,1),(-3,2,1),(-6,0,1),(-3,-2,1),(1,-1.5,1),(-3,0,1)]),visual)
    assert not res is None and "overlay between circle and bumpy bezier curve fail. Gives None."
    assert isinstance(res,Group) and len(res) == 2 and "overlay between circle and bumpy bezier curve fail. Gives not valid number of curves."

def test_circle_beziercurve_three_bumps(visual = False):
    if not Overlay.supportCGAL(): return
    res = overlay_with_cirle(NurbsCurve2D([(-2.5,0,1),(1,0.5,1),(-4,1,1),(1,1.5,1),(-3,2,1),(-6,0,1),(-3,-2,1),(1,-1.5,1),(-2.5,0,1)]),visual)
    assert not res is None and "overlay between circle and bumpy bezier curve fail. Gives None."
    assert isinstance(res,Group) and len(res) == 3 and "overlay between circle and bumpy bezier curve fail. Gives not valid number of curves."

def test_non_overlapping_circles(visual = False):
    if not Overlay.supportCGAL(): return
    res = overlay_with_cirle(Translated((-3.5,1,0),Polyline2D.Circle(1,32)),visual)
    assert res is None and "overlay between 2 non overlapping circles fail. Gives not None."

if __name__ == '__main__':
    import sys
    if '-v' in sys.argv or '--visual' in sys.argv :
        visual = True
    else:
        visual = False
    test_two_circle(visual)
    test_circle_beziercurve(visual)
    test_circle_beziercurve_two_bumps(visual)
    test_circle_beziercurve_three_bumps(visual)
    test_non_overlapping_circles(visual)
    if visual:
        Viewer.stop()
    