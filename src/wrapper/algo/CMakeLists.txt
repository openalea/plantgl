# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglalgo SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglalgo pglmath pgltool pglsg pglalgo)

if (Python3_FOUND)
	target_link_libraries(_pglalgo Python3::Python)
elseif (Python2_FOUND)
    target_link_libraries(_pglalgo Python2::Python)
endif()

if (QT5_FOUND)
    target_link_libraries(_pglalgo Qt5::Core Qt5::Widgets Qt5::OpenGL)
endif()

# Disable Boost Auto-Link
target_compile_definitions(_pglalgo PRIVATE BOOST_ALL_NO_LIB)

target_link_libraries(_pglalgo Boost::system Boost::thread Boost::python37 Boost::numpy)
# target_link_libraries(_pglalgo Boost::system Boost::thread Boost::${BOOST_PYTHON_LIB} Boost::${BOOST_NUMPY_LIB})

# --- Dependencies

add_dependencies(_pglalgo pglmath pgltool pglsg pglalgo)

# --- Output Library

set_target_properties(_pglalgo PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(_pglalgo PROPERTIES SUFFIX ".pyd")
endif()

if (APPLE)
    set_target_properties(_pglalgo PROPERTIES SUFFIX ".so")
endif()

install(TARGETS _pglalgo DESTINATION "${CMAKE_SOURCE_DIR}/src/plantgl/algo")
