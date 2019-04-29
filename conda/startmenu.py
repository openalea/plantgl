import os
import sys
import menuinst

if sys.platform == "win32":
    # Windows
    if 'LIBRARY_PREFIX' in os.environ:
        condaEnv = os.environ['LIBRARY_PREFIX']
    else:
        condaEnv = os.environ['CONDA_PREFIX'] + '/Library'
    
    menuinst.install('./menuinst-win.json', prefix = condaEnv)
