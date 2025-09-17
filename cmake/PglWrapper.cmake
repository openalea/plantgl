

function(pglwrapper_link_python libwrapname)
        if (APPLE)
            target_link_libraries(${libwrapname} "-undefined dynamic_lookup")
        else()
            target_link_libraries(${libwrapname} Python::Python)
        endif()
endfunction()

function(pglwrapper_link_boost libwrapname)
    # Disable Boost Auto-Link
    target_compile_definitions(${libwrapname} PRIVATE BOOST_ALL_NO_LIB)

    target_link_libraries(${libwrapname} Boost::system Boost::thread Boost::${boost_python} Boost::dynamic_linking Boost::disable_autolinking)

endfunction()

function(pglwrapper_link_numpy libwrapname)
    if(USE_BOOST_NUMPY)
        target_link_libraries(${libwrapname} Boost::${boost_numpy})
    endif()
endfunction()


function(pglwrapper_install libwrapname repository)
    set_target_properties(${libwrapname} PROPERTIES PREFIX "")

    if (WIN32)
        set_target_properties(${libwrapname} PROPERTIES SUFFIX ".pyd")
    endif()

    if (APPLE)
        set_target_properties(${libwrapname} PROPERTIES SUFFIX ".so")
    endif()
    message("Installing ${libwrapname} in ${CMAKE_SOURCE_DIR}/src/openalea/plantgl/${repository}")
    install(TARGETS ${libwrapname} DESTINATION "${CMAKE_SOURCE_DIR}/src/openalea/plantgl/${repository}")
endfunction()

