# Include Directory
find_path(ANN_INCLUDE_DIR "ANN/ANN.h" PATHS $ENV{PATH})

# Library Directory
find_library(ANN_LIBRARY NAMES "ann" "libann" PATHS $ENV{PATH})

if (ANN_INCLUDE_DIR AND ANN_LIBRARY)
    set(ANN_FOUND ON)
    set(ANN_INCLUDE_DIRS ${ANN_INCLUDE_DIR})
    set(ANN_LIBRARIES ${ANN_LIBRARY})
    
    # ANN found
    message(STATUS "Found ANN: ${ANN_LIBRARY}")
else()
    set(ANN_FOUND OFF)
    
    if (ANN_FIND_REQUIRED)
        # ANN not found
        message(SEND_ERROR "Unable to find ANN library.")
    endif()
endif()

if (ANN_FOUND)
    # Build with ANN
    add_compile_definitions(WITH_ANN=1)
    
    include_directories(${ANN_INCLUDE_DIRS})
    
elseif (NOT ANN_FIND_REQUIRED)
    message(WARNING "Building without ANN - library not found.")
endif()
