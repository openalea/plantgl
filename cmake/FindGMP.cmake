if (GMP_FOUND)
    # Build with GMP
    define_macro(CGAL_USE_GMP 1)
else()
    message(STATUS "Building without GMP - Library not found.")
endif()
