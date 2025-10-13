from . import sunDome as sd
import openalea.plantgl.all as pgl

from math import radians, sin, pi, ceil

# Light Fractalysis module: Computation of direct light in a scene
#
# The light fractalysis module (written by D. DaSilva during his PhD thesis) contains 3 mains functions.

# 1. directionalInterception computes the weighed cumulated direct light intercepted by each geometrical object (shape)
# in a 3D scene, given as an argument in a set of specified directions (typically representing the course of sun in
# space during a period of time)
# use:
# directionalInterception(lscene,[(azimuth,elev,weight), (azimuth,elev,weight), ...])
# where:
# * lscene is the lscene corresponding to the L-system geometric interpretation of the current lstring
# * azimuth and elev are angles in the LPy global reference system (x towards us, y to the right, z upwards)
# (note that elev angle is counted with reference to the plane (x,y) where it is 0.
# * weight is a set of weighting coefficients
#
# the value returned is a dictionary that associates each id in the lscene with its cumulated value of intercepted light
# (for all the considered directions)


# 2. diffuseInterception(lscene) calls directionalInterception, with a predefined set of directions,
# representing a sampling of the sky dome, and specific weighing coefs for these directions


# 3. A last function "directInterception" calls directionalInterception with a predefined set of directions,
# representing the path of the sun at a given latitude and longitudes, at a specific day date.
#
# directionalInterception(lscene, latitude, longitude, julian_day, starting_hour, ending_hour, step, dGMT)
# (see also ...)
#
# Note: In all these algorithms the plant projection to compute the intercepted surfaces is orthographic
# and based on a viewport whose resolution is 600x600

def diffuseInterception(scene, screenresolution = None):
  return  directionalInterception(scene, directions = sd.skyTurtle(), screenresolution = screenresolution)

def directInterception(scene, latitude=43.36, longitude=3.52, jj=221, start=7, stop=19, stp=30, dsun = 1, dGMT = 0, screenresolution = None):
  direct = sd.getDirectLight( latitude=latitude , longitude=longitude, jourJul=jj, startH=start, stopH=stop, step=stp, decalSun = dsun, decalGMT = dGMT)
  return  directionalInterception(scene, directions = direct, screenresolution = screenresolution)

def totalInterception(scene, latitude=43.36, longitude=3.52, jj=221, start=7, stop=19, stp=30, dsun = 1, dGMT = 0, screenresolution = None):
  diffu = sd.skyTurtle()
  direct =  sd.getDirectLight( latitude=latitude , longitude=longitude, jourJul=jj, startH=start, stopH=stop, step=stp, decalSun = dsun, decalGMT = dGMT)
  all = direct + diffu
  return directionalInterception(scene, directions = all, screenresolution = screenresolution)


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

def elaz2vect(el, az, north=0):
  """ converter for azimuth elevation 
      az,el are expected in degrees, in the North-clocwise convention
      In the scene, positive rotations are counter-clockwise
      north is the angle (degrees, positive counter_clockwise) between X+ and North """
  azimuth = -radians(north + az)
  zenith = radians(90 - el)
  v = -pgl.Vector3(pgl.Vector3.Spherical( 1., azimuth, zenith ) )
  v.normalize()
  return v



def directionalInterception_openGL(scene, directions, north = 0, horizontal = False, screenwidth = 600):
  
  pgl.Viewer.display(scene)
  redrawPol = pgl.Viewer.redrawPolicy
  pgl.Viewer.redrawPolicy = False
  pgl.Viewer.frameGL.maximize(True)
  
  #pgl.Viewer.widgetGeometry.setSize(screenwidth, screenwidth)
  pgl.Viewer.frameGL.setSize(screenwidth,screenwidth)
  
  cam_pos,cam_targ,cam_up = pgl.Viewer.camera.getPosition()
  pgl.Viewer.camera.setOrthographic()
  pgl.Viewer.grids.set(False,False,False,False)
  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}

  for el, az, wg in directions:
    if( az != None and el != None):
        dir = azel2vect(az, el, north)
        if horizontal :
            wg /= sin(radians(el))

    else :
      dir = -pgl.Viewer.camera.getPosition()[1]
      assert not horizontal

    pgl.Viewer.camera.lookAt(bbox.getCenter() + dir*(-2.5)*d_factor, bbox.getCenter()) #2.5 is for a 600x600 GLframe

    values = pgl.Viewer.frameGL.getProjectionPerShape()
    if not values is None:
      nbpixpershape, pixsize = values
      pixsize = pixsize*pixsize
      for key,val in nbpixpershape:
        if key in shapeLight:
          shapeLight[key] += val*pixsize*wg
        else:
          shapeLight[key] = val*pixsize*wg
  pgl.Viewer.camera.lookAt(cam_pos, cam_targ ) 
  pgl.Viewer.redrawPolicy = redrawPol

  
  return shapeLight



