#! /usr/bin/env python
"""
:Authors:
  - Da SILVA David
:Organization: Virtual Plants
:Contact: david.da_silva:cirad.fr
:Version: 1.0
:Date: July 2005
"""
import math
import os
import random as rdm
import scipy
from copy import deepcopy
#import PlantGL as pgl
import openalea.plantgl as pgl
import pathDB

"""
:Abstract: Contain PlantGL utils functions
"""

#********************** PlantGL utils ********************************
def color( r, g, b , trans=False, name='material'):
  c=pgl.scenegraph.Color3( r,g,b )
  diffuse=1
  specular=pgl.scenegraph.Color3(40,40,40)
  emission=pgl.scenegraph.Color3(0,0,0)
  shininess=0
  if trans:
    transparency=0.5
  else:
    transparency=0
  return pgl.scenegraph.Material( name, c, diffuse, specular, emission, shininess, transparency )

__transGreen = color( 0, 255, 0, True, 'transGreen' )
__Green = color( 0, 255, 0, False, 'Green' )

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
    return pgl.math.Vector3(rdm.uniform(-x,x), rdm.uniform(-y,y), rdm.uniform(-z,z))
  
def rdm_orientation():
    """ Return three random angles for euler rotation """    
    azimuth= rdm.uniform(-math.pi,math.pi)
    elevation= rdm.uniform(-math.pi,math.pi)
    roll= rdm.uniform(-math.pi,math.pi)
    return azimuth, elevation, roll
  
def rdm_geometry(size, geometry):
    azimuth, elevation, roll= rdm_orientation()
    rotated_geometry= pgl.scenegraph.EulerRotated(azimuth, elevation, roll, geometry)
    position= rdm_pos(size)
    translated_geometry= pgl.scenegraph.Translated(position, rotated_geometry)
    return translated_geometry
 
def rdm_scene(geometry, object_nbr = 10, size=( 10, 10, 10 ) ):
  col=color( 30, 210, 40 )
  return pgl.scenegraph.Scene( [ pgl.scenegraph.Shape( rdm_geometry( size, geometry ), col ) for i in range( object_nbr ) ] )

def sphere( x, y, z, radius=0.4, color=color( 80, 200, 100 ), slice=5, stack=5, id=0 ):
  v = pgl.math.Vector3( x, y, z )
  s = pgl.scenegraph.Sphere( radius,slice,stack )
  tr = pgl.scenegraph.Translated( v, s )
  return pgl.scenegraph.Shape( tr, color, id )

def disc( x,y,z, color=color( 30,10,140 ) ):
  v = pgl.math.Vector3( x, y, z )
  d = pgl.scenegraph.Disc( 0.4, 5 )
  azimuth, elevation, roll= rdm_orientation()
  rotated_geometry= pgl.scenegraph.EulerRotated(azimuth, elevation, roll, d)
  tr = pgl.scenegraph.Translated( v,rotated_geometry )
  return pgl.scenegraph.Shape( tr, color )

def cone( x,y,z, color=color( 30,10,140 ) ):
  v = pgl.math.Vector3( x, y, z )
  d = pgl.scenegraph.Cone( 0.4, 0.4 )
  tr = pgl.scenegraph.Translated( v,d )
  return pgl.scenegraph.Shape( tr, color )
 
def leafB( x, y, z, color=color( 30, 210, 40 ) ):
  v = pgl.math.Vector3( x, y, z )
  s = pgl.scenegraph.Sphere( 0.4, 5, 5 )
  ts = pgl.scenegraph.Tapered( 1.0, 0.3, s )
  tr = pgl.scenegraph.Translated( v, ts )
  return pgl.scenegraph.Shape( tr, color )

def tapCyl(x,y,z,radius,height, color=color(46,27,21)):
  v = pgl.math.Vector3( x, y, z )
  c = pgl.scenegraph.Cylinder(radius, height, 10)
  tc = pgl.scenegraph.Tapered(1.0,0.7,c)
  tr = pgl.scenegraph.Translated(v,tc)
  return pgl.scenegraph.Shape(tr,color)

