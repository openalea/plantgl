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

def estimate_dir_vectors( directions, north = 0, horizontal = False):
  results = []
  for el, az, irr in directions:
        dir = azel2vect(az, el, north)
        if horizontal :
            irr /= sin(radians(el))
        results.append((dir,irr))
  return results
   

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


eShapeBased, eTriangleBased = range(2)
eOpenGLProjection, eZBufferProjection, eTriangleProjection = range(3)


def directionalInterception_openGL(scene, directions, north = 0, horizontal = False, screenwidth = 600):
   return directionalInterception_openGL_from_dir_vectors(scene, estimate_dir_vectors(directions, north, horizontal), screenwidth)
  
def directionalInterception_openGL_from_dir_vectors(scene, directions, screenwidth = 600):
  pgl.Viewer.display(scene)
  redrawPol = pgl.Viewer.redrawPolicy
  pgl.Viewer.redrawPolicy = False
  pgl.Viewer.frameGL.maximize(True)
  
  pgl.Viewer.frameGL.setSize(screenwidth,screenwidth)
  
  cam_pos,cam_targ,cam_up = pgl.Viewer.camera.getPosition()
  pgl.Viewer.camera.setOrthographic()
  pgl.Viewer.grids.set(False,False,False,False)
  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}

  for dir, wg in directions:

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

def directionalInterception_zbuffer(scene, directions, 
                            north = 0, 
                            horizontal = False, 
                            screenresolution = None, 
                            multithreaded = True,
                            infinitize = None,
                            primitive = eShapeBased):
   return directionalInterception_zbuffer_from_dir_vectors(scene, estimate_dir_vectors(directions, north, horizontal), screenresolution, multithreaded, infinitize, primitive)


def directionalInterception_zbuffer_from_dir_vectors(scene, directions, 
                            screenresolution = None, 
                            multithreaded = True,
                            infinitize = None,
                            primitive = eShapeBased):

  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}
  if screenresolution is None:
    screenresolution = d_factor/100
  pixsize = screenresolution*screenresolution

  for dir, wg in directions:

    up = dir.anOrthogonalVector()
    pjbbx = projectedBBox(bbox, dir, up)
    worldWidth = pjbbx.getXRange()
    worldheight = pjbbx.getYRange()
    w, h = max(2,int(ceil(worldWidth/screenresolution))+1), max(2,int(ceil(worldheight/screenresolution))+1)

    worldWidth = w * screenresolution
    worldheight = h * screenresolution
    z = pgl.ZBufferEngine(w, h, renderingStyle=pgl.eIdBased, idPolicy=pgl.ePrimitiveIdBased if primitive == eTriangleBased else pgl.eShapeIdBased)
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
 

    if primitive == eShapeBased :
      values = z.idhistogram(False)
      if not values is None:
        for shid, val in values:
            shapeLight[shid] = shapeLight.get(shid,0) + val*pixsize*wg
    else:
      values = z.aggregateIdSurfaces()
    
      if not values is None:
        for shid, (val, triangleval) in values.items():
            shapeLight[shid] =  [v*pixsize*wg for v in triangleval] if not shid in shapeLight else [ov+v*pixsize*wg for ov,v in zip(shapeLight[shid],triangleval)]
  
  return shapeLight

def directionalInterception_triangleprojection(scene, directions, 
                            north = 0, 
                            horizontal = False,
                            primitive = eShapeBased):
   return directionalInterception_triangleprojection_from_dir_vectors(scene, estimate_dir_vectors(directions, north, horizontal), primitive)

