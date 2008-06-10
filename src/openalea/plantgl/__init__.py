
# Redirect path
import os

cdir = os.path.dirname(__file__)
pdir = os.path.join(cdir, "../../plantgl")
pdir = os.path.abspath(pdir)

__path__ = [pdir] + __path__[:]

from openalea.plantgl.__init__ import *
