# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(pglviewer SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(pglviewer pglgui)
target_link_libraries(pglviewer Qt5::Core Qt5::Widgets Qt5::OpenGL)

# --- Dependencies

add_dependencies(pglviewer pglgui)

# --- Output Executable

install(TARGETS pglviewer RUNTIME DESTINATION "bin")
