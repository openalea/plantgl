set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG)
find_package(Qt5OpenGL CONFIG)
find_package(Qt5Network CONFIG)
find_package(Qt5Widgets CONFIG)

if (Qt5Core_FOUND AND Qt5OpenGL_FOUND AND Qt5Network_FOUND AND Qt5Widgets_FOUND)
    # Build with Qt5
    set(QT5_FOUND ON)
    message(STATUS "Found Qt5: Qt5Core Qt5OpenGL Qt5Network Qt5Widgets (found version \"${Qt5Core_VERSION}\")")
    
    add_compile_definitions(WITHOUT_QT=0)
    
    include_directories(${Qt5Core_INCLUDE_DIRS})
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    include_directories(${Qt5Network_INCLUDE_DIRS})
    include_directories(${Qt5OpenGL_INCLUDE_DIRS})
    
else()
    set(QT5_FOUND OFF)
    message(WARNING "Building without Qt - library not found.")
    
    add_compile_definitions(WITHOUT_QT=1)
endif()
