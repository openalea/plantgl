# --- Bison and Flex

if (BISON_FOUND AND FLEX_FOUND)
    add_custom_target(
        pglparser
        DEPENDS "scne_scanner.cpp" "scne_parser.cpp" "scne_parser.hpp"
        COMMENT "Generating parsers...")

    set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})

    # Generate Scanner
    add_custom_command(
        OUTPUT  "scne_scanner.cpp"
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    --outfile="${BIN_DIR}/scne_scanner.cpp" "${SRC_DIR}/codec/scne_scanner.l"
        COMMENT "Generating scne_scanner.cpp...")

    # Generate Parser
    add_custom_command(
        OUTPUT  "scne_parser.cpp" "scne_parser.hpp"
        COMMAND ${BISON_EXECUTABLE}
        ARGS    --yacc "${SRC_DIR}/codec/scne_parser.y"
                --defines="${BIN_DIR}/scne_parser.hpp"
                --output="${BIN_DIR}/scne_parser.cpp"
                --name-prefix="scne_yy"
        DEPENDS "${BIN_DIR}/scne_scanner.cpp"
        COMMENT "Generating scne_parser.cpp and scne_parser.hpp...")
            
    # Install Generated Header File
    install(FILES "${BIN_DIR}/scne_parser.hpp" DESTINATION "include/plantgl/algo/codec")
endif()

# --- Source Files

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

if (BISON_FOUND AND FLEX_FOUND)
    # Bison/Flex Generated Files
    set(SRC_FILES ${SRC_FILES} "${BIN_DIR}/scne_scanner.cpp" "${BIN_DIR}/scne_parser.cpp")
endif()

add_library(pglalgo SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(pglalgo pgltool pglmath pglsg)

if (QT5_FOUND)
    target_link_libraries(pglalgo Qt5::Core Qt5::Widgets Qt5::OpenGL)
endif()

if (USE_BOOST)
    target_link_libraries(pglalgo Boost::system Boost::thread "Boost::${BOOST_PYTHON_LIB}")
endif()

if (CGAL_FOUND)
    target_link_libraries(pglalgo CGAL::CGAL)
endif()

target_link_libraries(pglalgo ${ANN_LIBRARIES})
target_link_libraries(pglalgo ${GMP_LIBRARIES})
target_link_libraries(pglalgo ${QHULL_LIBRARIES})

# --- Dependencies

add_dependencies(pglalgo pgltool pglmath pglsg pglparser)

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pglalgo PUBLIC ALGO_MAKEDLL)
endif()

# --- Output Library

install(TARGETS pglalgo LIBRARY DESTINATION "lib")
