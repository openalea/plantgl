set(CMAKE_SHARED_LIBRARY_PREFIX "")

if (APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif ()

add_subdirectory(math)
add_subdirectory(scenegraph)
add_subdirectory(algo)
#add_subdirectory(gui)
