# Installation

You must use conda environment : <https://docs.conda.io/en/latest/index.html>

## Users

### Create a new environment with plantgl installed in there

```bash

mamba create -n plantgl -c openalea3 -c conda-forge  openalea.plantgl
mamba activate plantgl
```

Install plantgl in a existing environment

```bash
mamba install -c openalea3 -c conda-forge openalea.plantgl
```

### (Optional) Test your installation

```bash
mamba install -c conda-forge pytest
git clone https://github.com/openalea/plantgl.git
cd plantgl/test; pytest
```

## Developers

### Install From source

```bash
# Clone plantgl
git clone https://github.com/openalea/plantgl.git
cd plantgl

# Install the dependencies with mamba (environment-macos.yaml or environment-win.yaml
# on macOS or Windows)
mamba create -n plantgl-devel -f environment-linux.yaml -f conda/build_deps.yml -f conda/run_deps.yml -f conda/optional_deps.yml
mamba activate plantgl-devel
pip install .

# (Optional) Test your installation
cd test; pytest
```
