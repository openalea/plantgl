cmake_minimum_required(VERSION 3.5)
project(plantgl CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

set(ENV{PKG_CONFIG_PATH} "$ENV{CONDA_PREFIX}/lib/pkgconfig")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;$ENV{CONDA_PREFIX}/lib/CGAL")

pkg_check_modules(QtCore REQUIRED QtCore>=4)
pkg_check_modules(QtOpenGL REQUIRED QtOpenGL>=4)
pkg_check_modules(QtNetwork REQUIRED QtNetwork>=4)
pkg_check_modules(QtGui REQUIRED QtGui>=4)
pkg_get_variable(QT_MOC_EXECUTABLE QtGui moc_location)
pkg_get_variable(QT_UIC_EXECUTABLE QtGui uic_location)
pkg_get_variable(QT_RCC_EXECUTABLE QtGui rcc_location)

pkg_check_modules(python REQUIRED python=2.7)
pkg_check_modules(libpng REQUIRED libpng)
pkg_check_modules(zlib REQUIRED zlib)
find_package (Threads REQUIRED)

set(BOOST_ROOT $ENV{CONDA_PREFIX})
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.66 REQUIRED COMPONENTS system thread python)

set(CGAL_DIR $ENV{CONDA_PREFIX}/lib/CGAL)
set(CGAL_NO_BLANKET_LINKING TRUE)
find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

set(GMP_DIR $ENV{CONDA_PREFIX}/lib/CGAL)
find_package(GMP REQUIRED)

set(MPFR_DIR $ENV{CONDA_PREFIX}/lib/CGAL)
find_package(MPFR REQUIRED)

find_package(OpenGL REQUIRED)

set(ANN_INCLUDE_DIRS $ENV{CONDA_PREFIX}/include/ANN)
set(ANN_LIBRARY_DIRS $ENV{CONDA_PREFIX}/lib)
set(ANN_LIBRARIES "ann")

set(QHULL_INCLUDE_DIRS $ENV{CONDA_PREFIX}/include/libqhull)
set(QHULL_LIBRARY_DIRS $ENV{CONDA_PREFIX}/lib)
set(QHULL_LIBRARIES "qhull")

set(FLEX_EXECUTABLE $ENV{CONDA_PREFIX}/bin/flex)
set(FLEX_INCLUDE_DIRS $ENV{CONDA_PREFIX}/include)
set(FLEX_LIBRARY_DIRS $ENV{CONDA_PREFIX}/lib)
set(FLEX_LIBRARIES "m;fl")

set(BISON_EXECUTABLE $ENV{CONDA_PREFIX}/bin/bison)

set(PGL_DEFINE_VARIABLES
        CGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES
        _GLIBCXX_USE_CXX11_ABI=1
        NDEBUG
        QT_GUI_LIB
        BISON_HPP
        WITH_BISON
        WITH_FLEX
        WITH_QHULL_2011
        WITH_QHULL
        BOOST_PYTHON_DYNAMIC_LIB
        WITH_ANN
        WITH_GMP
        WITH_CGAL
        PGL_USE_DOUBLE
        QT_SHARED
        )

set(PGL_LIBRARIES
        ${QtCore_LIBRARIES}
        ${QtGui_LIBRARIES}
        ${QtNetwork_LIBRARIES}
        ${QtOpenGL_LIBRARIES}
        ${python_LIBRARIES}
        ${libpng_LIBRARIES}
        ${zlib_LIBRARIES}
        Boost::system
        Boost::thread
        Boost::python
        ${CGAL_LIBRARIES}
        ${GMP_LIBRARIES}
        ${MPFR_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${ANN_LIBRARIES}
        ${QHULL_LIBRARIES}
        ${FLEX_LIBRARIES}
        )

include_directories(
        src/cpp
        ${QtCore_INCLUDE_DIRS}
        ${QtGui_INCLUDE_DIRS}
        ${QtNetwork_INCLUDE_DIRS}
        ${QtOpenGL_INCLUDE_DIRS}
        ${python_INCLUDE_DIRS}
        ${libpng_INCLUDE_DIRS}
        ${zlib_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${CGAL_INCLUDE_DIRS}
        ${GMP_INCLUDE_DIR}
        ${MPFR_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
        ${ANN_INCLUDE_DIRS}
        ${QHULL_INCLUDE_DIRS}
        ${FLEX_INCLUDE_DIRS}
)

link_directories(
        ${QtCore_LIBRARY_DIRS}
        ${QtGui_LIBRARY_DIRS}
        ${QtNetwork_LIBRARY_DIRS}
        ${QtOpenGL_LIBRARY_DIRS}
        ${python_LIBRARY_DIRS}
        ${libpng_LIBRARY_DIRS}
        ${zlib_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
        ${CGAL_LIBRARIES_DIR}
        ${GMP_LIBRARIES_DIR}
        ${MPFR_LIBRARIES_DIR}
        ${OPENGL_LIBRARIES_DIRS}
        ${ANN_LIBRARY_DIRS}
        ${QHULL_LIBRARY_DIRS}
        ${FLEX_LIBRARY_DIRS}
)

add_subdirectory(src/cpp)
add_subdirectory(src/wrapper)

install(DIRECTORY share/ DESTINATION share)