# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglsg SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglsg pglmath pglsg pglalgo pgltool)

if (Python3_FOUND)
	target_link_libraries(_pglsg Python3::Python)
endif()

# --- Dependencies

add_dependencies(_pglsg pglmath pglsg pglalgo pgltool)

# --- Output Library

install(TARGETS _pglsg LIBRARY DESTINATION "openalea/plantgl/scenegraph")
