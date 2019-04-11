if (GMP_FOUND)
    # Build with GMP
    add_compile_definitions(CGAL_USE_GMP=1)
else()
    message(STATUS "Building without GMP - Library not found.")
endif()