def arrow(x,y,z,length,az=0, el=0, radius=None, color=color(255,255,255),id=0):
  if radius==None:
    radius=length*0.05
  rad_az = math.radians(az)
  rad_el = math.pi/2 - math.radians(el)
  v = pgl.math.Vector3( x, y, z )
  c = pgl.scenegraph.Cylinder(radius, length*0.8, 10)
  con =pgl.scenegraph.Translated(pgl.math.Vector3(0,0,length*0.7), pgl.scenegraph.Cone(2*radius, length*0.3))
  arr = pgl.scenegraph.Group([c,con])
  dir_arr = pgl.scenegraph.EulerRotated(rad_az,rad_el,0,arr)
  tr_dir_arr = pgl.scenegraph.Translated(v,dir_arr)
  return pgl.scenegraph.Shape(tr_dir_arr, color, id)
  
def tgl():
  return pgl.scenegraph.TriangleSet( pgl.scenegraph.Point3Array( [ pgl.math.Vector3( 1,0,0 ),pgl.math.Vector3( -0.2,1,0 ) ,pgl.math.Vector3( -0.2,-1,0 ) ] ),pgl.scenegraph.Index3Array( [ pgl.scenegraph.Index3( 0,1,2 ) ] ) ) 
 
def box( x, y, z, radius=0.45, color=color( 80, 200, 100 ), id=None ):
  v = pgl.math.Vector3( x, y, z )
  b = pgl.scenegraph.Box( pgl.math.Vector3( radius, radius, radius ) )
  tr = pgl.scenegraph.Translated( v, b )
  return pgl.scenegraph.Shape( tr, color,id )

def V3toTpl( vect ):
  return ( vect.x, vect.y, vect.z )

def TpltoV3( tpl ):
  return pgl.math.Vector3( tpl[ 0 ], tpl[ 1 ], tpl[ 2 ] )

def V3toAngles( dir ):
  zero = pgl.math.Vector3( 0,0,0 )
  inv_dir = zero - dir
  spherical = pgl.math.Vector3.Spherical(inv_dir)
  print "theta :",spherical.theta,"   phi :",spherical.phi
  return (-spherical.theta ,  math.pi/2-spherical.phi )

def gridIndex(point,step):
 return ( int(point.x/step.x) , int(point.y/step.y), int(point.z/step.z) ) 

###########Scene utils####################################################

def loadScene( filename, newId=False ):
  pdb = pathDB.pathDB()
  absfile = os.path.join( pdb.getDir( 'SCENEDIR' ), filename )
  if( os.path.isfile( filename + '.geom' ) ):
    filepath = filename + '.geom'
  elif( os.path.isfile( filename + '.bgeom' ) ):
    filepath = filename + '.bgeom'
  elif ( os.path.isfile( absfile + '.geom' ) ):
    filepath = absfile + '.geom'
  elif( os.path.isfile( absfile + '.bgeom' ) ):
    filepath = absfile + '.bgeom'
  else:
    raise IOError, "No such file"
  scene = pgl.scenegraph.Scene( filepath )
  
  if newId:
   for i,sh in enumerate( scene ):
    sh.id = i+1
    
  return scene
 
def viewScene( scene ):
  pgl.gui.Viewer.display( scene )

def cvxHull( scene ):
  group = pgl.scenegraph.Group( [ sh.geometry for sh in scene ] )
  tglSet = pgl.algo.fit( 'convexhull', group )
  hull = pgl.scenegraph.Shape( tglSet, __Green )
  return hull

def centerScene( scene ):
  bbox = pgl.scenegraph.BoundingBox(scene)
  return pgl.scenegraph.Scene([pgl.scenegraph.Shape(pgl.scenegraph.Translated(-bbox.getCenter(),i.geometry),i.appearance,i.id) for i in scene])

