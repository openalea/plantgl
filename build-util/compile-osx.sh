cd build
export SDKPATH=`xcrun --show-sdk-path`
export SDKPATH=/Users/fboudon/Develop/SDK/MacOSX10.9.sdk

cmake -DCMAKE_INSTALL_PREFIX=${CONDA_PREFIX} \
      -DCMAKE_PREFIX_PATH=${CONDA_PREFIX} \
      -DCMAKE_OSX_SYSROOT=${SDKPATH} \
      -DCMAKE_BUILD_TYPE=Release  \
      -DOPENGL_INCLUDE_DIR=${SDKPATH}/System/Library/Frameworks/OpenGL.framework \
      -DOPENGL_gl_LIBRARY=${SDKPATH}/System/Library/Frameworks/OpenGL.framework \
      -DOPENGL_glu_LIBRARY=${SDKPATH}/System/Library/Frameworks/OpenGL.framework \
      -DBoost_DEBUG=ON \
      -DCMAKE_IGNORE_PATH=/opt/local/lib \
      -LAH ..

#cmake -DCMAKE_BUILD_TYPE=Release -DBoost_INCLUDE_DIR=/opt/local/include ..

make
make install
cd ..
python setup.py install

{
    
}