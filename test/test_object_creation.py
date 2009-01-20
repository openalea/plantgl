from openalea.plantgl.all import *


def create_default_objects():
    yield AmapSymbol('../share/plantgl/database/amapsymbols/nentn105.smb')
    yield AsymmetricHull()
    bc = BezierCurve([(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)])
    yield bc
    bc2 = BezierCurve2D([(0,0,1),(.5,1,1),(1.,1,1),(1.5,0,1)])
    yield bc2
    cpts = [[(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)],
            [(0,0,1,1),(.5,1,1,1),(1.,1,1,1),(1.5,0,1,1)],
            [(0,0,3,1),(.5,1,3,1),(1.,1,3,1),(1.5,0,3,1)]]
    yield BezierPatch(cpts)
    yield Box()
    yield Cone()
    yield Cylinder()
    yield Disc()
    yield ElevationGrid([[0,0],[0,0]])
    circle = Polyline2D.Circle(1,20)
    yield ExtrudedHull(circle,circle)
    yield Extrusion(bc,circle)
    yield FaceSet([(0,0,0),(1,0,0),(1,1,0),(0,1,0)],[range(4)])
    yield Frustum()
    yield NurbsCurve([(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)])
    yield NurbsCurve2D([(0,0,1),(.5,1,1),(1.,1,1),(1.5,0,1)])
    yield NurbsPatch(cpts,2,2)
    yield Paraboloid()
    yield PointSet([(0,0,0)])
    yield PointSet2D([(0,0)])
    yield Polyline([(0,0,0),(1,0,0)])
    yield Polyline2D([(0,0),(1,0)])
    yield QuadSet([(0,0,0),(1,0,0),(1,1,0),(0,1,0)],[range(4)])
    yield Revolution(bc2)
    yield Sphere()
    yield Swung([bc2],[0])
    yield Text('test')
    yield TriangleSet([(0,0,0),(1,0,0),(0,1,0)],[range(3)])

def create_default_transforms():
    ds = Sphere()
    yield AxisRotated((1,0,0),0,ds)
    yield EulerRotated(0,0,0,ds)
    yield IFS(0,[Transform4(Matrix4.translation((0,0,0)))],ds)
    yield Oriented(Oriented.DEFAULT_PRIMARY,Oriented.DEFAULT_SECONDARY,ds)
    yield Scaled(1,ds)
    yield Tapered(1.,1.,ds)
    yield Translated(0,0,0,ds)
    yield Group(ds,Translated((1,0,0),ds))

def create_default_shapes():
    m = Material()
    ds = Sphere()
    yield Shape(ds,m)
    t = ImageTexture('../share/plantgl/pixmap/geomviewer.png')
    yield Shape(Cylinder(),t)

def create_default_scene():
    s = Scene()
    for i,geom in enumerate(create_default_objects()):
        assert geom.isValid()
        s += Shape(Translated((0,i*2,0),geom))
    for i,geom in enumerate(create_default_transforms()):
        assert geom.isValid()
        s += Shape(Translated((5,i*2,0),geom))
    for i,sh in enumerate(create_default_shapes()):
        assert sh.isValid()
        sh.geometry = Translated((10,i*2,0),sh.geometry)
        s += sh
    return s

def test_create_default_objects():
    s = create_default_scene()
    d = Discretizer()
    s.apply(d)
    
def view_create_default_objects():
    Viewer.start()
    Viewer.display(create_default_scene())
    Viewer.stop()

if __name__ == '__main__':
    test_create_default_objects()
    pass
    
from random import uniform, randint

maxdim = 20
maxshape = 100
maxelem = 20

def randtuple(nbvalues = 3,minvalue = 0,maxvalue = maxdim):
    return tuple([uniform(minvalue,maxvalue) for i in xrange(nbvalues)])

def randinttuple(nbvalues = 3,minvalue = 0,maxvalue = maxdim):
    return tuple([randint(minvalue,maxvalue) for i in xrange(nbvalues)])

def randuninttuple(nbvalues = 3,minvalue = 0,maxvalue = maxdim):
    assert nbvalues < (maxvalue - minvalue)
    t = []
    while len(t) < nbvalues:
        v = randint(minvalue,maxvalue)
        if not v in t:
            t.append(v)
    return tuple(t)