def rotateSceneRad( scene, delta=0, phi=0, psi=0 ):
  """
  Rotation angle :
   - delta : around z-axis
   - psi : around y-axis
   - phi : around x_axis
  """
  sc2=pgl.scenegraph.Scene([pgl.scenegraph.Shape(pgl.scenegraph.EulerRotated(delta,phi,psi,i.geometry),i.appearance,i.id) for i in scene])
  #d = pgl.algo.Discretizer()
  #def discretize( discretizer, shape ):
  # shape.apply( discretizer )
  # return discretizer.getDiscretization()
  #
  #sc3 = pgl.scenegraph.Scene([pgl.scenegraph.Shape(discretize(d,i.geometry ),i.appearance,i.id ) for i in sc2 ] )
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
  """Return the center of a point list."""
  v= pgl.math.Vector3(0,0,0)
  for pt in pointList:
    v += pt
  v /= len(pointList)
  return v

def pointsByShape(scene): #aka ptdiscretize
  """Return a dict Shape_id:[point list]"""
  d = pgl.algo.Discretizer()
  pbs = {}
  for i in scene:
    if i.apply(d) == True:
      pts = d.getDiscretization().pointList #yield a pgl.scenegraph.Point3Array
      if pts:
  	pbs[i.id] = pts
  return pbs

def centerByShape(pointsbyshape):#aka centers
  """dict id:centre"""
  cbs = {}
  for id, points in pointsbyshape.iteritems():
    cbs[id] = center(points)
  return cbs

def ptUnion(pointsbyshape,idList=None):#aka ptsum
  """Union of points of the list ids"""
  union = pgl.scenegraph.Point3Array()
  if idList is None:
    idList = pointsbyshape.keys()
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
  shape = pgl.scenegraph.Shape(pgl.algo.fit('convexhull',pgl.scenegraph.Polyline(ptsList)),m)
  shape.id = id
  return shape

def oneScHullScene(scgroup,pointsbyshape):#aka hullscene
  """ Build convex hull from scgroup groups. """
  sc = pgl.scenegraph.Scene()
  m = color(20,150,20, trans=True)
  for ids in scgroup:
    pts = ptUnion(pointsbyshape,ids)
    sc += pgl.scenegraph.Shape(pgl.algo.fit('convexhull',pgl.scenegraph.Polyline(pts)),m)
  return sc

def allScHullSceneList(allsclist,pointsbyshape):#aka hullscenes
  """a partir de la listes de toutes les echelles cree une liste de scene des enveloppes"""
  sclist = []
  for scgroup in allsclist:
    hulls = oneScHullScene(scgroup,pointsbyshape)
    sclist.append(hulls)
  return sclist

def explorehulls(hullsList,sc = None):
  """ Traverse scenes scale by scale. """
  i = 0
  ok = True
  while ok:
    if not sc is None:
      nsc = pgl.scenegraph.Scene(sc)
    else:
      nsc = pgl.scenegraph.Scene()
    nsc += hullsList[i]
    pgl.gui.Viewer.display(nsc)
    res = pgl.gui.Viewer.dialog.question('Hulls Scene',"Representation at scale "+str(i),"Previous","Next","Stop")
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
  t = pgl.algo.Tesselator()
  for sh in sc:
    sh.geometry.apply(t)
    d = t.getTriangulation()
    res2=[]
    lcenter = pgl.math.Vector3( 0,0,0 )
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
  t = pgl.algo.Tesselator()
  for sh in sc:
    sh.geometry.apply(t)
    d = t.getTriangulation()
    for i in range(d.indexListSize()):
      center = (d.pointAt(i,0)+d.pointAt(i,1)+d.pointAt(i,2))/3
      surf = pgl.surface (d.pointAt(i,0),d.pointAt(i,1),d.pointAt(i,2))#where in openalea.plantgl ??
      res += [(center,surf)]
  return res 
 
