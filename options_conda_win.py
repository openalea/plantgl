#scons parameters file
#use this file to pass custom parameter to SConstruct script

build_prefix="build-scons"

import sys
import os
import glob

compiler = 'msvc'

EXTRA_CXXFLAGS="-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES  -D_GLIBCXX_USE_CXX11_ABI=1"

qhull_libs_suffix = 'static'

if 'CPU_COUNT' in os.environ:
    num_jobs = os.environ['CPU_COUNT']

try:
    qtconfig = file(os.path.join(os.environ['LIBRARY_INC'],'qt','QtCore','qglobal.h'),'r').read()
    pattern = '#define QT_VERSION '
    i = qtconfig.index(pattern)+len(pattern)
    qtconfig = qtconfig[i:].splitlines()[0]
    QT_VERSION = eval(qtconfig) >> 16
except:
    QT_VERSION = 5
