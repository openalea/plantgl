set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

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

    include_directories(${Qt5Core_INCLUDE_DIRS})
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    include_directories(${Qt5Network_INCLUDE_DIRS})
    include_directories(${Qt5OpenGL_INCLUDE_DIRS})
    include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
    
else()
    set(QT5_FOUND OFF)
    message(STATUS "Building without Qt - Library not found.")
    
    define_macro(PGL_WITHOUT_QT 1)
endif()
