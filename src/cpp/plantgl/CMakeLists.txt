# --- Projects List

add_subdirectory("tool")
add_subdirectory("math")
add_subdirectory("scenegraph")
add_subdirectory("algo")

if (Qt${QT_VERSION_MAJOR}_FOUND)
    add_subdirectory("gui")
endif()
