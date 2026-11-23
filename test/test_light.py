from math import sqrt


def test_triangle():
    from openalea.plantgl.all import TriangleSet, Scene, Shape,Vector3
    from openalea.plantgl.light import scene_irradiance, eTriangleProjection
    points = [(0, 0, 0), (0, 0, sqrt(2)), (0, sqrt(2), 0)]
    triangles = Scene([Shape(TriangleSet(points, [list(range(3))]),id=8)])

    lights = [(0,0,1)]
    res = scene_irradiance(triangles, lights, resolution=0.0005)
    print(res)

def test_azel2vect():
    from openalea.plantgl.light import azel2vect, vect2azel
    from openalea.plantgl.all import Vector3, norm
    for az, el, dir in [(0, 90, (0,0,1)), (0, 0, (0,1,0)), (90, 0, (1,0,0)), (180, 0, (0,-1,0)), (270, 0, (-1,0,0))]:
        dir = Vector3(*dir)
        v = -azel2vect(az, el, north = -90)
        print(f"Azimuth: {az}, Elevation: {el} => Vector: {v}, Expected: {dir}")
        assert norm(v - dir) < 1e-6, f"Expected {dir} but got {v}"
        az1, el1 = vect2azel(-v, north = -90)
        assert abs(el1 - el) < 1e-6, f"Expected elevation {el} but got {el1}"
        if el != 90: 
            assert abs(az1 - az) < 1e-6, f"Expected azimuth {az} but got {az1}"
    v = azel2vect(az, el)
    


if __name__ == '__main__':
    test_triangle()

