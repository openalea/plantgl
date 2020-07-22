from ._pglgui import *
from .qt.QtGui import QApplication

if QApplication.instance() is None:
    qPglApp =  QApplication([])