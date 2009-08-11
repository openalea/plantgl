from openalea.plantgl.all import *


def test_filter():
    p = Polyline2D([(0,0),(1,1),(0,2),(-1,1.5),(-0.9,1.4),(-0.8,1.2),(-0.7,1.3),(0,1.3),(0,0)])
    skel = Skeleton.getChordalAxisTransform(p,0.01)
    Viewer.display(p)
    Viewer.add(Shape(Group(skel),Material((200,0,0))))
    
if __name__ == '__main__':
    test_filter()