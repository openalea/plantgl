#! /usr/bin/env python
# -*- python -*-
#
#       OpenAlea.PlantGL.ext : PlantGL external utilities
#
#       Copyright or (C) or Copr. 2006 INRIA - CIRAD - INRA  
#
#       File author(s): Da SILVA David <david.da_silva@cirad.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#

"""
:Authors:
  - Da SILVA David
:Organization: Virtual Plants
:Contact: david.da_silva:cirad.fr
:Version: 1.0
:Date: July 2005
"""
import math
import random as rdm
from numpy import array, arange
from copy import deepcopy
import openalea.plantgl.all as pgl

"""
:Abstract: Contain PlantGL utils functions
"""

#********************** PlantGL utils ********************************
def color( r, g, b , trans=False, name='material', diffu = 1):
  c=pgl.Color3( r,g,b )
  diffuse=diffu
  specular=pgl.Color3(40,40,40)
  emission=pgl.Color3(0,0,0)
  shininess=0
  if trans:
    transparency=0.5
  else:
    transparency=0
  return pgl.Material( name, c, diffuse, specular, emission, shininess, transparency )

__transGreen = color( 0, 255, 0, True, 'transGreen' )
__Green = color( 0, 100, 0, False, 'Green', diffu=2 )
__niceGreen = color( 0, 50, 0, False, 'Green', diffu=3 )

def gradient( intense ):
  gradient=[]
  gradient.append( color( 47,255,0, trans=True ) )
  gradient.append( color( 115,255,0, trans=True ) )
  gradient.append( color( 187,255,0, trans=True ) )
  gradient.append( color( 255,255,0, trans=True ) )
  gradient.append( color( 255,204,0, trans=True ) )
  gradient.append( color( 255,162,0, trans=True ) )
  gradient.append( color( 255,95,0, trans=True ) )
  gradient.append( color( 255,0,0, trans=True ) )
  return gradient[ intense ]

def rdm_pos( size ):
    """ Return a point (Vector3) in a box centered on origin of size (x,y,z) """
    x,y,z= size
    return pgl.Vector3(rdm.uniform(-x,x), rdm.uniform(-y,y), rdm.uniform(-z,z))
  
def rdm_orientation():
    """ Return three random angles for euler rotation """    
    azimuth= rdm.uniform(-math.pi,math.pi)
    elevation= rdm.uniform(-math.pi,math.pi)
    roll= rdm.uniform(-math.pi,math.pi)
    return azimuth, elevation, roll
  
def rdm_geometry(size, geometry):
    azimuth, elevation, roll= rdm_orientation()
    rotated_geometry= pgl.EulerRotated(azimuth, elevation, roll, geometry)
    position= rdm_pos(size)
    translated_geometry= pgl.Translated(position, rotated_geometry)
    return translated_geometry
 
def rdm_scene(geometry, object_nbr = 10, size=( 10, 10, 10 ) ):
  col=color( 30, 210, 40 )
  return pgl.Scene( [ pgl.Shape( rdm_geometry( size, geometry ), col ) for i in range( object_nbr ) ] )

def sphere( x, y, z, radius=0.4, color=color( 80, 200, 100 ), slice=5, stack=5, id=0 ):
  v = pgl.Vector3( x, y, z )
  s = pgl.Sphere( radius,slice,stack )
  tr = pgl.Translated( v, s )
  return pgl.Shape( tr, color, id )

def disc( x,y,z, color=color( 30,10,140 ) ):
  v = pgl.Vector3( x, y, z )
  d = pgl.Disc( 0.4, 5 )
  azimuth, elevation, roll= rdm_orientation()
  rotated_geometry= pgl.EulerRotated(azimuth, elevation, roll, d)
  tr = pgl.Translated( v,rotated_geometry )
  return pgl.Shape( tr, color )

def cone( x,y,z, color=color( 30,10,140 ) ):
  v = pgl.Vector3( x, y, z )
  d = pgl.Cone( 0.4, 0.4 )
  tr = pgl.Translated( v,d )
  return pgl.Shape( tr, color )

