QTDIR='/opt/local'
prefix='/opt/local'

flex_bin = bison_bin = prefix+'/bin'
boost_libs_suffix='-mt'
num_jobs=4
EXTRA_CXXFLAGS= '-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES -DWITH_QHULL_2011'
EXTRA_LINKFLAGS = '-F/opt/local/Library/Frameworks'

lapack_includes = cgal_includes = boost_includes = qhull_includes = readline_includes = termcap_includes = flex_include = mpfr_includes = gmp_includes = ann_includes=prefix+'/include'
lapack_libpath = cgal_libpath = boost_lib = qhull_lib = readline_lib = termcap_lib = flex_lib = mpfr_libpath=gmp_libpath=ann_libpath=prefix+'/lib' 
