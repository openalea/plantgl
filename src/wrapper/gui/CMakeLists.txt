file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(_pglgui SHARED ${SRC_FILES})
target_link_libraries(_pglgui ${PGL_LIBRARIES} pglgui)
target_compile_definitions(_pglgui PRIVATE ${PGL_DEFINE_VARIABLES})
add_dependencies(_pglgui pglgui)

install(TARGETS _pglgui
        LIBRARY DESTINATION openalea/plantgl/gui)
