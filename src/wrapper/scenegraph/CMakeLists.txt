# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglsg SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglsg pglmath pglsg pglalgo pgltool)

pglwrapper_link_python(_pglsg)

if(USE_BOOST_NUMPY)
    target_link_libraries(_pglsg Boost::numpy)
endif()

# --- Dependencies

add_dependencies(_pglsg pglmath pglsg pglalgo pgltool)

# --- Output Library

pglwrapper_install(_pglsg scenegraph)

