# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(pglsg SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(pglsg pgltool pglmath)

if (QT5_FOUND)
    target_link_libraries(pglsg Qt5::Core)
endif()

# --- Dependencies

add_dependencies(pglsg pgltool pglmath)

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pglsg PRIVATE SG_MAKEDLL)
endif()

# --- Output Library

install(TARGETS pglsg LIBRARY DESTINATION "lib")
