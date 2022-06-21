# --- Source Files

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(_pglgui SHARED ${SRC_FILES})

# --- Linked Libraries

target_link_libraries(_pglgui pglmath pglgui pglsg pgltool)

if (Qt6_FOUND)
    target_link_libraries(_pglgui Qt6::Core Qt6::Widgets Qt6::OpenGL Qt6::Network)
endif()

pglwrapper_link_boost(_pglgui)

pglwrapper_link_python(_pglgui)

if(APPLE)
  target_link_libraries(_pglgui "-framework AGL")
endif()

# --- Dependencies

add_dependencies(_pglgui pglmath pglgui pglsg pgltool)

# --- Output Library

pglwrapper_install(_pglgui gui)

