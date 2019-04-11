find_package(Python3 COMPONENTS Development)

if (Python3_FOUND)
    if (Python3_VERSION_MINOR LESS 6)
        message(SEND_ERROR "Python 3.6 or greater is required.")
    else()
        include_directories(${Python3_INCLUDE_DIRS})
    endif()

    define_macro(WITH_PYTHON 1)

    set(BOOST_PYTHON_LIB "python3${Python3_VERSION_MINOR}")
else()
    message(STATUS "Building without Python - Library not found.")
endif()
