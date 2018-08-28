file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_custom_target(pgl_parcer
        DEPENDS scne_scanner.cc scne_parser.cpp scne_parser.hpp
        COMMENT "Creating parsers")

add_custom_command(OUTPUT scne_scanner.cc
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    --outfile=${CMAKE_CURRENT_BINARY_DIR}/scne_scanner.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/codec/scne_scanner.l
        COMMENT "Generating scne_scanner.cc")

add_custom_command(OUTPUT scne_parser.cpp scne_parser.hpp
        COMMAND ${BISON_EXECUTABLE}
        ARGS    --yacc ${CMAKE_CURRENT_SOURCE_DIR}/codec/scne_parser.y
                --defines=${CMAKE_CURRENT_BINARY_DIR}/scne_parser.hpp
                --output=${CMAKE_CURRENT_BINARY_DIR}/scne_parser.cpp
                --name-prefix=scne_yy
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/scne_scanner.cc
        COMMENT "Generating scne_parser.cpp and scne_parser.hpp")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/scne_parser.hpp DESTINATION include/plantgl/algo/codec)

set(SRC_FILES
        ${SRC_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/scne_scanner.cc
        ${CMAKE_CURRENT_BINARY_DIR}/scne_parser.cpp
        )
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/codec)

add_library(pglalgo SHARED ${SRC_FILES})
target_link_libraries(pglalgo ${PGL_LIBRARIES} pgltool pglmath pglsg)
target_compile_definitions(pglalgo PRIVATE ${PGL_DEFINE_VARIABLES} ALGO_MAKEDLL)
add_dependencies(pglalgo pgl_parcer pgltool pglmath pglsg)

install(TARGETS pglalgo
        LIBRARY DESTINATION lib)
