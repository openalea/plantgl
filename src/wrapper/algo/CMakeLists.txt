# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglalgo SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglalgo pglmath pgltool pglsg pglalgo)

if (Qt6_FOUND)
    target_link_libraries(_pglalgo Qt6::Core Qt6::Widgets Qt6::OpenGL)
endif()

pglwrapper_link_boost(_pglalgo)
pglwrapper_link_python(_pglalgo)

# --- Dependencies

add_dependencies(_pglalgo pglmath pgltool pglsg pglalgo)

# --- Output Library

pglwrapper_install(_pglalgo algo)