def voxelize( filename, gridSize, density=True ):
  """generate the scene resulting of grid discretization"""
  scene = loadScene( filename )
  bbox=pgl.scenegraph.BoundingBox(scene)
  epsilon = pgl.math.Vector3( 0.01, 0.01, 0.01 )
  origin = bbox.lowerLeftCorner - epsilon
  step = ( bbox.getSize() + epsilon )*2 / ( gridSize )
  origin_center = origin + step/2.
  
  tgl_list = surfPerTriangle( scene )

  grid = {}
  for tgl in tgl_list:
    pos = gridIndex( tgl[ 0 ] - origin, step )
    if( pos[ 0 ] >gridSize-1 or pos[ 1 ] >gridSize-1 or pos[ 2 ] >gridSize-1 ):
      print pos
    
    #print tgl[ 0 ]-origin
    if grid.has_key(  pos  ):
      grid[  pos  ] += tgl[ 1 ]
    else:
      grid[  pos  ] = tgl[ 1 ]
  kize = grid.keys()
  kize.sort()
  pts=[]
  mass=[]
  for k in kize:
    pts.append(  list( k )  )
    mass.append( grid[ k ] )
    
  massort = deepcopy( mass )
  massort.sort()
  qlist=[25, 50, 75]
  quants = [massort[int(len(massort)*q/100.0)] for q in qlist]
  
  voxSize = step/2.
  vox = pgl.scenegraph.Box( voxSize )
  vox.setName( 'voxel' )
  
  mat1 = color( 47,255,0, trans=True , name='c_green')
  mat2 = color( 255,255,0, trans=True , name='c_yellow')
  mat3 = color( 255,170,0, trans=True , name='c_orange')
  mat4 = color( 255,0,0, trans=True , name='c_red')

  #sc = pgl.scenegraph.Scene()
  for i in range( len( pts ) ):
    pt = pts[ i ]
    vect = pgl.math.Vector3( origin_center.x + ( pt[ 0 ] * step.x ) , 
                             origin_center.y + ( pt[ 1 ] * step.y ) ,
                             origin_center.z + ( pt[ 2 ] * step.z ) )
    geometry = pgl.scenegraph.Translated( vect, vox )
    if( density ):
      if ( mass[ i ] < quants[ 0 ] ) :
        sh = pgl.scenegraph.Shape( geometry, mat1, i )
      elif ( mass[ i ] < quants[ 1 ] ) :
        sh = pgl.scenegraph.Shape( geometry, mat2, i )
      elif ( mass[ i ] < quants[ 2 ] ) :
        sh = pgl.scenegraph.Shape( geometry, mat3, i )
      else :
        sh = pgl.scenegraph.Shape( geometry, mat4, i )
    else:
      sh = pgl.scenegraph.Shape( geometry, mat1, i )
    scene.add( sh )

  return scene
  
def scene2grid( scene, tgl_list, gridSize ):
  """
  Convert a scene into a matrix-grid.
  Non-zero values are the coresponding surface icluded in the coresponding voxel.
  """
  #tgl_list = surfPerTriangle( scene )
  #d=pgl.algo.Discretizer()
  #bbc = pgl.BBoxComputer(d)
  #bbc.process(scene)
  #bbox=bbc.getBoundingBox()
  bbox=pgl.scenegraph.BoundingBox(scene)
  epsilon = pgl.math.Vector3( 0.01, 0.01, 0.01 )
  origin = bbox.lowerLeftCorner - epsilon
  step = ( bbox.getSize() + epsilon )*2 / ( gridSize )
  #origin = bbox.lowerLeftCorner
  #step = bbox.getSize()*2.01 / ( gridSize )
  step_radius = ( step[ 0 ]*step[ 1 ]*step[ 2 ] )**( 1/3. )
  """
  gridSize is a radius vector whereas gridSize is the voxel number desired per dimension, 
  hence the gridSize must be *2
  """
  print "Bbox size : ", bbox.getSize()*2
  print "Step : ", step, "    gridSize : ", gridSize
  grid = {}
  for tgl in tgl_list:
    pos = gridIndex( tgl[ 0 ] - origin, step )
    
    #print tgl[ 0 ]-origin
    if grid.has_key(  pos  ):
      grid[  pos  ] += tgl[ 1 ]
    else:
      grid[  pos  ] = tgl[ 1 ]
  print "nbTgl : ", len( tgl_list ), "  <--->   nbVoxel : ", len( grid )
  kize = grid.keys()
  kize.sort()
  pts=[]
  mass=[]
  for k in kize:
    pts.append(  list( k )  )
    mass.append( grid[ k ] )
  return ( pts, mass, step_radius ) #gridSize )

