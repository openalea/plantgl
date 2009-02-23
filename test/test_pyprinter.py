from openalea.plantgl.all import *
from math import pi

printer = PyStrPrinter()

def eval_code(sceneobj):
  if len(sceneobj.name) == 0:
	  sceneobj.name = 'testedobject'
  sceneobj.apply(printer)
  code = printer.str()
  print code
  dic = {}
  exec code in globals(),dic
  assert dic.has_key(sceneobj.name) and "evaluated object not found"
  eval_object = dic[sceneobj.name]
  assert type(eval_object) == type(sceneobj) and eval_object.name == sceneobj.name
  return eval_object
  
def exec_code(printer):
  code = printer.str()
  print code
  exec(code)

def test_sphere():
  sphere = Sphere(radius=2)
  sphere.stacks = 3
  res = eval_code(sphere)
  assert res.stacks == sphere.stacks
  
def test_bezcurve():
  bezier = BezierCurve(Point4Array([(1,1,1,1),(2,2,2,1),(3,3,3,1),(4,4,4,1)]))
  eval_code(bezier)
     
def test_box():
  box = Box(Vector3(1,1,1))
  eval_code(box)
 
def test_translated():
  translate = Translated(Vector3(), Sphere())
  eval_code(translate)

def test_polyline():
  polyline = Polyline(Point3Array([(0,0,0),(1,1,0),(2,3,4),(4,5,6)]))
  res = eval_code(polyline)
  for i,v in enumerate(polyline.pointList):
    assert res.pointList[i] == v
  
def test_polyline2d():
  polyline2d = Polyline2D(Point2Array([(0,0),(1,1)]))
  eval_code(polyline2d)
 
def test_pointset():
  pointset = PointSet(Point3Array([(0,0,0),(1,1,1)]))
  pointset.colorList = [(1,1,1,1),(2,2,2,2)]
  eval_code(pointset)

def test_pointset2d():
  pointset2d = PointSet2D(Point2Array([(0,0),(1,1)]))
  eval_code(pointset2d)
  
def test_scaled():
  scale = Scaled(Vector3(0.5,0.25,0.1), Sphere(radius=3))
  eval_code(scale)
  
def test_shape():
  sphere = Sphere()
  sphere.radius = 2
  
  m1 = Material(ambient=(255,0,0))
  m1.diffuse = 2
  m1.emission = Color3(40,50,60)
  t1 = Translated((-5,0,0), sphere)
  shape1 = Shape(t1, m1)

  eval_code(shape1)

def test_scene():
  sc = Scene()
  sphere = Sphere()
  sphere.radius = 2
  
  m1 = Material(ambient=(255,0,0))
  m1.diffuse = 2
  m1.emission = Color3(40,50,60)
  t1 = Translated((-5,0,0), sphere)
  shape1 = Shape(t1, m1)
  sc += shape1
  
  m2 = Material(ambient=(0,255,0))
  m2.transparency = 0.5
  t2 = Scaled((0.5,0.25,0.1), sphere)
  shape2 = Shape(t2, m2)
  sc += shape2
  
  shape3 = Shape()
  shape3.appearance = m2
  sc += shape3

#  these lines fails. need to be fixed. TC. jan 2009
#  eval_code(sc)
#  exec_code(printer)

def test_nurbscurve():
  point = [(0,0,1),(4,2,1),(2,4,1),(4,6,1),(0,8,1)]
  nc = NurbsCurve2D(ctrlPointList = Point3Array([(0,0,1),(4,2,1),(2,4,1),(4,6,1),(0,8,1)]))  
  nc.degree = 4
  nc.knotList = RealArray([0.0, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 5.0])
  #nc.setKnotListToDefault()
  nc.stride = 3001
  eval_code(nc)
  
def test_cone():
  cone = Cone(radius=2, height=10)
  cone.solid = False
  cone.slices = 5
  cone.apply(printer)
  eval_code(cone)

def test_cylinder():
  cy = Cylinder(radius=2)
  cy.solid = False
  cy.slices = 5
  eval_code(cy)
  
def test_asymmetricHull():
  posXRadius = 1 
  posYRadius = 1 
  negXRadius = 1 
  negYRadius = 1.5
  posXHeight = 1 
  posYHeight = 1 
  negXHeight = 1.5 
  negYHeight = 1.2
  top = (0,0.5,5) 
  bottom = (0,0,0)
  topShape = 1.5 
  bottomShape = 0.8
  slices = 5 
  stacks = 40
  asym = AsymmetricHull(negXRadius, posXRadius, negYRadius, posYRadius, negXHeight, posXHeight, negYHeight, posYHeight,
                     bottom, top, bottomShape, topShape, slices, stacks)
  eval_code(asym)

def test_axisrotate():
  axis = AxisRotated(axis=Vector3(0,1,1), angle=1.8, geometry=Sphere(radius=3))
  eval_code(axis)

def test_eulerrotate():
  euler = EulerRotated(azimuth=1, elevation=2, roll=3, geometry=Sphere(radius=3))  
  eval_code(euler)
  
def test_extrudehull():
  p = [(0,0,1),(1,1,1),(2,1,1),(3,2,1),(4,2,1),(4,3,1),(5,4,1),(4,5,1),(4,6,1),(2,8,1),(1,8,1),(0,10,1),(-2,8,1),(-2,7,1),(-3,7,1),(-3,6,1),(-5,5,1),(-4,4,1),(-3,3,1),(-3,2,1),(0,0,1)]
  nc = NurbsCurve2D(p) 
  pc = Polyline2D.Circle(5,16)
  extrude = ExtrudedHull(nc, pc)
  eval_code(extrude)

