from openalea.plantgl.all import Viewer
 
class scene_writer(object):
    """  Write a plantgl scene to a PNG file """ 
 
    def __call__(self, *inputs):
 
        scene = inputs[0]
        fname = inputs[1]
        Viewer.display(scene)
        Viewer.camera.lookAt((0,0,300),(0,0,0))
        Viewer.frameGL.saveImage(fname)
 
        return fname
