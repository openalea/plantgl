# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(pglmath SHARED ${SRC_FILES})

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pglmath PRIVATE PGLMATH_MAKEDLL)
endif()

# --- Output Library

install_pgllib(pglmath)
