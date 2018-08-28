file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(_pglsg SHARED ${SRC_FILES})
target_link_libraries(_pglsg ${PGL_LIBRARIES} pglsg pglalgo)
target_compile_definitions(_pglsg PRIVATE ${PGL_DEFINE_VARIABLES})
add_dependencies(_pglsg pglsg pglalgo)

install(TARGETS _pglsg
        LIBRARY DESTINATION openalea/plantgl/scenegraph)