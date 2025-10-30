import openalea.plantgl.all as pgl
from math import radians, degrees, sin 
from numpy import around
from . import sunPositions as sp

elevations = [9.23, 9.23, 9.23, 9.23, 9.23, 9.23, 9.23, 9.23, 9.23, 9.23, 10.81, 10.81, 10.81, 10.81, 10.81, 26.57, 26.57, 26.57, 26.57, 26.57, 31.08, 31.08, 31.08,31.08, 31.08, 31.08, 31.08, 31.08, 31.08, 31.08, 47.41, 47.41, 47.41, 47.41, 47.41, 52.62, 52.62, 52.62, 52.62, 52.62, 69.16, 69.16, 69.16, 69.16,69.16, 90]

azimuths = [12.23, 59.77, 84.23, 131.77, 156.23, 203.77, 228.23, 275.77, 300.23, 347.77, 36, 108, 180, 252, 324, 0, 72, 144, 216, 288, 23.27, 48.73, 95.27, 120.73,167.27, 192.73, 239.27, 264.73, 311.27, 336.73, 0, 72, 144, 216, 288, 36, 108, 180, 252, 324, 0, 72, 144, 216, 288, 180]

weights = [0.026808309,0.026808309,0.026808309,0.026808309,0.026808309,0.026808309,0.026808309,0.026808309,0.026808309,0.026808309,0.029325083,0.029325083,0.029325083,0.029325083,0.029325083,0.031299545,0.031299545,0.031299545,0.031299545,0.031299545,0.038160959,0.038160959,0.038160959,0.038160959,0.038160959,0.038160959,0.038160959,0.038160959,0.038160959,0.038160959,0.045638829,0.045638829,0.045638829,0.045638829,0.045638829,0.050212264,0.050212264,0.050212264,0.050212264,0.050212264,0.052965108,0.052965108,0.052965108,0.052965108,0.052965108,0.0481]

def azel2vect(az, el, north=0):
  """ converter for azimuth elevation 
      az,el are expected in degrees, in the North-clocwise convention
      In the scene, positive rotations are counter-clockwise
      north is the angle (degrees, positive counter_clockwise) between X+ and North """
  azimuth = -radians(north + az)
  zenith = radians(90 - el)
  v = -pgl.Vector3(pgl.Vector3.Spherical( 1., azimuth, zenith ) )
  v.normalize()
  return v

def plotSkyTurtle(dim = 30):
  pgl.Viewer.start()
  sc = pgl.Scene()
  for i, (az,el) in enumerate(zip(azimuths,elevations)):
    pos = -dim*azel2vect (az,el) 
    sc += pgl.Shape(pgl.Translated(pos,pgl.Sphere(0.5)),pgl.Material(),i+1)
  pgl.Viewer.display(sc)
  return sc

def getSkyTurtleAt(i):
  return (elevations[i-1],azimuths[i-1],weights[i-1])

def getSkyTurtleDir(i):
  return -pgl.Vector3(pgl.Vector3.Spherical(1,radians(azimuths[i-1]),radians(90-elevations[i-1])))

def getSkyTurtleRotationAngles(i):
  return (- radians(azimuths[i-1]), radians(elevations[i-1]))

def skyTurtleDir():
  return [ getSkyTurtleDir(i+1) for i in range(getSkyTurtleSize()) ]

def skyTurtleWDir():
  return [ (getSkyTurtleDir(i+1),weights[i]) for i in range(getSkyTurtleSize()) ]

def skyTurtle():
  return [ getSkyTurtleAt(i+1) for i in range(getSkyTurtleSize()) ]

def getSkyTurtleSize():
  return len(azimuths)

def getDirectLight( latitude, longitude, jourJul, startH, stopH, step=30, decalSun = 1, decalGMT = 0):
  # startH and stopH represent starting and stoping hour for the light, given in hour
  # step are time step, given in minute
  # decalGMT give 'fuseau horaire' express in hour
  seq = sp.Sequence()
  hdeb = seq.heureTSV(jourJul, startH, decalSun, decalGMT, longitude)
  hfin = seq.heureTSV(jourJul, stopH, decalSun, decalGMT, longitude)
  el, az, time = seq.positionSoleil(step, radians(latitude), jourJul, hdeb, hfin)
  sw = 0
  for h in el :
    sw += 0.7**( 1./sin(h) ) * sin(h) 
  w = [ 0.7**( 1./sin(h) ) * sin(h) / sw for h in el ] 
  tot = 0
  for s in w:
    tot+= s
  return [ (  degrees(eli),  degrees(azi), wi ) for eli, azi, wi in zip(el, az, w) ]


def plotDirect( latitude, longitude, jourJul, startH, stopH, step=30, decalSun = 1, decalGMT = 0, dim = 30):
  sunPos = getDirectLight( latitude, longitude, jourJul, startH, stopH, step, decalSun, decalGMT )
  sc = pgl.Scene()
  for i, (el, az, t) in enumerate(sunPos):
    pos = -dim*azel2vect (az,el) 
    sc += pgl.Shape(pgl.Translated(pos,pgl.Sphere(1)),pgl.Material(),i+1)
  pgl.Viewer.display(sc)
  return sc