def directionalInterception_triangleprojection_from_dir_vectors(scene, directions, 
                            primitive = eShapeBased):

  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}

  for dir, wg in directions:

    up = dir.anOrthogonalVector()
    pjbbx = projectedBBox(bbox, dir, up)
    worldWidth = pjbbx.getXRange()
    worldheight = pjbbx.getYRange()
    z = pgl.DepthSortEngine(idPolicy=pgl.ePrimitiveIdBased if primitive == eTriangleBased else pgl.eShapeIdBased)
    z.setOrthographicCamera(-worldWidth/2., worldWidth/2., -worldheight/2., worldheight/2., d_factor , 3*d_factor)
    eyepos = bbox.getCenter() - dir* d_factor * 2
    z.lookAt(eyepos, bbox.getCenter(), up) 
    z.process(scene) 

    if primitive == eShapeBased :
      values = z.idsurfaces()
      if not values is None:
        for shid, val in values:
            shapeLight[shid] = shapeLight.get(shid,0) + val*wg
    else:
      values = z.aggregateIdSurfaces()
    
      if not values is None:
        for shid, (val, triangleval) in values.items():
            shapeLight[shid] =  [v*wg for v in triangleval] if not shid in shapeLight else [ov+v*wg for ov,v in zip(shapeLight[shid],triangleval)]
  
  return shapeLight


def directionalInterception(scene, directions, 
                            north = 0, 
                            horizontal = False,
                            method = eZBufferProjection, primitive = eShapeBased,  **args):

  if method == eOpenGLProjection:
      return directionalInterception_openGL(scene, directions, north, horizontal, **args)
  elif method == eZBufferProjection:
      return directionalInterception_zbuffer(scene, directions, north, horizontal, primitive=primitive, **args)
  elif method == eTriangleProjection:
      return directionalInterception_triangleprojection(scene, directions, north, horizontal, primitive=primitive)


def directionalInterception_from_dir_vectors(scene, directions, 
                            method = eZBufferProjection, primitive = eShapeBased,  **args):

  if method == eOpenGLProjection:
      return directionalInterception_openGL_from_dir_vectors(scene, directions, **args)
  elif method == eZBufferProjection:
      return directionalInterception_zbuffer_from_dir_vectors(scene, directions, primitive=primitive, **args)
  elif method == eTriangleProjection:
      return directionalInterception_triangleprojection_from_dir_vectors(scene, directions, primitive=primitive)

def _format_irradiance(scene, interception, primitive, scene_unit = 'm'):
    try:
      import pandas
      def convert(a) : return pandas.DataFrame(a)
    except ImportError as ie:
       def convert(a) : return a
    units = {'mm': 0.001, 'cm': 0.01, 'dm': 0.1, 'm': 1, 'dam': 10, 'hm': 100,
             'km': 1000}

    conv_unit = units[scene_unit]
    conv_unit2 = conv_unit**2

    if primitive == eShapeBased:
      
      surfaces = dict([(sid, conv_unit2*sum([pgl.surface(sh.geometry) for sh in shapes])) for sid, shapes in scene.todict().items()])
      irradiance = { sid : conv_unit2 * value / surfaces[sid] for sid, value in interception.items() }
      if conv_unit2 != 1:
        interception = { sid : conv_unit2 * value  for sid, value in interception.items() }

      return convert( {'area' : surfaces, 'irradiance' : irradiance, 'interception' : interception} )
    else :
       trshapeid0, trirradiance0, trsurface0, trinterception0 = {}, {}, {}, {}
       trid = 0
       for sh in scene:
          tr = pgl.tesselate(sh.geometry)
          nbtr = tr.indexListSize()
          interceptions = interception[sh.id]
          assert len(interceptions) >= nbtr
          if len(interceptions) > nbtr:
             interception[sh.id] = interceptions[nbtr:]
             interceptions = interceptions[:nbtr]
          for i,intercept in enumerate(interceptions):
            surf = pgl.surface(tr.pointAt(i,0),tr.pointAt(i,1),tr.pointAt(i,2))
            trsurface0[trid] = surf * conv_unit2
            trshapeid0[trid] = sh.id
            trirradiance0[trid] = intercept/surf
            trinterception0[trid] = intercept * conv_unit2
            trid += 1
       return convert( {'area' : trsurface0, 'irradiance' : trirradiance0, 'interception' : trinterception0, 'shapeid' : trshapeid0} )

def scene_irradiance(scene, directions, 
                     north = 0, horizontal = False, 
                     scene_unit = 'm', 
                     method = eZBufferProjection, primitive = eShapeBased, **args):
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
    if method == eOpenGLProjection:
       primitive = eShapeBased

    interception = directionalInterception( scene = scene,   directions = directions, 
                                   north = north,   horizontal = horizontal, 
                                   method = method, primitive = primitive,  
                                   **args )
    return _format_irradiance(scene, interception, primitive, scene_unit)
    

