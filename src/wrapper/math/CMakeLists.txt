# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglmath SHARED ${SRC_FILES})

# --- Linked Library

target_link_libraries(_pglmath pglmath)

if (Python3_FOUND)
	target_link_libraries(_pglmath Python3::Python)
endif()

# --- Dependencies

add_dependencies(_pglmath pglmath)

# --- Output Library

install(TARGETS _pglmath LIBRARY DESTINATION "lib")
