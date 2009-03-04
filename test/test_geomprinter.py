from openalea.plantgl.all import *
from test_object_creation import defaultobj_func_generator,randomobj_func_generator,randomshape_func_generator,randtransform,randint
from random import uniform

def eval_code(sceneobj,verbose = False):
  assert sceneobj.isValid()
  hasname = sceneobj.isNamed()
  if not hasname:
	  sceneobj.name = 'testedobject'
  printer = PglStrPrinter()
  sceneobj.apply(printer)
  txt = printer.str()
  if verbose:
    print txt
  b = isPglParserVerbose()
  pglParserVerbose(False)
  sc, dic = pgl_read(txt)
  pglParserVerbose(b)
  if not dic.has_key(sceneobj.name):
    print txt
    assert "evaluated object not found"
  eval_object = dic[sceneobj.name]
  assert type(eval_object) == type(sceneobj) and eval_object.name == sceneobj.name
  if not hasname:
	  sceneobj.name = ''
  return eval_object

  
def test_printer_on_default_object():
    for v in defaultobj_func_generator(eval_code):
        yield v


def test_printer_on_random_object():
    for v in randomobj_func_generator(eval_code):
        yield v

def test_printer_on_random_shape():
    for v in randomshape_func_generator(eval_code):
        yield v

import warnings 
        
def test_ifs():
    for i in xrange(5):
        ifs = IFS(randint(1,3),[Transform4(randtransform()) for i in xrange(1,4)],Sphere(radius = uniform(0.1,1),slices = randint(4,255), stacks = randint(4,255)))
        res = eval_code(ifs,True)
        for j in xrange(len(ifs.transfoList)):
            for k in xrange(4):
                if norm(ifs.transfoList[j].getMatrix().getColumn(k) - res.transfoList[j].getMatrix().getColumn(k)) > 1e-3:
                    print ifs.transfoList[j].getMatrix()
                    print res.transfoList[j].getMatrix()
                    print k,norm(ifs.transfoList[j].getMatrix().getColumn(k) - res.transfoList[j].getMatrix().getColumn(k))
                    warnings.warn("Transformation retrieval from matrix4 failed.")

def test_parser_already_declared_error():
    txt = """Group test { GeometryList [ Sphere test { } ] }"""
    b = isPglParserVerbose()
    pglParserVerbose(False)
    sc, dic = pgl_read(txt)    
    pglParserVerbose(b)
                    
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