from openalea.plantgl.scenegraph import *
from openalea.plantgl.algo import tesselate, discretize

def matplotlib_plot(scene):
    """Plot a PlantGL scene using Matplotlib.

    Parameters
    ----------
    scene : PlantGL Scene object
        The scene to be plotted.

    Returns
    -------
    None
    """
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    from mpl_toolkits.mplot3d.art3d import Poly3DCollection

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_box_aspect([1,1,1])  # Equal aspect ratio
    

    for shape in scene:
        geometry = shape.geometry
        color = shape.appearance.diffuseColor() if shape.appearance else (1, 1, 1)
        color = (color.clampedRed(), color.clampedGreen(), color.clampedBlue())
        transparency = shape.appearance.transparency if shape.appearance else 0.0
        geometry = discretize(geometry)
        if isinstance(geometry, PointSet) :
            xs, ys, zs = zip(*[(p.x, p.y, p.z) for p in geometry.pointList])
            ax.scatter(xs, ys, zs, c=[color])
        elif isinstance(geometry, Polyline) :
            xs, ys, zs = zip(*[(p.x, p.y, p.z) for p in geometry.pointList])
            ax.plot(xs, ys, zs, c=[color])
        else: 
            geometry = tesselate(geometry)
            
            pts = [[geometry.pointList[i] for i in triangle] for triangle in geometry.indexList]
            ax.add_collection3d(Poly3DCollection(pts, alpha=1-transparency, shade = True, facecolors=color))

    plt.show()