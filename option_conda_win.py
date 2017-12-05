#scons parameters file
#use this file to pass custom parameter to SConstruct script

build_prefix="build-scons"

import sys

import os


compiler = 'msvc'

PREFIX = os.environ.get('CONDA_PREFIX')
LIBPREFIX  = os.path.join(PREFIX,"Library")

QTDIR=os.path.join(PREFIX,"Library")

DepIncPrefix = os.path.join(LIBPREFIX, "include")
DepLibPrefix = os.path.join(LIBPREFIX, "lib")
DepBinPrefix = os.path.join(LIBPREFIX, "bin")

python_includes=PREFIX+"/include/"
python_libpath=PREFIX+"/libs"

boost_includes=DepIncPrefix
boost_lib=DepLibPrefix
boost_libs_suffix='-vc90-mt-1_65_1'

openalea_includes=DepIncPrefix
openalea_lib=DepLibPrefix

bison_bin =  os.path.join(LIBPREFIX, "usr", "bin")
flex_bin = os.path.join(LIBPREFIX, "usr", "bin")
flex_include = os.path.join(LIBPREFIX, "usr", "include")

cgal_includes=DepIncPrefix
cgal_libpath=DepLibPrefix
gl_lib=ann_libpath=mpfr_libpath=gmp_libpath=qhull_lib=pthread_lib=termcap_lib=readline_lib=flex_libpath=DepLibPrefix
gl_include=ann_includes=mpfr_includes=gmp_includes=qhull_includes=pthread_includes=termcap_includes=readline_includes=DepIncPrefix

eigen_includes=DepIncPrefix+"/eigen3"
EXTRA_CXXFLAGS="-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES  -D_GLIBCXX_USE_CXX11_ABI=1"
EXTRA_LIBS="Advapi32 glu32"

qhull_libs_suffix = 'static'

