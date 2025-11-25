from .utils import estimate_dir_vectors, vect2azel, azel2vect, to_horizontal_irradiance, to_normal_irradiance
import pandas as pd
import datetime

def to_clockwise(ccw_angle):
    return (-ccw_angle) % 360


class Light:
  """ Light represents a directional light source with a direction and irradiance.
  """

  def __init__(self, name, elevation, azimuth, direction, irradiance, **kwargs):
      self.name = name
      self.elevation = elevation
      self.azimuth = azimuth
      self.direction = direction
      self.irradiance = irradiance
      self.tags = kwargs
      self.enabled = True

  def __repr__(self):
      return f"Light(name={self.name}, elevation={self.elevation}, azimuth={self.azimuth}, irradiance={self.irradiance}, enabled={self.enabled}, direction={self.direction},"+",".join([key+'='+repr(value) for key, value in self.tags.items()]) + ")"

class LightManager:
  """ LightManager is used to manage lights in a PlantGL scene.
    It can create lights from environment maps, sun and sky models, etc.
    It also provides methods to compute irradiance on surfaces given the light setup.
  """

  def __init__(self):
      self.lights = {}
      self.localization = None
      self.localize_to_city("Montpellier")


  def clear_lights(self):
    """
    Clear all lights from the estimator.
    """
    self.lights = {}
    return self
  
  def iterate_lights_values(self):
    for light in self.lights.values():
        if light.enabled:
          yield light.direction, light.irradiance
  
  def lights_values(self):
    return list(self.iterate_lights_values())
        
  def lights_keys(self):
    return list(self.lights.keys())

  def select_lights_from_tags(self, **tag_filters):
    return [name for name, light in self.lights.items() if all(light.tags.get(k) == v for k, v in tag_filters.items())]

  def get_lights_dataframe(self):
    """
    Get the current lights in the estimator.

    Returns
    -------
    list
        A list of tuples (direction, irradiance) representing the current lights.
    """
    attributes = ['name', 'elevation', 'azimuth', 'irradiance', 'enabled']
    attributes += list(set([key for l in self.lights.values() for key in l.tags.keys()]))

    res = [ [getattr(light, attr) if hasattr(light, attr) else (light.tags[attr]) if attr in light.tags else None for attr in attributes] for light in self.lights.values()  ]
    return pd.DataFrame(res, columns=attributes)

  def enabled_light(self, name, enabled = True):
      """
      Enable or disable lights by name.

      Parameters
      ----------
      name : str
        The name of the light to enable or disable.
      enabled : bool, optional
        If True, enable the light; if False, disable it. Default is True.

      Returns
      -------
      self : object
        Returns self to allow method chaining.
      """
      self.lights[name].enabled = enabled
      return self
  
  def localize(self, latitude, longitude, altitude, timezone = None, name = None):
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
        from .utils import get_timezone
        timezone = get_timezone(latitude, longitude)
    self.localization = {'latitude': latitude, 'longitude': longitude, 'altitude': altitude, 'timezone': timezone}
    self.localization_name = name
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
      self.localize(name=city_name, **city_localization(city_name))
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
    for light in self.lights.values():
        horizontal_component = to_horizontal_irradiance(light.irradiance, light.elevation)
        total_irradiance += horizontal_component
    return total_irradiance



  def add_light(self, name, elevation, azimuth, irradiance, horizontal = False, **kwdargs):
    """
    Add a single light to the estimator.

    Parameters
    ----------
    elevation : float
        The elevation angle of the light in degrees.
    azimuth : float
        The azimuth angle of the light in degrees.
    irradiance : float
        The irradiance of the light.
    horizontal : bool, optional
        If True, the irradiance is adjusted for horizontal irradiance. Default is False.

    Returns
    -------
    self : object
        Returns self to allow method chaining.
    """
    if irradiance <= 0:
        return self
    dir = azel2vect(azimuth, elevation, self.north)
    if horizontal :
        from math import radians, sin
        irradiance = to_normal_irradiance(irradiance, elevation)
    self.lights[name] = Light(name, elevation, azimuth, dir, irradiance, **kwdargs)
    return self
 
  def add_lights_from_vectors(self, lights):
    """
    Add a set of lights to the estimator.

    Parameters
    ----------
    lights : list
        A list of tuples (name, direction, irradiance) representing the lights to add.
    """
    for params in lights:
        assert 2 <= len(params) <= 4, "Each light must be a tuple of (name, direction, irradiance, tags) or (name, direction, irradiance) or (direction, irradiance)."
        tags = {}
        if len(params) == 4:
            name, dir, irradiance, tags = params
        elif len(params) == 3:
            name, dir, irradiance = params
        else:
            name = f"light_{len(self.lights)}"
            dir, irradiance = params
        if irradiance > 0:
            self.lights[name] = Light(name, *vect2azel(dir), dir, irradiance, *tags)
    return self
  

  def add_lights(self, directions, horizontal = False):
    """
    Set lights based on specified directions.

    This function calculates light directional vectors and irradiance for each specified 
    direction (elevation, azimuth and irradiance) and adds them to the lights list. 
    If the horizontal flag is set to True, the irradiance is supposed to be measured 
    on an horizontal plane and thus adjusted according to the sine of the elevation angle.

    :param directions: A list of tuples, where each tuple contains 
                       an elevation, an azimuth and an irradiance.
    :param horizontal: A boolean flag specifying if the irradiance is given using a horizontal convention and should be adjusted 
                     (True) or not (False). Default is False.
    """
    for params in directions:
        assert 3 <= len(params) <= 5, "Each light must be a tuple of (name, elevation, azimuth, irradiance, tags) or (name, elevation, azimuth, irradiance) or (elevation, azimuth, irradiance)."
        tags = {}
        if len(params) == 5:
            name, elevation, azimuth, irradiance, tags = params
        elif len(params) == 4:
            name, elevation, azimuth, irradiance = params
        else:
            name = f"light_{len(self.lights)}"
            elevation, azimuth, irradiance = params
        if irradiance > 0:
            self.add_light(name, elevation, azimuth, irradiance, horizontal, **tags)
    return self 
  
  def _check_date(self, dates):      
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
               dates = pd.to_datetime([dates])
       elif type(dates) == list:
           dates = pd.to_datetime(dates)
    if dates.tzinfo is None:
          dates = dates.tz_localize(self.localization['timezone'])
    return dates
    
  def _check_daydate(self, date):      
    if date is None:
        date = datetime.datetime.today()
        date = datetime.datetime(date.year, date.month, date.day, 0, 0, 0)
    elif type(date) == str:
           from dateutil.parser import parse
           date = parse(date)
    date = pd.to_datetime(date)
    if date.tzinfo is None:
          date = date.tz_localize(self.localization['timezone'])
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
      from .sun import sun_positions, direct_horizontal_irradiance, direct_normal_irradiance
      dates = self._check_date(dates)
      latitude, longitude = self.localization['latitude'], self.localization['longitude']
      dirs = []
      sum_unit_irradiance = 0
      dlights = sun_positions(dates=dates, latitude=latitude, longitude=longitude)
      irrs = list(map(direct_horizontal_irradiance, dlights['elevation']))
      sum_unit_irradiance = sum(irrs)
      dlights['irradiance'] = irrs
      dlights['irradiance'] *= irradiance/sum_unit_irradiance

      for date,(el,az,irr) in dlights.iterrows():
         self.add_light(f"sun_{date.strftime('%Y%m%d_%H%M')}", el, az, irr, horizontal=True, date=date, type='SUN')
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
    from .sky import sky_turtle
    for i, (el, az, wg) in enumerate(sky_turtle()):
       self.add_light(f"sky_{i}", el, az, wg*irradiance, horizontal=False, type='SKY')
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
    from .sky import sky_turtle
    sky = sky_turtle()
    nb_lights = len(sky)
    irradiance_per_light = irradiance / nb_lights
    self.add_lights([(f"sky_{i}", el, az, irradiance_per_light, {'type': 'SKY'}) for i, (el, az, wg) in enumerate(sky)], horizontal=False)
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
      self.add_sun(irradiance=ghi-dhi, dates=dates)
      self.add_sky(dhi)
      return self

  def estimate_astk_sky_irradiance(self, dates = None, attenuation = None):
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
          from openalea.astk.sky_irradiance import sky_irradiance

          dates = self._check_daydate(dates)
          return sky_irradiance(daydate=dates,
                                attenuation=attenuation,
                                **self.localization)
      except ImportError:
          import warnings
          warnings.warn("ASTK is required for estimate_astk_sky_irradiance. Please install openalea.astk. Using add_sun_sky_from_irradiances instead.")
          return self.estimate_sky_irradiance(dates, attenuation)

  def estimate_sky_irradiance(self, dates = None, attenuation = None):
      """
      Estimate clear sky irradiance of a clear sky at a given location and date(s).
      
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
      from .sun import sun_irradiance
      dates = self._check_date(dates)
      tau = 0.7 * (1 - 0.7 * attenuation) if attenuation is not None else 0.7
      return sun_irradiance(dates, self.localization['latitude'], self.localization['longitude'], tau)

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
         self.add_lights([(f"sky_{i}",el, to_clockwise(az), irr, {'type': 'SKY'}) for i, (el, az, irr) in enumerate(sky)], horizontal=True)
         for date,(el,az,irr) in zip(sun, irradiances.index):
            self.add_light(f"sun_{date.strftime('%Y%m%d_%H%M')}", el, to_clockwise(az), irr, horizontal=True, date=date, type='SUN')
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
      The sun sources are added for each timestamp in the DataFrame using (GHI - DHI) as irradiance.
      A single sky source is added with the cumulative DHI as irradiance.
      """
      gdhi = 0
      for date, row in irradiances.iterrows():
          dhi = row['dhi']
          self.add_lights([(f'sun{date.strftime("%Y%m%d_%H%M")}', row['elevation'], row['azimuth'], row['dni'], {'date': date, 'type': 'SUN'})], horizontal=False)
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
  
  def add_estimated_sun_sky(self, dates = None, attenuation = None):
    """
    Estimate sky irradiance for the given date(s) and attenuation, then create and add corresponding sun and sky light sources to this manager.

    Parameters
    ----------
    dates : datetime.date, datetime.datetime, or sequence of such, optional
      Date or dates for which to estimate sky irradiance. If None, a default
      date or range defined by estimate_sky_irradiance will be used.
    attenuation : object, optional
      Parameter controlling atmospheric attenuation passed through to
      estimate_sky_irradiance. Accepted types and semantics depend on that
      function (for example a scalar coefficient, spectral attenuation object,
      or a callable).

    Returns
    -------
    object
      The result of add_sun_sky_from_irradiances, forwarded unchanged. This is
      typically the created light descriptors/objects or None depending on the
      implementation of the downstream method.

    Raises
    ------
    Any
      Exceptions raised by estimate_sky_irradiance or add_sun_sky_from_irradiances
      are propagated.

    Notes
    -----
    This is a convenience wrapper that combines irradiance estimation and the
    creation/registration of sun and sky light sources in a single call. It
    mutates the manager by adding the generated light sources.
    """

    return self.add_sun_sky_from_irradiances(self.estimate_sky_irradiance(dates=dates, attenuation=attenuation))

  def add_astk_sun_sky(self, dates = None, ghi = 1, dhi = 0.5, sky_type='blended', sky_subdivision=4):
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
        from .sun import direct_horizontal_irradiance
        if not ghi is None :
          if not dhi is None:
            assert ghi >= dhi, "GHI must be greater than or equal to DHI."
        else:
          dhi = None

        dates = self._check_date(dates)
        sky_discretization = [ 1, 6, 16, 26, 46, 66, 91, 136, 196, 251, 341, 406, 556, 751, 976][sky_subdivision]

        sky_irr = sky_irradiance(dates=dates, ghi = ghi, dhi = dhi, **self.localization)
        sun, sky = sky_sources(sky_type=sky_type, sky_irradiance=sky_irr, sky_dirs = sky_turtle(sky_discretization), scale="ghi", north=0)
        # azimuth is counter-clockwise positive in ASTK

        irr_values = [direct_horizontal_irradiance(el, tau=0.7) for (el, az, irr) in sun]
        irr_values_sum = sum(irr_values)
        irr_values = [w*(ghi-dhi)/irr_values_sum for w in irr_values]

        self.add_lights([(f"sky_{i}",el, to_clockwise(az), irr, {'type': 'SKY'}) for i, (el, az, irr) in enumerate(sky)], horizontal=True)
        for date,(el,az,_),irr in zip(sky_irr.index, sun, irr_values):
            self.add_light(f"sun_{date.strftime('%Y%m%d_%H%M')}", el, to_clockwise(az),  irr, horizontal=True, date=date, type='SUN')
      except ImportError:
          import warnings
          warnings.warn("ASTK is required for add_astk_sun_sky. Please install openalea.astk. Using add_sun_sky instead.")
          self.add_sun_sky(ghi, dhi, dates)
      return self

  def lightrepr(self, minvalue = None, maxvalue = None, center = (0,0,0), skyradius = 10, lightradius = 0.2):
    """
    Generate a visual representation of the lights in the scene.

    This method creates a PlantGL scene with spheres representing each light source.
    The position and size of each sphere are determined by the light's direction and irradiance.
    The color of each sphere is mapped according to its irradiance using a colormap.

    Parameters:
      minvalue (float, optional): Minimum irradiance value for the colormap. If None, uses the minimum irradiance among the lights.
      maxvalue (float, optional): Maximum irradiance value for the colormap. If None, uses the maximum irradiance among the lights.
      scale (float, optional): Scaling factor for the radius used to position the lights. Default is 1.5.

    Returns:
      tuple:
        - sg.Scene: A PlantGL scene containing the visual representation of the lights.
        - PglMaterialMap: The colormap used for mapping light intensities to materials.
    """
    import openalea.plantgl.scenegraph as sg
    from openalea.plantgl.scenegraph.colormap import PglMaterialMap

    min_irradiance = min([wg for dir, wg in self.iterate_lights_values()]) if minvalue is None else minvalue 
    max_irradiance = max([wg for dir, wg in self.iterate_lights_values()]) if maxvalue is None else maxvalue
    cmap = PglMaterialMap(min_irradiance, max_irradiance)
    sc = sg.Scene()
    for i,(dir, wg) in enumerate(self.iterate_lights_values()):
        sc += sg.Shape(sg.Translated(center-dir*skyradius,sg.Sphere(lightradius)),cmap(wg),i+1)
    return sc, cmap

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

     azimuths, zeniths, values = zip(*[ (light.azimuth, 90-light.elevation, light.irradiance)  for light in self.lights.values()])
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
        refpoints = [light.direction for light in self.lights.values()]
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
    
     scat = ax.scatter( np.radians(azimuths), projection_transform(np.array(zeniths)), s=50, edgecolors='black', c=values,  cmap=cmap)
     ticks = np.arange(0,91,30)
     ax.set_yticks(projection_transform(ticks), labels=[str(90-yt) for yt in ticks])
     xticks = np.arange(0,361,45)
     def toDir(v):
        return {0:'N (0°)', 45:'NE', 90:'E\n90°', 135:'SE', 180:'S (180°)', 225:'SW', 270:'W\n270°', 315:'NW', 360:'N (0°)'}[v]
     ax.set_xticks(np.radians(xticks), labels=[toDir(xt) for xt in xticks])
     ax.set_xlabel('Azimuth')
     ax.set_ylabel('Elevation (degrees)', labelpad=40)
     fig.colorbar(scat, label='Direct Irradiance' if irradiance == 'direct' else 'Horizontal Irradiance', pad=0.1)
     if not self.localization_name is None :
        ax.set_title(self.localization_name)
     else:
        ax.set_title(f"Lat: {self.localization['latitude']}, Lon: {self.localization['longitude']}")
     fig.tight_layout()
     plt.show()
     return self
  
