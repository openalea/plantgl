from openalea.plantgl.all import *
from openalea.plantgl.light import *

def projection(method = eZBufferProjection, primitive = eShapeBased):
    s = Scene([Shape(Sphere(), id=10)])
    direction = [(45,0,1)]
    result = scene_irradiance(s, direction, method=method, primitive=primitive)
    return result

def test_differential_projection(view = False):
    result = []
    for method in [eTriangleProjection, eZBufferProjection, eCaribu]:
        res = projection(method = method, primitive = eTriangleBased)
        result.append(res)
        print(res)
    if view:
        print('Plot comparative results')
        import matplotlib.pyplot as plt
        plt.plot(result[0]['irradiance'], result[1]['irradiance'],'.', label='ZBuffer')
        plt.plot(result[0]['irradiance'], result[2]['irradiance'],'.', label='Caribu')
        plt.xlabel('Reference Triangle projection')
        plt.legend()
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
    l.add_sun_sky(dhi = 0.4, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    for method in [eZBufferProjection, eTriangleProjection, eCaribu]:
        args = {}
        if method == eZBufferProjection:
            args['screenresolution'] = 0.01
        result = l(method = method, primitive=eTriangleBased, **args)
        assert 'irradiance' in result
        print('Method:', MethodNames[method], ' - Max irradiance:', max(result['irradiance']))
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