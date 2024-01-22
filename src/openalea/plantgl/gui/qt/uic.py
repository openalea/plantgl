"""
Provides Qt UI tools
"""
import os
import subprocess
from openalea.plantgl.gui.qt import QT_API, PYQT5_API, PYQT4_API, PYSIDE_API, PYSIDE2_API, PYSIDE6_API

try:
    if os.environ[QT_API] in PYSIDE6_API:
        def compileUi(uiFile, pyFile, **args):
            # convert ui file to python code
            uic_executable = 'pyside6-uic' # TODO check for mac and windows ? 
            uipy = subprocess.check_output([uic_executable, uiFile])

            #pyFile = 'ui_' + uiFile.removesuffix('.ui') + '.py'
            with open(pyFile, 'wb') as f:
                f.write(uipy)

            # execute python code
            #pyc = compile(uipy, ' < string>', 'exec')
            #frame = {}
            #exec(pyc, frame)
        compile_args = dict()
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
