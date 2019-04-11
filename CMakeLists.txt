# --- PlantGL Project

cmake_minimum_required(VERSION 3.12)
project(plantgl CXX)

# --- CMake Modules

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include("Anaconda")
include("CXX17")

# --- Libraries

find_package(Threads REQUIRED)
find_package(Python)
find_package(Qt)

# Boost
if (DEFINED CONDA_ENV)
    set(BOOST_ROOT ${CONDA_ENV})
endif()

set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS OFF)

find_package(Boost 1.67 COMPONENTS system thread ${BOOST_PYTHON_LIB})

if (Boost_FOUND)
    # Build with Boost
    add_compile_definitions(WITH_BOOST=1 BOOST_PYTHON_DYNAMIC_LIB)

    set(USE_BOOST ON)
    include_directories(${Boost_INCLUDE_DIR})
else()
    set(USE_BOOST OFF)
    message(STATUS "Building without Boost - Library not found.")
endif()

# Other Libraries
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

find_package(ANN)
find_package(Bison)
find_package(CGAL QUIET)
find_package(Eigen)
find_package(Flex)
find_package(GMP)
find_package(Qhull)

if (BISON_FOUND AND FLEX_FOUND)
    # Build with Bison and Flex
    add_compile_definitions(WITH_BISONFLEX=1 BISON_HPP=1 WITH_BISON=1 WITH_FLEX=1)
else()
    message(STATUS "Building without Bison/Flex - Not found.")
endif()

if (CGAL_FOUND)
    # Build with CGAL
    add_compile_definitions(WITH_CGAL=1)
    
    message(STATUS "Found CGAL")
else()
    message(STATUS "Building without CGAL - Library not found.")
endif()

# --- Preprocessor Definitions

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1)
add_compile_definitions(QT_GUI_LIB=1 QT_SHARED=1)
add_compile_definitions(PGL_USE_DOUBLE=1)

# --- Include Directories

include_directories("src/cpp")
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

# --- Source Directories

add_subdirectory("src/cpp")
#add_subdirectory("src/wrapper")
