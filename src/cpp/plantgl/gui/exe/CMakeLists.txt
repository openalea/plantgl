# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(pglviewer ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(pglviewer pglgui)
target_link_libraries(pglviewer Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL)

# --- Dependencies

add_dependencies(pglviewer pglgui)

# --- Output Executable

install(TARGETS pglviewer RUNTIME DESTINATION "bin")
