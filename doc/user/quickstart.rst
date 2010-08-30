
.. testsetup::

        from PyQt4 import QtGui
        app = QtGui.QApplication([])

.. topic:: Overview

    Create a colored shape, add it to a scene and view the results

    :Author: Thomas Cokelaer <Thomas.Cokelaer@sophia.inria.fr>

.. contents::

Quickstart
===========

Scene creation
--------------

First, create a scene:


.. doctest::

    >>> from vplants.plantgl.all import *
    >>> scene = Scene()

Create a shape
---------------

Then, create a colored shaped, let us say a Cylinder:

.. doctest::

    >>> c = Cylinder(1, 10)
    >>> color = Material(Color3(255,0,0),0,Color3(0,0,0),Color3(0,0,0),1,0)
    >>> shape = Shape(c, color)

Visualize the scene
-------------------

The colored shape may now be added to the scene and visualize via the Viewer:

.. doctest::

    >>> scene.add(shape)
    >>> Viewer.display(scene)

.. warning:: to run the folling commands in ipython, you must use the -q4thread option::

        ipython -q4thread

.. doctest::
    :options: +SKIP

        >>> from PyQt4 import QtGui
        >>> app = QtGui.QApplication([])
        >>> Viewer.start()

Output
------

Finally, it is time to save the results in a PNG file. First, we set the background color and grids:

.. doctest::

    >>> Viewer.frameGL.setBgColor(255,255,200)
    >>> Viewer.grids.setXYPlane(True)
    >>> Viewer.grids.setYZPlane(False)
    >>> Viewer.grids.setXZPlane(False)
    >>> Viewer.frameGL.saveImage("user/result.png")

.. image:: result.png
    :width: 50%


