#scons parameters file
#use this file to pass custom parameter to SConstruct script

build_prefix="build-scons"

import sys
import os
import glob

compiler = 'msvc'

EXTRA_CPPDEFINES="CONDA_WINDOWS PGL_CORE_WITHOUT_QT"

qhull_libs_suffix = 'static'

#if 'CPU_COUNT' in os.environ:
#    num_jobs = os.environ['CPU_COUNT']

library_inc = os.environ.get('LIBRARY_INC',os.path.join(os.environ['CONDA_PREFIX'],'Library','include'))

try:
    qversionconfig = file(os.path.join(library_inc,'qt','QtCore','qconfig.h'),'r').read()
    pattern = '#define QT_VERSION_MAJOR '
    try:
        i = qversionconfig.index(pattern)+len(pattern)
        qversionconfig = qversionconfig[i:].splitlines()[0]
        QT_VERSION = eval(qversionconfig)
    except ValueError, ie:
        qversionconfig = file(os.path.join(library_inc,'qt','QtCore','qglobal.h'),'r').read()
        pattern = '#define QT_VERSION '
        i = qversionconfig.index(pattern)+len(pattern)
        qversionconfig = qversionconfig[i:].splitlines()[0] >> 16
        QT_VERSION = eval(qversionconfig)
except Exception, ie:
    print 'Autodetect qt error:', ie
    QT_VERSION = 4

EXTRA_LINKFLAGS ='/NODEFAULTLIB:boost_python-vc140-mt-1_65_1'
EXTRA_LINKFLAGS +=' /MACHINE:'+('X86' if  sys.maxsize.bit_length() < 63 else 'X64')
