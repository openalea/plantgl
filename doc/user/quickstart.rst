


Quickstart
===========

:Author: Thomas Cokelaer <Thomas.Cokelaer@sophia.inria.fr>

First, create a scene::

    >>> from vplants.plantgl.all import *
    >>> scene = Scene()

Then, create a colored shaped, let us say a Cylinder::

    >>> c = Cylinder(1, 10)
    >>> color = Material(Color3(255,0,0),0,Color3(0,0,0),Color3(0,0,0),1,0)
    >>> shape = Shape(c, color)

The colored shape may now be added to the scene and visualize via the Viewer::

    >>> scene.add(shape)
    >>> Viewer.display(scene)

.. warning:: to run these commands in ipython, you must use the -q4thread option::

        >>> ipython -q4thread

.. note:: another solution is to type::

        >>> from PyQt4 import QtGui
        >>> app = QtGui.QApplication([])

Finally, it is time to save the results in a PNG file::

    >>> Viewer.frameGL.saveImage("result.png")





