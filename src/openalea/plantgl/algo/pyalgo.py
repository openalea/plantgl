import openalea.plantgl.scenegraph.pglinspect as inspect 


def for_types(types):
    if not type(types) in [tuple,list]:
        types = (types,)
    types = sum([inspect.get_all_subclasses(t) for t in types if inspect.is_sceneobject_subclass(t)],[])+[t for t in types if not inspect.is_sceneobject_subclass(t)]
    types = tuple(types)
    def for_given_types(f):
        f.__for_identifiers__ = types
        return f
    return for_given_types

def for_identifiers(identifiers):
    if not type(identifiers) in [tuple,list]:
        identifiers = (identifiers,)
    def for_given_types(f):
        f.__for_identifiers__ = identifiers
        return f
    return for_given_types

class PyAlgo:
    def __init__(self):        
        self.build_conversion_map()
        self.__default_mapping = None

    def build_conversion_map(self):
        self.__conversion_map = {  }
        for funcname in dir(self):
            func = getattr(self,funcname)
            if hasattr(func,'__for_identifiers__'):
                for t in func.__for_identifiers__:
                    self.add_to_conversion_map(t,func)

    def add_to_conversion_map(self, identifier, func):
        self.__conversion_map[identifier] = func

    def set_default_mapping(self, func):
        self.__default_mapping = func

    def apply(self, value, identifier = None):
        if identifier is None:
            identifier = type(value)
        return self.__conversion_map.get(identifier,self.__default_mapping)(value)