def scene_irradiance_from_dir_vectors(scene, directions, 
                     scene_unit = 'm', 
                     method = eZBufferProjection, primitive = eShapeBased, **args):
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
    if method == eOpenGLProjection:
       primitive = eShapeBased

    interception = directionalInterception_from_dir_vectors( scene = scene,   directions = directions, 
                                   method = method, primitive = primitive,  
                                   **args )
    return _format_irradiance(scene, interception, primitive, scene_unit)
    

def get_timezone(latitude, longitude):
  """
  Return the timezone identifier for a given geographic coordinate.

  This function is a thin wrapper around tzfpy.get_tz and returns whatever
  identifier tzfpy provides for the location (for example 'Europe/Paris').

  Args:
    latitude (float): Latitude in decimal degrees. Positive values indicate north;
      expected range is -90.0 to 90.0.
    longitude (float): Longitude in decimal degrees. Positive values indicate east;
      expected range is -180.0 to 180.0.

  Returns:
    str or None: Timezone identifier string as returned by tzfpy.get_tz, or None
    if no timezone could be determined for the given coordinates.

  Raises:
    Propagates exceptions raised by tzfpy.get_tz (for example, if tzfpy is not
    installed or an internal error occurs).

  Notes:
    - Ensure the tzfpy package is available in the environment where this
      function is called.
    - No validation beyond documenting expected ranges is performed here;
      callers should validate coordinates if needed.

  Example:
    >>> get_timezone(48.8566, 2.3522)
    'Europe/Paris'
  """
  from tzfpy import get_tz
  result = get_tz(lat=latitude, lng=longitude)
  return result

