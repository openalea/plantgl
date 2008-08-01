# Redirect path
import os
import openalea.plantgl

__path__ = openalea.plantgl.__path__ + __path__[:]

from openalea.plantgl.__init__ import *
