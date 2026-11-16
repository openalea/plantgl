# -*- coding: utf-8 -*-
#
# Copyright © 2011 Spyder
# Contributor: Pierre Raybaut
# Website: http://github.com/spyder-ide/spyder
#
# Copyright © 2012-2013 pyLot - andheo
# Contributor: Guillaume Baty
#
# Copyright © 2014 INRIA - CIRAD - INRA
# Contributor: Frédéric Boudon
# http://github.com/openalea/openalea
#
# Copyright © 2015 PyQode
# Contributor: Colin Duquesnoy
# Website: http://github.com/pyqode/pyqode.qt
#
# -------------------------------------------
# Licensed under the terms of the MIT License
# -------------------------------------------


import os
import sys

from openalea.plantgl.config import PGL_QT_VERSION

#: Qt API environment variable name
QT_API = 'QT_API'
#: names of the expected PyQt5 api
PYQT5_API = ['pyqt5']
#: names of the expected PyQt6 api
PYQT6_API = ['pyqt6']
#: names of the expected PyQt4 api
PYQT4_API = [
    'pyqt',  # name used in IPython.qt
    'pyqt4'  # pyqode.qt original name
]
#: names of the expected PySide api
PYSIDE_API = ['pyside']
PYSIDE2_API = ['pyside2']
PYSIDE6_API = ['pyside6']

# Warn if a binding of the other Qt major version is already loaded (e.g. matplotlib
# imported before PlantGL picks PySide6 when the full matplotlib package is installed):
# both Qt versions then run in the same process, with separate event loops.
_OTHER_QT_BINDINGS = {5: ('PyQt6', 'PySide6'), 6: ('PyQt5', 'PySide2')}.get(PGL_QT_VERSION, ())
_loaded = [name for name in _OTHER_QT_BINDINGS if name in sys.modules]
if _loaded:
    import warnings
    warnings.warn(
        f"{', '.join(_loaded)} is already loaded (probably by matplotlib), but PlantGL "
        f"uses Qt{PGL_QT_VERSION}: both Qt versions will run in this process, with "
        f"separate event loops. Import openalea.plantgl first, or set the environment "
        f"variable QT_API=pyqt{PGL_QT_VERSION} before starting Python.",
        RuntimeWarning, stacklevel=2)
del _loaded

if PGL_QT_VERSION == 6:
    try:
        import PySide6
        os.environ[QT_API] = PYSIDE6_API[0]
    except ImportError as ie:
        # the openalea.plantgl.gui.qt modules only support PySide6 for Qt6
        raise ImportError("PlantGL is built with Qt6 and needs PySide6, "
                          "which cannot be imported") from ie

elif PGL_QT_VERSION == 5:
        try :
            import PySide2
            os.environ[QT_API] = PYSIDE2_API[0]
        except ImportError as ie:
            os.environ[QT_API] = PYQT5_API[0]
elif PGL_QT_VERSION == 4:
        try :
            import PySide
            os.environ[QT_API] = PYSIDE_API[0]
        except ImportError as ie:
            os.environ[QT_API] = PYQT4_API[0]
            os.environ.setdefault('QT_API_VERSION', '2')
else :
    if sys.version_info[0] == 2:
        os.environ[QT_API] = 'pyqt'+str(PGL_QT_VERSION)
    else:
        try :
            import PySide2
            os.environ[QT_API] = PYSIDE2_API[0]
        except ImportError as ie:
            os.environ[QT_API] = 'pyqt'+str(PGL_QT_VERSION)







