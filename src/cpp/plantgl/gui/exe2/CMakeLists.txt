# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(pglviewer2 ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(pglviewer2 pglgui)
target_link_libraries(pglviewer2 Qt6::Core Qt6::Widgets Qt6::OpenGL)

# --- Dependencies

add_dependencies(pglviewer2 pglgui)

# --- Output Executable

install(TARGETS pglviewer2 RUNTIME DESTINATION "bin")
