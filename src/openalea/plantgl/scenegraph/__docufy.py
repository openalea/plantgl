# -*- python -*-
#
#       plantgl: complete boos python doc with python docstrings
#
#       Copyright 2006 INRIA - CIRAD - INRA  
#
#       File author(s): Thomas Cokelaer <thomas.cokelaer@sophia.inria.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
"""docufy module

This module generates the full documentation of the scenegraph objects.

.. testsetup::

    from openalea.plantgl.gui.qt import QtGui
    app = QtGui.QApplication([])
    Viewer.start()

"""
__license__= "Cecill-C"
__revision__=" $Id$ "

from ._pglsg import *


AmapSymbolDoc = """AmapSymbol

    The AmapSymbol describes an object of class of Mesh stored in the SMB file format of the Amap software. This is provided for ascendant compatibility. 


:param FileName: specifies the name of the SMB file to bind to the symbol. It must contain the full path and .smb extension if needed. The corresponding file must exist.
:param Solid:   specifies whether the symbol represents a closed surface. (default is False)

:Example:

    .. doctest::

        from openalea.plantgl.scenegraph import AmapSymbol
        from openalea.plantgl.all import get_shared_data, Viewer, Scene
        from os.path import join as pj
        sym = AmapSymbol(get_shared_data(pj('amapsymbols','enpomf.smb')))
        scene = Scene()
        scene += sym
        Viewer.display(scene)   #doctest: +SKIP
        Viewer.frameGL.saveImage('amapsymbol.png', 'png') #doctest: +SKIP

    .. image:: amapsymbol.png
        :width: 50%

    .. image:: amapsymbol2.png
        :width: 50%

"""
AmapSymbol.__doc__ += AmapSymbolDoc

