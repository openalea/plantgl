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
else
    LIBPREFIX1=${BUILD_PREFIX}/${HOST}/sysroot/lib64
    LIBPREFIX2=${BUILD_PREFIX}/x86_64-conda-linux-gnu/sysroot/lib64
    USRLIBPREFIX1=${BUILD_PREFIX}/${HOST}/sysroot/usr/lib64
    USRLIBPREFIX2=${BUILD_PREFIX}/x86_64-conda-linux-gnu/sysroot/usrlib64
    echo "****** CDT and OpenGL search"
    #for p in ${LIBPREFIX1} ${LIBPREFIX2} ${USRLIBPREFIX1} ${USRLIBPREFIX2} "${BUILD_PREFIX}/lib" "${BUILD_PREFIX}/lib64" "${BUILD_PREFIX}/x86_64-conda-linux-gnu/sysroot/lib"  "${BUILD_PREFIX}/x86_64-conda-linux-gnu/sysroot/usr/lib" ; 
    #do
    #  if [[ -d $p ]]; then
    #    echo "*** Check" $p
    #    ls $p
    #  fi
    #done
    #echo "*** done checking"

    #LIBCANDIDATE1=${LIBPREFIX1}/libselinux.so.1
    #LIBCANDIDATE2=${LIBPREFIX2}/libselinux.so.1
    #if [[ -f "$LIBCANDIDATE1" ]]; then
    #  echo "OLD CDT SELinux"
    #  SYSTEM_DEPENDENT_ARGS=(
    #      "-DLIBSELINUX_LIBRARY=${LIBCANDIDATE1}"          
    #  )    
    #elif [[ -f "$LIBCANDIDATE2" ]]; then
    #    echo "NEW CDT SE Linux"
    #    SYSTEM_DEPENDENT_ARGS=(
    #      "-DLIBSELINUX_LIBRARY=${LIBCANDIDATE2}"
    #    )
    #else
    #    echo "Cannot find libselinux."      
    #fi

    LIBCANDIDATE1=${USRLIBPREFIX1}/libGL.so
    LIBCANDIDATE2=${USRLIBPREFIX2}/libGL.so
    if [[ -f "$LIBCANDIDATE1" ]]; then
      echo "OLD CDT OpenGL"
      SYSTEM_DEPENDENT_ARGS=(
          ${SYSTEM_DEPENDENT_ARGS}
          "-DOPENGL_opengl_LIBRARY=${LIBCANDIDATE1}"
          "-DOPENGL_glx_LIBRARY=${LIBCANDIDATE1}"
      )    
    elif [[ -f "$LIBCANDIDATE2" ]]; then
        echo "NEW CDT OpenGL"
        SYSTEM_DEPENDENT_ARGS=(
          ${SYSTEM_DEPENDENT_ARGS}
          "-DOPENGL_opengl_LIBRARY=${LIBCANDIDATE2}"
          "-DOPENGL_glx_LIBRARY=${LIBCANDIDATE2}"
        )
    else
        echo "Cannot find OpenGL."      
    fi
    echo
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
