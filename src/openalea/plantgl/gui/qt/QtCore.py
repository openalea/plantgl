"""
Provides QtCore classes and functions.
"""
import os

from openalea.plantgl.gui.qt import QT_API, PYQT5_API, PYQT4_API, PYSIDE_API, PYSIDE2_API, PYSIDE6_API
from openalea.plantgl.config import PGL_QT_VERSION

if os.environ[QT_API] in PYSIDE6_API:
    from PySide6.QtCore import *
    from PySide6.QtCore import QSortFilterProxyModel
    # use a common __version__
    import PySide6.QtCore
    __version__ = PySide6.QtCore.__version__
elif os.environ[QT_API] in PYQT5_API:
    from PyQt5.QtCore import *
    # compatibility with pyside
    from PyQt5.QtCore import pyqtSignal as Signal
    from PyQt5.QtCore import pyqtSlot as Slot
    from PyQt5.QtCore import pyqtProperty as Property
    # use a common __version__
    from PyQt5.QtCore import QT_VERSION_STR as __version__
elif os.environ[QT_API] in PYQT4_API:
    from PyQt4.QtCore import *
    # compatibility with pyside
    from PyQt4.QtCore import pyqtSignal as Signal
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtCore import pyqtProperty as Property
    from PyQt4.QtGui import QSortFilterProxyModel
    # use a common __version__
    from PyQt4.QtCore import QT_VERSION_STR as __version__
elif os.environ[QT_API] in PYSIDE_API:
    from PySide.QtCore import *
    from PySide.QtGui import QSortFilterProxyModel
    # use a common __version__
    import PySide.QtCore
    __version__ = PySide.QtCore.__version__

elif os.environ[QT_API] in PYSIDE2_API:
    from PySide2.QtCore import *
    from PySide2.QtGui import QSortFilterProxyModel
    # use a common __version__
    import PySide2.QtCore
    __version__ = PySide2.QtCore.__version__

if QT_VERSION == 4:
    import sip
    assert sip.getapi('QString') == 2
