# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglsg SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglsg pglmath pglsg pglalgo pgltool)

if (Python3_FOUND)
	target_link_libraries(_pglsg Python3::Python)
endif()

# Disable Boost Auto-Link
target_compile_definitions(_pglsg PRIVATE BOOST_ALL_NO_LIB)

target_link_libraries(_pglsg Boost::system Boost::thread Boost::${BOOST_PYTHON_LIB} Boost::${BOOST_NUMPY_LIB})

# --- Dependencies

add_dependencies(_pglsg pglmath pglsg pglalgo pgltool)

# --- Output Library

set_target_properties(_pglsg PROPERTIES SUFFIX ".pyd")

install(TARGETS _pglsg DESTINATION "${CMAKE_SOURCE_DIR}/src/plantgl/scenegraph")
