find_package(Qt5Core CONFIG)
find_package(Qt5OpenGL CONFIG)
find_package(Qt5Network CONFIG)
find_package(Qt5Widgets CONFIG)
find_package(Qt5PrintSupport CONFIG)

if (Qt5Core_FOUND AND Qt5OpenGL_FOUND AND Qt5Network_FOUND AND Qt5Widgets_FOUND AND Qt5PrintSupport_FOUND)
    # Build with Qt5
    set(QT5_FOUND ON)
    message(STATUS "Found Qt5: Version ${Qt5Core_VERSION}")

    define_macro(PGL_QT_VERSION 5)
    
    define_py_macro(PGL_WITHOUT_QT "False")
    define_py_macro(PGL_QT_VERSION 5)

    include_directories(${Qt5Core_INCLUDE_DIRS})
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    include_directories(${Qt5Network_INCLUDE_DIRS})
    include_directories(${Qt5OpenGL_INCLUDE_DIRS})
    include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
    
    # X11Extras
    if (UNIX AND NOT APPLE AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        find_package(Qt5X11Extras CONFIG REQUIRED)
        
        if (Qt5X11Extras_FOUND)
            set(USE_QT5_X11EXTRAS ON)
            include_directories(${Qt5X11Extras_INCLUDE_DIRS})
        endif()
    endif()
    
else()
    set(QT5_FOUND OFF)
    message(STATUS "Building without Qt - Library not found.")
    
    define_macro(PGL_WITHOUT_QT 1)
    
    define_py_macro(PGL_WITHOUT_QT "True")
    define_py_macro(PGL_QT_VERSION 5)
endif()
