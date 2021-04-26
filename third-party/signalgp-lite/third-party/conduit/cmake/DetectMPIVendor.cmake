if (${MPI_CXX_LIBRARY_VERSION_STRING} MATCHES "Open MPI")
    message("Using Open MPI")
    set(UIT_HAS_OPENMPI TRUE)
endif()
