"""
Provides QtTest and functions

.. warning:: PySide is not supported here, that's why there is not unit tests
    running with PySide.

"""
import os
from openalea.plantgl.gui.qt import QT_API, PYQT5_API, PYQT4_API, PYSIDE_API, PYSIDE2_API

if os.environ[QT_API] in PYQT5_API:
    from PyQt5.QtTest import *

elif os.environ[QT_API] in PYQT4_API:
    from PyQt4.QtTest import *
    from PyQt4.QtTest import QTest as OldQTest

    class QTest(OldQTest):

        @staticmethod
        def qWaitForWindowActive(QWidget):
            OldQTest.qWaitForWindowShown(QWidget)

elif os.environ[QT_API] in PYSIDE_API:
    import datetime
    from PySide.QtTest import *
    from PySide.QtGui import QApplication

    @staticmethod
    def qWait(t):
        end = datetime.datetime.now() + datetime.timedelta(milliseconds=t)
        while datetime.datetime.now() < end:
            QApplication.processEvents()
    QTest.qWait = qWait

elif os.environ[QT_API] in PYSID2_API:
    import datetime
    from PySide2.QtTest import *
    from PySide2.QtGui import QApplication

    @staticmethod
    def qWait(t):
        end = datetime.datetime.now() + datetime.timedelta(milliseconds=t)
        while datetime.datetime.now() < end:
            QApplication.processEvents()
    QTest.qWait = qWait
