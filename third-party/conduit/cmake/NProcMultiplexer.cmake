include("../cmake/CatchInt.cmake")
include("../cmake/DetectMPIVendor.cmake")

if (NOT DEFINED UIT_NPROC_MIN)
    set(UIT_NPROC_MIN 1)
endif()

if (NOT DEFINED UIT_NPROC_MAX)
    set(UIT_NPROC_MAX 8)
endif()

function(nproc_multiplex_tests TARGET)
    foreach(nproc RANGE ${UIT_NPROC_MIN} ${UIT_NPROC_MAX})
        if (UIT_HAS_OPENMPI)
            set(UIT_MPI_FLAGS "--oversubscribe" "--quiet" "--mca" "btl_base_warn_component_unused" "0")
        else()
            set(UIT_MPI_FLAGS "")
        endif()
        set(OptionalCatchTestLauncher ${MPIEXEC_EXECUTABLE} ${UIT_MPI_FLAGS} ${MPIEXEC_NUMPROC_FLAG} ${nproc} ${MPIEXEC_PREFLAGS})
        set(_EXTRA_ARGS ${MPIEXEC_POSTFLAGS})

        catch_discover_tests(${TARGET}
            TEST_SPEC "[nproc:${nproc}]"
            TEST_SUFFIX "-nproc${nproc}"
        )

        catch_discover_tests(${TARGET}
            TEST_SPEC "~[nproc:1]~[nproc:2]~[nproc:3]~[nproc:4]~[nproc:5]~[nproc:6]~[nproc:7]~[nproc:8]"
            TEST_SUFFIX "-nproc${nproc}"
        )

        unset(OptionalCatchTestLauncher)
    endforeach(nproc)

endfunction()
