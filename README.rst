.. image:: https://img.shields.io/badge/License-GPL-blue.svg
   :target: https://opensource.org/licenses/GPL-3.0

.. image:: https://travis-ci.org/openalea/plantgl.svg?branch=master
    :target: https://travis-ci.org/openalea/plantgl

.. image:: https://ci.appveyor.com/api/projects/status/pbfi5p0bfslqij3s/branch/master?svg=true
   :target: https://ci.appveyor.com/project/fredboudon/plantgl

..  Future command for readthedoc:   image:: https://readthedocs.org/projects/plantgl/badge/?version=latest  :target: http://plantgl.readthedocs.io/en/latest/

=======
PlantGL
=======

An open-source graphic toolkit for the creation, simulation and analysis of 3D virtual plants.

**PlantGL** is released under a **CeCILL** license, which is compatible to the **GNU GPL**.


.. contents::

============
Installation
============

To build PlantGL, you need to clone PlantGL-Recipes first :

.. code:: bash

    git clone https://github.com/fredboudon/plantgl-recipes --recursive
    cd plantgl-recipes

You also need Miniconda3, that you can download here : https://docs.conda.io/en/latest/miniconda.html

Then, setup your Conda environment :

.. code:: bash

    conda create -n plantgl python=3 conda conda-env conda-build
    conda activate plantgl

Once your environment is ready, you must build PlantGL's dependencies :

.. code:: bash

    conda build openalea.deploy ann qhull pyqglviewer

    # If you're on Linux or macOS, you may also want to build CGAL :
    conda build cgal

Then, install all required dependencies :

.. code:: bash

    # Linux or macOS
    conda env create -f plantgl-devel.yaml

    # Windows
    conda env create -f plantgl-devel-win.yaml
    
    conda activate plantgl-devel

Now, you can build, then install PlantGL :

.. code:: bash

    cd plantgl
    mkdir build
    cd build

    # Linux
    cmake .. -DCMAKE_INSTALL_PREFIX="the install directory"

    # Windows -> Visual Studio 2015 is required
    cmake .. -G "Visual Studio 14 2015 Win64" -T host=x64 -DCMAKE_INSTALL_PREFIX="the install directory"

    cmake --build . --target install --config Release

    cd ..
    python setup.py install --prefix="the install directory"

You're done !

If you're on Windows, you can create start menu and desktop icons by calling :

.. code:: bash

    python src/pgl_postinstall.py

=============
Documentation
=============

The documentation is available at `<http://openalea.gforge.inria.fr/dokuwiki/doku.php?id=packages:visualization:plantgl:plantgl>`_

Help and Support
----------------

Please open an **Issue** if you need support or that you run into any error (Installation, Runtime, etc.).
We'll try to resolve it as soon as possible.

Authors
-------

* Frédéric Boudon
* Christophe Pradal
* Christophe Nouguier

With contributions of Christophe Godin, Nicolas Dones, Boris Adam, Pierre Barbier de Reuille.
