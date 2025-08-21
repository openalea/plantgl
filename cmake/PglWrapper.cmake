# --- Link with Python
function(pglwrapper_link_python libwrapname)
    if (Python3_FOUND)
        # Prefer the "Module" target (C API for extensions, no libpython needed on 3.13+)
        if (TARGET Python3::Module)
            target_link_libraries(${libwrapname} PRIVATE Python3::Module)
        endif()
        target_include_directories(${libwrapname} PRIVATE ${Python3_INCLUDE_DIRS})

    elseif (Python2_FOUND)
        if (TARGET Python2::Python)
            target_link_libraries(${libwrapname} PRIVATE Python2::Python)
        endif()
        target_include_directories(${libwrapname} PRIVATE ${Python2_INCLUDE_DIRS})

    else()
        message(FATAL_ERROR "Neither Python3 nor Python2 found for target ${libwrapname}")
    endif()
endfunction()

# --- Link with Boost
function(pglwrapper_link_boost libwrapname)
    # Disable Boost auto-linking (important on MSVC)
    target_compile_definitions(${libwrapname} PRIVATE BOOST_ALL_NO_LIB)

    # Core Boost libs
    target_link_libraries(${libwrapname} PRIVATE Boost::system Boost::thread)

    # Python binding library (only if found)
    if (TARGET Boost::${boost_python})
        target_link_libraries(${libwrapname} PRIVATE Boost::${boost_python})
    else()
        message(WARNING "Boost.Python component '${boost_python}' not found, skipping")
    endif()

    # These are safe interface targets
    if (TARGET Boost::dynamic_linking)
        target_link_libraries(${libwrapname} PRIVATE Boost::dynamic_linking)
    endif()
    if (TARGET Boost::disable_autolinking)
        target_link_libraries(${libwrapname} PRIVATE Boost::disable_autolinking)
    endif()
endfunction()

# --- Link with Boost.Numpy (if available)
function(pglwrapper_link_numpy libwrapname)
    if (USE_BOOST_NUMPY AND TARGET Boost::${boost_numpy})
        target_link_libraries(${libwrapname} PRIVATE Boost::${boost_numpy})
    endif()
endfunction()

# --- Install wrapper libraries
function(pglwrapper_install libwrapname repository)
    set_target_properties(${libwrapname} PROPERTIES PREFIX "")

    if (WIN32)
        set_target_properties(${libwrapname} PROPERTIES SUFFIX ".pyd")
    else()
        # Use ".so" on Linux and macOS for Python extension modules
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


