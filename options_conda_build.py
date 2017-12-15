import os

PREFIX = os.environ.get('PREFIX')
SRC_DIR = os.environ.get('SRC_DIR')

build_prefix="build-scons"


EXTRA_CXXFLAGS="-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES  -D_GLIBCXX_USE_CXX11_ABI=1"

if 'CPU_COUNT' in os.environ:
    num_jobs = os.environ['CPU_COUNT']

EXTRA_LIBS = "boost_system boost_thread png" 
