# PlantGL

_________________

[![Docs](https://readthedocs.org/projects/plantgl/badge/?version=latest)](https://plantgl.readthedocs.io/)
[![Build Status](https://github.com/openalea/plantgl/actions/workflows/openalea_ci.yml/badge.svg?branch=main)](https://github.com/openalea/plantgl/actions/workflows/openalea_ci.yml?query=branch%3Amaster)
[![License](https://img.shields.io/badge/License--CeCILL-C-blue)](https://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html)
[![Anaconda-Server Badge](https://anaconda.org/openalea3/openalea.plantgl/badges/version.svg)](https://anaconda.org/openalea3/openalea.plantgl)
[![Platforms](https://anaconda.org/openalea3/openalea.plantgl/badges/platforms.svg)](https://anaconda.org/openalea3/openalea.plantgl)
[![Latest Release](https://anaconda.org/openalea3/openalea.plantgl/badges/latest_release_date.svg)](https://anaconda.org/openalea3/openalea.plantgl)

![PlantGL Teaser](doc/_images/pglteaser.png)

**PlantGL** is an open-source graphic toolkit for the creation, simulation and analysis of 3D virtual plants.

Several methods are provided to create plant architectures from field measurements or from procedural algorithms. Because they reveal particularly useful in plant design and simulation, special attention has been paid to the definition and use of branching system envelops.

PlantGL is designed to be:

- **Open source**: PlantGL is an open source software and can thus be freely used and extended. Providing a standard graphic toolkit to the plant modeling community, it benefits in return of the tests and improvements of users.
- **Portable**: PlantGL is available on major operating systems (GNU Linux, Microsoft Windows). It is also compatible with various plant modeling systems (L-studio, AMAP, etc.) and graphic toolkits (Pov-Ray, Vrml, etc.).
- **Simple**: The intended audience is researchers of the plant modeling community with no knowledge in computer graphics. Researchers could create images to illustrate and explore their results.
- **Modular**: PlantGL is composed of several independent modules like a geometric library, GUI components and Python wrappers. They can be used alone or combined in a specific application.
- **Hybrid System**: Core computational components of PlantGL are implemented in the C++ compiled language for performance. In addition for flexibility of use, these components are also exported in the Python interpreted language.

## Installation

`PlantGL` distribution is based on the `conda` software environment management system.
To install conda, you may refer to its installation page: https://docs.conda.io/projects/conda/en/latest/user-guide/install/

To install PlantGL, you need to create an environment (named for instance pgl):

```bash
conda create -n pgl openalea.plantgl -c openalea3 -c conda-forge
```

The package `openalea.plantgl` is retrieved from the `fredboudon` channel (development) and its dependencies will be taken from `conda-forge` channel.

Then, you need to activate the pgl environment:

```bash
conda activate pgl
```

You can then run the PlantGL viewer:

```bash
pglviewer
```

Or use the PlantGL modules in Python:

```bash
ipython
```

```python
>>> %gui qt
>>> from openalea.plantgl.all import *
>>> Viewer.display(Sphere())
```

## Compiling

PlantGL is compiled in a conda environment. Its dependencies are listed in `conda/`:
`environment-linux.yml`, `environment-macos.yml` or `environment-win.yml` (compilers and
build tools of your system), `build_deps.yml` (libraries), `run_deps.yml` (runtime),
`optional_deps.yml` (optional, needed by the tests) and `doc_deps.yml` (documentation).
The commands below are for Linux: on macOS use `conda/environment-macos.yml` (requires the
Xcode Command Line Tools), on Windows `conda/environment-win.yml` (requires Visual Studio
2022 with the C++ tools).

Solution 1: build an environment from scratch

```bash
mamba create -n plantgl-devel -f conda/environment-linux.yml -f conda/build_deps.yml -f conda/run_deps.yml -f conda/optional_deps.yml
mamba activate plantgl-devel
```

Solution 2: add the dependencies to an existing environment

```bash
mamba install -n myenv -f conda/environment-linux.yml -f conda/build_deps.yml -f conda/run_deps.yml -f conda/optional_deps.yml
mamba activate myenv
```

Add `-f conda/doc_deps.yml` to either command to build the documentation.

Then compile and install PlantGL in the active environment, from the root of the
repository:

```bash
pip install . --no-deps --no-build-isolation -v
```

CMake runs through scikit-build-core: the build directory is `build/`, reused by the next
builds. The C++ libraries, headers and `pglviewer` are installed in the environment, the
Python package in its `site-packages`. Run the tests with:

```bash
cd test
pytest -v --ignore=tofix --ignore=ui --ignore=oatest
```

## Documentation

Documentation is available at [https://plantgl.readthedocs.io/en/latest](https://plantgl.readthedocs.io/en/latest)

### Help and Support

Please open an **Issue** if you need support or that you run into any error (Installation, Runtime, etc.).
We'll try to resolve it as soon as possible.

## Authors

PlantGL was developed by Frédéric Boudon, Christophe Pradal, Christophe Nouguier with contributions of Christophe Godin, Nicolas Dones, Boris Adam, Pierre Barbier de Reuille, etc.

## Contributors

Thanks to all that contribute making this package what it is !

<a href="https://github.com/openalea/plantgl/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=openalea/plantgl" />
</a>


## Citation

If you find our work useful in your research, please consider citing:

> Pradal C., Boudon F., Nouguier C., Chopard J., Godin C.. 2009. PlantGL: A python-based geometric library for 3D plant modelling at different scales. Graphical Models, 71: p. 1-21.

