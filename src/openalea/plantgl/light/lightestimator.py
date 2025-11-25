from . import light as lg
from .utils import get_timezone, estimate_dir_vectors, vect2azel
from .light import scene_irradiance_from_dir_vectors, eZBufferProjection
from .lightmanager import LightManager

class LightEstimator (LightManager):
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
  
  def __init__(self, scene = None, north = 90):
      super().__init__()
      self.scene = scene
      self.north = north
      self.sensors = {}

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
    self.precomputed_lights = dict(zip(self.precompute_lights.keys(), [None for i in range(len(self.precomputed_lights))]))
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
      precomputedresult = None
      light_values = []
      for light in self.lights.values():
         if light.enabled:
            lightid = self.get_light_angles(light.name)
            if lightid in self.precomputed_lights:
                if not self.precomputed_lights[lightid] is None:
                    value = self.precomputed_lights[lightid]
                else:
                    value = scene_irradiance_from_dir_vectors(self.scene, [(light.direction, 1)], method=method, **args)
                    self.precomputed_lights[lightid] = value
                if precomputedresult is None:               
                  precomputedresult = value.copy()
                  precomputedresult['irradiance'] *= light.irradiance
                  precomputedresult['interception'] *= light.irradiance
                else:
                  precomputedresult['irradiance'] += value['irradiance']*light.irradiance
                  precomputedresult['interception'] += value['interception']*light.irradiance
            else:
               light_values.append((light.direction, light.irradiance))
      if len(light_values) > 0 :
          print('Compute lights for ',len(light_values),' light sources')
          self.result = scene_irradiance_from_dir_vectors(self.scene, light_values, method=method, **args)
          if not precomputedresult is None:
            self.result['irradiance'] += precomputedresult['irradiance']
            self.result['interception'] += precomputedresult['interception']
      else:
         self.result = precomputedresult
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
    import openalea.plantgl.math as mt
    if not self.scene is None:
        bbx = sg.BoundingBox(self.scene)
    else:
        bbx = sg.BoundingBox((-1,-1,-1),(1,1,1))
    center = bbx.getCenter()
    skyradius = mt.norm(bbx.getSize()) * scale
    lightradius = skyradius/50
    return super().lightrepr(minvalue, maxvalue, center, skyradius, lightradius = 0.2)

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
       
          
  def plot(self, a_property = 'irradiance', minval = None, maxval = None, lightrepscale = 1, sensorformat = 'x', sensorsize = 1, cmapdisplay = True):
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
    import openalea.plantgl.scenegraph as sg
    if self.scene is None:
       sc, cmap = self.lightrepr(minval, maxval)
    else:
      assert hasattr(self, 'result') or hasattr(self, 'sensors_result'), "No result to plot. Please run estimate() or estimate_sensors() first."

      if hasattr(self, 'result'):
        assert a_property in self.result, f"Property '{a_property}' not found in results."

      cmap = None
      if minval is None :
        if hasattr(self, 'result'):
            min_value = min(self.result[a_property])
            if hasattr(self, 'sensors_result') and a_property == 'irradiance':
                min_value = min(min(self.sensors_result.values()), min_value)
        elif a_property == 'irradiance':
            min_value = min(self.sensors_result.values())
        if lightrepscale is not None and a_property == 'irradiance':
            min_value = min(min_value, min(self.lights.values(), key=lambda x: x.irradiance).irradiance)
      else:
          min_value = minval
      if maxval is None:
        if hasattr(self, 'result'):
            max_value = max(self.result[a_property])
            if hasattr(self, 'sensors_result') and a_property == 'irradiance':
                max_value = max(max(self.sensors_result.values()), max_value)
        elif a_property == 'irradiance':
            max_value = max(self.sensors_result.values())
        if lightrepscale is not None and a_property == 'irradiance':
            max_value = max(max_value, max(self.lights.values(), key=lambda x: x.irradiance).irradiance)
      else:
          max_value = maxval

      sc = sg.Scene()
      if hasattr(self, 'result'):
          sc, cmap = self.scenerepr(a_property, min_value, max_value)
      else:
          sc += self.scene 

      if hasattr(self, 'sensors_result'):
         scs, cmap = self.sensors_repr(format=sensorformat, size=sensorsize, minval=min_value, maxval=max_value)
         sc += scs

      if not (lightrepscale is None):
        scl, cmapl = self.lightrepr(min_value if a_property == 'irradiance' else minval, max_value if a_property == 'irradiance' else maxval, lightrepscale)
        sc += scl
        if cmap is None:
          cmap = cmapl

    from openalea.plantgl.gui import Viewer
    if cmapdisplay:
      sc.add(cmap.pglrepr())
    Viewer.display(sc)
    return sc

  def add_sensor(self, id, position, direction = (0,0,1), up=(0,1,0), view_angle=180, angular_resolution=1):
      """
      Add sensors to the estimator using a visibility mask.

      Parameters:
        position (tuple): The position of the sensor in 3D space.
        directions (tuple): The direction vector of the sensor. Default is (0,0,1).
        up (tuple): The up vector for the sensor. Default is (0,1,0).
        view_angle (float): The view angle of the sensor in degrees. Default is 180.
        angular_resolution (float): The angular resolution of the sensor in degrees. Default is 1.

      Returns:
        self : object
            Returns self to allow method chaining.
      """
      from openalea.plantgl.algo.visibilitymask import PonctualVisibilityMask
      mask = PonctualVisibilityMask(self.scene, position, direction, up, view_angle, angular_resolution)
      self.sensors[id] = mask
      return self
  
  def estimate_sensors(self):
      """
      Estimate the irradiance at the sensor positions using the visibility masks.

      Returns:
        sensors_result : list
            A list containing the irradiance results for each sensor.
      """
      assert self.scene is not None, "Scene is not set. Please set the scene before estimating sensors."
      import openalea.plantgl.scenegraph as sg
      import openalea.plantgl.math as mt
      self.sensors_result = {}
      bbx = sg.BoundingBox(self.scene)
      dist = mt.norm(bbx.getSize()) * 2
      for sid, mask in self.sensors.items():
          irradiance = 0.0
          for direction, wg in self.iterate_lights_values():
              visible = mask.is_visible(mask.position-direction*dist)
              if visible:
                  irradiance += wg*mt.dot(-direction, mask.direction)
          sensor_result = irradiance
          self.sensors_result[sid] = sensor_result
      return self.sensors_result
  
  def sensors_repr(self, format = 'x', size = 1, minval = None, maxval = None):
      """
      Generate a PlantGL Scene visualizing sensor locations and a corresponding material colormap.

      This method converts the previously-computed sensor irradiance results (stored in
      self.sensors_result) into a PlantGL Scene (sg.Scene) where each sensor is
      represented by a simple geometric marker and colored according to its irradiance
      value using a PglMaterialMap.

      Parameters
      ----------
      format : str, optional
        One-character code selecting the marker geometry for each sensor. Supported
        values:
          - 'x' : a flat quad (default)
          - 'O' : a sphere
          - 'X' : a box
          - 'o' : a disc
        Any other value will raise a KeyError when the internal shape mapping is
        indexed.
      size : float, optional
        Size (scale) of the marker geometry in scene units. Defaults to 1.
      minval : float or None, optional
        Minimum irradiance value used to construct the colormap. If None (default),
        the minimum of self.sensors_result is used.
      maxval : float or None, optional
        Maximum irradiance value used to construct the colormap. If None (default),
        the maximum of self.sensors_result is used.

      Returns
      -------
      tuple
        A tuple (scene, cmap) where:
          - scene (openalea.plantgl.scenegraph.Scene): a Scene containing one
          sg.Shape per sensor. Each shape is positioned at the corresponding
          sensor camera position and oriented according to the sensor camera's
          world-to-camera axes. The shape's material is obtained by calling the
          returned colormap on the sensor irradiance value. Each shape is added
          with id equal to the sensor index.
          - cmap (openalea.plantgl.scenegraph.colormap.PglMaterialMap): the
          PglMaterialMap instance constructed with the provided or inferred
          minval and maxval; use cmap(value) to obtain the material for a given
          irradiance.

      Raises
      ------
      AssertionError
        If self.sensors_result is None. You must call estimate_sensors() (or
        otherwise populate self.sensors_result) before calling this method.
      KeyError
        If an unsupported format code is passed (the method looks up the format
        in an internal shape mapping).

      Notes
      -----
      - The marker orientation is computed from the sensor camera: direction is taken
        from the camera world-to-camera matrix column 2 and up from column 1. The
        perpendicular axis is computed with mt.cross(dir, up) and passed to
        sg.Oriented together with up to orient the marker geometry.
      - The colormap maps irradiance values in the [minval, maxval] interval to
        materials; values outside that interval are handled by PglMaterialMap's
        behavior.
      - This function imports PlantGL modules internally; the returned Scene and
        materials are PlantGL objects and require PlantGL to be available.

      Example
      -------
      # Visualize sensors with default parameters:
      scene, cmap = obj.sensors_repr()
      """
      assert self.sensors_result is not None, "No sensor results to represent. Please run estimate_sensors() first."
      import openalea.plantgl.scenegraph as sg
      import openalea.plantgl.math as mt
      shapes = {'x' : lambda s : sg.QuadSet([mt.Vector3(-s/2,-s/2,0), mt.Vector3(s/2,-s/2,0), mt.Vector3(s/2,s/2,0), mt.Vector3(-s/2,s/2,0)], [[0,1,2,3]]),
                'O' : lambda s : sg.Sphere(s/2),
                'X' : lambda s : sg.Box(s/2, s/2, s/2),
                'o' : lambda s : sg.Disc(s/2)}
      sc = sg.Scene()

      if minval is None:
        minval = min(self.sensors_result.values())
      if maxval is None:
        maxval = max(self.sensors_result.values())
      from openalea.plantgl.scenegraph.colormap import PglMaterialMap
      cmap  = PglMaterialMap(minval, maxval)

      for sid, irr in self.sensors_result.items():
          sensor = self.sensors[sid]
          pos = sensor.position
          dir = sensor.direction
          up = sensor.up
          sc.add(sg.Shape(sg.Translated(pos, sg.Oriented(mt.cross(dir, up), up, shapes[format](size))), cmap(irr), id=sid))
      return sc, cmap
