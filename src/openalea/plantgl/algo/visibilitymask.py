class _MaskEngine:
    def __init__(self, position = (0,0,0), direction=(0,0,1), up=(0,1,0), view_angle=180, angular_resolution=1):
        from openalea.plantgl.math import Vector3
        from ._pglalgo import SphericalCamera
        self.size = self.view_angle/self.angular_resolution
        self.camera = SphericalCamera(self.view_angle)
        self.camera.lookAt(Vector3(position), Vector3(position)+Vector3(direction), Vector3(up))

    def set_mask(self, data):
        self.mask_data = data
    
    def is_visible(self, point):
        raster = self.camera.worldToRaster(point)
        if raster.x < 0 or raster.x >= self.size or raster.y < 0 or raster.y >= self.size:
            return False
        depth = self.mask_data[raster.x, raster.y]
        if depth <= raster.z:
            return True
        else:
            return False

class PonctualVisibilityMask:
    def __init__(self, scene=None, position = (0,0,0), direction=(0,0,1), up=(0,1,0), view_angle=180, angular_resolution=1):
        from openalea.plantgl.math import Vector3
        self.position = Vector3(position)
        self.direction = Vector3(direction)
        self.up = Vector3(up)
        self.scene = scene
        self.view_angle = view_angle
        self.angular_resolution = angular_resolution
    
    def compute(self):
        from ._pglalgo import ZBufferEngine, eIdBased
        from math import ceil
        self.mask = ZBufferEngine(ceil(self.view_angle/self.angular_resolution), ceil(self.view_angle/self.angular_resolution), renderingStyle=eIdBased)
        self.mask.setSphericalCamera(self.view_angle)
        self.mask.lookAt(self.position, self.position+self.direction, self.up) 
        self.mask.process(self.scene)
    
    def is_visible(self, point):
        if not hasattr(self, 'mask'):
            self.compute()
        if norm(point-self.position) < 1e-5 : return False
        return self.mask.isVisible(point)
    
    def get_mask_image(self):
        if not hasattr(self, 'mask'):
            self.compute()
        return self.mask.getDepthBuffer()

    def view(self):
        img = self.get_mask_image().to_array()
        import matplotlib.pyplot as plt
        plt.imshow(img)
        plt.show()
