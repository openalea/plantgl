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
import os

#: Qt API environment variable name
QT_API = 'QT_API'
#: names of the expected PyQt5 api
PYQT5_API = ['pyqt5']
#: names of the expected PyQt4 api
PYQT4_API = [
    'pyqt',  # name used in IPython.qt
    'pyqt4'  # pyqode.qt original name
]
#: names of the expected PySide api
PYSIDE_API = ['pyside']
PYSIDE2_API = ['pyside2']


if PGL_QT_VERSION == 4:
        try :
            import PySide
            os.environ[QT_API] = PYSIDE_API[0]
        except ImportError, ie:
            os.environ[QT_API] = PYQT4_API[0]
            os.environ.setdefault('QT_API_VERSION', '2')
else :
    if sys.version_info[0] == 2:
        os.environ[QT_API] = 'pyqt'+str(PGL_QT_VERSION)
    else:
        try :
            import PySide2
            os.environ[QT_API] = PYSIDE2_API[0]
        except ImportError, ie:
            os.environ[QT_API] = 'pyqt'+str(PGL_QT_VERSION)







