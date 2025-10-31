from . import light as lg
from . import sunDome as sd
from .utils import get_timezone, estimate_dir_vectors, vect2azel
from .light import scene_irradiance_from_dir_vectors, eZBufferProjection


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
      return self

  def add_astk_sun_sky(self, ghi = 1, dhi = 0.5, sky_type='all_weather', dates = None, north = 0, latitude = 42.77, longitude = 2.86, timezone = None):
      from openalea.astk.sky_map import sky_grid, sky_map, show_sky
      from openalea.astk.sky_sources import sky_turtle
      from openalea.astk.sky_luminance import sky_luminance
      from openalea.astk.sky_irradiance import sky_irradiance
      if dates is None:
            import datetime
            dates = [datetime.datetime.today()]
      if timezone is None:
            timezone = get_timezone(latitude, longitude)
      if type(dates) == list:
          import pandas as pd
          dates = pd.DatetimeIndex(dates, tz=timezone)    
      sky_irr = sky_irradiance(dates=dates, ghi = ghi, dhi = dhi, latitude=latitude, longitude=longitude, timezone=timezone)
      grid = sky_grid()
      sun, sky= sky_luminance(grid, sky_type, sky_irr)
      disc = sky_turtle()
      sky_agg, grid_agg, sky_disc = sky_map(grid, sky, disc)
      self.addLights( [(90-el, az, wg) for az, el, wg in zip(*grid_agg)], north=north)
      return self

  def estimate(self,  method=eZBufferProjection, **args):
      """
      Estimates the irradiance of the scene using the specified method.

      Parameters:
        method (callable, optional): The method used for irradiance estimation. Defaults to eZBufferProjection.
        **args: Additional keyword arguments passed to the irradiance estimation method.

      Returns:
        result: The computed irradiance result for the scene.
      """
      assert method in lg.available_projection_methods(), f"Method {method} is not available. Available methods are: {lg.available_projection_methods()}"
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
    assert hasattr(self, 'result'), "No result to plot. Please run estimate() first."
    assert a_property in self.result, f"Property '{a_property}' not found in results."

    if minval is None :
       min_value = min(self.result[a_property])
       if lightrepscale is not None and a_property == 'irradiance':
          min_value = min(min_value, min(self.lights, key=lambda x: x[1])[1])
    else:
         min_value = minval
    if maxval is None:
       max_value = max(self.result[a_property])
       if lightrepscale is not None and a_property == 'irradiance':
          max_value = max(max_value, max(self.lights, key=lambda x: x[1])[1])
    else:
         max_value = maxval

    sc, cmap = self.scenerepr(a_property, min_value, max_value)

    if not (lightrepscale is None):
       sc += self.lightrepr(min_value if a_property == 'irradiance' else minval, max_value if a_property == 'irradiance' else maxval, lightrepscale)[0]

    from openalea.plantgl.gui import Viewer
    if cmapdisplay:
      sc.add(cmap.pglrepr())
    Viewer.display(sc)
    return sc
  
  def plot_sky(self, cmap='jet'):
     import matplotlib.pyplot as plt
     from math import radians
     fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
     ax.set_theta_zero_location("N")
     azimuths, elevations, values = zip(*[ (radians(vect2azel(vect)[0]), radians(90-vect2azel(vect)[1]), value)  for vect, value in self.lights])
     scat = ax.scatter( azimuths, elevations, s=50, c=values, cmap=cmap)
     fig.colorbar(scat, label='Irradiance')
     plt.show()
  
