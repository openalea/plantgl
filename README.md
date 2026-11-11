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

The package `openalea.plantgl` is retrieved from the `openalea3` channel (development) and its dependencies will be taken from `conda-forge` channel.

Then, you need to activate the pgl environment and add optional dependencies:

```bash
conda activate pgl
conda install -c conda-forge ipython matplotlib-base pandas tzfpy geopy trimesh
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

The simplest way to build PlantGL is to use conda, with mamba 2 (it combines several
environment files). The dependencies are split into:

- `build-util/plantgl-devel-<os>.yaml`: compilers and build tools of your system;
- `conda/host_deps.yml`: libraries PlantGL is compiled against (also used by `conda/meta.yaml`);
- `conda/run_deps.yml`: runtime dependencies (also used by `conda/meta.yaml`);
- `doc/doc_deps.yml`: documentation tools, only needed to build the documentation.

Setup your environment with all required dependencies (on Windows, Visual Studio 2022 with
the C++ tools is required; on macOS, the Xcode Command Line Tools):

```bash
# Linux
mamba create -n plantgl-devel -f build-util/plantgl-devel-linux.yaml -f conda/host_deps.yml -f conda/run_deps.yml -f doc/doc_deps.yml

# Macos
mamba create -n plantgl-devel -f build-util/plantgl-devel-macos.yaml -f conda/host_deps.yml -f conda/run_deps.yml -f doc/doc_deps.yml

# Windows
mamba create -n plantgl-devel -f build-util/plantgl-devel-win.yaml -f conda/host_deps.yml -f conda/run_deps.yml -f doc/doc_deps.yml

mamba activate plantgl-devel
```

`-f doc/doc_deps.yml` can be left out if you do not build the documentation.

Now, you can build, then install PlantGL with its test and optional dependencies:

```bash
pip install -e ".[dev,test,light,algo]" --no-build-isolation -v
```

In editable mode (`-e`), changes to the Python files are used directly. After a change in
the C++ code (`src/cpp`, `src/wrapper`), run the same command again: the build directory
`build/` is reused, so only what changed is recompiled.

Run the tests:

```bash
cd test
pytest -v
```

Build the documentation (PlantGL must be installed: the documentation reads its version
and docstrings from the installed package):

```bash
cd doc
make html
```

Then open `doc/_build/html/index.html`. On Windows, in PowerShell, type `.\make html`.
`make rtdhtml` uses the settings of Read the Docs (warnings become errors), `make clean`
removes the built documentation.

You're done!

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

