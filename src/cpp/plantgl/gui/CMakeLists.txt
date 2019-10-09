# --- Qt UI Files

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE UI_FILES_BASE "${CMAKE_CURRENT_SOURCE_DIR}/base/*.ui")
file(GLOB_RECURSE UI_FILES_VIEWER "${CMAKE_CURRENT_SOURCE_DIR}/viewer/*.ui")

# --- Source Files

file(GLOB_RECURSE SRC_FILES_BASE "${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp")
file(GLOB_RECURSE SRC_FILES_VIEWER "${CMAKE_CURRENT_SOURCE_DIR}/viewer/*.cpp")

add_library(pglgui SHARED ${UI_FILES_BASE} ${SRC_FILES_BASE} ${UI_FILES_VIEWER} ${SRC_FILES_VIEWER})

# --- Linked Libraries

target_link_libraries(pglgui pgltool pglmath pglsg pglalgo)
target_link_libraries(pglgui Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Network Qt5::PrintSupport)

if (USE_QT5_X11EXTRAS)
    target_link_libraries(pglgui Qt5::X11Extras)
endif()

# --- Dependencies

add_dependencies(pglgui pgltool pglmath pglsg pglalgo)

# --- Include Directory

target_include_directories(pglgui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# --- Preprocessor

if (WIN32)
    # Export DLL on Windows
    target_compile_definitions(pglgui PRIVATE VIEW_MAKEDLL)
endif()

# --- Output Library

install(TARGETS pglgui LIBRARY DESTINATION "lib")

# --- Viewer Program

add_subdirectory("exe")
