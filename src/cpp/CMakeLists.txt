# --- PlantGL Library

add_subdirectory("plantgl")

# --- Install Headers

install(DIRECTORY "plantgl" DESTINATION "include" FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "gui/exe" EXCLUDE)
