file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(_pglalgo SHARED ${SRC_FILES})
target_link_libraries(_pglalgo ${PGL_LIBRARIES} pglalgo)
target_compile_definitions(_pglalgo PRIVATE ${PGL_DEFINE_VARIABLES})
add_dependencies(_pglalgo pglalgo)

install(TARGETS _pglalgo
        LIBRARY DESTINATION openalea/plantgl/algo)