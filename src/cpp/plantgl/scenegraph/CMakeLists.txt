file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(pglsg SHARED ${SRC_FILES})
target_link_libraries(pglsg ${PGL_LIBRARIES} pgltool pglmath)
target_compile_definitions(pglsg PRIVATE ${PGL_DEFINE_VARIABLES} SG_MAKEDLL)
add_dependencies(pglsg pgltool pglmath)

install(TARGETS pglsg
        LIBRARY DESTINATION lib)
