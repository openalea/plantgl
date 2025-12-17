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
# Install dependency with conda
mamba env create -n phm -f conda/environment.yml
mamba activate plantgl

# Clone plantgl and install
git clone https://github.com/openalea/plantgl.git
cd plantgl
pip install .

# (Optional) Test your installation
cd test; pytest
```
