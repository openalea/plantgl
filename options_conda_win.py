#scons parameters file
#use this file to pass custom parameter to SConstruct script

build_prefix="build-scons"

import sys
import os
import glob

compiler = 'msvc'

EXTRA_CXXFLAGS="-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES "

qhull_libs_suffix = 'static'

if 'CPU_COUNT' in os.environ:
    num_jobs = os.environ['CPU_COUNT']

try:
    qversionconfig = file(os.path.join(os.environ['LIBRARY_INC'],'qt','QtCore','qconfig.h'),'r').read()
    pattern = '#define QT_VERSION_MAJOR '
    try:
        i = qversionconfig.index(pattern)+len(pattern)
        qversionconfig = qversionconfig[i:].splitlines()[0]
        QT_VERSION = eval(qversionconfig)
    except ValueError, ie:
        qversionconfig = file(os.path.join(os.environ['LIBRARY_INC'],'qt','QtCore','qglobal.h'),'r').read()
        pattern = '#define QT_VERSION '
        i = qversionconfig.index(pattern)+len(pattern)
        qversionconfig = qversionconfig[i:].splitlines()[0] >> 16
        QT_VERSION = eval(qversionconfig)
except Exception, ie:
    print 'Autodetect qt error:', ie
    QT_VERSION = 4

if QT_VERSION == 5:
    QT5_CPPPATH = os.path.join(os.environ['LIBRARY_INC'],'qt')

EXTRA_LINKFLAGS ='/NODEFAULTLIB:boost_python-vc140-mt-1_65_1'
