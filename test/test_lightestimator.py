from openalea.plantgl.all import *
from openalea.plantgl.light import *
import pandas as pd


def test_total_horizontal_irradiance():
    print('Pure direct sun')
    l = LightEstimator()
    l.add_sun(irradiance = 1, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    print('Total lights:')
    print(l.get_lights_dataframe())
    print(l.get_lights_dataframe()['irradiance'])
    print('Total horizontal irradiance:', l.total_horizontal_irradiance())
    assert( abs(l.total_horizontal_irradiance()-1) < 1e-3 )

    print('Pure diffuse sky')
    l = LightEstimator()
    l.add_sky(irradiance = 1)
    print('Total lights:')
    print(l.get_lights_dataframe())
    assert(len(l.lights) == 46)
    print('Total horizontal irradiance:', l.total_horizontal_irradiance())
    assert( abs(l.total_horizontal_irradiance()-1) < 1e-3 )


    print('Mixed sun and sky')
    l = LightEstimator()
    l.add_sun_sky(ghi=1, dhi = 0.4, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    print('Total lights:')
    print(l.get_lights_dataframe())
    print('Total horizontal irradiance:', l.total_horizontal_irradiance())
    assert( abs(l.total_horizontal_irradiance()-1) < 1e-3 )

try:
    import openalea.astk as atk
except ImportError:
    pass
else:
    def test_astk_total_horizontal_irradiance():
        l = LightEstimator()
        l.add_astk_sun_sky(ghi = 1, dhi = 0.4, sky_type='blended', dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
        print('Total lights:')
        print(l.get_lights_dataframe())
        print('Total horizontal irradiance:', l.total_horizontal_irradiance())
        assert( abs(l.total_horizontal_irradiance()-1) < 1e-3 )
        df = l.get_lights_dataframe()
        print(df[df['type']=='SUN']['irradiance'])

def test_lightestimator_irradiance(view = False):
    l = LightEstimator(Scene([Shape(QuadSet([[-1,-1,0],[-1,1,0],[1,1,0],[1,-1,0]],[list(range(4))]),id=10)])) #.addLights([(0,0,1)])
    l.add_sun_sky(ghi=1, dhi = 0.4, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    print('Total lights:')
    print(l.get_lights_dataframe())
    print('Total horizontal irradiance:', l.total_horizontal_irradiance())
    assert( abs(l.total_horizontal_irradiance()-1) < 1e-3 )
    for primitive in [eShapeBased, eTriangleBased]:
        print('Primitive:', 'ShapeBased' if primitive==eShapeBased else 'TriangleBased')
        for method in available_projection_methods(primitive):
            if method == eOpenGLProjection and view == False:
                continue
            args = {}
            if method == eZBufferProjection:
                args['screenresolution'] = 0.01
            result = l(method = method, primitive=primitive, **args)
            assert 'irradiance' in result
            print('Method:', MethodNames[method], ' - Max irradiance:', max(result['irradiance']))
            assert max(result['irradiance']-1) < 1e-3
            print(result)
            if view:
                l.plot(lightrepscale = 1)

def test_lightestimator_irradiance_with_cache(view = False):
    l = LightEstimator(Scene([Shape(QuadSet([[-1,-1,0],[-1,1,0],[1,1,0],[1,-1,0]],[list(range(4))]),id=10)])) #.addLights([(0,0,1)])
    l.add_sun_sky(ghi=1, dhi = 0.4, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    l.precompute_lights(type='SKY')
    l.precompute_lights(type='SUN')
    for dhi in range(0,11,1):
        l.clear_lights()
        l.add_sun_sky(ghi=1, dhi = dhi/10., dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
        print('Total horizontal irradiance:', l.total_horizontal_irradiance())
        assert( abs(l.total_horizontal_irradiance()-1) < 1e-3 )
        result = l(method = eTriangleProjection, primitive=eTriangleBased)
        assert 'irradiance' in result
        print('DHI:', dhi/10., ' - Max irradiance:', max(result['irradiance']))
        assert max(result['irradiance']-1) < 1e-3
        print(result)


def test_lightestimator(view = False):
    from datetime import datetime
    l = LightEstimator(Scene([Shape(Sphere(),id=10),Shape(Box(0.1,0.1,0.1),id=12)])) #.addLights([(0,0,1)])
    l.add_sun_sky(dhi = 0.5, dates = pd.date_range("27/10/2025 7:00:00","27/10/2025 19:30:00", freq="h"))
    print(l(method = eTriangleProjection, primitive=eTriangleBased))
    if view:
        l.plot(lightrepscale = 1)

if __name__ == '__main__':
    test_lightestimator_irradiance_with_cache()
    exit()
    #for f in ['test_total_horizontal_irradiance',
    #          'test_astk_total_horizontal_irradiance']:
    for f in list(globals().keys()):
        if f.startswith('test_'):
            print('***',f)
            func = globals()[f]
            if not func.__defaults__ is None:
                func(True)
            else:
                func()