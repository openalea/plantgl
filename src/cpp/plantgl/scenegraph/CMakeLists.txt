# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(pglsg SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(pglsg pgltool pglmath)

if (Qt6_FOUND)
    target_link_libraries(pglsg Qt6::Core)
endif()

target_link_libraries(pglsg PNG::PNG JPEG::JPEG)

# --- Dependencies

add_dependencies(pglsg pgltool pglmath)

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pglsg PRIVATE SG_MAKEDLL)
endif()

# --- Output Library

install(TARGETS pglsg LIBRARY DESTINATION "lib")
