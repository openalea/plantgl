"""
Provides QtNetwork classes and functions.
"""
import os
from openalea.plantgl.gui.qt import QT_API, PYQT5_API, PYQT4_API, PYSIDE_API, PYSIDE2_API, PYSIDE6_API

if os.environ[QT_API] in PYSIDE6_API:
    from PySide6.QtSql import *
elif os.environ[QT_API] in PYQT5_API:
    from PyQt5.QtSql import *
elif os.environ[QT_API] in PYQT4_API:
    from PyQt4.QtSql import *
elif os.environ[QT_API] in PYSIDE_API:
    from PySide.QtSql import *
elif os.environ[QT_API] in PYSIDE2_API:
    from PySide2.QtSql import *
