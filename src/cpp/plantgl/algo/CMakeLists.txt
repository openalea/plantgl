# --- Bison and Flex

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})

if (BISON_FOUND AND FLEX_FOUND)
    set(GENERATE_PARSER OM)
endif()

if (GENERATE_PARSER)
    add_custom_target(
        pglparser
        DEPENDS "scne_scanner.cpp" "scne_parser.cpp" "scne_parser.hpp"
        COMMENT "Generating parsers")

    # Generate Scanner
    add_custom_command(
        OUTPUT  "scne_scanner.cpp"
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    --outfile="${BIN_DIR}/scne_scanner.cpp"
                "${SRC_DIR}/codec/scne_scanner.l"
        COMMENT "Generating scne_scanner.cpp")

    # Generate Parser
    add_custom_command(
        OUTPUT  "scne_parser.cpp" "scne_parser.hpp"
        COMMAND ${BISON_EXECUTABLE}
        ARGS    --yacc "${SRC_DIR}/codec/scne_parser.y"
                --defines="${BIN_DIR}/scne_parser.hpp"
                --output="${BIN_DIR}/scne_parser.cpp"
                --name-prefix="scne_yy"
        DEPENDS "${BIN_DIR}/scne_scanner.cpp"
        COMMENT "Generating scne_parser.cpp and scne_parser.hpp")
endif()

# --- Source Files

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

if (GENERATE_PARSER)
    # Bison/Flex Generated Files
    set(SRC_FILES ${SRC_FILES} "${BIN_DIR}/scne_scanner.cpp" "${BIN_DIR}/scne_parser.cpp")
endif()

add_library(pglalgo SHARED ${SRC_FILES})

if (GENERATE_PARSER)
    # Flex Include Directories
    target_include_directories(pglalgo PRIVATE ${FLEX_INCLUDE_DIRS})
    
    # Generated Headers
    target_include_directories(pglalgo PRIVATE ${BIN_DIR})
    
    # Codec Directory
    target_include_directories(pglalgo PRIVATE "${SRC_DIR}/codec")
endif()

# --- Linked Libraries

target_link_libraries(pglalgo pgltool pglmath pglsg)

if (QT5_FOUND)
    target_link_libraries(pglalgo Qt5::Core Qt5::Widgets Qt5::OpenGL)
endif()

if (CGAL_FOUND)
    include_directories(${CGAL_INCLUDE_DIRS})
    target_link_libraries(pglalgo CGAL::CGAL)
    target_link_libraries(pglalgo ${GMP_LIBRARIES})
endif()

target_link_libraries(pglalgo ${ANN_LIBRARIES})
target_link_libraries(pglalgo ${QHULL_LIBRARIES})

if(APPLE)
  target_link_libraries(pglalgo "-framework AGL")
  target_link_libraries(pglalgo "-framework OpenGL")
else(APPLE)
  target_link_libraries(pglalgo OpenGL::GL OpenGL::GLU)

    #if (USE_CONDA_BUILD)
    #if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    #    target_link_libraries(pglalgo ${LIBSELINUX_LIBRARY})
    #endif()
    #endif()


endif(APPLE)

target_link_libraries(pglalgo Boost::system Boost::thread)

# --- Dependencies

add_dependencies(pglalgo pgltool pglmath pglsg)

if (GENERATE_PARSER)
    add_dependencies(pglalgo pglparser)
endif()

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pglalgo PRIVATE ALGO_MAKEDLL _WIN32_WINNT=0x0601)
endif()

# --- Output Library

install(TARGETS pglalgo LIBRARY DESTINATION "lib")
