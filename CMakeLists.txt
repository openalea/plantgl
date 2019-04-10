# --- PlantGL Project

cmake_minimum_required(VERSION 3.12)
project(plantgl CXX)

# --- CMake Modules

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include("Anaconda")
include("CXX17")

# --- Libraries

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Python)
find_package(Qt)

# Boost
set(BOOST_ROOT ${CONDA_ENV})
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS OFF)

find_package(Boost 1.67 COMPONENTS system thread ${BOOST_PY_LIB})

if (Boost_FOUND)
    # Build with Boost
    add_compile_definitions(WITH_BOOST=1 BOOST_PYTHON_DYNAMIC_LIB)
else()
    message(WARNING "Building without Boost - library not found.")
endif()

# Other Libraries
set(ANN_ROOT ${CONDA_ENV})
set(CGAL_ROOT ${CONDA_ENV})
set(GMP_ROOT ${CONDA_ENV})
set(QHULL_ROOT ${CONDA_ENV})

find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

find_package(ANN)
find_package(Bison)
find_package(CGAL)
find_package(Eigen)
find_package(Flex)
find_package(GMP)
find_package(Qhull)

if (BISON_FOUND)
    # Build with Bison
    add_compile_definitions(WITH_BISON=1 BISON_HPP=1)
else()
    message(WARNING "Building without Bison - library not found.")
endif()

if (FLEX_FOUND)
    # Build with Flex
    add_compile_definitions(WITH_FLEX=1)

    include_directories(${FLEX_INCLUDE_DIRS})

else()
    message(WARNING "Building without Flex - library not found.")
endif()

# --- Preprocessor Macros

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1)
add_compile_definitions(QT_GUI_LIB QT_SHARED)
add_compile_definitions(PGL_USE_DOUBLE)

# --- Include Directories

include_directories("src/cpp")
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

# --- Libraries Directories

link_directories(${Boost_LIBRARY_DIRS})

# --- Source Directories

add_subdirectory("src/cpp")
add_subdirectory("src/wrapper")