def randbool(): return bool(randint(0,1))

from math import pi

def randcircle(nbpoints, minradius = 0.1, maxradius = maxdim, minangle = -pi/2, maxangle = 3*pi/2):
    return Polyline2D([Vector2(Vector2.Polar(uniform(minradius,maxradius),minangle+k*(maxangle-minangle)/(nbpoints-1))) for k in xrange(nbpoints)])

def randhalfcircle(nbpoints, minradius = 0.1, maxradius = maxdim):
    return randcircle(nbpoints, minradius, maxradius, maxangle = pi/2)
    
def create_random_objects():
    yield AsymmetricHull(negXRadius = uniform(0,maxdim), posXRadius = uniform(0,maxdim), 
                         negYRadius = uniform(0,maxdim), posYRadius = uniform(0,maxdim),
                         negXHeight = uniform(0,maxdim), posXHeight = uniform(0,maxdim), 
                         negYHeight = uniform(0,maxdim), posYHeight = uniform(0,maxdim),
                         bottom = randtuple(), top = randtuple(), bottomShape = uniform(0.1,maxshape), topShape = uniform(0.1,maxshape),
                         slices = randint(1,255), stacks = randint(4,255))
    bc = BezierCurve([randtuple(4) for i in xrange(randint(3,maxelem))])
    yield bc
    bc2 = BezierCurve2D([randtuple(3) for i in xrange(randint(3,maxelem))])
    yield bc2
    nbrow = xrange(randint(2,maxelem))
    yield BezierPatch([[randtuple(4) for i in nbrow] for j in xrange(randint(2,maxelem))])
    yield Box(size = randtuple())
    yield Cone(radius = uniform(0.1,maxdim), height = uniform(0.1,maxdim), solid = randbool(), slices = randint(4,255))
    yield Cylinder(radius = uniform(0.1,maxdim), height = uniform(0.1,maxdim), solid = randbool(), slices = randint(4,255))
    yield Disc(radius = uniform(0.1,maxdim),slices = randint(4,255))
    nbrow = xrange(randint(2,maxelem))
    yield ElevationGrid([[uniform(-maxdim,maxdim) for i in nbrow] for j in xrange(randint(2,maxelem))])
    yield ExtrudedHull(randcircle(randint(10,maxelem)),randcircle(randint(10,maxelem)))
    yield Extrusion(BezierCurve([randtuple(4) for i in xrange(randint(3,maxelem))]),randcircle(randint(10,maxelem)))
    dim = randint(10,maxelem)
    yield FaceSet([randtuple() for i in xrange(dim)],[randuninttuple(randint(3,8),0,dim-1) for i in xrange(10,maxelem)])
    yield Frustum(radius = uniform(0.1,1), height = uniform(0.1,maxdim), taper = uniform(0,maxdim), solid = randbool(), slices = randint(4,255))
    nbp = randint(3,maxelem)
    yield NurbsCurve([randtuple(4) for i in xrange(nbp)], randint(2,min(5,nbp-1)))
    nbp = randint(3,maxelem)
    yield NurbsCurve2D([randtuple() for i in xrange(nbp)], randint(2,min(5,nbp-1)))
    nbrow = randint(3,maxelem)
    nbcol = randint(3,maxelem)
    yield NurbsPatch([[randtuple(4) for i in xrange(nbrow)] for j in xrange(nbcol)], randint(2,min(5,nbcol-1)), randint(2,min(5,nbrow-1)))
    yield Paraboloid(radius = uniform(0.1,maxdim), height = uniform(0.1,maxdim), shape = uniform(0.1,maxshape), solid = randbool(), slices = randint(4,255), stacks = randint(4,255))
    yield PointSet([randtuple() for i in xrange(randint(1,maxelem))])
    yield PointSet2D([randtuple(2) for i in xrange(randint(2,maxelem))])
    yield Polyline([randtuple() for i in xrange(randint(2,maxelem))])
    yield Polyline2D([randtuple(2) for i in xrange(randint(2,maxelem))])
    yield QuadSet([randtuple() for i in xrange(dim)],[randuninttuple(4,0,dim-1) for i in xrange(10,maxelem)])
    yield Revolution(randhalfcircle(randint(4,20)),slices = randint(4,255))
    yield Sphere(radius = uniform(0.1,maxdim),slices = randint(4,255), stacks = randint(4,255))
    dim = randint(10,maxelem)
    ang = [uniform(0,1) for i in xrange(dim)]
    sa = sum(ang)
    anglist = [2*pi*sum(ang[0:i+1])/sa for i in xrange(dim)]
    nbpts = randint(5,10)
    pflist = [randhalfcircle(nbpts) for i in xrange(dim)]
    yield Swung(pflist,anglist)
    yield Text('test2',position=randtuple(),screencoordinates=randbool(),fontstyle=Font(family="courrier new",size=randint(6,20),bold=randbool(),italic=randbool()))
    yield TriangleSet([randtuple() for i in xrange(dim)],[randuninttuple(3,0,dim-1) for i in xrange(10,maxelem)])