def tapCyl(x,y,z,radius,height, color=color(46,27,21)):
  v = pgl.Vector3( x, y, z )
  c = pgl.Cylinder(radius, height, 10)
  tc = pgl.Tapered(1.0,0.7,c)
  tr = pgl.Translated(v,tc)
  return pgl.Shape(tr,color)

def arrow(x,y,z,length,az=0, el=0, radius=None, color=color(255,255,255),id=0):
  if radius==None:
    radius=length*0.05
  rad_az = math.radians(az)
  rad_el = math.pi/2 - math.radians(el)
  v = pgl.Vector3( x, y, z )
  c = pgl.Cylinder(radius, length*0.8, 10)
  con =pgl.Translated(pgl.Vector3(0,0,length*0.7), pgl.Cone(2*radius, length*0.3))
  arr = pgl.Group([c,con])
  dir_arr = pgl.EulerRotated(rad_az,rad_el,0,arr)
  tr_dir_arr = pgl.Translated(v,dir_arr)
  return pgl.Shape(tr_dir_arr, color, id)
  
def tgl():
  return pgl.TriangleSet( pgl.Point3Array( [ pgl.Vector3( 1,0,0 ),pgl.Vector3( -0.2,1,0 ) ,pgl.Vector3( -0.2,-1,0 ) ] ),pgl.Index3Array( [ pgl.Index3( 0,1,2 ) ] ) ) 
 
def box( x, y, z, radius=0.45, color=color( 80, 200, 100 ), id=None ):
  v = pgl.Vector3( x, y, z )
  b = pgl.Box( pgl.Vector3( radius, radius, radius ) )
  tr = pgl.Translated( v, b )
  return pgl.Shape( tr, color,id )

def leafB( x, y, z, color=color( 30, 210, 40 ) ):
  v = pgl.Vector3( x, y, z )
  s = pgl.Sphere( 0.4, 5, 5 )
  ts = pgl.Tapered( 1.0, 0.3, s )
  tr = pgl.Translated( v, ts )
  return pgl.Shape( tr, color )

def mangoLeaf(length = 1, alpha=0, color = __niceGreen):
  x = array(arange(0.1,length,0.1))
  y = (-3/(length*5.)) * (x**2/length - x)
  cos = math.cos(math.radians(alpha/2.0))
  sin = math.sin(math.radians(alpha/2.0))
  list = []
  idxList = []
  for i in range(len(x)):
    list.append(pgl.Vector3(x[i],0,0))
    list.append(pgl.Vector3(x[i],y[i]*cos,y[i]*sin))
    list.append(pgl.Vector3(x[i],-y[i]*cos,y[i]*sin))
  list.append(pgl.Vector3(0,0,0))
  list.append(pgl.Vector3(length,0,0))
  for i in range(len(x) -1):
    idxList.append(pgl.Index(3*(i+1), 3*(i+1)+1, 3*i+1, 3*i ))
    idxList.append(pgl.Index(3*i+2, 3*(i+1)+2, 3*(i+1), 3*i ))
    #idxList.append(pgl.Index(3*i,3*i+1,3*(i+1)+1, 3*(i+1)))
    #idxList.append(pgl.Index(3*i,3*(i+1),3*(i+1)+2,3*i+2))

  idxList.append(pgl.Index(0, 1, len(list)-2 ))
  idxList.append(pgl.Index(2, 0, len(list)-2 ))
  idxList.append(pgl.Index(3*(len(x)-1)+1, 3*(len(x)-1), len(list)-1 ))
  idxList.append(pgl.Index(3*(len(x)-1), 3*(len(x)-1)+2, len(list)-1 ))
  #idxList.append(pgl.Index(len(list)-2, 1, 0))
  #idxList.append(pgl.Index(len(list)-2, 0, 2))
  #idxList.append(pgl.Index(len(list)-1, 3*(len(x)-1), 3*(len(x)-1)+1))
  #idxList.append(pgl.Index(len(list)-1, 3*(len(x)-1)+2, 3*(len(x)-1)))
  #p3r = pgl.Point3Array(list)
  #ir = pgl.IndexArray(idxList)
  fs = pgl.FaceSet(list,idxList)
  return pgl.Shape(fs, color)

