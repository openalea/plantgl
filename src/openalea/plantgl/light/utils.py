from math import radians, degrees, sin
import openalea.plantgl.all as pgl


def to_horizontal_irradiance(normal_irradiance, elevation):
    if elevation <= 0:
      # horizontal sun cannot have any horizontal component
      return 0
    else:
      return normal_irradiance * sin(radians(elevation))

def to_normal_irradiance(horizontal_irradiance, elevation):
    if elevation <= 0:
      # horizontal sun cannot have any horizontal component
      return horizontal_irradiance
    else:
      return horizontal_irradiance / sin(radians(elevation))

def azel2vect(az, el, north = 0):
  """ converter for azimuth elevation 
      az,el are expected in degrees, in the North-clocwise convention
      In the scene, positive rotations are counter-clockwise
      north is the angle (degrees, positive clockwise) between X+ and North """
  azimuth = radians(-north - az)
  zenith = radians(90 - el)
  v = -pgl.Vector3(pgl.Vector3.Spherical( 1., azimuth, zenith ) )
  v.normalize()
  return v

def elaz2vect(el, az, north = 0):
  """ converter for azimuth elevation 
      az,el are expected in degrees, in the North-clocwise convention
      In the scene, positive rotations are counter-clockwise
      north is the angle (degrees, positive clockwise) between X+ and North """
  return azel2vect(az, el, north)

def vect2azel(vector, north = 0):
  svec = pgl.Vector3.Spherical(-vector)
  el = 90 - degrees( svec.phi )
  az = - north - degrees( svec.theta )
  return (az, el)

def vect2elaz(vector, north = 0):
    az, el = vect2azel(vector, north)
    return (el, az)


def estimate_dir_vectors( directions, north = 0, horizontal = False):
    """ Estimate direction vectors and associated irradiance values from a list of direction tuples.

    Args:
        directions (list of tuple): Each tuple contains (elevation, azimuth, irradiance).
        north (float, optional): North orientation in degrees. Default is 0.
        horizontal (bool, optional): If True, adjusts irradiance to horizontal plane. Default is False.

    Returns:
        list of tuple: Each tuple contains (direction_vector, irradiance).
    """
    results = []
    for el, az, irr in directions:
        dir = azel2vect(az, el, north)
        if horizontal :
            irr = to_horizontal_irradiance(irr, el)
        results.append((dir,irr))
    return results


def getProjectionMatrix(forward, up = pgl.Vector3(0,0,1)):
    """
    Compute a projection matrix that transforms vectors from world coordinates to a local coordinate system.

    The matrix transforms vectors so that the 'forward' direction becomes the Z axis, and the 'up' direction
    helps define the Y axis of the new coordinate system.

    Parameters
    ----------
    forward : pgl.Vector3
        The forward direction vector that will become the Z axis in the new coordinate system.
        Will be normalized during computation.
    up : pgl.Vector3, optional
        The approximate up direction vector, by default (0,0,1).
        Will be normalized during computation.
        The actual up vector in the result may differ as it needs to be perpendicular to forward.

    Returns
    -------
    pgl.Matrix3
        A 3x3 transformation matrix that converts from world coordinates to the local coordinate system
        where:
        - X axis is the 'side' vector (perpendicular to both forward and up)
        - Y axis is the adjusted 'up' vector (perpendicular to forward and side)
        - Z axis is the normalized 'forward' vector
    """
    forward.normalize()
    up.normalize();
    side = pgl.cross(up, forward);
    side.normalize();
    up = pgl.cross(forward, side);
    up.normalize();
    return pgl.Matrix3(side, up, forward).inverse()

def projectedBBox(bbx, direction, up):
    """
    Projects the bounding box `bbx` onto a plane defined by the given `direction` and `up` vectors.

    Args:
        bbx: A bounding box object with methods getXMin(), getXMax(), getYMin(), getYMax(), getZMin(), getZMax().
        direction: A vector specifying the projection direction.
        up: A vector specifying the up direction for the projection.

    Returns:
        A new BoundingBox object representing the projected bounding box in the specified orientation.
    """
    from itertools import product
    proj = getProjectionMatrix(direction,up)
    pts = [proj*pt for pt in product([bbx.getXMin(),bbx.getXMax()],[bbx.getYMin(),bbx.getYMax()],[bbx.getZMin(),bbx.getZMax()])]
    projbbx = pgl.BoundingBox(pgl.PointSet(pts))
    return projbbx


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

LOCALIZATION_CACHE = { 'Montpellier': {'latitude': 43.610769, 'longitude': 3.876716, 'altitude': 40, 'timezone': 'Europe/Paris'}
}

