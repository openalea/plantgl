from openalea.plantgl.all import *

def test_nurbspatch3d(U=6,V=4,W=2, du = 0.1):
    import numpy as np
    n = NurbsPatch3D.default(U,V,W)
    d = Discretizer()
    for u in np.arange(0,1.0001,du):
        p = n.getUPatch(u)
        p.apply(d)
        p.isValid()

        p = n.getVPatch(u)
        p.apply(d)
        p.isValid()

        for v in np.arange(0,1.0001,du):
            l = n.getIsoUVSectionAt(u,v)
            l.isValid()
            l.apply(d)

            l = n.getIsoUWSectionAt(u,v)
            l.isValid()
            l.apply(d)

            l = n.getIsoVWSectionAt(u,v)
            l.isValid()
            l.apply(d)

            for w in np.arange(0,1.0001,du):
                pt = n.getPointAt(u,v,w)


def test_nurbspatch3d_vpatch():
    n = NurbsPatch3D.default(6,4,2)
    p = n.getVPatch(0.5)
    p2 = NurbsPatch(p.ctrlPointMatrix)
    assert p.udegree == p2.udegree  and p.vdegree == p2.vdegree
    assert list(p.uknotList) == list(p2.uknotList)  and list(p.vknotList) == list(p2.vknotList)

if __name__ == '__main__':
    test_nurbspatch3d()
    test_nurbspatch3d_vpatch()