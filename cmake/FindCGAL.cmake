if (CGAL_FOUND)
    # Build with CGAL
    include(${CGAL_USE_FILE})
    add_compile_definitions(WITH_CGAL=1)
else()
    message(WARNING "Building without CGAL - library not found.")
endif()
