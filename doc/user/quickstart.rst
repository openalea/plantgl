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

.. seealso:: tutorial.rst  

Visualize the scene
-------------------

The colored shape may now be added to the scene and visualize via the Viewer:

.. doctest::

    >>> scene.add(shape)
    >>> Viewer.display(scene)

.. warning:: to run the folling commands in ipython, you must use the following option::

        ipython  --gui=qt

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


Apply processing
----------------
A number of algorithm are defined and can be applied to the geometric representation that have been created.
For instance, to compute the boundingbox of the scene

.. doctest::

    >>> bbc = BBoxComputer()
    >>> scene.apply(bbc)
    >>> boundingbox = bbc.result

Numerous algorithms have been implemented as Action that adapt to the specific structure of each scene graph.

Turtle Geometry
---------------

To assemble iterativelly and easily different shapes, the turtle geometry have been introduced in PlantGL. To use it, a turtle object 
has to be created that will register the different drwaing actions to produce a final representation. For instance to produce a torus
    >>> turtle = PglTurtle()
    >>> for i in xrange(12):
    >>>      turtle.left(30).F(1)
    >>> scene = turtle.getScene()
    >>> Viewer.display(scene)

