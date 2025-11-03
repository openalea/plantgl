from math import radians, degrees, sin
import openalea.plantgl.all as pgl

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
  return azel2vect(az, el, north)

def vect2azel(vector, north=0):
  svec = pgl.Vector3.Spherical(-vector)
  el = 90 - degrees( svec.phi )
  az = -degrees( svec.theta ) - north
  return (az, el)

def vect2elaz(vector, north=0):
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
            irr /= sin(radians(el))
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