# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglsg SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglsg pglmath pglsg pglalgo pgltool)

pglwrapper_link_boost(_pglsg)

pglwrapper_link_python(_pglsg)

pglwrapper_link_numpy(_pglsg)

# --- Dependencies

add_dependencies(_pglsg pglmath pglsg pglalgo pgltool)

# --- Output Library

pglwrapper_install(_pglsg scenegraph)

