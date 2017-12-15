#scons parameters file
#use this file to pass custom parameter to SConstruct script

build_prefix="build-scons"

import sys
import os


compiler = 'msvc'

EXTRA_CXXFLAGS="-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES  -D_GLIBCXX_USE_CXX11_ABI=1"

qhull_libs_suffix = 'static'

if 'CPU_COUNT' in os.environ:
	num_jobs = os.environ['CPU_COUNT']
