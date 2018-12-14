from math import sqrt


def test_triangle():
    from openalea.plantgl.all import TriangleSet, Scene, Shape,Vector3
    from openalea.plantgl.light import scene_irradiance
    points = [(0, 0, 0), (0, 0, sqrt(2)), (0, sqrt(2), 0)]
    triangles = Scene([Shape(TriangleSet(points, [range(3)]),id=8)])

    lights = [(0,0,1)]
    res = scene_irradiance(triangles, lights, screenresolution=0.0005)
    print res


if __name__ == '__main__':
    test_triangle()

