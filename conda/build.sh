#!/bin/bash

export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig

scons -h

$PYTHON setup.py scons --scons-ext-param="WITH_LAPACK=False EXTRA_LIBS='boost_system png'" install --prefix=$PREFIX
