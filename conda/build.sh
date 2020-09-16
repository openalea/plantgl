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

    echo "****** SDK search"
    xcrun --show-sdk-path
    ls /opt
else
    SYSTEM_DEPENDENT_ARGS=(
        "-DOPENGL_opengl_LIBRARY=${BUILD_PREFIX}/${HOST}/sysroot/usr/lib64/libGL.so"
        "-DOPENGL_glx_LIBRARY=${BUILD_PREFIX}/${HOST}/sysroot/usr/lib64/libGL.so"
    )
fi

export SYSTEM_DEPENDENT_ARGS

echo
echo "****** CMAKE"
which cmake
echo 'CONDA_BUILD_SYSROOT:' $CONDA_BUILD_SYSROOT
echo
echo "****** ENV"
env

echo
echo "****** CMAKE CONFIG"

export GMPDIR=${PREFIX}

cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} \
      -DCMAKE_PREFIX_PATH=${PREFIX} \
      -DCMAKE_BUILD_TYPE=Release  \
      -DPython3_EXECUTABLE=${PYTHON} \
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

echo
echo "****** INSTALL PYTHON LIB"
cd ..
echo "PYTHON:" ${PYTHON}

#echo "** PYTHON CALL"
#export PYTHONPATH=${PREFIX}/lib/python${PY_VER}/site-packages/
${PYTHON} setup.py install --prefix=${PREFIX} 

echo
echo "****** CHECK PYTHON LIB"
# To check if Python lib is not in the dependencies with conda-forge distribution.
# See https://github.com/conda-forge/boost-feedstock/issues/81
if [ `uname` = "Darwin" ]; then
    export LDD='otool -L'
else
    export LDD='ldd'    
fi

${LDD} `${PYTHON} -c "import openalea.plantgl.math._pglmath as pm ; print(pm.__file__)"`
${LDD} `${PYTHON} -c "import openalea.plantgl.scenegraph._pglsg as pm ; print(pm.__file__)"`
${LDD} `${PYTHON} -c "import openalea.plantgl.algo._pglalgo as pm ; print(pm.__file__)"`
${LDD} `${PYTHON} -c "import openalea.plantgl.gui._pglgui as pm ; print(pm.__file__)"`

echo "****** END OF BUILD PROCESS"
