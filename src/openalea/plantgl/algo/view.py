from math import radians
from openalea.plantgl.math import Vector3, Matrix4, Matrix3, cross
from openalea.plantgl.scenegraph import BoundingBox, Scene
from ._pglalgo import ZBufferEngine, eColorBased
import numpy

def orthoimage(scene : Scene, imgsize : tuple = (800,800), zoom : float = 1, azimuth : float = 0, elevation : float = 0 ) -> numpy.ndarray:
    """
    Compute an orthographic view of a scene.
    :param scene: The scene to render
    :param imgsize: The size of the image
    :param azimuth: The azimuth (in degrees) of view to render
    :param elevation: The elevation (in degrees) of view to render
    :return: The resulting image
    """
    z = ZBufferEngine(imgsize[0],imgsize[1], (255,255,255), eColorBased)
        
    bbx = BoundingBox(scene)
    center = bbx.getCenter()
    bbx.translate(-center)

    v  = Vector3(Vector3.Spherical(-1, radians(azimuth), radians(90-elevation)))
    up  = Vector3(Vector3.Spherical(-1, radians(azimuth), radians(180-elevation)))
    right = cross(up,v)

    frame = [v, right, up]

    m = Matrix4(Matrix3(*frame))

    bbx.transform(m.inverse())

    xmin = bbx.lowerLeftCorner.x
    xmax = bbx.upperRightCorner.x
    size = bbx.getSize()
    msize = max(size[1],size[2])*1.05
    dist = 1

    v *= (size[0]+dist)

    z.setOrthographicCamera(-msize, msize, -msize, msize, dist, dist + xmax - xmin)


    position = center+v
    z.lookAt(position,center,up)
    z.setLight(position, (255,255,255))

    z.process(scene)

    i = z.getImage()
    return i.to_array()


def perspectiveimage(scene : Scene, imgsize : tuple = (800,800), zoom : float = 1, azimuth : float = 0 , elevation : float = 0 ) -> numpy.ndarray:
    """
    Compute an orthographic view of a scene.
    :param scene: The scene to render
    :param imgsize: The size of the image
    :param azimuth: The azimuth (in degrees) of view to render
    :param elevation: The elevation (in degrees) of view to render
    :return: The resulting image
    """
    z = ZBufferEngine(imgsize[0],imgsize[1], (255,255,255), eColorBased)
        
    bbx = BoundingBox(scene)
    center = bbx.getCenter()
    bbx.translate(-center)

    head  = Vector3(Vector3.Spherical(-1, radians(azimuth), radians(90-elevation)))
    up  = Vector3(Vector3.Spherical(-1, radians(azimuth), radians(180-elevation)))
    right = cross(up,head)

    frame = [head, right, up]

    m = Matrix4(Matrix3(*frame))

    bbx.transform(m.inverse())

    size = bbx.getSize()
    msize = max(size[1],size[2])
    dist = ((msize*1.05*zoom+size[0]))
    near = max(0.01,((msize*zoom)-size[0]))

    z.setPerspectiveCamera(90, imgsize[1]/imgsize[0], near, near + 4 * size[0])


    position = center+head*dist
    z.lookAt(position,center,up)
    z.setLight(position, (255,255,255))

    z.process(scene)

    i = z.getImage()
    return i.to_array()

def view(scene: Scene, imgsize : tuple = (800,800), perspective : bool = False, zoom : float = 1, azimuth : float = 0 , elevation : float = 0) -> None:
    """
    Display an orthographic view of a scene.
    :param scene: The scene to render
    :param imgsize: The size of the image
    :param azimuth: The azimuth (in degrees) of view to render
    :param elevation: The elevation (in degrees) of view to render
    """
    if perspective:
        img = perspectiveimage(scene, imgsize=imgsize, zoom=zoom, azimuth=azimuth, elevation=elevation)
    else:
        img = orthoimage(scene, imgsize=imgsize, azimuth=azimuth, elevation=elevation)
    if not img is None:
        import matplotlib.pyplot as plt
        fig, ax = plt.subplots(figsize=(9, 9))
        ax.imshow(img)
        plt.show()

def interactive_view(scene: Scene, imgsize : tuple = (800,800)) -> None:
    from ipywidgets import interact, fixed
    interact(view, scene=fixed(scene), imgsize=fixed(imgsize), perspective=False, zoom=(0.01,100),azimuth=(-180,180), elevation=(-90,90) )
