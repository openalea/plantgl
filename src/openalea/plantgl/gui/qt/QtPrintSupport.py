"""
Provides widget classes and functions.

.. warning:: All PyQt4/PySide gui classes are exposed but when you use
    PyQt5, those classes are not available. Therefore, you should treat/use
    this package as if it was ``PyQt5.QtPrintSupport`` module.
"""
import os
from openalea.plantgl.gui.qt import QT_API, PYQT5_API, PYQT4_API, PYSIDE_API, PYSIDE2_API, PYSIDE6_API

if os.environ[QT_API] in PYSIDE6_API:
    from PySide6.QtGui import *
elif os.environ[QT_API] in PYQT5_API:
    from PyQt5.QtPrintSupport import *
elif os.environ[QT_API] in PYQT4_API:
    from PyQt4.QtGui import *
elif os.environ[QT_API] in PYSIDE_API:
    from PySide.QtGui import *
elif os.environ[QT_API] in PYSIDE2_API:
    from PySide2.QtGui import *
