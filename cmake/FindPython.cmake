if (DEFINED ENV{PY3K} AND "$ENV{PY3K}" STREQUAL "0")
    # Python 2
    find_package(Python2 COMPONENTS Development)
else()
    # Python 3
    find_package(Python3 COMPONENTS Development)
    
    if (NOT Python3_FOUND)
        # If Python 3 is not found, we try to find Python 2
        find_package(Python2 COMPONENTS Development)
    endif()
endif()

if (Python3_FOUND OR Python2_FOUND)
    set(Python_FOUND ON)
else()
    set(Python_FOUND OFF)
endif()

if (Python3_FOUND)
    if (Python3_VERSION_MINOR LESS 6)
        message(SEND_ERROR "Python 3.6 or greater is required.")
    else()
        include_directories(${Python3_INCLUDE_DIRS})
    endif()

    define_macro(WITH_PYTHON 1)

    set(BOOST_PYTHON_LIB "python3${Python3_VERSION_MINOR}")
    set(BOOST_NUMPY_LIB "numpy3${Python3_VERSION_MINOR}")
    
elseif (Python2_FOUND)
    if (Python2_VERSION_MINOR LESS 7)
        message(SEND_ERROR "Python 2.7 or greater is required.")
    else()
        include_directories(${Python2_INCLUDE_DIRS})
    endif()

    define_macro(WITH_PYTHON 1)

    set(BOOST_PYTHON_LIB "python2${Python2_VERSION_MINOR}")
    set(BOOST_NUMPY_LIB "numpy2${Python2_VERSION_MINOR}")

else()
    message(STATUS "Building without Python - Library not found.")
endif()
