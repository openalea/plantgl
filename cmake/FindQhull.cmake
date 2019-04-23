# Include Directory
find_path(QHULL_INCLUDE_DIR "qhull/qhull_a.h" "libqhull/qhull_a.h" PATHS $ENV{PATH})

# Library Directory
if (WIN32)
    find_library(QHULL_LIBRARY NAMES "qhullstatic" "libqhullstatic" PATHS $ENV{PATH})
else()
    find_library(QHULL_LIBRARY NAMES "qhull" "libqhull" PATHS $ENV{PATH})
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
    define_macro(WITH_QHULL 1)
    define_macro(WITH_QHULL_2011 1)
    
    include_directories(${QHULL_INCLUDE_DIRS})
    
elseif (NOT Qhull_FIND_REQUIRED)
    message(STATUS "Building without Qhull - Library not found.")
endif()