def toPglScene( ptList, mass=None, radius=0.5 ):
  """generate a box-based scene from a point list"""
  scene = pgl.scenegraph.Scene()
  x = y = z = 0
  if ( mass ==None ):
    colors=[ gradient( 0 ) ]*len( ptList )
  else:
    colors=[]
    step=( max( mass ) - min( mass ) )/7
    if( step == 0 ):
      step =1
    for m in mass:
      colors.append( gradient( int( ( m - min( mass ) )/step ) ) )
  
  for i in range( len( ptList ) ) :
    pt = ptList[ i ]
    try:
      x = pt[ 0 ]*2*radius
    except IndexError:
      x = 00.3
    try:
      y = pt[ 1 ]*2*radius
    except IndexError:
      y = 0
    try:
      z = pt[ 2 ]*2*radius
    except IndexError:
      z = 0
    #scene.add( disc( x,y,z ) )
    #scene.add( sphere( x, y, z , color=colors[ i ]) )
    scene.add( box( x, y, z ,radius=radius, color=colors[ i ],id=i+1) )
  return scene

def shapeProjection( scene, dir ):
  """ Return the area of the scene projected along the direction dir. """
  viewer = pgl.gui.Viewer
  camera = viewer.camera
    
  viewer.animation( True )
  viewer.grids.set(False,False,False,False)
  camera.setOrthographic()
  sc2=pgl.scenegraph.Scene([ pgl.scenegraph.Shape(pgl.scenegraph.Translated(-pgl.scenegraph.BoundingBox(i.geometry).getCenter(),i.geometry),i.appearance,i.id) for i in scene])
  bbox=pgl.scenegraph.BoundingBox( sc2 )
  viewer.frameGL.setSize(800,600)
  camera.lookAt(bbox.getCenter() + dir*-3*bbox.getXRange(), bbox.getCenter()) # the -3 is a empirical coefficient to set the camera distance from the scene
  proj = viewer.frameGL.getProjectionSizes(sc2)
  #print proj
  return proj

def makeRotatedView( plantName ):
  """generate a set of picture of the scene as the camera rotate around"""
  os.chdir( 'anim' )
  viewer = pgl.gui.Viewer
  camera = viewer.camera
  sc = loadScene( plantName )
  viewer.display( sc )
  bbox = pgl.scenegraph.BoundingBox( sc )
  radius=scipy.sqrt((2*bbox.getXRange())**2 + (2*bbox.getYRange())**2)
  tetarange = scipy.arange( 0., 2*scipy.pi, scipy.pi/120 )
  i=1
  for t in tetarange:
    x=radius*scipy.cos(t)
    y=radius*scipy.sin(t)
    z=2*(bbox.getZMin() + t*(bbox.getZMax() - bbox.getZMin())/(2*scipy.pi))
    camera.lookAt(TpltoV3((x,y,z)), bbox.getCenter())
    if i < 10:
      nb = '00'+str( i )
    elif i < 100 :
      nb = '0' + str( i )
    else:
      nb = str( i )
    i+=1
    filename=plantName+nb+'.png'
    viewer.frameGL.saveImage( filename , 'PNG')