__myLeaf = mangoLeaf(alpha = 20)

def leaf(**kwds):
  rp = rdm_pos((1,1,1))
  rx, ry, rz = rp.x, rp.y, rp.z
  az, el, rl = rdm_orientation()
  x = kwds.get('x', rx)
  y = kwds.get('y', ry)
  z = kwds.get('z', rz)
  delta = kwds.get('rotz', az)
  phi = kwds.get('roty', el)
  psi = kwds.get('rotx', rl)
  size = kwds.get('len', 10.0)
  color = kwds.get('color', __niceGreen)
  if not size:
    size = 10.0
    print("length is None, using 10 instead : small leaf")
  v = pgl.Vector3( x, y, z )
  #print("leaf size : ", size)
  #sx = pgl.Vector3(4,1,1) * size/40.
  #scaled_geometry = pgl.Scaled(sx, pgl.Translated(pgl.Vector3(0.5,0,0), pgl.Disc(0.5, 6)) )
  sx = pgl.Vector3(1,1,1) * size/10.
  scaled_geometry = pgl.Scaled(sx, __myLeaf.geometry)
  rotated_geometry= pgl.EulerRotated(delta, phi, psi, scaled_geometry)
  tr = pgl.Translated( v,rotated_geometry )
  return pgl.Shape( tr, color )

def genericLeaf(allo_length, allo_surf, **kwds):
  if 'surface' in kwds:
    surface = kwds['surface']
    length = math.sqrt(surface / (allo_length*allo_surf) )
  else:  
    length = kwds.get('length', 1)

  alpha = kwds.get('alpha', 0)
  color = kwds.get('color', __niceGreen)
  step = length/10.
  x = array(arange(step,length,step)) #begin and end are specific
  y = (-3.0*allo_length*allo_surf) * (x**2/length - x)
  cos = math.cos(math.radians(alpha/2.0))
  sin = math.sin(math.radians(alpha/2.0))
  list = []
  idxList = []
  for i in range(len(x)):
    list.append(pgl.Vector3(x[i],0,0))
    list.append(pgl.Vector3(x[i],y[i]*cos,y[i]*sin))
    list.append(pgl.Vector3(x[i],-y[i]*cos,y[i]*sin))
  list.append(pgl.Vector3(0,0,0))
  list.append(pgl.Vector3(length,0,0))
  for i in range(len(x) -1):
    idxList.append(pgl.Index(3*(i+1), 3*(i+1)+1, 3*i+1, 3*i ))
    idxList.append(pgl.Index(3*i+2, 3*(i+1)+2, 3*(i+1), 3*i ))

  idxList.append(pgl.Index(0, 1, len(list)-2 ))
  idxList.append(pgl.Index(2, 0, len(list)-2 ))
  idxList.append(pgl.Index(3*(len(x)-1)+1, 3*(len(x)-1), len(list)-1 ))
  idxList.append(pgl.Index(3*(len(x)-1), 3*(len(x)-1)+2, len(list)-1 ))
  p3r = pgl.Point3Array(list)
  ir = pgl.IndexArray(idxList)
  fs = pgl.FaceSet(p3r, ir)
  return pgl.Shape(fs, color)


def profile(heightPeriph,heightTop,radius,shapeBot = 2 ,shapeTop = 2,stacks = 20):
	radius = float(radius)
	points = [pgl.Vector2(0,0)]
	rStep = radius / stacks
	rIter = 0
	for  j in range(0,stacks - 1):
		rIter += rStep
		points.append(pgl.Vector2(rIter, heightPeriph* pow(rIter/radius,shapeBot) ))
	rIter = radius
	heightTopPart = heightTop - heightPeriph
	points.append(pgl.Vector2(radius,heightPeriph))
	for  j in range(0,stacks - 1):
		rIter -= rStep;
		points.append(pgl.Vector2(rIter, heightTop - heightTopPart * pow(rIter/radius,shapeTop)))
	points.append(pgl.Vector2(0,heightTop))
	return points

