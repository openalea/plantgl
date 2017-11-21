#!/bin/bash

export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig

#cp options_conda.py options.py

echo -e "\n\n\nscons -h :"
scons -h
echo -e "\n\n\n\n\n"

echo -e "\n\n\nls $PREFIX/include"
ls $PREFIX/include
echo -e "\n\n\n\n\n"

echo "ls $PREFIX/include/qt"
ls $PREFIX/include/qt
echo -e "\n\n\n\n\n"

echo "ls $PREFIX/include/qt/QtCore/"
ls $PREFIX/include/qt/QtCore/
echo -e "\n\n\n\n\n"

$PYTHON setup.py scons --scons-ext-param="WITH_LAPACK=False EXTRA_LIBS='boost_system png' --debug=stacktrace " install --prefix=$PREFIX
