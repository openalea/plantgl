import openalea.plantgl.scenegraph
from ._pglalgo import *
from copy import deepcopy

def pushEvent(self):
    self._pystack.append([c(getattr(self,p)) for p,c in self._params.items()])

def popEvent(self):
    pparams = self._pystack.pop(-1)
    for (p,c),v in zip(self._params.items(),pparams):
        setattr(self, p, v)

Turtle.pushEvent = pushEvent
PglTurtle.pushEvent = pushEvent
Turtle.popEvent = popEvent
PglTurtle.popEvent = popEvent

del pushEvent
del popEvent

def add_parameter(self, name, value, copy = deepcopy):
    """ Add a parameter to the Turtle that will be push and pop. For this a copy function is asked """
    setattr(self, name, value)
    if not hasattr(self,'_params'):
        self._params = {}
        self._pystack = []
        self._register_pushpop(self.pushEvent,self.popEvent)
    self._params[name] = copy

def clear_parameters(self):
    """ Clear the python parameters of the Turtle."""
    if not hasattr(self,'_params'):
        self._register_pushpop(self.pushEvent,self.popEvent)
    self._params = {}
    self._pystack = []


Turtle.add_parameter = add_parameter
PglTurtle.add_parameter = add_parameter
del add_parameter

Turtle.clear_parameters = clear_parameters
PglTurtle.clear_parameters = clear_parameters
del clear_parameters
