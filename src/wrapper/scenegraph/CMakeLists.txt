# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglsg SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglsg pglmath pglsg pglalgo pgltool)

if (Python3_FOUND)
	target_link_libraries(_pglsg Python3::Python)
elseif (Python2_FOUND)
    target_link_libraries(_pglsg Python2::Python)
endif()

# Disable Boost Auto-Link
target_compile_definitions(_pglsg PRIVATE BOOST_ALL_NO_LIB)

target_link_libraries(_pglsg Boost::system Boost::thread Boost::python)

if(USE_BOOST_NUMPY)
    target_link_libraries(_pglsg Boost::numpy)
endif()

# --- Dependencies

add_dependencies(_pglsg pglmath pglsg pglalgo pgltool)

# --- Output Library

set_target_properties(_pglsg PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(_pglsg PROPERTIES SUFFIX ".pyd")
endif()

if (APPLE)
    set_target_properties(_pglsg PROPERTIES SUFFIX ".so")
endif()


install(TARGETS _pglsg DESTINATION "${CMAKE_SOURCE_DIR}/src/plantgl/scenegraph")
