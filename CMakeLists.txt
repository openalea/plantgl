# --- CMake Modules

cmake_minimum_required(VERSION 3.20)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("Anaconda")
include("PglConfig")
include("PglWrapper")
include("detectPlantGL")

# --- PlantGL Project

project(plantgl CXX)
#project(
#  ${SKBUILD_PROJECT_NAME}
#  LANGUAGES CXX
#  VERSION ${SKBUILD_PROJECT_VERSION})

# --- Build setup

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)

if("${isSystemDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")



# ---  CXX17 Compilation

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# --- Config Header File

init_cpp_config("${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/plantgl/userconfig.h")
init_py_config("${CMAKE_CURRENT_SOURCE_DIR}/src/openalea/plantgl/config.py")

# --- (Win32) Multithreaded Compilation

if (MSVC)
    string(REGEX REPLACE "/W3" "/W0" ${CMAKE_CXX_FLAGS} "${${CMAKE_CXX_FLAGS}}") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")

    # To fix compilation error with vc14 and boost
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DHAVE_SNPRINTF")
endif()


# --- PlantGL Configuration

detect_plantgl_version(${CMAKE_CURRENT_SOURCE_DIR}/src/cpp)
message(STATUS "PlantGL version : " ${PGL_VERSION_STR})

define_py_macro(PGL_VERSION ${PGL_VERSION})
define_py_macro(PGL_VERSION_STR "'${PGL_VERSION_STR}'")
define_py_macro(PGL_USE_DOUBLE "True")
define_cpp_macro(PGL_USE_DOUBLE 1)


## ###################################################################
## Dependencies 
## ###################################################################

# --- Python
set(Python_FIND_VIRTUALENV FIRST)
if (WIN32)
  # needed when we run cmake in a conda environment
  set(Python_FIND_REGISTRY LAST)
endif()


find_package (Python COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python_INCLUDE_DIRS})
message(STATUS "Python version : " ${Python_VERSION})
message(STATUS "Python lib : " ${Python_LIBRARIES})

# --- Libraries

find_package(Threads REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network OpenGL PrintSupport)

if(Qt6_FOUND)
find_package(Qt6 REQUIRED COMPONENTS Gui OpenGLWidgets Test) 
endif()

find_library(METAL Metal)

define_cpp_macro(PGL_QT_VERSION ${QT_VERSION_MAJOR})
define_py_macro(PGL_WITHOUT_QT "False")
define_py_macro(PGL_QT_VERSION ${QT_VERSION_MAJOR})

if(POLICY CMP0167) # Removes find boost warning
  cmake_policy(SET CMP0167 NEW)
endif()

set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS OFF)
set(BUILD_SHARED_LIBS ON)

