import math
import pandas as pd
from datetime import datetime, timezone
from .utils import *

# ---------------------------------------------------------
# Helpers astronomiques standards (NOAA)
# ---------------------------------------------------------

def day_of_year(dt):
    return dt.timetuple().tm_yday

def solar_declination(gamma):
    return (0.006918
            - 0.399912 * math.cos(gamma)
            + 0.070257 * math.sin(gamma)
            - 0.006758 * math.cos(2 * gamma)
            + 0.000907 * math.sin(2 * gamma)
            - 0.002697 * math.cos(3 * gamma)
            + 0.00148  * math.sin(3 * gamma))

def equation_of_time(gamma):
    return (229.18 * (0.000075
                      + 0.001868 * math.cos(gamma)
                      - 0.032077 * math.sin(gamma)
                      - 0.014615 * math.cos(2 * gamma)
                      - 0.040849 * math.sin(2 * gamma)))

def solar_hour_angle(solar_time_minutes):
    return math.radians((solar_time_minutes / 4.0) - 180.0)

# ---------------------------------------------------------
# Calcul principal
# ---------------------------------------------------------

def sun_position(dt, latitude, longitude):
    """
    dt : datetime timezone-aware
    latitude, longitude : degrés décimaux
    Retourne (elevation_deg, azimuth_deg)
    """

    if dt.tzinfo is None:
        raise ValueError("Datetime must be timezone-aware.")

    # Convert to UTC
    dt_utc = dt.astimezone(timezone.utc)

    # Fraction de l'année
    N = day_of_year(dt_utc)
    gamma = 2 * math.pi * (N - 1) / 365

    # Déclinaison solaire
    decl = solar_declination(gamma)

    # Equation of time
    E = equation_of_time(gamma)

    # Temps local
    local_offset = dt.utcoffset().total_seconds() / 3600.0
    minutes = dt.hour * 60 + dt.minute + dt.second / 60

    # Temps solaire vrai
    solar_time = minutes + E + 4 * longitude - 60 * local_offset

    # Angle horaire
    H = solar_hour_angle(solar_time)

    # Conversion rad
    lat = math.radians(latitude)

    # Elevation
    elevation = math.asin(
        math.sin(lat) * math.sin(decl) +
        math.cos(lat) * math.cos(decl) * math.cos(H)
    )

    # Azimut
    azimuth = math.atan2(
        -math.sin(H),
        math.cos(lat) * math.tan(decl) - math.sin(lat) * math.cos(H)
    )

    # Conversion degrés + normalisation azimut [0,360)
    return math.degrees(elevation), (math.degrees(azimuth) + 360) % 360


# ---------------------------------------------------------
# Interface équivalente à ton ancienne méthode `sun_positions`
# ---------------------------------------------------------

def sun_positions(dates, latitude, longitude):
    """
    dates : iterable of timezone-aware datetime
    latitude, longitude : en degrés
    Retourne DataFrame avec elevation/azimuth en degrés
    """
    el = []
    az = []
    idx = []

    for dt in dates:
        elev, azi = sun_position(dt, latitude, longitude)
        if elev > 0:  # Sun above horizon
            el.append(elev)
            az.append(azi)
            idx.append(dt)

    return pd.DataFrame({"elevation": el, "azimuth": az}, index=idx)



# Mean Solar Constant (W/m²)
SOLAR_CONSTANT = 1361

def relative_optical_air_mass(elevation):
    """
    Compute relative optical air mass at given elevation angle (degrees)
    using Kasten & Young formula with horizon limit.
    Returns relative air mass (1.0 at zenith).
    """
    if elevation <= 0.1:
        return 40
    # Limit elevation to avoid extreme air mass
    elevation = max(elevation, 0.1)
    el_rad = math.radians(elevation)
    sin_el = math.sin(el_rad)
    return 1.0 / (sin_el + 0.50572 * (elevation + 6.07995)**(-1.6364))

def direct_normal_irradiance(elevation, tau=0.7):
    """
    Direct Normal Irradiance (DNI) in W/m² at given solar elevation.
    Tau: atmospheric transmittance (0-1)
    """
    assert tau >= 0.0 and tau <= 1.0, "Tau must be in [0, 1]"
    if elevation <= 0:
        return 0.0
    m = relative_optical_air_mass(elevation)
    dni = SOLAR_CONSTANT * tau**(m**0.678)
    return dni

def direct_horizontal_irradiance(elevation, tau=0.7):
    """
    Direct irradiance projected on horizontal surface (W/m²)
    """
    assert tau >= 0.0 and tau <= 1.0, "Tau must be in [0, 1]"
    dni = direct_normal_irradiance(elevation, tau)
    return to_horizontal_irradiance(dni, elevation)

def diffuse_horizontal_irradiance(elevation, tau=0.7):
    """
    Simplified clear-sky diffuse irradiance on horizontal surface (W/m²)
    Approximation of Bird & Hulstrom diffuse component.
    """
    assert tau >= 0.0 and tau <= 1.0, "Tau must be in [0, 1]"
    if elevation <= 0:
        return 0.0
    m = relative_optical_air_mass(elevation)
    diffuse_ratio = 0.271 - 0.294 * tau**(m**0.576) * sin(radians(elevation))
    # Clamp diffuse_ratio between 0 and 1
    diffuse_ratio = max(min(diffuse_ratio, 1.0), 0.0)
    return SOLAR_CONSTANT * diffuse_ratio

def total_horizontal_irradiance(elevation, tau=0.7):
    """
    Total irradiance on horizontal surface = direct + diffuse (W/m²)
    """
    assert tau >= 0.0 and tau <= 1.0, "Tau must be in [0, 1]"
    dhi = diffuse_horizontal_irradiance(elevation, tau)
    dhi = max(dhi, 0.0)  # Ensure non-negative
    dhi = min(dhi, SOLAR_CONSTANT)  # Clamp to reasonable max
    return direct_horizontal_irradiance(elevation, tau) + dhi


def sun_irradiance(dates, latitude, longitude, tau =0.7):
    result = sun_positions(dates, latitude, longitude)
    dni = []
    dhi = []
    ghi = []
    
    for elevation in result['elevation']:
        dni.append(direct_normal_irradiance(elevation, tau))
        dhi.append(diffuse_horizontal_irradiance(elevation, tau))
        ghi.append(total_horizontal_irradiance(elevation, tau))
    
    result['dhi'] = dhi
    result['dni'] = dni
    result['ghi'] = ghi

    return result

def sun_sources(dates, latitude, longitude, irradiance):
    result = []
    sources = sun_irradiance(dates, latitude, longitude, tau)
    sumirr = sources['ghi'].sum() - sources['dhi'].sum()
    for date, (elevation, azimuth, dhi, dni, ghi) in sun_irradiance(dates, latitude, longitude, tau).iterrow():
        result.append((elevation, azimuth, (ghi-dhi)*irradiance/sumirr))
    return result

