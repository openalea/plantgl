file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(pglmath SHARED ${SRC_FILES})
target_link_libraries(pglmath ${PGL_LIBRARIES})
target_compile_definitions(pglmath PRIVATE ${PGL_DEFINE_VARIABLES} PGLMATH_MAKEDLL)

install(TARGETS pglmath
        LIBRARY DESTINATION lib)
