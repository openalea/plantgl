from . import light as lg
from . import sunDome as sd
from .utils import get_timezone, estimate_dir_vectors, vect2azel
from .light import scene_irradiance_from_dir_vectors, eZBufferProjection

Montpellier = {'latitude': 43.610769, 'longitude': 3.876716, 'altitude': 40, 'timezone': 'Europe/Paris'}

class LightEstimator:
  """
  LightEstimator(scene)
  High-level helper class to build directional light sets (sun + sky or arbitrary
  directional sources), attach them to a scene and estimate directional or
  horizontal irradiance over that scene using projection-based methods.
  This class centralizes:
  - Construction and management of directional light lists (3D unit vectors +
    scalar irradiance/weight).
  - Localization (latitude, longitude, altitude, timezone) used to compute sun positions.
  - Convenience generators for common sky/sun distributions (Turtle sky, ASTK,
    uniform sky, sun sequences for given datetimes).
  - Conversion helpers and visualization helpers for lights and results.
  - Execution wrapper to call an irradiance estimation routine with the
    accumulated light set.
  Constructor
  -----------
  scene : object
      A scene object compatible with the downstream irradiance estimator (the
      underlying code expects an object exposing geometry/shape lists and a
      .todict() helper used by visualization helpers).
  Attributes
  scene : object
      Scene passed at construction.
  lights : list of (vector, float)
      List of directional lights. Each element is a tuple (direction, value)
      where `direction` is a 3D vector (pointing from scene origin toward the
      light) and `value` is the irradiance / weight associated with that
      direction. Values may represent direct irradiance or horizontal irradiance
      depending on how they were added.
  localization : (latitude, longitude, altitude, timezone)
      Tuple describing the geographic localization used to compute sun positions.
  result : pandas.DataFrame
      Container holding the output returned by the irradiance estimation method
      after calling estimate(). The exact structure depends on the chosen
      estimator (per-sample irradiance, per-shape irradiance, optional
      'shapeid' column for shape-aggregated outputs, ...)

  Design notes and behavior
  -------------------------
  - Light directions are expected as 3D unit vectors pointing from the scene center
    toward the light (the same convention used by the conversion utilities).
  - The class intentionally separates how irradiance is stored (direct vs
    horizontal) from how it is displayed/used: many helpers accept a `horizontal`
    flag or perform the appropriate sin(elevation) conversions where needed.
  - Date handling is permissive; _check_date normalizes multiple input forms and
    ensures timezone-awareness to avoid silent UTC/local mistakes.
  - Estimation back-ends are pluggable; this class only prepares lights and calls
    the chosen estimator. The result structure depends on that estimator and
    downstream visualization helpers make assumptions (e.g. 'shapeid' presence).
  Examples
  - Create estimator, add sun/sky for today, estimate and visualize:
      est = LightEstimator(scene).localize(42.77, 2.86)
      est.add_sun_sky(ghi=600, dhi=200, dates=None)
      result = est.estimate(method=my_projection_method)
      est.plot('irradiance')
  - Add custom directional lights:
      custom_dirs = [(45, 30, 1.0), (60, -45, 0.5)]  # (elev_deg, az_deg, weight)
      est.add_lights(custom_dirs, north=0, horizontal=True).estimate()
  Thread-safety and performance
  -----------------------------
  - The class is not thread-safe; do not share a single instance across threads
    without external synchronization.
  - Large sky samplings or fine background grids in plot_sky can be computationally
    heavy; sampling resolution and number of lights should be tuned for interactive use.
  """
  
  def __init__(self, scene = None):
      self.scene = scene
      self.north = 90
      self.lights = []
      self.localization = None
      self.result = None
      self.localize(**Montpellier)

  def set_scene(self, scene, north = 90):
    """
    Set the scene and north orientation for the estimator.

    Parameters
    ----------
    scene : object
        The scene object to be used for light estimation.
    north : float
        The angle between the north direction and +X axis (in degrees-positive counter clockwise). Default is 90.

    Returns
    -------
    self : object
        Returns self to allow method chaining.
    """
    self.scene = scene
    self.north = north
    return self
  
  def clear_lights(self):
    """
    Clear all lights from the estimator.
    """
    self.lights = []
    return self
  
  def localize(self, latitude, longitude, altitude, timezone = None):
    """
    Set the localization parameters for the estimator.

    Parameters
    ----------
    latitude : float
        The latitude of the location.
    longitude : float
        The longitude of the location.
    timezone : str or None, optional
        The timezone for the location. If None, it is determined from latitude and longitude.

    Returns
    -------
    self : object
        Returns self to allow method chaining.
    """
    if timezone is None:
        timezone = get_timezone(latitude, longitude)
    self.localization = (latitude, longitude, altitude, timezone)
    return self

  def localize_to_city(self, city_name):
      """
      Set the location of the light estimator using a city name.

      This method uses the geopy library to retrieve the geographical coordinates (latitude and longitude)
      of a given city and sets the location of the light estimator accordingly.

      Parameters
      ----------
      city_name : str
        The name of the city to locate. The city name should be specific enough for geopy to find it uniquely.

      Raises
      ------
      ImportError
        If the geopy package is not installed.
      ValueError
        If the city cannot be found or if there's an error in the geolocation process.

      Examples
      --------
      >>> light_estimator = LightEstimator()
      >>> light_estimator.localize_to_city("Paris, France")
      """
      from .utils import city_localization
      self.localize(*city_localization(city_name))
      return self

  def total_horizontal_irradiance(self):
    """
    Calculate the total horizontal irradiance from the current lights.

    Returns
    -------
    float
        The total horizontal irradiance.
    """
    from math import radians, sin
    total_irradiance = 0
    for direction, intensity in self.lights:
        azimuth, elevation = vect2azel(direction)
        horizontal_component = intensity * max(0, sin(radians(elevation)))
        total_irradiance += horizontal_component
    return total_irradiance

  def get_lights(self):
    """
    Get the current lights in the estimator.

    Returns
    -------
    list
        A list of tuples (direction, intensity) representing the current lights.
    """
    from .utils import vect2elaz
    res = [list(vect2elaz(light[0])) + [light[1]] for light in self.lights]
    import pandas as pd
    return pd.DataFrame(res, columns=['elevation', 'azimuth', 'intensity'])

  def add_light(self, elevation, azimuth, intensity, horizontal = False):
    """
    Add a single light to the estimator.

    Parameters
    ----------
    elevation : float
        The elevation angle of the light in degrees.
    azimuth : float
        The azimuth angle of the light in degrees.
    intensity : float
        The intensity of the light.
    horizontal : bool, optional
        If True, the intensity is adjusted for horizontal irradiance. Default is False.

    Returns
    -------
    self : object
        Returns self to allow method chaining.
    """
    if intensity <= 0:
        return self
    dir = lg.azel2vect(azimuth, elevation, self.north)
    if horizontal :
        from math import radians, sin
        intensity /= sin(radians(elevation))
    self.lights.append((dir, intensity))
    return self
 
  def add_light_from_vectors(self, lights):
    """
    Add a set of lights to the estimator.

    Parameters
    ----------
    lights : list
        A list of tuples (direction, intensity) representing the lights to add.
    """
    self.lights += [(dir,wg) for dir,wg in lights if wg > 0]
    return self
  

  def add_lights(self, directions, horizontal = False):
    """
    Set lights based on specified directions.

    This function calculates light directional vectors and intensity for each specified 
    direction (elevation, azimuth and weight) and adds them to the lights list. 
    If the horizontal flag is set to True, the intensity is supposed to be measured 
    on an horizontal plane and thus adjusted according to the sine of the elevation angle.

    :param directions: A list of tuples, where each tuple contains 
                       an elevation, an azimuth and a weight.
    :param horizontal: A boolean flag specifying if the intensity is given using a horizontal convention and should be adjusted 
                     (True) or not (False). Default is False.
    """

    self.lights += [(d,w) for d,w in estimate_dir_vectors(directions, self.north, horizontal) if w > 0]
    return self 
  
  def _check_date(self, dates):      
    import datetime
    import pandas as pd
    if dates is None:
        today = datetime.datetime.today()
        initdate = datetime.datetime(today.year, today.month, today.day, 1, 0, 0)
        enddate = datetime.datetime(today.year, today.month, today.day, 23, 30, 0)
        dates = pd.date_range(initdate, enddate, freq='h')
    else:
       if type(dates) == datetime.date:
          initdate = datetime.datetime(today.year, today.month, today.day, 1, 0, 0)
          enddate = datetime.datetime(today.year, today.month, today.day, 23, 30, 0)
          dates = pd.date_range(initdate, enddate, freq='h')
       elif type(dates) == str:
           from dateutil.parser import parse
           dates = parse(dates)           
           if dates.hour == 0 and dates.minute == 0 and dates.second == 0:
               initdate = datetime.datetime(dates.year, dates.month, dates.day, 1, 0, 0)
               enddate = datetime.datetime(dates.year, dates.month, dates.day, 23, 30, 0)
               dates = pd.date_range(initdate, enddate, freq='h')
           else:
               dates = pd.to_datetime(dates)
       elif type(dates) == list:
           dates = pd.to_datetime(dates)
    if dates.tzinfo is None:
          timezone = self.localization[3]
          dates = dates.tz_localize(timezone)
    return dates
    
  def _check_daydate(self, date):      
    import datetime
    import pandas as pd
    if date is None:
        date = datetime.datetime.today()
        date.hour = 0
        date.minute = 0
        date.second = 0
    elif type(date) == str:
           from dateutil.parser import parse
           date = parse(date)
    date = pd.to_datetime(date)
    if date.tzinfo is None:
          timezone = self.localization[3]
          date = date.tz_localize(timezone)
    return date
    
  def add_sun(self, dates = None,  irradiance = 1) :
      """
        Adds sunlight sources to the scene based on specified dates and geographic location.

        Parameters
        ----------
        irradiance : float, optional
          The total irradiance of the sun (default is 1).
        dates : list of datetime.datetime or None, optional
          List of dates for which to compute sun positions. If None, uses the current date.
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
      dates = self._check_date(dates)
      latitude, longitude, altitude,timezone = self.localization
      dirs = []
      for date in dates:
        cdate = date.astimezone('UTC')
        dirs += sd.getDirectLight(latitude=latitude, longitude=longitude, 
                                  jourJul=cdate.day_of_year, 
                                  startH=cdate.hour, stopH=cdate.hour+1, step=120, decalSun = 0, decalGMT = 0)
      nbdirs = len(dirs)
      self.add_lights([(el, az, wg*irradiance/nbdirs) for el,az,wg in dirs], horizontal=True)
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
    self.add_lights([(el, az, wg*irradiance) for el, az, wg in sd.skyTurtle()])
    return self
  
  def add_uniform_sky(self,  irradiance = 1) :
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
    sky = sd.skyTurtle()
    nb_lights = len(sky)
    irradiance_per_light = irradiance / nb_lights
    self.add_lights([(el, az, irradiance_per_light) for el, az, wg in sky])
    return self
  
  def add_sun_sky(self, dates = None, ghi = 1, dhi = 0.5):
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
      assert ghi >= dhi, "GHI must be greater than or equal to DHI."
      dates = self._check_date(dates)
      self.add_sun(ghi-dhi, dates=dates)
      self.add_sky(dhi)
      return self

  def estimate_sky_irradiance(self, dates = None, attenuation = None):
      """
      Estimate clear sky irradiance of a clear sky at a given location and date(s).
      This method uses the ASTK (Astronomical ToolKit) package to compute sky irradiance.
      
      Parameters
      ----------
      dates : datetime.datetime or list of datetime.datetime, optional
        Date(s) for which to estimate sky irradiance. If None, uses today's date.
      attenuation : float between 0 and 1, optional
        Optional attenuation coefficient(s) applied to the computed irradiance.
        If None, the default behavior of the underlying ASTK function is used.
        The estimated sky irradiance in W/m². Returns a scalar when a single date
        is provided, or an array with one value per input date when multiple dates
        are provided.
      Returns
      -------
      float or numpy.ndarray
        The estimated sky irradiance (W/m²) for the given date(s) and location.
        Returns a single float if one date is provided, or an array for multiple dates.
      Raises
      ------
      ImportError
        If the required ASTK package is not installed.
      Notes
      -----
      The location parameters (latitude, longitude, altitude, timezone) are taken 
      from the object's localization attribute.
      """
      try:
         import openalea.astk
      except ImportError:
         raise ImportError("ASTK is required for estimate_sky_irradiance. Please install openalea.astk.")    
        
      from openalea.astk.sky_irradiance import sky_irradiance
      dates = self._check_daydate(dates)
 
      latitude, longitude, altitude, timezone = self.localization
      return sky_irradiance(daydate=dates, 
                            attenuation=attenuation, 
                            latitude=latitude, 
                            longitude=longitude, 
                            altitude=altitude, 
                            timezone=timezone)

  def add_astk_sun_sky_from_irradiances(self, irradiances, sky_type='blended', sky_subdivision=4):
      """
      Adds sun and sky light sources to the scene using irradiance data and the ASTK library.

      This method uses the `openalea.astk` package to generate sun and sky sources based on the provided irradiances.
      The sky is discretized according to the specified `sky_subdivision` level, which determines the number of sky directions.
      If `openalea.astk` is not available, it falls back to `add_sun_sky_from_irradiances`.

      Parameters
      ----------
      irradiances : pandas.DataFrame
        DataFrame containing irradiance data with columns:
        - 'ghi': Global Horizontal Irradiance
        - 'dhi': Diffuse Horizontal Irradiance
        The DataFrame index should contain datetime information for the sun positions.
      sky_type : str, optional
        Identifier or configuration for the ASTK sky luminance model to use
        (e.g. 'soc', 'uoc', 'clear_sky', 'sun_soc', 'blended', 'all_weather' ;
        'all_weather' by default). Passed to ASTK's sky luminance routine.
      sky_subdivision : int, optional
        Number of subdivisions for the sky sampling. Default is 4.
        Subdivisions numbers correspond to Turtle sky levels:
        [ 1, 6, 16, 26, 46, 66, 91, 136, 196, 251, 341, 406, 556, 751, 976]

      Returns
      -------
      self : object
        The instance with added sun and sky light sources.

      Warns
      -----
      UserWarning
        If `openalea.astk` is not installed, a warning is issued and a fallback method is used.
      """
      try:
         import openalea.astk
         from openalea.astk.sky_sources import sky_sources, sky_turtle
         sky_discretization = [ 1, 6, 16, 26, 46, 66, 91, 136, 196, 251, 341, 406, 556, 751, 976][sky_subdivision]
         sun, sky = sky_sources(sky_type=sky_type, sky_irradiance=irradiances, sky_dirs = sky_turtle(sky_discretization), scale="ghi", north = 0)
         self.add_lights( sun+sky, horizontal=True)
      except ImportError:
          import warnings
          warnings.warn("ASTK is required for add_astk_sun_sky. Please install openalea.astk. Using add_sun_sky_from_irradiances instead.")
          return self.add_sun_sky_from_irradiances(irradiances)
      return self

  def add_sun_sky_from_irradiances(self, irradiances):
      """
      Add sun and sky light sources based on irradiance data.

      This method creates light sources from a DataFrame containing irradiance measurements.
      It adds directional sun light sources based on the difference between global horizontal
      irradiance (GHI) and diffuse horizontal irradiance (DHI), and a sky light source
      based on the cumulative DHI.

      Parameters
      ----------
      irradiances : pandas.DataFrame
        DataFrame containing irradiance data with columns:
        - 'ghi': Global Horizontal Irradiance
        - 'dhi': Diffuse Horizontal Irradiance
        The DataFrame index should contain datetime information for the sun positions.

      Returns
      -------
      self : LightEstimator
        Returns self for method chaining.

      Notes
      -----
      The sun sources are added for each timestamp in the DataFrame using (GHI - DHI) as intensity.
      A single sky source is added with the cumulative DHI as intensity.
      """
      gdhi = 0
      for date, row in irradiances.iterrows():
          dhi = row['dhi']
          self.add_lights([(row['elevation'], row['azimuth'], row['ghi'] - row['dhi'])], horizontal=True)
          gdhi += dhi
      self.add_sky(gdhi)
      return self

  def add_astk_estimated_sun_sky(self, dates = None, attenuation = None, sky_type='blended', sky_subdivision=4):
    """
    Add an ASTK sun and sky to the scene using irradiances estimated from the provided dates/attenuation.

    This is a convenience wrapper that:
    1. Calls self.estimate_sky_irradiance(...) with the given dates and attenuation to compute sky irradiances.
    2. Calls self.add_astk_sun_sky_from_irradiances(...) with the computed irradiances and the supplied sky parameters
      to create and add the corresponding ASTK sun+sky sources.

    Parameters
    ----------
    dates : optional
       Date, datetime, or iterable of dates (or any format accepted by estimate_sky_irradiance).
       If None, the method's default behavior for estimate_sky_irradiance is used.
    attenuation : optional
       Attenuation parameter(s) forwarded to estimate_sky_irradiance (e.g. atmospheric attenuation factor or map).
    sky_type : str, optional
       Sky model to use when creating the ASTK sky. Default is 'blended'.
    sky_subdivision : int, optional
       Angular subdivision / resolution for the sky discretization. Default is 4.

    Returns
    -------
    object
       The result returned by add_astk_sun_sky_from_irradiances (typically the created ASTK sun/sky object or scene node).

    Raises
    ------
    Any exceptions raised by estimate_sky_irradiance or add_astk_sun_sky_from_irradiances are propagated.

    Examples
    --------
    # Use a single date:
    scene.add_astk_estimated_sun_sky(dates=my_date)

    # Use a time period with higher sky resolution:
    scene.add_astk_estimated_sun_sky(dates=date_range, attenuation=0.7, sky_type='blended', sky_subdivision=8)
    """
    return self.add_astk_sun_sky_from_irradiances(
         self.estimate_sky_irradiance(dates=dates, attenuation=attenuation),
         sky_type=sky_type,
         sky_subdivision=sky_subdivision
     )

  def add_astk_sun_sky(self, ghi = 1, dhi = 0.5, sky_type='blended', sky_subdivision=4, dates = None):
      """
      Add sun and sky lights computed by ASTK to this estimator.
      This method computes an ASTK sun-and-sky distribution from the provided
      irradiance values and dates, converts ASTK's elevation/azimuth into the
      inclination/azimuth format expected by the estimator, and adds the resulting
      directional lights via self.add_lights(...).
      Parameters
      ----------
      ghi : float, optional
        Global horizontal irradiance (GHI) to drive the sun+sky computation.
        Units should match what ASTK expects (typically W/m²). Default is 1.
      dhi : float, optional
        Diffuse horizontal irradiance (DHI). Must be less than or equal to ghi.
        Default is 0.5.
      sky_type : str or object, optional
        Identifier or configuration for the ASTK sky luminance model to use
        (e.g. 'soc', 'uoc', 'clear_sky', 'sun_soc', 'blended', 'all_weather' ;
        'all_weather' by default). Passed to ASTK's sky luminance routine.
      sky_discretization : int, optional
        Number of subdivisions for the sky sampling. Default is 4.
        Subdivisions numbers correspond to Turtle sky levels:
        [ 1, 6, 16, 26, 46, 66, 91, 136, 196, 251, 341, 406, 556, 751, 976]
      dates : datetime/date or iterable, optional
        A date or sequence of dates/times used to compute sun positions and
        sky irradiance. If None, dates are resolved via self._check_date().
      Returns
      -------
      self
        The estimator instance (allows method chaining).
      Raises
      ------
      AssertionError
        If ghi < dhi.
      Notes
      -----
      - This method uses self.localization (latitude, longitude, altitude, timezone) to
        compute solar geometry and sky irradiance.
      - Internally it calls ASTK helpers (sky_irradiance, sky_grid, sky_luminance,
        sky_turtle, sky_map), aggregates the sky sampling, converts elevations to
        inclinations (inclination = 90 - elevation), and calls self.add_lights
        with the computed (inclination, azimuth, weight) tuples.
      """
      try:
        from openalea.astk.sky_sources import sky_sources, sky_turtle
        from openalea.astk.sky_irradiance import sky_irradiance
        if not ghi is None :
          if not dhi is None:
            assert ghi >= dhi, "GHI must be greater than or equal to DHI."
        else:
          dhi = None

        dates = self._check_date(dates)
        sky_discretization = [ 1, 6, 16, 26, 46, 66, 91, 136, 196, 251, 341, 406, 556, 751, 976][sky_subdivision]
        latitude, longitude, altitude, timezone = self.localization
        sky_irr = sky_irradiance(dates=dates, ghi = ghi, dhi = dhi, latitude=latitude, longitude=longitude, altitude=altitude, timezone=timezone)
        sun, sky = sky_sources(sky_type=sky_type, sky_irradiance=sky_irr, sky_dirs = sky_turtle(sky_discretization), scale="ghi", north=0)
        self.add_lights( sun+sky, horizontal=True)
      except ImportError:
          import warnings
          warnings.warn("ASTK is required for add_astk_sun_sky. Please install openalea.astk. Using add_sun_sky instead.")
          self.add_sun_sky(ghi, dhi, dates)
      return self

  def estimate(self,  method=eZBufferProjection, **args):
      """
      Estimates the irradiance datese scene using the specified method.

      Parameters:
        method (callable, optional): The method used for irradiance estimation. Defaults to eZBufferProjection.
        **args: Additional keyword arguments passed to the irradiance estimation method.

      Returns:
        result: The computed irradiance result for the scene.
      """
      assert self.scene is not None, "Scene is not set. Please set the scene before estimating irradiance."
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
    if not self.scene is None:
        bbx = sg.BoundingBox(self.scene)
    else:
        bbx = sg.BoundingBox((-1,-1,-1),(1,1,1))
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
    if self.scene is None:
       sc, cmap = self.lightrepr(minval, maxval)
    else:
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


  
  def plot_sky(self, cmap='jet', background='closest', bgresolution=1, polar =True, projection ='sin', irradiance = 'horizontal'):
     """
    Visualize directional lights on a polar (or planar) sky plot.
    Parameters
    ----------
    cmap : str, optional
      Matplotlib colormap name used to map irradiance values to colors. Default: 'jet'.
    background : {None, 'closest', 'interpolated'}, optional
      Background shading mode:
        - None: no background shading (only light markers).
        - 'closest': assign each background cell the irradiance of the nearest light (nearest-neighbor).
        - 'interpolated': compute each background cell from the 3 nearest lights using distance-based weighting.
      Default: 'closest'.
    bgresolution : int, optional
      Sampling step in degrees for the background azimuth × elevation grid. Typical value 1 (1°×1° grid).
      Larger values coarsen sampling and speed up computation. Default: 1.
    polar : bool, optional
      If True, use a polar projection (azimuth → theta, 0° at geographic North). If False, use a Cartesian plot.
      Default: True.
    projection : {'sin', 'flat'}, optional
      Radial projection transform applied to elevation/zenith:
        - 'sin' (default): use sin(el) to approximate equal‑area mapping.
        - 'flat': use linear scaling.
    irradiance : {'horizontal', 'direct'}, optional
      How stored irradiance values are interpreted:
        - 'horizontal' (default): convert directional irradiance to horizontal by multiplying by sin(elevation).
        - 'direct': use the stored irradiance values unchanged.
    Expected input on self
    ----------------------
    self.lights : iterable of (vector, value)
      Each entry must be a tuple (vector, value) where
        - vector: a 3D Cartesian direction (pointing from scene origin toward the light).
        - value: numeric irradiance associated with that direction.
    Behavior
    --------
    - Converts each 3D direction to azimuth and elevation using the module's utils.vect2azel helper.
    - Maps:
      - azimuth (degrees) → polar theta in radians with 0° at North.
      - elevation (degrees) → radial coordinate using r = 90° − elevation so that horizon (0° elevation) is outermost and zenith (90°) is center.
    - If irradiance == 'horizontal', adjusts irradiance by sin(elevation).
    - Plots light directions as a scatter of markers with edgecolor and colors mapped to irradiance via cmap, and attaches a colorbar labeled "Irradiance".
    - Optional background:
      - Builds a 2D azimuth × zenith grid (default −180..180 azimuth, 0..90 zenith, or a grid based on unique sampled angles when 'interpolated').
      - Uses a 2-D KD-tree (ANNKDTree2) on XY components of the light direction vectors for nearest-neighbor queries.
      - 'closest': assign each grid cell the irradiance of the single nearest light.
      - 'interpolated': combine the 3 nearest lights using simple inverse-distance-like weighting.
      - Draws the background as a shaded pcolormesh (with alpha) so that the scatter markers remain visible.
    Returns
    -------
    None
      Displays the matplotlib figure (plt.show()).
    ------
    AssertionError
      If self.lights is empty.
      If background is not one of {None, 'interpolated', 'closest'}.
      If projection is not one of {'sin', 'flat'}.
      If irradiance is not one of {'horizontal', 'direct'}.
    Notes
    -----
    - Relies on utils.vect2azel / azel2vect for conversions between vectors and azimuth/elevation.
    - Background neighbor queries require openalea.plantgl.algo.ANNKDTree2 and openalea.plantgl.math utilities (e.g. Vector2).
    - The implementation uses a simple distance-based weighting for interpolation (3 nearest neighbors).
    - The radial mapping choices are provided to support either area-preserving-like ('sin') or linear ('flat') visualizations.
     """
     assert len(self.lights) > 0, "No lights to plot. Please add lights first."
     assert background in [None, 'interpolated', 'closest'], "Background must be None, 'interpolated' or 'closest'."
     assert projection in ['sin', 'flat'], "Projection must be 'sin' or 'flat'."
     assert irradiance in ['horizontal', 'direct'], "Irradiance must be 'horizontal' or 'direct'."
     import matplotlib.pyplot as plt
     from math import radians, degrees
     import openalea.plantgl.math as mt
     from .utils import azel2vect
     import numpy as np
     if polar:
        fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
        ax.set_theta_offset(radians(self.north))
        ax.set_theta_direction(-1) # clockwise
     else:
        fig, ax = plt.subplots()

     azimuths, zeniths, values = zip(*[ (vect2azel(vect)[0], 90-vect2azel(vect)[1], value)  for vect, value in self.lights])
     azimuths = np.array(azimuths)
     zeniths = np.array(zeniths)
     values = np.array(values)
     
     if irradiance == 'horizontal': 
        values = [v*np.cos( np.radians(z)) if z != 90 else v/1e-6 for z,v in zip(zeniths, values)]

     def projection_transform(el):
        if projection == 'sin':
            el = np.sin( np.radians(el) )
        else:
            el = el/90.0
        return el
     
     if background is not None:
        from openalea.plantgl.algo import ANNKDTree2
        from openalea.plantgl.math import norm
        refpoints = [vect for vect, value in self.lights]
        refpoints2d = [mt.Vector2(vect.x, vect.y) for vect in refpoints]

        kdtree = ANNKDTree2(refpoints2d)

        if background == 'interpolated':
          pX = np.sort(np.unique(azimuths))
          if pX[0] > -180:
            pX = np.concatenate( (np.array([-180]), pX) )
          if pX[-1] < 180:
            pX = np.concatenate( (pX, np.array([180]) ) )
          
          pY = np.sort(np.unique(zeniths))
          if pY[0] > 0:
            pY = np.concatenate( (np.array([0]), pY) )
          if pY[-1] < 90:
            pY = np.concatenate( (pY, np.array([90]) ) )
        else:
          pX = np.arange(-180,181,bgresolution)
          pY = np.arange(0,91,bgresolution)

        pV = np.zeros( (len(pX), len(pY)) )
        for i, az in enumerate(pX ):
              for j, zen in enumerate(pY):
                    pt = azel2vect(az, 90-zen)
                    pt2 =mt.Vector2(pt.x, pt.y)
                    idx = kdtree.k_closest_points( pt2, 1 )[0]
                    pV[i,j] = values[idx]
        ax.pcolormesh( np.radians(pX-self.north) , projection_transform(pY), pV.T,  shading = 'gouraud', edgecolors=None, cmap=cmap)
    
     scat = ax.scatter( np.radians(azimuths-self.north), projection_transform(np.array(zeniths)), s=50, edgecolors='black', c=values,  cmap=cmap)
     ticks = np.arange(0,91,30)
     ax.set_yticks(projection_transform(ticks), labels=[str(90-yt) for yt in ticks])
     xticks = np.arange(0,361,45)
     def toDir(v):
        return {0:'N', 45:'NE', 90:'E', 135:'SE', 180:'S', 225:'SW', 270:'W', 315:'NW', 360:'N'}[v]
     ax.set_xticks(np.radians(xticks), labels=[toDir(xt) for xt in xticks])
     ax.set_xlabel('Azimuth')
     ax.set_ylabel('Elevation (degrees)')
     fig.colorbar(scat, label='Direct Irradiance' if irradiance == 'direct' else 'Horizontal Irradiance')
     plt.show()
     return self
  
