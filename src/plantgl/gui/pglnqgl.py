import PyQGLViewer  as qgl
import openalea.plantgl.all as pgl

toVec = lambda v : qgl.Vec(v.x,v.y,v.z)
toV3  = lambda v : pgl.Vector3(v.x,v.y,v.z)

toC3  = lambda v : pgl.Color3(v.red(),v.green(),v.blue())
toC4  = lambda v : pgl.Color4(v.red(),v.green(),v.blue(),v.alpha())
toQC  = lambda v : pgl.QColor(v.red,v.green,v.blue)

bbx2qgl = lambda bbx : (toVec(bbx.lowerLeftCorner),toVec(bbx.upperRightCorner))