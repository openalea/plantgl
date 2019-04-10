find_package(Python3 COMPONENTS Development)

if (Python3_FOUND)
    if (Python3_VERSION_MINOR LESS 6)
        message(SEND_ERROR "Python 3.6 or greater is required.")
    else()
        include_directories(${Python3_INCLUDE_DIRS})
    endif()

    add_compile_definitions(WITH_PYTHON=1)

    set(BOOST_PY_LIB "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
else()
    message("Building without Python - library not found.")
endif()
