import openalea.plantgl.scenegraph
from ._pglalgo import *
from copy import deepcopy

def pushEvent(self):
    self._pystack.append(dict([(p,(c,c(getattr(self,p)))) for p,c in self._params.items()]))

def popEvent(self):
    if len(self._pystack) > 0 :
        pparams = self._pystack.pop(-1)
        _pparams = {}
        for p,(c,v) in pparams.items():
            setattr(self, p, v)
            _pparams[p] = c
        for p in set(self._params).difference(_pparams):
            delattr(self,p)
        self._params = _pparams
    else:
        self.clear_parameters()

Turtle.pushEvent = pushEvent
PglTurtle.pushEvent = pushEvent
Turtle.popEvent = popEvent
PglTurtle.popEvent = popEvent

del pushEvent
del popEvent

PglTurtle.customGeometry = PglTurtle.pglShape

def add_parameter(self, name, value, copy = deepcopy):
    """ Add a parameter to the Turtle that will be push and pop. For this a copy function is asked """
    setattr(self, name, value)
    if not hasattr(self,'_params'):
        self._params = {}
        self._pystack = []
        self._register_pushpop(self.pushEvent,self.popEvent)
    self._params[name] = copy

def remove_parameter(self, name):
    """ Remove a parameter to the Turtle that was pushed and popped. """
    delattr(self, name)
    if not hasattr(self,'_params'):
        self._params = {}
        self._pystack = []
        self._register_pushpop(self.pushEvent,self.popEvent)

def clear_parameters(self):
    """ Clear the python parameters of the Turtle."""
    if not hasattr(self,'_params'):
        self._register_pushpop(self.pushEvent,self.popEvent)
        self._pystack = []
    else:
        for p,c in self._params.items():
            delattr(self,p)
    self._params = {}


Turtle.add_parameter = add_parameter
PglTurtle.add_parameter = add_parameter
del add_parameter

Turtle.clear_parameters = clear_parameters
PglTurtle.clear_parameters = clear_parameters
del clear_parameters

Turtle.remove_parameter = remove_parameter
del remove_parameter
