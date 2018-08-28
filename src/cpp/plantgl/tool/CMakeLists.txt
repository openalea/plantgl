file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(pgltool SHARED ${SRC_FILES})
target_link_libraries(pgltool ${PGL_LIBRARIES})
target_compile_definitions(pgltool PRIVATE ${PGL_DEFINE_VARIABLES} TOOLS_MAKEDLL)

install(TARGETS pgltool
        LIBRARY DESTINATION lib)
