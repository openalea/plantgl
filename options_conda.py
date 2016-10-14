
PREFIX = $CONDA_PREFIX


QTDIR=PREFIX
QT4_FRAMEWORK=False

python_includes=PREFIX+"/include/python2.7"
python_libpath=PREFIX+"/lib"
boost_includes=PREFIX+"/include"
boost_lib=PREFIX+"/lib"
openalea_lib=PREFIX+"/lib"
openalea_includes=PREFIX+"/include"

WITH_CGAL=True
cgal_includes=PREFIX+"/include"
cgal_libpath=PREFIX+"/lib"
bison_bin=PREFIX+"/bin"
flex_bin=PREFIX+"/bin"
gl_lib=ann_libpath=mpfr_libpath=gmp_libpath=qhull_lib=pthread_lib=termcap_lib=readline_lib=flex_libpath=PREFIX+"/lib"
gl_include=ann_includes=mpfr_includes=gmp_includes=qhull_includes=pthread_includes=termcap_includes=readline_includes=flex_include=PREFIX+"/include"
eigen_includes=PREFIX+"/include/eigen3"
EXTRA_CXXFLAGS="-DCGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES -DWITH_QHULL_2011"

WITH_NCURSES=True
WITH_QHULL_2011=True
WITH_GMP=True
WITH_MPFR=True
WITH_LAPACK=False
WITH_EIGEN=True

