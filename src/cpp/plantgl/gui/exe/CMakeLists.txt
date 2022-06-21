# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(pglviewer ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(pglviewer pglgui)
target_link_libraries(pglviewer Qt6::Core Qt6::Widgets Qt6::OpenGL)

# --- Dependencies

add_dependencies(pglviewer pglgui)

# --- Output Executable

install(TARGETS pglviewer RUNTIME DESTINATION "bin")
