#!/bin/bash

set -ve

export PKG_CONFIG_PATH=$CONDA_PREFIX/lib/pkgconfig

cp options_conda.py options.py

python setup.py install

set +ve