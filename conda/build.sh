#!/bin/bash

if [[ -d build ]]; then
    rm -rf build
fi
mkdir build
cd build

if [ `uname` = "Darwin" ]; then
    SYSTEM_DEPENDENT_ARGS=(
        "-DCMAKE_OSX_SYSROOT=${CONDA_BUILD_SYSROOT}"
   )
    export LDFLAGS="-undefined dynamic_lookup ${LDFLAGS}"
else
    SYSTEM_DEPENDENT_ARGS=(
        "-DOPENGL_opengl_LIBRARY=${BUILD_PREFIX}/${HOST}/sysroot/usr/lib64/libGL.so"
        "-DOPENGL_glx_LIBRARY=${BUILD_PREFIX}/${HOST}/sysroot/usr/lib64/libGL.so"
    )
fi

export CXXFLAGS="-O2 -DNDEBUG ${CXXFLAGS}"


echo
echo "****** CMAKE"
which cmake
echo 'CONDA_BUILD_SYSROOT:' $CONDA_BUILD_SYSROOT
echo
echo "****** ENV"
env

echo
echo "****** CMAKE CONFIG"

cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} \
      -DCMAKE_PREFIX_PATH=${PREFIX} \
      -DCMAKE_BUILD_TYPE=Release  \
       ${SYSTEM_DEPENDENT_ARGS[@]} \
      -LAH .. 

echo
echo "****** PGL CONFIG"
cat $SRC_DIR/src/cpp/plantgl/userconfig.h
echo
echo "****** COMPILE"
export VERBOSE=1
make -j${CPU_COUNT}
echo "****** INSTALL CXX LIB"
make install

echo "****** INSTALL PYTHON LIB"
cd ..
echo "PYTHON:" ${PYTHON}
#${PYTHON} --version
#echo "PYTHON VERSION" ${PY_VER}

#echo "** PYTHON CALL"
#export PYTHONPATH=${PREFIX}/lib/python${PY_VER}/site-packages/
${PYTHON} setup.py install --prefix=${PREFIX} 

echo "****** END OF BUILD PROCESS"