class LightEstimator:
  def __init__(self, scene):
      self.scene = scene
      self.lights = []

  def addLightFromVectors(self, lights):
    """
    Add a set of lights to the estimator.

    Parameters
    ----------
    lights : list
        A list of tuples (direction, intensity) representing the lights to add.
    """
    self.lights += lights
    return self
  
  def addLights(self, directions, north = 0, horizontal = False):
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

    self.lights += [(d,w) for d,w in estimate_dir_vectors(directions, north, horizontal) if w > 0]
    return self 
    
  def add_sun(self,  irradiance = 1, dates = None, north = 0, latitude = 42.77, longitude = 2.86, timezone = None) :
      def add_sun(self, irradiance=1, dates=None, north=0, latitude=42.77, longitude=2.86, timezone=None):
        """
        Adds sunlight sources to the scene based on specified dates and geographic location.

        Parameters
        ----------
        irradiance : float, optional
          The total irradiance of the sun (default is 1).
        dates : list of datetime.datetime or None, optional
          List of dates for which to compute sun positions. If None, uses the current date.
        north : float, optional
          The orientation of the north direction in degrees (default is 0).
        latitude : float, optional
          The latitude of the location in degrees (default is 42.77).
        longitude : float, optional
          The longitude of the location in degrees (default is 2.86).
        timezone : str or None, optional
          The timezone for the dates. If None, it is determined from latitude and longitude.

        Returns
        -------
        self : object
          Returns self to allow method chaining.

        Notes
        -----
        This method computes the sun's position and adds corresponding light sources to the scene,
        distributing the specified irradiance among them.
        """
      import datetime
      if dates is None:
          dates = [datetime.datetime.today()]
      if type(dates) == list:
          import pandas as pd
          dates = pd.DatetimeIndex(dates, tz=timezone)
      if dates.tzinfo is None:
         if timezone is None:
            timezone = get_timezone(latitude, longitude)
         dates = dates.tz_localize(timezone)
      dirs = []
      for date in dates:
        cdate = date.astimezone('UTC')
        dirs += sd.getDirectLight(latitude=latitude, longitude=longitude, 
                                  jourJul=cdate.day_of_year, 
                                  startH=cdate.hour, stopH=cdate.hour+1, step=120, decalSun = 0, decalGMT = 0)
      nbdirs = len(dirs)
      self.addLights([(el, az, wg*irradiance/nbdirs) for el,az,wg in dirs], horizontal=True, north=north)
      return self

  def add_sky(self,  irradiance = 1) :
    """
    Adds sky lights to the scene using a predefined sky distribution.

    Parameters
    ----------
    irradiance : float, optional
      The global horizontal irradiance emitted from each sky light (default is 1).

    Returns
    -------
    self
      Returns self to allow method chaining.
    """
    self.addLights([(el, az, wg*irradiance) for el, az, wg in sd.skyTurtle()])
    return self
  
  def add_sun_sky(self, ghi = 1, dhi = 0.5, dates = None, north = 0, latitude = 42.77, longitude = 2.86, timezone = None):
      """
      Adds both sun and sky components to the light model using specified irradiance values and location/time parameters.

      Parameters
      ----------
      ghi : float, optional
        Global horizontal irradiance (default is 1).
      dhi : float, optional
        Diffuse horizontal irradiance (default is 0.5).
      dates : list of datetime.datetime, optional
        List of dates/times for sun position calculation. If None, uses current date/time.
      north : float, optional
        North orientation in degrees (default is 0).
      latitude : float, optional
        Latitude of the location in degrees (default is 42.77).
      longitude : float, optional
        Longitude of the location in degrees (default is 2.86).
      timezone : int or None, optional
        Timezone offset from UTC. If None, uses system default.

      Notes
      -----
      - The sun component is added with direct irradiance (ghi - dhi).
      - The sky component is added with diffuse irradiance (dhi).
      """
      if dates is None:
          import datetime
          dates = [datetime.datetime.today()]
      self.add_sun(ghi-dhi, dates=dates, north=north, latitude=latitude, longitude=longitude, timezone=timezone)
      self.add_sky(dhi)
  
  def estimate(self,  method=eZBufferProjection, **args):
      """
      Estimates the irradiance of the scene using the specified method.

      Parameters:
        method (callable, optional): The method used for irradiance estimation. Defaults to eZBufferProjection.
        **args: Additional keyword arguments passed to the irradiance estimation method.

      Returns:
        result: The computed irradiance result for the scene.
      """
      self.result = scene_irradiance_from_dir_vectors(self.scene, self.lights, method=method, **args)
      return self.result
   
  def __call__(self, method=eZBufferProjection, **args):
    """
    Allows the instance to be called as a function to estimate lighting using the specified method.

    Parameters
    ----------
    method : callable, optional
      The lighting estimation method to use. Defaults to `eZBufferProjection`.
    **args
      Additional keyword arguments passed to the `estimate` method.

    Returns
    -------
    result
      The result of the lighting estimation as returned by the `estimate` method.
    """
    return self.estimate(method=method, **args)
  
  def lightrepr(self, minvalue = None, maxvalue = None, scale = 1.5):
    """
    Generate a visual representation of the lights in the scene.

    This method creates a PlantGL scene with spheres representing each light source.
    The position and size of each sphere are determined by the light's direction and intensity.
    The color of each sphere is mapped according to its intensity using a colormap.

    Parameters:
      minvalue (float, optional): Minimum intensity value for the colormap. If None, uses the minimum intensity among the lights.
      maxvalue (float, optional): Maximum intensity value for the colormap. If None, uses the maximum intensity among the lights.
      scale (float, optional): Scaling factor for the radius used to position the lights. Default is 1.5.

    Returns:
      tuple:
        - sg.Scene: A PlantGL scene containing the visual representation of the lights.
        - PglMaterialMap: The colormap used for mapping light intensities to materials.
    """
    import openalea.plantgl.scenegraph as sg
    import openalea.plantgl.math as mt
    from openalea.plantgl.scenegraph.colormap import PglMaterialMap
    bbx = sg.BoundingBox(self.scene)
    center = bbx.getCenter()
    posradius = mt.norm(bbx.getSize()) * scale
    reprradius = posradius/50
    min_intensity = min([wg for dir, wg in self.lights]) if minvalue is None else minvalue 
    max_intensity = max([wg for dir, wg in self.lights]) if maxvalue is None else maxvalue
    cmap = PglMaterialMap(min_intensity, max_intensity)
    sc = sg.Scene()
    for i,(dir, wg) in enumerate(self.lights):
        sc += sg.Shape(sg.Translated(center-dir*posradius,sg.Sphere(reprradius)),cmap(wg),i+1)
    return sc, cmap

  def scenerepr(self, a_property = 'irradiance', minval = None, maxval = None):
    """
    Generate a PlantGL Scene representation with property-based coloring.

    This method creates a PlantGL Scene (`sg.Scene`) where each shape is colored according to the values
    of a specified property (default: 'irradiance') from the `self.result` dictionary. The coloring is
    determined by a colormap that maps property values to materials or colors, within the specified
    minimum and maximum value range.

    If 'shapeid' is not present in `self.result`, each shape is colored individually using a material map.
    If 'shapeid' is present, shapes are tessellated and colored per shape using a color map.

    Parameters
    ----------
    a_property : str, optional
      The property name in `self.result` to use for coloring (default is 'irradiance').
    minval : float, optional
      The minimum value for the colormap. If None, uses the minimum value found in the property.
    maxval : float, optional
      The maximum value for the colormap. If None, uses the maximum value found in the property.

    Returns
    -------
    sc : sg.Scene
      The generated PlantGL Scene with colored shapes.
    cmap : PglMaterialMap or PglColorMap
      The colormap used for mapping property values to colors/materials.
    """
    import openalea.plantgl.scenegraph as sg
    import openalea.plantgl.algo as ag
    if minval is None:
       minval = min(self.result[a_property])
    if maxval is None:
       maxval = max(self.result[a_property])
    scenedict = self.scene.todict()
    sc = sg.Scene()
    if not 'shapeid' in self.result:
       from openalea.plantgl.scenegraph.colormap import PglMaterialMap
       cmap  = PglMaterialMap(minval, maxval)
       for i, value in self.result[a_property].items():
          mat = cmap(value)
          for sh in scenedict[i]:
             sc.add(sg.Shape(sh.geometry, mat, sh.id, sh.parentId))
    else:
       from openalea.plantgl.scenegraph.colormap import PglColorMap
       cmap  = PglColorMap(minval, maxval)
       defaultmat = sg.Material()
       for sid in self.result['shapeid'].unique():
          tr = ag.tesselate(scenedict[sid][0].geometry)
          if len(scenedict[sid]) > 1:
            for sh in scenedict[sid][1:]:
              tr = ag.merge_geometry(tr,ag.tesselate(sh.geometry))
          tr.colorList = [cmap(v) for i, v in self.result[self.result['shapeid'] == sid][a_property].items()]
          tr.colorPerVertex = False
          sc.add(sg.Shape(tr, defaultmat, int(sid)))
    return sc, cmap
       
          
  def plot(self, a_property = 'irradiance', minval = None, maxval = None, lightrepscale = None, cmapdisplay = True):
    """
    Visualizes the specified property of the light simulation results.

    Parameters:
      a_property (str): The property to visualize (default: 'irradiance').
      minval (float, optional): Minimum value for color mapping. If None and lightrepscale is provided, computed from results.
      maxval (float, optional): Maximum value for color mapping. If None and lightrepscale is provided, computed from results.
      lightrepscale (float, optional): Scale for light representation. If provided, adds light representation to the scene.
      cmapdisplay (bool): If True, displays the color map legend in the visualization (default: True).

    Returns:
      Scene: The PlantGL scene object representing the visualization.
    """
    if minval is None and not lightrepscale is None:
       minval = min(min(self.result[a_property]),min(self.result[a_property]))
    if maxval is None and not lightrepscale is None:
       maxval = max(max(self.result[a_property]),max(self.result[a_property]))
    sc, cmap = self.scenerepr(a_property, minval, maxval)
    if not (lightrepscale is None):
       sc += self.lightrepr(minval, maxval, lightrepscale)[0]
    from openalea.plantgl.gui import Viewer
    if cmapdisplay:
      sc.add(cmap.pglrepr())
    Viewer.display(sc)
    return sc
  
