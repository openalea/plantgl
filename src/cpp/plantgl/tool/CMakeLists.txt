# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(pgltool SHARED ${SRC_FILES})

# --- Linked Libraries

if (QT5_FOUND)
    target_link_libraries(pgltool Qt5::Core)
endif()

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pgltool PUBLIC TOOLS_MAKEDLL)
endif()

# --- Output Library

install(TARGETS pgltool LIBRARY DESTINATION "lib")
