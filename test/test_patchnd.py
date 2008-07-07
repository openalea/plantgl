from openalea.plantgl.scenegraph.nurbspatch_nd import *

def createCubePatch():
    pts1 = [[[Vector4(i,j,k,1)  for i in xrange (10)]  for j in xrange (10)]  for k in xrange (10)]
    return NurbsPatch3D(pts1)

########################################################

def test_creation():
    """ Creation of a cube patch 3D """
    nb3d = createCubePatch()

########################################################

def test_gpt():
    """ Creation of a cube patch 3D """
    nb3d = createCubePatch()
    res = nb3d.getPointAt(0.5,0.5,0.5)
    print res

########################################################

def test_gdt():
    """ Creation of a cube patch 3D """
    nb3d = createCubePatch()
    res = nb3d.getTDerivativeAt(0.5,0.5,0.5)
    print res

########################################################
    
    
if __name__ == '__main__':
    import traceback as tb
    test_func = [ (n,v) for n,v in globals().items() if 'test' in n]
    test_func.sort(lambda x,y : cmp(x[1].func_code.co_firstlineno,y[1].func_code.co_firstlineno))
    for tfn,tf in test_func:
        print tfn
        try:
            tf()
        except:
            tb.print_exc()
