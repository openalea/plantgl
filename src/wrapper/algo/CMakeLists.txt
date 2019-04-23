# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglalgo SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglalgo pglmath pgltool pglsg pglalgo)

if (Python3_FOUND)
	target_link_libraries(_pglalgo Python3::Python)
endif()

if (QT5_FOUND)
    target_link_libraries(_pglalgo Qt5::Core Qt5::Widgets Qt5::OpenGL)
endif()

# --- Dependencies

add_dependencies(_pglalgo pglmath pgltool pglsg pglalgo)

# --- Output Library

install(TARGETS _pglalgo LIBRARY DESTINATION "openalea/plantgl/algo")