def createSwung(heightPeriph,heightTop,radii,shapeBot = 2 ,shapeTop = 2,slices = 20, stacks = 20):
	profiles = []
	angles = []
	for radius,angle in radii:
		if radius == 0:
			radius = 0.1
		profiles.append(pgl.Polyline2D(profile(heightPeriph,heightTop,radius,shapeBot,shapeTop,stacks)))
		angles.append(math.radians(angle))
	if len(profiles) > 1:
		profiles.append(profiles[0])
		angles.append(angles[0]+math.radians(360))
	return pgl.Swung(profiles,angles,slices,True,3)


def V3toTpl( vect ):
  return ( vect.x, vect.y, vect.z )

def TpltoV3( tpl ):
  return pgl.Vector3( tpl[ 0 ], tpl[ 1 ], tpl[ 2 ] )

def V3toAngles( dir ):
  zero = pgl.Vector3( 0,0,0 )
  inv_dir = zero - dir
  spherical = pgl.Vector3.Spherical(inv_dir)
  print("theta :",spherical.theta,"   phi :",spherical.phi)
  return (-spherical.theta ,  math.pi/2-spherical.phi )

def gridIndex(point,step):
 return ( int(point.x/step.x) , int(point.y/step.y), int(point.z/step.z) ) 

###########Scene utils####################################################

def viewScene( scene ):
  pgl.Viewer.display( scene )

def cvxHull( scene ):
  #work with scene or shape list
  group = pgl.Group( [ sh.geometry for sh in scene ] )
  tglSet = pgl.fit( 'convexhull', group )
  hull = pgl.Shape( tglSet, __Green )
  return hull

def centerScene( scene ):
  bbox = pgl.BoundingBox(scene)
  return pgl.Scene([pgl.Shape(pgl.Translated(-bbox.getCenter(),i.geometry),i.appearance,i.id) for i in scene])

def rotateSceneRad( scene, delta=0, phi=0, psi=0 ):
  """
  Rotation angle :
   - delta : around z-axis
   - psi : around y-axis
   - phi : around x_axis
  """
  sc2=pgl.Scene([pgl.Shape(pgl.EulerRotated(delta,phi,psi,i.geometry),i.appearance,i.id) for i in scene])
  #d = pgl.Discretizer()
  #def discretize( discretizer, shape ):
  # shape.apply( discretizer )
  # return discretizer.getDiscretization()
  #
  #sc3 = pgl.Scene([pgl.Shape(discretize(d,i.geometry ),i.appearance,i.id ) for i in sc2 ] )
  #return sc3
  return sc2

def rotateGridRad( scene, delta=0, phi=0, psi=0 ):
  return rotateSceneRad( scene, delta=-delta, phi=-phi, psi=-psi )
 
def rotateScene( scene, delta=0, phi=0, psi=0 ):
  return rotateSceneRad(scene, delta=math.radians(delta), phi=math.radians(phi), psi=math.radians(psi))

def rotateGrid( scene, delta=0, phi=0, psi=0 ):
  return rotateScene( scene, delta=-delta, phi=-phi, psi=-psi )
 

########################Inside a scene #########################################
def center(pointList):
  """centre d'une liste de points"""
  v= pgl.Vector3(0,0,0)
  for pt in pointList:
    v += pt
  v /= len(pointList)
  return v

def pointsByShape(scene): #aka ptdiscretize
  """renvoie un dictionnaire Shape_id:[liste de points]"""
  d = pgl.Discretizer()
  pbs = {}
  for i in scene:
    if i.apply(d) == True:
      pts = d.getDiscretization().pointList #yield a pgl.Point3Array
      if pts:
          pbs[i.id] = pts
  return pbs

