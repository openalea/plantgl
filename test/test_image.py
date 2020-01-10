from openalea.plantgl.all import *
import matplotlib.pyplot as plot
import numpy as np
from os.path import join, dirname, abspath, exists

path = abspath(join(dirname(__file__),'../share/plantgl/pixmap/geomviewer.png'))
def test_image(view = False):
    print('read')
    i = Image(path)
    print('convert')
    a = i.to_array()
    b  = a / 255.
    print(a.shape)
    npimg = plot.imread(path)

    print(b[0:10,100])
    print(npimg[0:10,100])

    if view:
        i.plot()


def test_image2():
    i = Image()
    a0 = np.array([[[255,0,0], [0,255,0], [0,0,255]],
                        [[0,255,255], [255,0,255], [255,255,0]]], dtype=np.uint8)
    print(i.from_array(a0))
    print(a0.shape)
    print(i.height(), i.width(), i.nbChannels())
    a = i.to_array()
    print(a.shape)
    print("a0")
    print(a0)
    print("a")
    print(a)

def test_image3(view = False):
    npimg = plot.imread(path)
    npimg *= 255
    npimg = npimg.astype(np.uint8)
    i = Image()
    print(i.from_array(npimg))
    if view:
        i.plot()


if __name__ == '__main__':
    test_image3(True)
