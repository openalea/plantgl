#!/bin/bash

export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig

cp options_conda.py options.py

scons -h

echo "ls $PREFIX/include"
ls $PREFIX/include

echo "ls $PREFIX/include/qt"
ls $PREFIX/include/qt

echo "ls $PREFIX/include/qt/QtCore/"
ls $PREFIX/include/qt/QtCore/

$PYTHON setup.py scons --scons-ext-param="WITH_LAPACK=False EXTRA_LIBS='boost_system png'" install --prefix=$PREFIX