set(boost_python python${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
set(boost_numpy numpy${Python_VERSION_MAJOR}${Python_VERSION_MINOR})

find_package(ZLIB REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

find_package(ANN)
find_package(BISON)
find_package(Eigen3)
if (WIN32)
    # winflexbison installs FlexLexer.h in Library/share/winflexbison, not searched by
    # FindFLEX. NO_CMAKE_FIND_ROOT_PATH: use these paths as is under conda-build.
    find_path(FLEX_INCLUDE_DIR FlexLexer.h
              PATHS "$ENV{BUILD_PREFIX}/Library/share/winflexbison"
                    "$ENV{CONDA_PREFIX}/Library/share/winflexbison"
              NO_CMAKE_FIND_ROOT_PATH)
endif()
find_package(FLEX)
# Config packages of conda-forge qhull: Qhull::qhull_r (reentrant, shared) is used.
find_package(Qhull CONFIG)


find_package(Boost COMPONENTS headers thread filesystem chrono ${boost_python} ${boost_numpy})
find_package(CGAL)

if (Boost_FOUND)
    # Build with Boost
    define_cpp_macro(PGL_WITH_BOOST 1)

    if(DEFINED Boost_NUMPY_FOUND)
        define_macro(PGL_WITH_BOOST_NUMPY 1)
        set(USE_BOOST_NUMPY ON)
        message(STATUS "Building with Boost.Numpy")
    elseif(DEFINED Boost_NUMPY${Python_VERSION_MAJOR}${Python_VERSION_MINOR}_FOUND)
        define_macro(PGL_WITH_BOOST_NUMPY 1)
        set(USE_BOOST_NUMPY ON)    
        message(STATUS "Building with Boost.Numpy")
    else()
        define_macro(PGL_WITH_BOOST_NUMPY 0)
        set(USE_BOOST_NUMPY OFF)
        message(STATUS "Building without Boost.Numpy - Library not found.")
    endif()

    set(USE_BOOST ON)

    include_directories(${Boost_INCLUDE_DIR})
else()
    define_py_macro(PGL_WITH_BOOST 0)
    set(USE_BOOST OFF)
    set(USE_BOOST_NUMPY OFF)
    message(STATUS "Building without Boost - Library not found.")
endif()


if (BISON_FOUND AND FLEX_FOUND)
    # Build with Bison and Flex
    define_cpp_macro(PGL_WITH_FLEX 1)
    define_cpp_macro(PGL_WITH_BISON 1)
    define_cpp_macro(PGL_BISON_HPP 1)
    define_py_macro(PGL_WITH_BISONFLEX "True")
else()
    message(STATUS "Build PlantGL without Bison/Flex - Not found.")
    
    define_py_macro(PGL_WITH_BISONFLEX "False")
endif()

if (CGAL_FOUND)
    # Build with CGAL
    define_cpp_macro(PGL_WITH_CGAL 1)
    define_py_macro(PGL_WITH_CGAL "True")
    message(STATUS "Build PlantGL with CGAL")

    if (DEFINED GMP_LIBRARIES)
        define_macro(PGL_CGAL_USE_GMP 1)
        message(STATUS "Build PlantGL with GMP")
    else()
        message(STATUS "Build PlantGL without GMP - Library not found. Use CGAL without it.")
    endif()
    
else()
    message(STATUS "Build PlantGL without CGAL - Library not found.")
    
    define_py_macro(PGL_WITH_CGAL "False")
endif()

# Eigen is only used through CGAL (jet fitting of principal_curvatures, Eigen
# diagonalization in algo/base/pointmanipulation2.cpp). CGAL::Eigen3_support defines
# CGAL_EIGEN3_ENABLED and adds the Eigen include directory; pglalgo links it.
if (CGAL_FOUND AND Eigen3_FOUND)
    include(CGAL_Eigen3_support)
endif()
if (TARGET CGAL::Eigen3_support)
    define_cpp_macro(PGL_WITH_EIGEN 1)
    define_py_macro(PGL_WITH_EIGEN "True")
    message(STATUS "Build PlantGL with Eigen (CGAL Eigen3 support)")
else()
    define_py_macro(PGL_WITH_EIGEN "False")
    message(STATUS "Build PlantGL without Eigen - Eigen3 or CGAL not found.")
endif()

# Qhull: convex hull fitting (Fit.convexHull, Fit.convexPolyline in algo/fitting/fit.cpp)
if (TARGET Qhull::qhull_r)
    define_cpp_macro(PGL_WITH_QHULL 1)
    define_py_macro(PGL_WITH_QHULL "True")
    message(STATUS "Build PlantGL with Qhull")
else()
    define_py_macro(PGL_WITH_QHULL "False")
    message(STATUS "Build PlantGL without Qhull - Library not found.")
endif()

# --- Include Directories

include_directories("src/cpp")
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

# --- Library Directory

if (DEFINED CONDA_ENV)
    link_directories("${CONDA_ENV}/lib")
endif()

# --- Source Directories

add_subdirectory("src/cpp")

if (Boost_FOUND)
    add_subdirectory("src/wrapper")
endif()

install_share("share/plantgl" "plantgl")