def city_localization(city_name):
      """
      Retrieve the location (latitude, longitude, altitude, timezone) of a city from its name.

      This method uses the geopy library to retrieve the geographical coordinates (latitude and longitude)
      of a given city.

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
      if city_name in LOCALIZATION_CACHE:
          return LOCALIZATION_CACHE[city_name]
      try:
        import geopy
      except ImportError:
        raise ImportError("geopy is required for localize_from_city. Please install geopy.")
      try:
        from geopy.geocoders import Nominatim
        geolocator = Nominatim(user_agent="plantgl_lightestimator")
        location = geolocator.geocode(city_name)
        LOCALIZATION_CACHE[city_name] = {'latitude': location.latitude, 
                                         'longitude': location.longitude, 
                                         'altitude': location.altitude, 
                                         'timezone': get_timezone(location.latitude, location.longitude)}
        return LOCALIZATION_CACHE[city_name]
      except Exception as e:
        raise ValueError(f"Could not localize city '{city_name}': {e}")


def haversine_distance(lat1_deg, lon1_deg, lat2_deg, lon2_deg, R=1):
    """
    Calcule la distance géodésique (grand cercle) entre deux points sur une sphère.
    - lat1_deg, lon1_deg : latitude et longitude du premier point (en degrés)
    - lat2_deg, lon2_deg : latitude et longitude du second point (en degrés)
    - R : rayon de la sphère (par défaut : 1, unité arbitraire)
    """
    import math
    # Conversion en radians
    phi1 = math.radians(lat1_deg)
    phi2 = math.radians(lat2_deg)
    lam1 = math.radians(lon1_deg)
    lam2 = math.radians(lon2_deg)

    # Normalisation de la différence de longitude dans [-π, π]
    dlam = (lam2 - lam1 + math.pi) % (2 * math.pi) - math.pi

    # Différence de latitude
    dphi = phi2 - phi1

    # Formule haversine
    a = math.sin(dphi / 2) ** 2 + math.cos(phi1) * math.cos(phi2) * math.sin(dlam / 2) ** 2
    c = 2 * math.asin(min(1.0, math.sqrt(a)))
    
    return R * c

def plot_sky(azimuths, elevations, values, cmap='jet', background='closest', bgresolution=1, 
             representation = 'polar', projection ='sin', north = -90, colorbarlabel = 'values', elevationticks = True, pointsize = 50, edgecolors= 'black',
             marker = None, vmin = None, vmax = None, colorbar = True, xylabel = True):
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
     assert background in [None, 'interpolated', 'closest'], "Background must be None, 'interpolated' or 'closest'."
     assert representation in ['polar', 'angle','vector'], "Representation should be in 'polar', 'angle','vector'"
     assert projection in ['sin', 'flat'], "Projection must be 'sin' or 'flat'."
     import matplotlib.pyplot as plt
     from math import radians, degrees
     import openalea.plantgl.math as mt
     import numpy as np

     azimuths = np.array(azimuths)
     zeniths = 90.0 - np.array(elevations)
     values = np.array(values)

     if representation == 'polar':
        fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
        ax.set_theta_offset(-radians(north))
        ax.set_theta_direction(-1) # clockwise
        def projection_transform(zen):
            if projection == 'sin':
                zen = np.sin( np.radians(zen) )
            else:
                zen = zen/90.0
            return zen
     elif representation == 'angle':
         fig, ax = plt.subplots()
         def projection_transform(zen): return 90.-zen
     else:
         fig, ax = plt.subplots()
         # lights positions are opposite to lights directions
         lightpositions = [-azel2vect(az, el, north) for az, el in zip(azimuths, elevations)]
         azimuths = [p.x for p in lightpositions]
         zeniths = [p.y for p in lightpositions]
         def projection_transform(zen): return zen
     
     if representation != 'vector' and background is not None:
        from openalea.plantgl.algo import ANNKDTree2
        from openalea.plantgl.math import norm
        refpoints = [azel2vect(az, 90-el, north) for az, el in zip(azimuths, zeniths)]
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
          pX = np.arange(0,361,bgresolution)
          pY = np.arange(0,91,bgresolution)

        pV = np.zeros( (len(pY), len(pX)) )
        for i, zen in enumerate(pY):
          for j, az in enumerate(pX ):
                    pt = azel2vect(az, 90-zen)
                    pt2 =mt.Vector2(pt.x, pt.y)
                    idx = kdtree.k_closest_points( pt2, 1 )[0]
                    pV[i,j] = values[idx]
        ax.pcolormesh( np.radians(pX-north) , projection_transform(pY), pV,  shading = 'gouraud', edgecolors=None, cmap=cmap)
    
     scat = ax.scatter( np.radians(azimuths), projection_transform(zeniths), s=pointsize, edgecolors=edgecolors, c=values,  cmap=cmap, marker = marker, vmin = vmin, vmax = vmax)
     if representation != 'vector':
        ticks = np.arange(0,91,30)
        if elevationticks:
            ax.set_yticks(projection_transform(ticks), labels=[str(90-yt) for yt in ticks])
        else:
            ax.set_yticks(projection_transform(ticks), labels=['' for yt in ticks])
        xticks = np.arange(0,361,45)
        def toDir(v):
            return {0:'N (0°)', 45:'NE', 90:'E\n90°', 135:'SE', 180:'S (180°)', 225:'SW', 270:'W\n270°', 315:'NW', 360:'N (0°)'}[v]
        ax.set_xticks(np.radians(xticks), labels=[toDir(xt) for xt in xticks])
        if xylabel == True:
            ax.set_xlabel('Azimuth')
            ax.set_ylabel('Elevation (degrees)', labelpad=40)
     if colorbar:
      fig.colorbar(scat, label=colorbarlabel, pad=0.1)
     fig.tight_layout()
     return fig, ax

#def scene_checksum(scene):
#   import hashlib
#   checker = hashlib.md5()
#   checker.update(tobinarystring(scene))
#   return checker.hexdigest()