def getProjectionMatrix(forward, up = pgl.Vector3(0,0,1)):
    forward.normalize()
    up.normalize();
    side = pgl.cross(up, forward);
    side.normalize();
    up = pgl.cross(forward, side);
    up.normalize();
    return pgl.Matrix3(side, up, forward).inverse()



def projectedBBox(bbx, direction, up):
    from itertools import product
    proj = getProjectionMatrix(direction,up)
    pts = [proj*pt for pt in product([bbx.getXMin(),bbx.getXMax()],[bbx.getYMin(),bbx.getYMax()],[bbx.getZMin(),bbx.getZMax()])]
    projbbx = pgl.BoundingBox(pgl.PointSet(pts))
    return projbbx



def directionalInterception_zbuffer(scene, directions, 
                            north = 0, 
                            horizontal = False, 
                            screenresolution = None, 
                            multithreaded = True,
                            infinitize = None):

  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}
  if screenresolution is None:
    screenresolution = d_factor/100
  pixsize = screenresolution*screenresolution

  for el, az, wg in directions:
    if( az != None and el != None):
        dir = azel2vect(az, el, north)
        if horizontal :
            wg /= sin(radians(el))

    up = dir.anOrthogonalVector()
    pjbbx = projectedBBox(bbox, dir, up)
    worldWidth = pjbbx.getXRange()
    worldheight = pjbbx.getYRange()
    w, h = max(2,int(ceil(worldWidth/screenresolution))+1), max(2,int(ceil(worldheight/screenresolution))+1)

    worldWidth = w * screenresolution
    worldheight = h * screenresolution
    z = pgl.ZBufferEngine(w,h,pgl.eIdBased)
    z.multithreaded = multithreaded
    z.setOrthographicCamera(-worldWidth/2., worldWidth/2., -worldheight/2., worldheight/2., d_factor , 3*d_factor)
    eyepos = bbox.getCenter() - dir* d_factor * 2
    z.lookAt(eyepos, bbox.getCenter(), up) 
    z.process(scene)
    if not infinitize is None:
        assert len(infinitize) == 3
        center = bbox.getCenter()
        for i in range(2):
            if infinitize[0] > 0 : z.periodizeBuffer(center, center+pgl.Vector3(infinitize[0],0,0)) #, False)
            if infinitize[1] > 0 : z.periodizeBuffer(center, center+pgl.Vector3(0,infinitize[1],0))
            if infinitize[2] > 0 : z.periodizeBuffer(center, center+pgl.Vector3(0,0,infinitize[2]))
 

    values = z.idhistogram(False)
    if not values is None:
        for shid, val in values:
            shapeLight[shid] = shapeLight.get(shid,0) + val*pixsize*wg
  
  return shapeLight

def directionalInterception_triangleprojection(scene, directions, 
                            north = 0, 
                            horizontal = False):

  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}

  for el, az, wg in directions:
    if( az != None and el != None):
        dir = azel2vect(az, el, north)
        if horizontal :
            wg /= sin(radians(el))

    up = dir.anOrthogonalVector()
    print(dir, up)
    pjbbx = projectedBBox(bbox, dir, up)
    worldWidth = pjbbx.getXRange()
    worldheight = pjbbx.getYRange()
    z = pgl.DepthSortEngine(pgl.ePrimitiveIdBased)
    z.setOrthographicCamera(-worldWidth/2., worldWidth/2., -worldheight/2., worldheight/2., d_factor , 3*d_factor)
    eyepos = bbox.getCenter() - dir* d_factor * 2
    z.lookAt(eyepos, bbox.getCenter(), up) 
    z.process(scene) 

    values = z.aggregateIdSurfaces()
    pgl.Viewer.display(z.getSurfaceResult(cameraCoordinates=False))
    print(values[2])
    
    if not values is None:
        for shid, (val, triangleval) in values.items():
            shapeLight[shid] = (shapeLight.get(shid,(0, None))[0] + val*wg, [v*wg for v in triangleval] if not shid in shapeLight else [ov+v*wg for ov,v in zip(shapeLight[shid][1],triangleval)])
  
  return shapeLight

eOpenGLProjection, eZBufferProjection, eTriangleProjection = range(3)

