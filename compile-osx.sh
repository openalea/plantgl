cd build
cmake -DCMAKE_INSTALL_PREFIX=${CONDA_PREFIX} \
      -DCMAKE_PREFIX_PATH=${CONDA_PREFIX} \
      -DCMAKE_OSX_SYSROOT=/Users/fboudon/Develop/SDK/MacOSX10.9.sdk \
      -DCMAKE_BUILD_TYPE=Release  \
      ..
make
make install
python setup.py install
cd ..