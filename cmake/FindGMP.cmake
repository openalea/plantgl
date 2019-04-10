if (GMP_FOUND)
    # Build with GMP
    add_compile_definitions(WITH_GMP=1)
else()
    message(WARNING "Building without GMP - library not found.")
endif()
