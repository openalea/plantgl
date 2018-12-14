"""
Provides QtGui classes and functions.

.. warning:: All PyQt4/PySide gui classes are exposed but when you use
    PyQt5, those classes are not available. Therefore, you should treat/use
    this package as if it was ``PyQt5.QtGui`` module.
"""
import os
from openalea.plantgl.gui.qt import QT_API
from openalea.plantgl.gui.qt import PYQT5_API
from openalea.plantgl.gui.qt import PYQT4_API
from openalea.plantgl.gui.qt import PYSIDE_API


if os.environ[QT_API] in PYQT5_API:
    from PyQt5.QtGui import *
    from PyQt5.QtWidgets import QTabWidget
    _tab_position = {
        0: QTabWidget.North,
        1: QTabWidget.South,
        2: QTabWidget.West,
        3: QTabWidget.East,
    }
elif os.environ[QT_API] in PYQT4_API:
    from PyQt4.QtGui import *
    _tab_position = {
        0: QTabWidget.North,
        1: QTabWidget.South,
        2: QTabWidget.West,
        3: QTabWidget.East,
    }
elif os.environ[QT_API] in PYSIDE_API:
    from PySide.QtGui import *
    _tab_position = {
        0: QTabWidget.TabPosition.North,
        1: QTabWidget.TabPosition.South,
        2: QTabWidget.TabPosition.West,
        3: QTabWidget.TabPosition.East,
    }
    for idx, position in _tab_position.items():
        setattr(QTabWidget, position.name, position)
elif os.environ[QT_API] in PYSIDE2_API:
    from PySide2.QtGui import *
    _tab_position = {
        0: QTabWidget.TabPosition.North,
        1: QTabWidget.TabPosition.South,
        2: QTabWidget.TabPosition.West,
        3: QTabWidget.TabPosition.East,
    }
    for idx, position in _tab_position.items():
        setattr(QTabWidget, position.name, position)
