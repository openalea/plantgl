file(GLOB_RECURSE SRC_FILES_BASE ${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp)
file(GLOB_RECURSE SRC_FILES_VIEWER ${CMAKE_CURRENT_SOURCE_DIR}/viewer/*.cpp)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/interface)

add_custom_target(pgl_ui echo "Generating ui file")

file(GLOB_RECURSE ui_files ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
foreach (it ${ui_files})
    get_filename_component(outfile ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)
    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/interface/${outfile}.h)
    add_custom_command(TARGET pgl_ui
            COMMAND ${QT_UIC_EXECUTABLE}
            ARGS -o ${outfile} ${infile}
            MAIN_DEPENDENCY ${infile} VERBATIM)
    set_property(SOURCE ${outfile} PROPERTY SKIP_AUTOMOC TRUE)  # don't run automoc on this file
    set_property(SOURCE ${outfile} PROPERTY SKIP_AUTOUIC TRUE)  # don't run autouic on this file
    set(${outfiles} ${${outfiles}} ${outfile})
endforeach ()

find_package(Qt4)
SET(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(pglgui SHARED ${SRC_FILES_BASE} ${SRC_FILES_VIEWER})
target_link_libraries(pglgui ${PGL_LIBRARIES} pgltool pglmath pglsg pglalgo)
target_compile_definitions(pglgui PRIVATE ${PGL_DEFINE_VARIABLES} VIEW_MAKEDLL)
add_dependencies(pglgui pgltool pglmath pglsg pglalgo pgl_ui)

install(TARGETS pglgui
        LIBRARY DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/interface/
        DESTINATION include/plantgl/gui
        FILES_MATCHING PATTERN "*.h")

add_subdirectory(exe)
