# Include Directory
find_path(EIGEN_INCLUDE_DIR "Eigen/Core" PATHS "${EIGEN_ROOT}/include")

if (EIGEN_INCLUDE_DIR)
    set(EIGEN_FOUND ON)
    set(EIGEN_INCLUDE_DIRS ${EIGEN_INCLUDE_DIR})
    
    # EIGEN found
    message(STATUS "Found Eigen: ${EIGEN_INCLUDE_DIR}")
else()
    set(EIGEN_FOUND OFF)
    
    if (Eigen_FIND_REQUIRED)
        # EIGEN not found
        message(SEND_ERROR "Unable to find Eigen library.")
    endif()
endif()

if (EIGEN_FOUND)
    # Build with Eigen
    add_compile_definitions(WITH_EIGEN=1)
    
    include_directories(${EIGEN_INCLUDE_DIRS})
    
elseif (NOT Eigen_FIND_REQUIRED)
    message(WARNING "Building without Eigen - library not found.")
endif()
