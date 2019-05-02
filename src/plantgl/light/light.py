from . import sunDome as sd
import openalea.plantgl.all as pgl

from math import radians, sin, pi, ceil


def diffuseInterception(scene):
  return  directionalInterception(scene, directions = sd.skyTurtle())

def directInterception(scene, lat=43.36, long=3.52, jj=221, start=7, stop=19, stp=30, dsun = 1, dGMT = 0):
  direct = sd.getDirectLight( latitude=lat , longitude=int, jourJul=jj, startH=start, stopH=stop, step=stp, decalSun = dsun, decalGMT = dGMT)
  return  directionalInterception(scene, directions = direct)

def totalInterception(scene, lat=43.36, long=3.52, jj=221, start=7, stop=19, stp=30, dsun = 1, dGMT = 0):
  diffu = sd.skyTurtle()
  direct =  sd.getDirectLight( latitude=lat , longitude=int, jourJul=jj, startH=start, stopH=stop, step=stp, decalSun = dsun, decalGMT = dGMT)
  all = direct + diffu
  return directionalInterception(scene, directions = all)


# converter for azimuth elevation 
# az,el are expected in degrees, in the North-clocwise convention
# In the scene, positive rotations are counter-clockwise
#north is the angle (degrees, positive counter_clockwise) between X+ and North
def azel2vect(az, el, north=0):
  azimuth = radians(north - az)
  zenith = radians(90 - el)
  v = -pgl.Vector3(pgl.Vector3.Spherical( 1., azimuth, zenith ) )
  v.normalize()
  return v



def directionalInterceptionGL(scene, directions, north = 0, horizontal = False, screenwidth = 600):
  
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

  for az, el, wg in directions:
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
  #valist = [shapeLight[key] for key in shapeLight.keys() ]
  #print "Min value : ", min(valist)
  #print "Max value : ", max(valist)
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



def directionalInterception(scene, directions, north = 0, horizontal = False, screenresolution = 1, verbose = False):

  bbox=pgl.BoundingBox( scene )
  d_factor = max(bbox.getXRange() , bbox.getYRange() , bbox.getZRange())
  shapeLight = {}
  pixsize = screenresolution*screenresolution

  for az, el, wg in directions:
    if( az != None and el != None):
        dir = azel2vect(az, el, north)
        if horizontal :
            wg /= sin(radians(el))

    up = dir.anOrthogonalVector()
    pjbbx = projectedBBox(bbox, dir, up)
    worldWidth = pjbbx.getXRange()
    worldheight = pjbbx.getYRange()
    w, h = max(2,int(ceil(worldWidth/screenresolution))+1), max(2,int(ceil(worldheight/screenresolution))+1)
    if verbose : 
        print('direction :', dir)
        print('image size :', w,'x',h)
    worldWidth = w * screenresolution
    worldheight = h * screenresolution
    noid = pgl.Shape.NOID
    z = pgl.ZBufferEngine(w,h,noid)
    z.setOrthographicCamera(-worldWidth/2., worldWidth/2., -worldheight/2., worldheight/2., d_factor , 3*d_factor)
    eyepos = bbox.getCenter() - dir* d_factor * 2
    z.lookAt(eyepos, bbox.getCenter(), up) 
    z.render(scene)
    img = z.getImage()
    values = img.histogram()
    if not values is None:
        for shid, val in values:
            if shid != noid:
                shapeLight[shid] = shapeLight.get(shid,0) + val*pixsize*wg
  
  return shapeLight


def scene_irradiance(scene, directions, north = 0, horizontal = False, scene_unit = 'm', screenresolution = 1, verbose = False):
    """
    Compute the irradiance received by all the shapes of a given scene.
   :Parameters:
    - `scene` : scene for which the irradiance has to be computed
    - `directions` : list of tuple composed of the an azimuth, an elevation and an irradiance (in J.s-1.m-2)
    - `north` : the angle between the north direction of the azimuths (in degrees)
    - `horizontal` : specify if the irradiance use an horizontal convention (True) or a normal direction (False)
    - `scene_unit` : specify the units in which the scene is built. Convert then all the result in m.

    :returns: the area of the shapes of the scene in m2 and the irradiance in J.s-1.m-2
    :returntype: pandas.DataFrame
    """

    units = {'mm': 0.001, 'cm': 0.01, 'dm': 0.1, 'm': 1, 'dam': 10, 'hm': 100,
             'km': 1000}

    conv_unit = units[scene_unit]
    conv_unit2 = conv_unit**2


    res = directionalInterception(scene = scene, directions = directions, north = north, horizontal = horizontal, screenresolution=screenresolution, verbose=verbose)
    res = { sid : conv_unit2 * value for sid, value in res.items() }

    surfaces = dict([(sid, conv_unit2*sum([pgl.surface(sh.geometry) for sh in shapes])) for sid, shapes in scene.todict().items()])


    irradiance = { sid : value / surfaces[sid] for sid, value in res.items() }

    import pandas
    return pandas.DataFrame( {'area' : surfaces, 'irradiance' : irradiance} )
