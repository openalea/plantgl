# -*- coding: utf-8 -*-

import os

if os.environ['QT_API'] == 'pyqt':
    from PyQt4.phonon import *
else:
    from PySide.phonon import *
