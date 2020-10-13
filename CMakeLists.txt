# --- CMake Modules

cmake_minimum_required(VERSION 3.12)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("Anaconda")
include("PglConfig")
include("PglWrapper")
include("detectPlantGL")

# --- PlantGL Project

project(plantgl CXX)

# --- Build setup

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)

if("${isSystemDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")



# ---  CXX11 Compilation

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# --- Config Header File

init_cpp_config("${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/plantgl/userconfig.h")
init_py_config("${CMAKE_CURRENT_SOURCE_DIR}/src/openalea/plantgl/config.py")

# --- (Win32) Multithreaded Compilation

if (MSVC)
    string(REGEX REPLACE "/W3" "/W0" ${CMAKE_CXX_FLAGS} "${${CMAKE_CXX_FLAGS}}") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")
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

find_package (Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
message(STATUS "Python version : " ${Python3_VERSION})
message(STATUS "Python lib : " ${Python3_LIBRARIES})

# --- Libraries

find_package(Threads REQUIRED)
find_package(Qt)


set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS OFF)
set(BUILD_SHARED_LIBS ON)

if (USE_CONDA)
    set(boost_python python)
    set(boost_numpy numpy)
else()
    set(boost_python python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR})
    set(boost_numpy numpy${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR})
endif()

find_package(ZLIB REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

find_package(CGAL)
find_package(ANN)
find_package(BISON)
find_package(Eigen)
find_package(FLEX)
find_package(Qhull)

#if (USE_CONDA_BUILD)
#if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
#      find_library(LIBSELINUX_LIBRARY "selinux" PATHS $ENV{BUILD_PREFIX}/x86_64-conda-linux-gnu/sysroot/lib64 $ENV{BUILD_PREFIX}/x86_64-conda_cos6-linux-gnu/sysroot/lib64  REQUIRED)
#      message(STATUS "Found libselinux: ${LIBSELINUX_LIBRARY}")
#endif()
#endif()



find_package(Boost COMPONENTS thread system chrono ${boost_python} ${boost_numpy})

if (Boost_FOUND)
    # Build with Boost
    define_cpp_macro(PGL_WITH_BOOST 1)

    if(DEFINED Boost_NUMPY_LIBRARY_RELEASE)
        define_macro(PGL_WITH_BOOST_NUMPY 1)
        set(USE_BOOST_NUMPY ON)
    elseif(DEFINED Boost_NUMPY${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}_LIBRARY_RELEASE)
        define_macro(PGL_WITH_BOOST_NUMPY 1)
        set(USE_BOOST_NUMPY ON)    
    else()
        define_macro(PGL_WITH_BOOST_NUMPY 0)
        set(USE_BOOST_NUMPY OFF)
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
