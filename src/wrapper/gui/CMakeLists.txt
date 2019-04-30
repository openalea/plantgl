# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglgui SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglgui pglmath pglgui pglsg pgltool)

if (Python3_FOUND)
	target_link_libraries(_pglgui Python3::Python)
endif()

if (QT5_FOUND)
    target_link_libraries(_pglgui Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Network)
endif()

# Disable Boost Auto-Link
target_compile_definitions(_pglgui PRIVATE BOOST_ALL_NO_LIB)

target_link_libraries(_pglgui Boost::system Boost::thread Boost::${BOOST_PYTHON_LIB} Boost::${BOOST_NUMPY_LIB})

# --- Dependencies

add_dependencies(_pglgui pglmath pglgui pglsg pgltool)

# --- Output Library

set_target_properties(_pglgui PROPERTIES SUFFIX ".pyd")

install(TARGETS _pglgui LIBRARY DESTINATION "lib")
