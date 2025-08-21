# --- Link with Python
function(pglwrapper_link_python libwrapname)
    if (Python3_FOUND)
        # Always prefer the "Module" target (no libpython needed on 3.13+)
        target_link_libraries(${libwrapname} PRIVATE Python3::Module)
        target_include_directories(${libwrapname} PRIVATE ${Python3_INCLUDE_DIRS})

    elseif (Python2_FOUND)
        target_link_libraries(${libwrapname} PRIVATE Python2::Python)
        target_include_directories(${libwrapname} PRIVATE ${Python2_INCLUDE_DIRS})
    else()
        message(FATAL_ERROR "Neither Python3 nor Python2 found for target ${libwrapname}")
    endif()
endfunction()

# --- Link with Boost
function(pglwrapper_link_boost libwrapname)
    # Disable Boost auto-linking (important on MSVC)
    target_compile_definitions(${libwrapname} PRIVATE BOOST_ALL_NO_LIB)

    target_link_libraries(${libwrapname}
        PRIVATE
            Boost::system
            Boost::thread
            Boost::${boost_python}
            Boost::dynamic_linking
            Boost::disable_autolinking
    )
endfunction()

# --- Link with Boost.Numpy (if available)
function(pglwrapper_link_numpy libwrapname)
    if(USE_BOOST_NUMPY)
        target_link_libraries(${libwrapname} PRIVATE Boost::${boost_numpy})
    endif()
endfunction()

# --- Install wrapper libraries
function(pglwrapper_install libwrapname repository)
    set_target_properties(${libwrapname} PROPERTIES PREFIX "")

    if (WIN32)
        set_target_properties(${libwrapname} PROPERTIES SUFFIX ".pyd")
    elseif (APPLE)
        # Use ".so" for Python extension modules even on macOS
        set_target_properties(${libwrapname} PROPERTIES SUFFIX ".so")
    endif()

    install(
        TARGETS ${libwrapname}
        DESTINATION "${CMAKE_SOURCE_DIR}/src/openalea/plantgl/${repository}"
    )
endfunction()

# --- Install shared resources
function(install_share sharedirectory project)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${sharedirectory}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/${project}"
    )
endfunction()

