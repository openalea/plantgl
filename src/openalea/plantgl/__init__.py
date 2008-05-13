# Redirect path
import os

cdir = os.path.dirname(__file__)
pgldir = os.path.join(cdir, "../../plantgl")
pgldir = os.path.abspath(pgldir)

__path__.append(pgldir)
