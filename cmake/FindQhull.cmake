# Include Directory
if (DEFINED ENV{CONDA_PREFIX})
    find_path(QHULL_INCLUDE_DIR "libqhull/qhull_a.h" "qhull/qhull_a.h" PATHS $ENV{CONDA_PREFIX}/include)
else ()
    find_path(QHULL_INCLUDE_DIR "libqhull/qhull_a.h" "qhull/qhull_a.h" PATHS $ENV{PATH})
endif()

# Library Directory
if (WIN32)
    if (DEFINED ENV{CONDA_PREFIX})
        find_library(QHULL_LIBRARY NAMES "qhullstatic" "libqhullstatic" PATHS $ENV{CONDA_PREFIX}/lib)
    else()
        find_library(QHULL_LIBRARY NAMES "qhullstatic" "libqhullstatic" PATHS $ENV{PATH} )
    endif()
else()
    if (DEFINED ENV{CONDA_PREFIX})
        find_library(QHULL_LIBRARY NAMES "qhull" "libqhull" PATHS $ENV{CONDA_PREFIX}/lib)
    else()
        find_library(QHULL_LIBRARY NAMES "qhull" "libqhull" PATHS $ENV{PATH})
    endif()
endif()

if (QHULL_INCLUDE_DIR AND QHULL_LIBRARY)
    set(QHULL_FOUND ON)
    set(QHULL_INCLUDE_DIRS ${QHULL_INCLUDE_DIR})
    set(QHULL_LIBRARIES ${QHULL_LIBRARY})
    
    # Qhull found
    message(STATUS "Found Qhull: ${QHULL_LIBRARY}")
else()
    set(QHULL_FOUND OFF)
    
    if (Qhull_FIND_REQUIRED)
        # Qhull not found
        message(SEND_ERROR "Unable to find Qhull library.")
    endif()
endif()

if (QHULL_FOUND)
    # Build with Qhull
    define_cpp_macro(PGL_WITH_QHULL 1)
    define_cpp_macro(PGL_WITH_QHULL_2011 1)
    define_py_macro(PGL_WITH_QHULL "True")
    
    include_directories(${QHULL_INCLUDE_DIRS})
    
elseif (NOT Qhull_FIND_REQUIRED)
    message(STATUS "Building without Qhull - Library not found.")
    
    define_py_macro(PGL_WITH_QHULL "False")
endif()
