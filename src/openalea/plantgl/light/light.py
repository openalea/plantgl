import openalea.plantgl.all as pgl
from .utils import azel2vect, estimate_dir_vectors, getProjectionMatrix, projectedBBox

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

def diffuseInterception(scene, ghi, screenresolution = None):
  from .sky import soc_sky_sources
  return  directionalInterception(scene, directions = soc_sky_sources(ghi), screenresolution = screenresolution)

def directInterception(scene, dates, irradiance, latitude=43.36, longitude=3.52,  screenresolution = None):
  from .sun import sun_sources
  return  directionalInterception(scene, sun_sources(dates, latitude, longitude, irradiance), screenresolution = screenresolution)

def totalInterception(scene, ghi, dhi, dates, latitude=43.36, longitude=3.52, screenresolution = None):
  from .sky import soc_sky_sources
  from .sun import sun_sources
  directions = soc_sky_sources(dhi) + sun_sources(dates, latitude, longitude, ghi-dhi)
  return directionalInterception(scene, directions = directions, screenresolution = screenresolution)


eShapeBased, eTriangleBased = range(2)
eOpenGLProjection, eZBufferProjection, eTriangleProjection, eCaribu = range(4)
MethodNames = { eOpenGLProjection : "OpenGL Projection", eZBufferProjection : "Z-Buffer Projection", eTriangleProjection : "Triangle Projection", eCaribu : "Caribu"}

def caribu_available():
    try:
        import openalea.caribu
        return True
    except ImportError:
        return False

def available_projection_methods(primitive = eShapeBased):
    result = [eZBufferProjection, eTriangleProjection] 
    if primitive == eShapeBased :
      result.append(eOpenGLProjection)
    if caribu_available():
        result.append(eCaribu)
    return result

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
    dir = pgl.Vector3(dir)

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
  from math import  ceil

  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}
  if screenresolution is None:
    screenresolution = d_factor/100
  pixsize = screenresolution*screenresolution

  for dir, wg in directions:
    dir = pgl.Vector3(dir)
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
   return parallel_directionalInterception_triangleprojection_from_dir_vectors(scene, estimate_dir_vectors(directions, north, horizontal), primitive)

def directionalInterception_triangleprojection_from_dir_vectors(scene, directions, 
                            primitive = eShapeBased):

  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}

  for dir, wg in directions:
    dir = pgl.Vector3(dir)
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

def __pditfdv_process_direction(args):
    dir, wg, path, primitive = args
    scene = pgl.Scene(path)
    bbox=pgl.BoundingBox( scene )
    d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
    dir = pgl.Vector3(dir)
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
    else:
      values = z.aggregateIdSurfaces()
    return values

def parallel_directionalInterception_triangleprojection_from_dir_vectors(scene, directions, 
                            primitive = eShapeBased):
  import os, tempfile
  with tempfile.TemporaryDirectory() as tmp:
    path = os.path.join(tmp, 'tmpscenefile.bgeom')
    scene.save(path)
    import multiprocessing
    with multiprocessing.Pool() as pool:
      allvalues = pool.map(__pditfdv_process_direction, [(dir, wg, path, primitive) for dir, wg in directions])

  from math import nan
  shapeLight = {}
  for values, (dir, wg) in zip(allvalues, directions):
    if primitive == eShapeBased :
      if not values is None:
        for shid, val in values:
            assert shapeLight.get(shid,0) != nan
            shapeLight[shid] = shapeLight.get(shid,0) + val*wg
    else:
      if not values is None:
        for shid, (val, triangleval) in values.items():
            shapeLight[shid] =  [v*wg for v in triangleval] if not shid in shapeLight else [ov+v*wg for ov,v in zip(shapeLight[shid],triangleval)]
  
  return shapeLight



def directionalInterception(scene, directions, 
                            north = 0, 
                            horizontal = False,
                            method = eZBufferProjection, primitive = eShapeBased,  **args):
  return directionalInterception_from_dir_vectors(scene, estimate_dir_vectors(directions, north, horizontal), method, primitive, **args)


def directionalInterception_from_dir_vectors(scene, directions, 
                            method = eZBufferProjection, primitive = eShapeBased,  **args):

  if method == eOpenGLProjection:
      return directionalInterception_openGL_from_dir_vectors(scene, directions, **args)
  elif method == eZBufferProjection:
      return directionalInterception_zbuffer_from_dir_vectors(scene, directions, primitive=primitive, **args)
  elif method == eTriangleProjection:
      if len(directions) <= 1:
          return directionalInterception_triangleprojection_from_dir_vectors(scene, directions, primitive=primitive)
      else:
          return parallel_directionalInterception_triangleprojection_from_dir_vectors(scene, directions, primitive=primitive)

def _format_irradiance(scene, interception, primitive, scene_unit = 'm'):
    try:
      import pandas
      def convert(a) : 
         res = pandas.DataFrame(a)
         res.fillna(0, inplace=True)
         return res
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

def scene_irradiance_caribu_from_dir_vectors(scene, directions, primitive = eShapeBased, scene_unit = 'm'):
   from openalea.caribu.CaribuScene import CaribuScene
   from math import sin, radians
   from .utils import vect2azel
   import pandas
   # Fake optical property. Not necessary for direct light computation
   opt_prop = { 'GEN' : dict([(sh.id, (0.04,0.0,0.04,0.00) ) for sh in scene]) }
   cs = CaribuScene(scene, opt=opt_prop, scene_unit=scene_unit)
   # light irradiance should be expressed horizontally in Caribu
   lights = [(wg * sin(radians(vect2azel(dir)[1])), tuple(dir)) for dir, wg in directions]
   cs.setLight(lights)

   raw, agg = cs.run(direct=True, infinite = False, split_face = False)
   if primitive == eShapeBased:
      return pandas.DataFrame(dict(area = agg['GEN']['area'], irradiance = agg['GEN']['Ei'], absorbance = agg['GEN']['Eabs'] ))
   else:
      return pandas.DataFrame(dict(area = sum(raw['GEN']['area'].values(),[]), 
                                   irradiance = sum(raw['GEN']['Ei'].values(),[]), 
                                   absorbance = sum(raw['GEN']['Eabs'].values(),[]),
                                   shapeid = [sid for sid, values in raw['GEN']['area'].items() for i in range(len(values))] ))

def scene_irradiance_caribu(scene, directions, 
                            north = 0, 
                            horizontal = False,
                            primitive = eShapeBased, scene_unit = 'm'):
    return scene_irradiance_caribu_from_dir_vectors(scene, estimate_dir_vectors(directions, north, horizontal), primitive, scene_unit)


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
    assert method in [eZBufferProjection, eTriangleProjection,  eCaribu, eOpenGLProjection], "Unknown method for projection"
    if method == eCaribu:
        return scene_irradiance_caribu(scene, directions=directions, north=north, horizontal=horizontal, primitive=primitive, scene_unit=scene_unit)
    else:
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
    assert method in [eZBufferProjection, eTriangleProjection,  eCaribu, eOpenGLProjection], "Unknown method for projection"
    if method == eCaribu:
        return scene_irradiance_caribu_from_dir_vectors(scene, directions=directions, primitive=primitive, scene_unit=scene_unit)
    else:
      if method == eOpenGLProjection:
        primitive = eShapeBased
        

      interception = directionalInterception_from_dir_vectors( scene = scene,   directions = directions, 
                                    method = method, primitive = primitive,  
                                    **args )
      return _format_irradiance(scene, interception, primitive, scene_unit)
    

