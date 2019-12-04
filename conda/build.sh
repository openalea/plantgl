#!/bin/bash

if [[ -d build ]]; then
    rm -rf build
fi
mkdir build
cd build

echo "****** CMAKE"
which cmake
echo $CONDA_BUILD_SYSROOT
echo "****** ENV"
env

echo "****** CMAKE CONFIG"
#export VERBOSE=1

cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} \
      -DCMAKE_PREFIX_PATH=${PREFIX} \
      -DCMAKE_OSX_SYSROOT=${CONDA_BUILD_SYSROOT} \
      -DCMAKE_BUILD_TYPE=Release  \
      -LAH .. 


echo "****** COMPILE"
make -j${CPU_COUNT}
echo "****** INSTALL CXX LIB"
make install

echo "****** INSTALL PYTHON LIB"
cd ..
echo "PYTHON" ${PYTHON}
export PYTHONPATH=${PREFIX}/lib/python3.7/site-packages/
${BUILD_PREFIX}/bin/python setup.py install --prefix=${PREFIX}
