file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package(Qt4)
SET(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(pglviewer ${SRC_FILES})
target_link_libraries(pglviewer ${PGL_LIBRARIES} pglgui pglalgo pgltool pglmath pglsg)
add_dependencies(pglviewer pglgui)

install(TARGETS pglviewer
        RUNTIME DESTINATION bin)
