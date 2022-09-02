

function(detect_plantgl_version pgldir)
    message(STATUS ${pgldir}/plantgl/version.h)
    file(STRINGS "${pgldir}/plantgl/version.h" PGL_VERSION
      REGEX "#define[ ]+PGL_VERSION[ ]+0x[0-9A-F]+")
    string(REGEX MATCH "0x[0-9A-F]+" PGL_VERSION ${PGL_VERSION})
    set(PGL_VERSION ${PGL_VERSION} PARENT_SCOPE)

    if (PGL_VERSION)
      math(EXPR PGL_VERSION_MAJOR "(${PGL_VERSION}  & 0xff0000) >> 16")
      math(EXPR PGL_VERSION_MINOR "(${PGL_VERSION}  & 0x00ff00) >> 8")
      math(EXPR PGL_VERSION_SUBMINOR "${PGL_VERSION} & 0x0000ff")
      set(PGL_VERSION_STR "${PGL_VERSION_MAJOR}.${PGL_VERSION_MINOR}.${PGL_VERSION_SUBMINOR}" PARENT_SCOPE)
    endif()

endfunction()