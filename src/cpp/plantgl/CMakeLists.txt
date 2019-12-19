# --- Projects List

add_subdirectory("tool")
add_subdirectory("math")
add_subdirectory("scenegraph")
add_subdirectory("algo")

if (QT5_FOUND)
    add_subdirectory("gui")
endif()
