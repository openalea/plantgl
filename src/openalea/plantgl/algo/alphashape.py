
def alphashape(pointList, alpha = None): 
    import alphashape as ash
    import numpy as np
    import openalea.plantgl.all as pgl
    from trimesh.base import Trimesh
    if alpha is None:
        alpha = ash.optimizealpha(np.array(pointList))
    alpha_shape = ash.alphashape(np.array(pointList), alpha)
    if not isinstance(alpha_shape, Trimesh):
        raise TypeError(type(alpha_shape))
    facedegree = alpha_shape.faces.shape[1]
    nbfaces = alpha_shape.faces.shape[0]
    if nbfaces > 0:
        if facedegree == 3:
            return pgl.TriangleSet(alpha_shape.vertices, [(int(a),int(b),int(c)) for a,b,c in alpha_shape.faces])
        elif facedegree == 4:
            return pgl.QuadSet(alpha_shape.vertices, [(int(a),int(b),int(c),int(d)) for a,b,c,d in alpha_shape.faces])
        elif facedegree > 4:
            return pgl.FaceSet(alpha_shape.vertices, [list(map(int,ind)) for ind in alpha_shape.faces])
