from openalea.plantgl.all import *

def test_visibility_mask():
    sphere = Scene([Translated(Vector3(0,0,5), Sphere(1))])
    mask = PonctualVisibilityMask(sphere, Vector3(0,0,0))
    #mask.view()
    assert mask.is_visible(Vector3(0.1,0,10)) == False
    assert mask.is_visible(Vector3(0,0,-5)) == False
    assert mask.is_visible(Vector3(5,0,0)) == True
    assert mask.is_visible(Vector3(10,0,0)) == True
    assert mask.is_visible(Vector3(0,5,0.01)) == True
    assert mask.is_visible(Vector3(0,0,0.0)) == False
    assert mask.is_visible(Vector3(0.1,0.1,0.0)) == True
    assert mask.is_visible(Vector3(5,0,5)) == True


if __name__ == "__main__":
    test_visibility_mask()
