from openalea.plantgl.all import *
from openalea.plantgl.light import *

def projection(method = eZBufferProjection, primitive = eShapeBased):
    s = Scene([Shape(Sphere(), id=10)])
    direction = [(0,0,1)]
    result = scene_irradiance(s, direction, method=method, primitive=primitive)
    return result

def test_shape(view = False):
    view = False
    result = []
    for method in [eZBufferProjection, eTriangleProjection]:
        res = projection(method = method, primitive = eTriangleBased)
        print(res)
        result.append(res)
    print(result[0]['irradiance'] - result[1]['irradiance'])
    if view:
        import matplotlib.pyplot as plt
        plt.plot(result[1]['irradiance'], result[0]['irradiance'],'.')
        plt.xlabel('triangle projection')
        plt.ylabel('ZBuffer')
        plt.show()

def test_triangle():
    result = []
    for method in [eZBufferProjection, eTriangleProjection]:
        res = projection(method = method, primitive = eShapeBased)
        print(res)
        result.append(res)
    print(result[0]['irradiance'] - result[1]['irradiance'])
    
def test_lightestimator_irradiance(view = False):
    import pandas as pd
    l = LightEstimator(Scene([Shape(QuadSet([[-1,-1,0],[-1,1,0],[1,1,0],[1,-1,0]],[list(range(4))]),id=10)])) #.addLights([(0,0,1)])
    l.add_sun_sky(dhi = 0.5, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    result = l(method = eTriangleProjection, primitive=eTriangleBased)
    assert 'irradiance' in result
    assert max(result['irradiance']-1) < 1e-3
    print(result)
    if view:
        l.plot(lightrepscale = 1)


def test_lightestimator(view = False):
    import pandas as pd
    from datetime import datetime
    l = LightEstimator(Scene([Shape(Sphere(),id=10),Shape(Box(0.1,0.1,0.1),id=10)])) #.addLights([(0,0,1)])
    l.add_sun_sky(dhi = 0.5, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    print(l(method = eTriangleProjection, primitive=eTriangleBased))
    if view:
        l.plot(lightrepscale = 1)

if __name__ == '__main__':
    for f in list(globals().keys()):
        if f.startswith('test_'):
            print('***',f)
            func = globals()[f]
            if not func.__defaults__ is None:
                func(True)
            else:
                func()