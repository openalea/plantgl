# Redirect path
import os
import openalea.plantgl

cdir = os.path.dirname(__file__)
pdir = os.path.join(cdir, "../../plantgl")
pdir = os.path.abspath(pdir)

__path__ = openalea.plantgl.__path__ + __path__[:]

from openalea.plantgl.__init__ import *
