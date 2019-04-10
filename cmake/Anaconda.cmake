# Anaconda Check
if (NOT DEFINED ENV{CONDA_PREFIX})
    message(FATAL_ERROR "Your are not in an Anaconda environment.")
endif()

# Anaconda Environment
if (WIN32)
    set(CONDA_ENV "$ENV{CONDA_PREFIX}/Library")
else()
    set(CONDA_ENV "$ENV{CONDA_PREFIX}")
endif()
