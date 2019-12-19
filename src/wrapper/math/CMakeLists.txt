# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglmath SHARED ${SRC_FILES})

# --- Linked Library

target_link_libraries(_pglmath pglmath)

if (Python3_FOUND)
	target_link_libraries(_pglmath Python3::Python)
elseif (Python2_FOUND)
    target_link_libraries(_pglmath Python2::Python)
endif()

# Disable Boost Auto-Link
target_compile_definitions(_pglmath PRIVATE BOOST_ALL_NO_LIB)

target_link_libraries(_pglmath Boost::system Boost::thread Boost::python )

# --- Dependencies

add_dependencies(_pglmath pglmath)

# --- Output Library

set_target_properties(_pglmath PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(_pglmath PROPERTIES SUFFIX ".pyd")
endif()

if (APPLE)
    set_target_properties(_pglmath PROPERTIES SUFFIX ".so")
endif()


install(TARGETS _pglmath DESTINATION "${CMAKE_SOURCE_DIR}/src/plantgl/math")
