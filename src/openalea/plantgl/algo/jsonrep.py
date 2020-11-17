import openalea.plantgl.scenegraph as sg
import openalea.plantgl.math as mt
import openalea.plantgl.scenegraph.pglinspect as inspect 
from openalea.plantgl.algo.pyalgo import * 


class ToJsonRepConverter (PyAlgo):
    def __init__(self):
        PyAlgo.__init__(self)
        self.clear()

    def clear(self):
        self.cache = set()

    def pglobject(self, obj, *args):
        res = dict( type = obj.__class__.__name__, id = obj.getObjectId())
        return self.pglattributes(res, obj, *args)

    def pglattributes(self, res, obj):
        attributes = inspect.get_pgl_attributes(obj)

        for att in attributes:
            if inspect.is_pgl_attribute_to_default(obj, att):
                continue
            res[att] = self.convert(getattr(obj,att))
        return res

    def convert(self,obj):
        if obj is None:
            return None
        elif hasattr(obj,'getObjectId') :
            if  obj.getObjectId() in self.cache :
                return { 'type' : 'Instance', 'id' : obj.getObjectId() }
            else:
                self.cache.add(obj.getObjectId())
                return self.apply(obj)
        else:
            return self.apply(obj)

    @for_types((bool, int, float, str))
    def simpletype(self, value):
        return value

    @for_types(sg.Color3)
    def color3(self, value):
        return [value.red,value.green,value.blue]

    @for_types(sg.Color4)
    def color4(self, value):
        return [value.red,value.green,value.blue, value.alpha]

    @for_types((mt.Vector2,mt.Vector3,mt.Vector4,sg.Index,sg.Index3,sg.Index4))
    def vector(self, value):
        return list(value)

    @for_types(sg.Transform4)
    def transform4(self, value):
        return { 'type' : value.__class__.__name__ , 'data' : self.convert(value.getMatrix()) }

    @for_types((sg.Point2Array,sg.Point3Array,sg.Point4Array,sg.Color3Array,sg.Color4Array,sg.IndexArray,sg.Index3Array,sg.Index4Array, sg.GeometryArray, sg.RealArray, sg.Transform4Array))
    def array(self, value):
        return { 'type' : value.__class__.__name__ , 'id' : value.getPglId() ,  'data' : list(map(self.convert,value)) }

    @for_types((sg.Point2Matrix,sg.Point3Matrix,sg.Point4Matrix,sg.RealArray2))
    def array2(self, value):
        return { 'type' : value.__class__.__name__ , 'id' : value.getPglId() ,  'data' : [list(map(self.convert,row)) for row in value] }

    @for_types((mt.Matrix2,mt.Matrix3,mt.Matrix4))
    def matrix(self, value):
        return { 'type' : value.__class__.__name__ , 'data' : value.data() }

    @for_types(sg.SceneObject)
    def sceneobject(self, value):
        return self.pglobject(value)

    @for_types(sg.Scene)
    def scene(self, value):
        return { 'type' : 'Scene', 'data' : list(map(self.convert,value)) }

    @for_types(sg.QuantisedFunction)
    def qfunction(self, qfunc):
        return { 'type' : 'QuantisedFunction', 'clamped' : qfunc.clamped, 'sampling' : qfunc.sampling,  'data' : self.convert(qfunc.input) }        


class FromJsonRepConverter(PyAlgo):
    def __init__(self):
        PyAlgo.__init__(self)
        self.clear()
        self.set_default_mapping(self.default)

    def clear(self):
        self.cache = dict()

    @for_identifiers(('Matrix2','Matrix3','Matrix4'))
    def matrix(self, jsonrep):
        ptype = jsonrep['type']
        constructor = mt.__dict__[ptype]
        dim = int(ptype[-1])
        values = jsonrep['data']
        values = tuple([tuple(values[dim*i:dim*(i+1)]) for i in range(dim)])
        return constructor(*values)

    @for_identifiers('Scene')
    def scene(self, jsonrep):
        return sg.Scene(list(map(self.convert,jsonrep['data'])))

    @for_identifiers('Instance')
    def instance(self, jsonrep):
        return self.cache[jsonrep.get('id') ]

    @for_identifiers('QuantisedFunction')
    def qfunction(self, jsonrep):
        input = self.convert(jsonrep['data'])
        res = sg.QuantisedFunction(input, clamped = jsonrep['clamped'], sampling = jsonrep['sampling'])
        res.input = input
        return res

    def array(self, values):
        return list(map(self.convert, values))

    def default(self, jsonrep):
        name = jsonrep.get('name')
        attributes = [key for key in jsonrep if not key in ['type','name','id']]
        values = {}
        for att in attributes:
            value = self.convert(jsonrep[att])
            values[att] = value
        if 'data' in values:
            rawdata = values['data']
            del values['data']
        else:
            rawdata = None
        ptype = jsonrep['type']
        try:
            constructor = sg.__dict__[ptype]
        except KeyError as ke:
            constructor = mt.__dict__[ptype]
        toremove = []
        for param, pvalue in values.items():
            if pvalue is None:
                toremove.append(param)
        for p in toremove:
            del values[p]
        try:
            if rawdata:
                res = constructor(rawdata, **values)
            else:
                res = constructor(**values)
        except Exception as e:
            print(jsonrep)
            raise e
        if name:
            res.name = name
        return res

    def convert(self, jsonrep):
        if type(jsonrep) != dict:
            if type(jsonrep) == list:
                return self.array(jsonrep)
            else:
                return jsonrep
        res = self.apply(jsonrep, jsonrep['type'])
        self.cache[jsonrep.get('id')] = res
        return res


def to_json_rep(object):
    fj = ToJsonRepConverter()
    return fj.convert(object)

def from_json_rep(jsonrep):
    fj = FromJsonRepConverter()
    return fj.convert(jsonrep)



pattern ="""    @for_types([sg.%s])
    def %s(self, obj):
        return self.pglobject(obj, %s)
"""


def generate_class_methods():
    candidates = []
    for classname,cls in sg.__dict__.items():
        if inspect.is_sceneobject_subclass(cls) and not inspect.is_pgl_abstract(cls):
            candidates.append((classname,cls))
    candidates.sort(key = lambda v : v[0])
    for classname,cls in candidates:
            attributes = inspect.get_pgl_attributes(cls)
            print(pattern % (classname, classname.lower(), ', '.join(map(repr,attributes))))

#generate_class_methods()