from . import _pglsg as sg
from openalea.plantgl.math import Vector2,Vector3,Vector4

def get_attributes(obj):
    import inspect
    attributes = [n for n in dir(obj) if not '__' in n and n != 'name' and inspect.isdatadescriptor(getattr(obj,n))]
    toremove = []
    def att_rank(v):
        try:
            return obj.__init__.__doc__.index(v)
        except ValueError as ve:
            #print(ValueError(cls, v))
            toremove.append(v)
            return len(obj.__init__.__doc__)
    attributes.sort(key = att_rank )
    for v in toremove:
        if v != 'ctrlPointMatrix':
            del attributes[attributes.index(v)]
    return attributes


def for_types(types):
    if type(types) != list:
        types = [types]
    toprocess = list(types)
    while len(toprocess) > 0:
        newtoprocess = []
        for t in toprocess:
            newtoprocess += t.__subclasses__()
        types += newtoprocess
        toprocess = newtoprocess
    def for_given_types(f):
        f.__for_types__ = types
        return f
    return for_given_types


class JsonRepConverter:
    def __init__(self):
        self.clear()
        self.conversion_map = {  }
        self._build_conversion_map()

    def _build_conversion_map(self):
        for funcname in dir(self):
            func = getattr(self,funcname)
            if hasattr(func,'__for_types__'):
                for t in func.__for_types__:
                    self.add_to_conversion_map(t,func)

    def add_to_conversion_map(self, ptype, func):
        self.conversion_map[ptype] = func

    def clear(self):
        self.cache = {}

    def pglobject(self, obj, *args):
        res = dict(name = obj.name, id = obj.getObjectId(), type = obj.__class__.__name__)
        return self.pglattributes(res, obj, *args)

    def pglattributes(self, res, obj):
        attributes = get_attributes(obj)

        for att in attributes:
            defaulttest = 'is'+att[0].upper()+att[1:]+'ToDefault'
            if hasattr(obj,defaulttest) and getattr(obj,defaulttest)():
                continue
            res[att] = self.convert(getattr(obj,att))
        return res

    def convert(self,obj):
        if type(obj) == sg.Scene:
            return self.scene(obj)
        if obj.getObjectId() in self.cache :
            return { 'type' : 'Instance', 'id' : obj.getObjectId() }
        else:
            return self.conversion_map[type(obj)](obj)

    @for_types([int,float,str])
    def simpletype(self, value):
        return value

    @for_types(sg.Color3)
    def color3(self, value):
        return [value.red,value.green,value.blue]

    @for_types(sg.Color4)
    def color4(self, value):
        return [value.red,value.green,value.blue, value.alpha]

    @for_types([Vector2,Vector3,Vector4,sg.Index,sg.Index3,sg.Index4])
    def vector(self, value):
        return list(value)

    @for_types([sg.Point2Array,sg.Point3Array,sg.Point4Array,sg.Color3Array,sg.Color4Array,sg.IndexArray,sg.Index3Array,sg.Index4Array])
    def array(self, value):
        return list(map(self.convert,value))

    @for_types([sg.Point2Matrix,sg.Point3Matrix,sg.Point4Matrix,sg.RealArray2])
    def matrix(self, value):
        return [list(map(self.convert,row)) for row in value]

    @for_types(sg.SceneObject)
    def sceneobject(self, value):
        return self.pglobject(value)

    @for_types(sg.Scene)
    def scene(self, value):
        return { 'type' : 'Scene', 'values' : list(map(self.convert,value)) }


def from_json(jsonrep, cache = None):
    if type(jsonrep) != dict:
        return jsonrep
    if cache is None:
        cache = {}
    ptype = jsonrep['type']
    pid = jsonrep.get('id')
    if ptype ==  'Instance':
        return cache[pid]
    if ptype ==  'Scene':
        return sg.Scene([from_json(v, cache) for v in jsonrep['values']])
    name = jsonrep.get('name','')
    attributes = [key for key in jsonrep if not key in ['type','name','id']]
    values = {}
    for att in attributes:
        value = from_json(jsonrep[att], cache)
        values[att] = value

    res = sg.__dict__[ptype](**values)
    res.name = name
    cache[res.getObjectId()] = res
    return res


pattern ="""    @for_types([sg.%s])
    def %s(self, obj):
        return self.pglobject(obj, %s)
"""

def isabstract(cls):
    try:
        cls()
        return False
    except RuntimeError as re:
        return True
    except:
        return False

def generate_class_methods():
    import inspect
    candidates = []
    for classname,cls in sg.__dict__.items():
        if inspect.isclass(cls) and not isabstract(cls) and issubclass(cls,sg.SceneObject):
            candidates.append((classname,cls))
    candidates.sort(key = lambda v : v[0])
    for classname,cls in candidates:
            attributes = get_attributes(cls)
            print(pattern % (classname, classname.lower(), ', '.join(map(repr,attributes))))

#generate_class_methods()