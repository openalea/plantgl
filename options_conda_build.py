import os


EXTRA_CXXFLAGS="-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES"

if 'CPU_COUNT' in os.environ:
    num_jobs = os.environ['CPU_COUNT']


EXTRA_LIBS = "boost_system png" 
