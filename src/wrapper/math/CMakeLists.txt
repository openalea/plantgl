# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglmath SHARED ${SRC_FILES})

# --- Linked Library

target_link_libraries(_pglmath pglmath)

pglwrapper_link_python(_pglmath)

# --- Dependencies

add_dependencies(_pglmath pglmath)

# --- Output Library

pglwrapper_install(_pglmath math)