rdtransgen = {0 : (lambda : Matrix4.translation(randtuple(maxvalue=1))),
              1 : (lambda : Matrix4(Matrix3.scaling(randtuple(maxvalue=1)))),
              2 : (lambda : Matrix4(Matrix3.axisRotation(randtuple(), uniform(0,2*pi)))),
              3 : (lambda : Matrix4(Matrix3.eulerRotationZYX((uniform(0,2*pi),uniform(0,2*pi),uniform(0,2*pi))))),
              }    
def randtransform():
    return rdtransgen[randint(0,3)]()
    
def create_random_transforms():
    yield AxisRotated(randtuple(),uniform(0,2*pi),Sphere(radius = uniform(0.1,maxdim),slices = randint(4,255), stacks = randint(4,255)))
    yield EulerRotated(uniform(0,2*pi),uniform(0,2*pi),uniform(0,2*pi),Sphere(radius = uniform(0.1,maxdim),slices = randint(4,255), stacks = randint(4,255)))
    yield IFS(randint(0,3),[Transform4(randtransform()) for i in xrange(1,4)],Sphere(radius = uniform(0.1,1),slices = randint(4,255), stacks = randint(4,255)))
    prim = Vector3(randtuple())
    prim.normalize()
    sec = Vector3(randtuple())
    sec.normalize()    
    sec = cross(prim,sec)
    yield Oriented(prim,sec,Sphere(radius = uniform(0.1,maxdim),slices = randint(4,255), stacks = randint(4,255)))
    yield Scaled(randtuple(),Sphere(radius = uniform(0.1,1),slices = randint(4,255), stacks = randint(4,255)))
    yield Tapered(uniform(0.1,maxdim),uniform(0.1,maxdim),Sphere(radius = uniform(0.1,1),slices = randint(4,255), stacks = randint(4,255)))
    yield Translated(randtuple(),Sphere(radius = uniform(0.1,maxdim),slices = randint(4,255), stacks = randint(4,255)))
    yield Group([Translated(randtuple(),Sphere(radius = uniform(0.1,maxdim),slices = randint(4,255), stacks = randint(4,255))) for i in xrange(2,maxelem)])


def randmaterial():
    amb = randinttuple(3,0,255)
    diffuse = float(randint(0,255)) / max(amb)    
    return Material(ambient=amb,diffuse=diffuse,specular=randinttuple(3,0,255),emission=randinttuple(3,0,255),shininess= uniform(0,1),transparency=uniform(0,1))    

def create_random_scene():
    s = Scene()
    for i,geom in enumerate(create_random_objects()):
        assert geom.isValid()
        s += Shape(Translated((0,i*maxdim,0),geom),randmaterial())
    for i,geom in enumerate(create_random_transforms()):
        assert geom.isValid()
        s += Shape(Translated((maxdim*2,i*maxdim,0),geom),randmaterial())
    return s

def test_create_random_objects():
    s = create_default_scene()
    d = Discretizer()
    s.apply(d)
    
def view_create_random_objects(nb = 5):
    Viewer.start()
    for i in xrange(nb):
        Viewer.display(create_random_scene())
    Viewer.stop()
    
if __name__ == '__main__':
    view_create_random_objects()
        