def test_swung():
  p1 = [(0,0,0),(4,2,0),(2,4,0),(4,6,0),(0,8,0)]
  p2 = [(0,0,0),(2,1,0),(1,2,0),(4,3,0),(1,4,0),(4,5,0),(0,8,0)]
  p3 = [(0,0,0),(4,1,0),(1,3,0),(4,5,0),(0,8,0)]
  p4 = [(0,0,0),(4,1,0),(2,2,0),(4,3,0),(2,4,0),(4,5,0),(2,6,0),(4,7,0),(0,8,0)]
  b1 = BezierCurve2D(p1)
  b2 = BezierCurve2D(p2)
  b3 = BezierCurve2D(p3)
  b4 = BezierCurve2D(p4)
  
  p21 = [(0,0,1),(4,2,1),(2,4,1),(4,6,1),(0,8,1)]
  p22 = [(0,0,1),(2,1,1),(1,2,1),(4,3,1),(1,4,1),(4,5,1),(0,8,1)]
  p23 = [(0,0,1),(4,1,1),(1,3,1),(4,5,1),(0,8,1)]
  p24 = [(0,0,1),(4,1,1),(2,2,1),(4,3,1),(2,4,1),(4,5,1),(2,6,1),(4,7,1),(0,8,1)]
  nc1 = NurbsCurve2D(p21)
  nc2 = NurbsCurve2D(p22)
  nc3 = NurbsCurve2D(p23)
  nc4 = NurbsCurve2D(p24)
  
  an = [0,pi/2.,3*pi/2,2*pi,0,pi/2.,3*pi/2,2*pi]
  ls = [nc1,nc2,nc3,nc4,b1,b2,b3,b4] 
  swung = Swung(ls,an, 10, True, 18, 20)
  #swung = Swung(profiles=[nc1,nc2,nc3,nc4,b1,b2,b3,b4] , angles = [0,pi/2.,3*pi/2,2*pi,0,pi/2.,3*pi/2,2*pi])
  eval_code(swung)

def test_beziercurve2d():
  bc = BezierCurve2D([(0,0,0),(4,2,0),(2,4,0),(4,6,0),(0,8,0)])
  eval_code(bc)

def test_nurbscurve2d():
  nc = NurbsCurve2D([(0,0,1),(2,1,1),(1,2,1),(4,3,1),(1,4,1),(4,5,1),(0,8,1)])  
  eval_code(nc)
  
def test_bezierpatch():
  bp = BezierPatch([[(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)],[(0,0,1,1),(.5,1,1,1),(1.,1,1,1),(1.5,0,1,1)],[(0,0,3,1),(.5,1,3,1),(1.,1,3,1),(1.5,0,3,1)]])
  eval_code(bp)
  
def test_nurbscurve():
  nc = NurbsCurve([(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)])
  eval_code(nc)
  
def test_nurbspatch():
  np = NurbsPatch([[(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)],[(0,0,1,1),(.5,1,1,1),(1.,1,1,1),(1.5,0,1,1)],[(0,0,3,1),(.5,1,3,1),(1.,1,3,1),(1.5,0,3,1)]])
  np.vdegree = 10
  np.ustride = 20
  eval_code(np);
  
def test_quadset():
  point3 = Point3Array([(0,0,0),(1,0,0),(1,1,0),(0,1,0),(0,0,1),(1,0,1),(1,1,1),(0,1,1)])
  index4 = Index4Array([(0,1,2,3),(0,3,7,4),(1,0,4,5),(2,1,5,6),(3,2,6,7),(4,7,6,5)])
  color4 = [(255,0,0,0),(0,255,0,0),(0,0,255,0),(255,255,0,0)]
  colorindex = [(0,1,2,3), (0,1,2,3),(0,1,2,3), (0,1,2,3),(0,1,2,3), (0,1,2,3)]
  qs = QuadSet(point3, index4)
  qs.colorPerVertex = True
  qs.colorList = color4
  qs.colorIndexList =  colorindex
  eval_code(qs)

def test_triangleset():
  point3 = Point3Array([(0,0,0),(1,0,0),(1,1,0),(0,1,0),(0,0,1),(1,0,1),(1,1,1),(0,1,1)])
  index3 = Index3Array([(0,1,2),(0,3,7),(1,0,4),(2,1,5),(3,2,6),(4,7,6)])
  color4 = [(255,0,0,0),(0,255,0,0),(0,0,255,0),(255,255,0,0)]
  colorindex = [(0,1,2), (0,1,2),(0,1,2), (0,1,2),(0,1,2), (0,1,2)]
  tri = TriangleSet(point3, index3)
  tri.colorPerVertex = True
  tri.colorList = color4
  tri.colorIndexList =  colorindex
  eval_code(tri)

def test_group():
  group = Group([Sphere(),Translated((1,0,0),Sphere())])
  eval_code(group)
 
def test_material():
  mat = Material(ambient=Color3(100,200,0), diffuse=2, specular=Color3(0,100,200))
  eval_code(mat)
  
def test_disc():
  disc = Disc(2,3)
  eval_code(disc)
  
"""
def test_imagetex():
  imagetex = ImageTexture(r"C:\Documents and Settings\admin\Mes documents\Develop\Practice\bark.png")
  imagetex.apply(printer)
  eval_code(imagetex)
"""


if __name__ == '__main__':
    #test_quadset()
    import traceback as tb
    test_func = [ (n,v) for n,v in globals().items() if 'test' in n]
    test_func.sort(lambda x,y : cmp(x[1].func_code.co_firstlineno,y[1].func_code.co_firstlineno))
    for tfn,tf in test_func:
        print tfn
        try:
            tf()
        except:
            tb.print_exc()