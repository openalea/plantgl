from openalea.plantgl.all import *
from openalea.plantgl.light import *

def projection(method = eZBufferProjection, primitive = eShapeBased):
    s = Scene([Shape(Sphere(), id=10)])
    direction = [(45,0,1)]
    result = scene_irradiance(s, direction, method=method, primitive=primitive)
    return result

def test_differential_projection(view = False):
    result = {}
    for method in available_projection_methods(eTriangleBased) :
        res = projection(method = method, primitive = eTriangleBased)
        result[method] = res
        print(res)
    if view:
        print('Plot comparative results')
        import matplotlib.pyplot as plt
        ref = eTriangleProjection
        refirr = result[ref]['irradiance']
        for method, values in result.items():
            if method != ref:
                plt.plot(refirr, values['irradiance'],'.', label=MethodNames[method])
        plt.xlabel('Reference : '+MethodNames[ref])
        plt.legend()
        plt.show()

def test_triangle():
    result = []
    for method in [eZBufferProjection, eTriangleProjection]:
        res = projection(method = method, primitive = eShapeBased)
        print(res)
        result.append(res)
    print(result[0]['irradiance'] - result[1]['irradiance'])


if __name__ == '__main__':
    for f in list(globals().keys()):
        if f.startswith('test_'):
            print('***',f)
            func = globals()[f]
            if not func.__defaults__ is None:
                func(True)
            else:
                func()