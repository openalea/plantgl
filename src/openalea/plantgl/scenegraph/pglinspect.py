from inspect import *
from . import _pglsg as sg

def is_deprecated_attribute(obj, attname):
    d = getattr(getattr(obj.__class__,attname),'__doc__')
    if not d is None:
        return d.startswith('DEPRECATED')
    return False

def get_pgl_attributes(obj):
    attributes = [n for n in dir(obj) if not '__' in n and isdatadescriptor(getattr(obj.__class__,n)) and not is_deprecated_attribute(obj,n)]
    toremove = []
    def att_rank(v):
        try:
            return obj.__init__.__doc__.index(v)
        except ValueError as ve:
            if v != 'name':
                toremove.append(v)
            return len(obj.__init__.__doc__)
    attributes.sort(key = att_rank )
    for v in toremove:
        del attributes[attributes.index(v)]
    return attributes

def is_pgl_attribute_to_default(obj, attname):
    if attname == 'name' : 
        return not obj.isNamed()
    specific = { 'ccw' : 'isCCWToDefault'}
    prefix = 2 if attname[0] in 'uv' else 1
    todefault = 'is'+attname[:prefix].upper()+attname[prefix:]+'ToDefault'
    defaulttest = specific.get(attname, todefault)
    if hasattr(obj,defaulttest) :
        return getattr(obj,defaulttest)()
    else:
        return False


def is_pgl_abstract(cls):
    import inspect
    if not inspect.isclass(cls) : return False
    #if not issubclass(cls,sg.SceneObject): return False
    try:
        cls()
        return False
    except RuntimeError as re:
        return True
    except:
        return False


def is_sceneobject_subclass(cls):
    return issubclass(cls, sg.SceneObject)

def get_all_subclasses(cls_or_tuple):
    if type(cls_or_tuple) in [tuple,list]:
        toprocess = list(cls_or_tuple)
    else:
        toprocess = [cls_or_tuple]
    subclasses = []
    while len(toprocess) > 0:
        newtoprocess = []
        for t in toprocess:
            newtoprocess += t.__subclasses__()
        subclasses += newtoprocess
        toprocess = newtoprocess
    return subclasses