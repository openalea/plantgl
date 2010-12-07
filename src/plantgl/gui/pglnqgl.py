import PyQGLViewer  as qgl
import openalea.plantgl.all as pgl

toVec = lambda v : qgl.Vec(v.x,v.y,v.z)
toV3  = lambda v : pgl.Vector3(v.x,v.y,v.z)

bbx2qgl = lambda bbx : (toVec(bbx.lowerLeftCorner),toVec(bbx.upperRightCorner))