def directionalInterception(scene, directions, 
                            north = 0, 
                            horizontal = False,
                            method = eZBufferProjection, **args):

  if method == eOpenGLProjection:
      return directionalInterception_openGL(scene=scene, directions=directions, north=north, horizontal=horizontal, **args)
  elif method == eZBufferProjection:
      return directionalInterception_zbuffer(scene=scene, directions=directions, north=north, horizontal=horizontal, **args)
  elif method == eTriangleProjection:
      return directionalInterception_triangleprojection(scene=scene, directions=directions, north=north, horizontal=horizontal)

def scene_irradiance(scene, directions, north = 0, horizontal = False, scene_unit = 'm', method = eZBufferProjection, **args):
    """
    Compute the irradiance received by all the shapes of a given scene.
   :Parameters:
    - `scene` : scene for which the irradiance has to be computed
    - `directions` : list of tuple composed of the an azimuth, an elevation and an irradiance (in J.s-1.m-2)
    - `north` : the angle between the north direction of the azimuths (in degrees-counter clockwise). Warning: it is given in clockwise in Caribu.
    - `horizontal` : specify if the irradiance use an horizontal convention (True) or a normal direction (False)
    - `scene_unit` : specify the units in which the scene is built. Convert then all the result in m.

    :returns: the area of the shapes of the scene in m2 and the irradiance in J.s-1.m-2
    :returntype: pandas.DataFrame
    """

    units = {'mm': 0.001, 'cm': 0.01, 'dm': 0.1, 'm': 1, 'dam': 10, 'hm': 100,
             'km': 1000}

    conv_unit = units[scene_unit]
    conv_unit2 = conv_unit**2


    res = directionalInterception(scene = scene, directions = directions, north = north, horizontal = horizontal, method=method,  **args)
    res = { sid : conv_unit2 * value for sid, value in res.items() }

    surfaces = dict([(sid, conv_unit2*sum([pgl.surface(sh.geometry) for sh in shapes])) for sid, shapes in scene.todict().items()])

    if method == eTriangleProjection:
       trsurface = {}
       for sh in scene:
          tr = pgl.tesselate(sh.geometry)
          trsurface.setdefault(sh.id,[])
          trsurface[sh.id] += [conv_unit2 * pgl.surface(tr.pointAt(i,0),tr.pointAt(i,1),tr.pointAt(i,2)) for i in range(tr.indexListSize())]
      
       trirradiance = {}
       for sid,value  in res.items():
          lres = []
          for tid, (v, trsurf) in enumerate(zip(value[1],trsurface[sid])):
            #if not (0 <= v <= trsurf) :
            #   raise ValueError("invalid projected surface", v, trsurf, sid, tid)
            lres.append(v/trsurf)
          trirradiance[sid] = lres
    else:
       trirradiance = None
    
    irradiance = { sid : value[0] / surfaces[sid] for sid, value in res.items() }

    import pandas
    return pandas.DataFrame( {'area' : surfaces, 'irradiance' : irradiance} ), trirradiance


class LightEstimator:
  def __init__(self, method, scene):
      self.method = method
      self.scene = scene
      self.lights = []

  def setLights(self, lights):
    """
    Add a set of lights to the estimator.

    Parameters
    ----------
    lights : list
        A list of tuples (direction, intensity) representing the lights to add.
    """
    self.lights.append(lights)
  
  def setLightFromDirections(self, directions, north = 0, horizontal = False):
    """
    Set lights based on specified directions.

    This function calculates light directional vectors and intensity for each specified 
    direction (elevation, azimuth and weight) and adds them to the lights list. 
    If the horizontal flag is set to True, the intensity is supposed to be measured 
    on an horizontal plane and thus adjusted according to the sine of the elevation angle.

    :param directions: A list of tuples, where each tuple contains 
                       an elevation, an azimuth and a weight.
    :param north: The angle between the north direction of the azimuths 
                  (in degrees-positive clockwise). Default is 0.
    :param horizontal: A boolean flag specifying if the intensity is given using a horizontal convention and should be adjusted 
                     (True) or not (False). Default is False.
    """

    for el, az, wg in directions:
      if( az != None and el != None):
            dir = azel2vect(az, el, north)
            if horizontal :
                wg /= sin(radians(el))
            self.lights.append( (dir, wg) )
    

  def estimate(self,  method, **args):
      return directionalInterception(self.scene, self.lights, method, **args)
   
  def plot(self, a_property, minval = None, maxval = None, display = True):
      pass