""" import qt.py from IPython to set QString and QVariant

The goal is to have the same version of QString and QVariant in all OpenAlea
"""

try:
    from openalea.plantgl.gui.qt import QtCore
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtGui
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtWidgets
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtPrintSupport
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtOpenGL
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtTest
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtSql
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtWebKit
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import QtSvg
except ImportError:
    pass

try:
    from openalea.plantgl.gui.qt import phonon
except ImportError:
    pass