def centerByShape(pointsbyshape):#aka centers
  """dictionnaire id:centre"""
  cbs = {}
  for id, points in pointsbyshape.items():
    cbs[id] = center(points)
  return cbs

def ptUnion(pointsbyshape,idList=None):#aka ptsum
  """union des points de la liste ids"""
  union = pgl.Point3Array()
  if idList is None:
    idList = list(pointsbyshape.keys())
  for id in idList:
    union += pointsbyshape[id]
  return union

def shapeDict(sc):
  sh = {}
  for i in sc:
    sh[i.id] = i
  return sh

def surfaceByShape(sc):#aka surfaces
  surfbysh = {}
  for i in sc:
    surfbysh[i.id] = pgl.surface(i.geometry) #where in openalea.plantgl ??
  return surfbysh

def surfaceSum(ids,surfbysh):#aka groupsurface
  """somme surface groupe de shape"""
  sum = 0
  for i in ids:
    sum += surfbysh[i]
  return sum

def ptsHull( ptsList, id = 0 ):
  m = color( 20,150,20,trans=True )
  shape = pgl.Shape(pgl.fit('convexhull',pgl.Polyline(ptsList)),m)
  shape.id = id
  return shape

def oneScHullScene(scgroup,pointsbyshape):#aka hullscene
  """cree les cvxHull des groupes de scgroup"""
  sc = pgl.Scene()
  m = color(20,150,20, trans=True)
  for ids in scgroup:
    pts = ptUnion(pointsbyshape,ids)
    sc += pgl.Shape(pgl.fit('convexhull',pgl.Polyline(pts)),m)
  return sc

def allScHullSceneList(allsclist,pointsbyshape):#aka hullscenes
  """a partir de la listes de toutes les echelles cree une liste de scene des enveloppes"""
  sclist = []
  for scgroup in allsclist:
    hulls = oneScHullScene(scgroup,pointsbyshape)
    sclist.append(hulls)
  return sclist

def explorehulls(hullsList,sc = None):
  """visite des scenes echelle par echelle"""
  i = 0
  ok = True
  while ok:
    if not sc is None:
      nsc = pgl.Scene(sc)
    else:
      nsc = pgl.Scene()
    nsc += hullsList[i]
    pgl.Viewer.display(nsc)
    res = pgl.Viewer.dialog.question('Hulls Scene',"Representation at scale "+str(i),"Previous","Next","Stop")
    if res == 1:
      i -= 1
      i = i % len(hullsList)
    elif res == 2:
      i += 1
      i = i % len(hullsList)
    else:
      ok = False
#################################################################################

def surfPerLeaf(sc):
  """return an array of center, surface cople for every leaf in the scene"""
  res = []
  t = pgl.Tesselator()
  for sh in sc:
    sh.geometry.apply(t)
    d = t.triangulation
    res2=[]
    lcenter = pgl.Vector3( 0,0,0 )
    for i in range(d.indexListSize()):
      lcenter += (d.pointAt(i,0)+d.pointAt(i,1)+d.pointAt(i,2))/3
      surf = pgl.surface (d.pointAt(i,0),d.pointAt(i,1),d.pointAt(i,2)) #where in openalea.plantgl ??
      res2 += [surf]
    lcenter /= len( res2 )
    lsurf=sum( res2 )
    res+=[ ( lcenter, lsurf ) ]
  return res 
 
def surfPerTriangle(sc):
  """return an array of center, surface cople for every triangle in the scene"""
  res = []
  t = pgl.Tesselator()
  for sh in sc:
    sh.geometry.apply(t)
    d = t.triangulation
    for i in range(d.indexListSize()):
      center = (d.pointAt(i,0)+d.pointAt(i,1)+d.pointAt(i,2))/3
      surf = pgl.surface (d.pointAt(i,0),d.pointAt(i,1),d.pointAt(i,2))
      res += [(center,surf)]
  return res 
 

