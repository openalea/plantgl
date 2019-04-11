# --- Source Files

file(GLOB_RECURSE SRC_FILES_BASE "${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp")
file(GLOB_RECURSE SRC_FILES_VIEWER "${CMAKE_CURRENT_SOURCE_DIR}/viewer/*.cpp")

add_library(pglgui SHARED ${SRC_FILES_BASE} ${SRC_FILES_VIEWER})

# --- Linked Libraries

target_link_libraries(pglgui pgltool pglmath pglsg pglalgo)
target_link_libraries(pglgui Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Network Qt5::PrintSupport)

# --- Dependencies

add_dependencies(pglgui pgltool pglmath pglsg pglalgo)

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pglgui PUBLIC VIEW_MAKEDLL)
endif()

# --- Output Library

install(TARGETS pglgui LIBRARY DESTINATION "lib")

# --- Viewer Program

add_subdirectory("exe")
