"""
Provides Qt UI tools
"""
import os
from openalea.plantgl.gui.qt import QT_API, PYQT5_API, PYQT4_API, PYSIDE_API, PYSIDE2_API, PYSIDE6_API

try:
    if os.environ[QT_API] in PYSIDE6_API:
        # TODO not here. use the binary pyside6-uic ?? 
        from pysideuic import compileUi
        compile_args = dict(execute=False, indent=4, from_imports=False)
    elif os.environ[QT_API] in PYQT5_API:
        from PyQt5.uic import compileUi
        compile_args = dict(execute=False, indent=4, from_imports=True)
    elif os.environ[QT_API] in PYQT4_API:
        from PyQt4.uic import compileUi
        compile_args = dict(execute=False, indent=4)
    elif os.environ[QT_API] in PYSIDE_API:
        from pysideuic import compileUi
        compile_args = dict(execute=False, indent=4, from_imports=False)
    else :
        raise NotImplementedError
except ImportError as e:
    message = 'You must install %s-tools' % os.environ['QT_API']
    e.args = (message,)
    raise e
