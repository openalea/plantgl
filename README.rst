.. image:: https://img.shields.io/badge/license-CeCILL--C-blue 
   :target: LICENSE

Distribution based on Python 2:

.. image:: https://travis-ci.org/openalea/plantgl.svg?branch=master 
    :target: https://travis-ci.org/openalea/plantgl

.. image:: https://ci.appveyor.com/api/projects/status/pbfi5p0bfslqij3s/branch/master?svg=true
    :target: https://ci.appveyor.com/project/fredboudon/plantgl


.. image:: https://anaconda.org/openalea/openalea.plantgl/badges/version.svg
    :target: https://anaconda.org/openalea/openalea.plantgl

.. image:: https://anaconda.org/openalea/openalea.plantgl/badges/platforms.svg
    :target: https://anaconda.org/openalea/openalea.plantgl

.. image:: https://anaconda.org/openalea/openalea.plantgl/badges/latest_release_date.svg
    :target: https://anaconda.org/openalea/openalea.plantgl

.. image:: https://readthedocs.org/projects/plantgl/badge/?version=latest  
    :target: http://plantgl.readthedocs.io/en/latest/

Distribution based on Python 3:

.. image:: https://travis-ci.org/fredboudon/plantgl.svg?branch=master
    :target: https://travis-ci.org/fredboudon/plantgl

.. image:: https://ci.appveyor.com/api/projects/status/5a4xss61gm8v88yo/branch/master?svg=true
    :target: https://ci.appveyor.com/project/fredboudon/plantgl-7p8p4

.. image:: https://anaconda.org/fredboudon/openalea.plantgl/badges/version.svg
    :target: https://anaconda.org/fredboudon/openalea.plantgl

.. image:: https://anaconda.org/fredboudon/openalea.plantgl/badges/platforms.svg
    :target: https://anaconda.org/fredboudon/openalea.plantgl

.. image:: https://anaconda.org/fredboudon/openalea.plantgl/badges/latest_release_date.svg
    :target: https://anaconda.org/fredboudon/openalea.plantgl

.. image:: https://readthedocs.org/projects/plantgl-fb/badge/?version=latest
    :target: https://plantgl-fb.readthedocs.io/en/latest
    :alt: Documentation Status


=======
PlantGL
=======


**PlantGL** is an open-source graphic toolkit for the creation, simulation and analysis of 3D virtual plants.

Several methods are provided to create plant architectures from field measurements or from procedural algorithms. Because they reveal particularly useful in plant design and simulation, special attention has been paid to the definition and use of branching system envelops.

PlantGL is design to be :

- **Open source** : PlantGL is an open source software and can thus be freely used and extended. Providing a standard graphic toolkit to the plant modeling community, it benefits in return of the tests and improvements of users.
- **Portable** : PlantGL is available on major operating systems (GNU Linux, Microsoft Windows). It is also compatible with various plant modeling systems (L-studio, AMAP, etc.) and graphic toolkits (Pov-Ray, Vrml, etc.).
- **Simple** : The intended audience is researchers of the plant modeling community with no knowledge in computer graphics. Researchers could create images to illustrate and explore their results.
- **Modular** : PlantGL is composed of several independent modules like a geometric library, GUI components and Python wrappers. They can be used alone or combined in a specific application.
- **Hybrid System** : Core computational components of PlantGL are implemented in the C++ compiled language for performance. In addition for flexibility of use, these components are also exported in the Python interpreted language.


.. image:: doc/_images/pglteaser.png

=============
Installation
=============


``PlantGL`` distribution is based on the ``conda`` software environment management system.
To install conda, you may refer to its installation page: https://docs.conda.io/projects/conda/en/latest/user-guide/install/

To install PlantGL, you need to create an environment (named for instance pgl) :

.. code-block:: bash

        conda create -n pgl openalea.plantgl -c fredboudon -c conda-forge

The package ``openalea.plantgl`` is retrieved from the ``fredboudon`` channel (developement) and its dependencies will be taken from ``conda-forge`` channel.

Then, you need to activate the pgl environment

.. code-block:: bash

        conda activate pgl

You can then run the PlantGL viewer

.. code-block:: bash

        pglviewer

Or use the PlantGL modules in Python

.. code-block:: bash

        ipython


.. code-block:: python

        >>> from openalea.plantgl.all import *
        >>> Viewer.display(Sphere())



=============
Compiling
=============


The simplest way to build PlantGL is to use conda (see below).

Then, setup your Conda environment with all required dependencies :

.. code:: bash

    # Linux or macOS
    conda env create -f build-util/plantgl-devel.yaml

    # Windows
    conda env create -f build-util/plantgl-devel-win.yaml
    
    conda activate plantgl-devel

Now, you can build, then install PlantGL :

.. code:: bash

    cd plantgl
    mkdir build
    cd build

    # Linux
    cmake .. -DCMAKE_INSTALL_PREFIX=${CONDA_PREFIX}

    # Windows -> Visual Studio 2015 is required
    cmake .. -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX%  ..

    cmake --build . --target install --config Release

    cd ..
    python setup.py install --prefix=${CONDA_PREFIX}

You're done !


=============
Documentation
=============

Documentation is available at `<https://plantgl-fb.readthedocs.io/en/latest>`_

A previous documentation is available at `<http://openalea.gforge.inria.fr/dokuwiki/doku.php?id=packages:visualization:plantgl:plantgl>`_

Help and Support
----------------

Please open an **Issue** if you need support or that you run into any error (Installation, Runtime, etc.).
We'll try to resolve it as soon as possible.

==============
Authors
==============

PlantGL was developed by Frédéric Boudon, Christophe Pradal, Christophe Nouguier with contributions of Christophe Godin, Nicolas Dones, Boris Adam, Pierre Barbier de Reuille, etc.

==============
Citation
==============

If you find our work useful in your research, please consider citing:

   Pradal C., Boudon F., Nouguier C., Chopard J., Godin C.. 2009. PlantGL : A python-based geometric library for 3D plant modelling at different scales. Graphical Models, 71 : p. 1-21.

