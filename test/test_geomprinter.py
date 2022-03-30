import openalea.plantgl.all as pgl
from test_object_creation import shapebenchmark_generator,randtransform,randint
from random import uniform

import pytest

def eval_code(sceneobj,verbose = True):
  assert sceneobj.isValid()
  hasname = sceneobj.isNamed()
  if not hasname:
    sceneobj.name = 'testedobject'
  fname = 'geomprinteroutput.geom'
  printer = pgl.PglFilePrinter(fname)
  sceneobj.apply(printer)
  txt = open(fname,'r').read()
  printer = pgl.PglStrPrinter()
  sceneobj.apply(printer)
  txt2 = printer.str()
  print(repr(txt))
  print(repr(txt2))
  assert txt == txt2
  if verbose: print(txt)
  if 'PGL_ASCII_PARSER' in pgl.get_pgl_supported_extensions():
      b = pgl.isPglParserVerbose()
      pgl.pglParserVerbose(False)
      print('read')
      sc, dic = pgl.pgl_read(txt)
      pgl.pglParserVerbose(b)
      if sceneobj.name not in dic:
        print(txt)
        assert "evaluated object not found"
      eval_object = dic[sceneobj.name]
      assert type(eval_object) == type(sceneobj) and eval_object.name == sceneobj.name
      if not hasname:
         sceneobj.name = ''
      return eval_object
  else:
    return sceneobj


@pytest.mark.parametrize('sceneobj', list(shapebenchmark_generator()))
def test_printer_on_benchmark_objects(sceneobj):
    eval_code(sceneobj)

# def test_printer_on_default_object():
#     for v in defaultobj_func_generator(eval_code):
#         yield v


# def test_printer_on_random_object():
#     for v in randomobj_func_generator(eval_code):
#         yield v

# def test_printer_on_random_shape():
#     for v in randomshape_func_generator(eval_code):
#         yield v

import warnings 
        
def test_ifs():
    for i in range(5):
        ifs = pgl.IFS(randint(1,3),[pgl.Transform4(randtransform()) for i in range(1,4)],pgl.Sphere(radius = uniform(0.1,1),slices = randint(4,255), stacks = randint(4,255)))
        res = eval_code(ifs,True)
        for j in range(len(ifs.transfoList)):
            for k in range(4):
                if pgl.norm(ifs.transfoList[j].getMatrix().getColumn(k) - res.transfoList[j].getMatrix().getColumn(k)) > 1e-3:
                    print(ifs.transfoList[j].getMatrix())
                    print(res.transfoList[j].getMatrix())
                    print(k,pgl.norm(ifs.transfoList[j].getMatrix().getColumn(k) - res.transfoList[j].getMatrix().getColumn(k)))
                    warnings.warn("Transformation retrieval from matrix4 failed.")

def test_parser_already_declared_error():
    txt = """Group test { GeometryList [ Sphere test { } ] }"""
    b = pgl.isPglParserVerbose()
    pgl.pglParserVerbose(False)
    if 'PGL_ASCII_PARSER' in pgl.get_pgl_supported_extensions():
            sc, dic = pgl.pgl_read(txt)
            pgl.pglParserVerbose(b)

def test_longid() :
    txt = ''' Shape s0 {  Id  2599904224 Geometry  Sphere{} Appearance  Material {} } '''

    b = pgl.isPglParserVerbose()
    pgl.pglParserVerbose(False)
    if 'PGL_ASCII_PARSER' in pgl.get_pgl_supported_extensions():
            sc, dic = pgl.pgl_read(txt)
            pgl.pglParserVerbose(b)


if __name__ == '__main__':
    import traceback as tb
    test_func = [ (n,v) for n,v in list(globals().items()) if 'test_' in n]
    test_func.sort(key=lambda x : x[1].__code__.co_firstlineno)
    for tfn,tf in test_func:
        print(tfn)
        if hasattr(tf, 'pytestmark'):
            args = tf.pytestmark[0].args[1]
            for arg in args:
                try:
                    tf(arg)
                except:
                    tb.print_exc()
        else:
            try:
                tf()       
            except:
                tb.print_exc()        