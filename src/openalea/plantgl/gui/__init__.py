from ._pglgui import *
from .qt.QtWidgets import QApplication

#if QApplication.instance() is None:
#    qPglApp =  QApplication([])

qPglApp = None
def viewer_qapp_check() :
    if QApplication.instance() is None:
        global qPglApp
        qPglApp =  QApplication([])

pgl_register_qapp_initiator(viewer_qapp_check)

del viewer_qapp_check
del pgl_register_qapp_initiator