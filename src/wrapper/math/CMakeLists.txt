file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(_pglmath SHARED ${SRC_FILES})
target_link_libraries(_pglmath ${PGL_LIBRARIES} pglmath)
target_compile_definitions(_pglmath PRIVATE ${PGL_DEFINE_VARIABLES})
add_dependencies(_pglmath pglmath)

install(TARGETS _pglmath
        LIBRARY DESTINATION openalea/plantgl/math)