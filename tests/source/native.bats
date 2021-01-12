function setup_file {
  TO_ROOT=$(git rev-parse --show-cdup)
  cd "${TO_ROOT}"
  make clean && make ${TEST_MODE}
  cd -
}

function teardown_file {
  rm -f rundishtiny
}

function setup {
  TO_ROOT=$(git rev-parse --show-cdup)
  cp "${TO_ROOT}/rundishtiny" .
}

function teardown {
  make clean
}

function invoking_dishtiny_exits_success { #@test
  "${DISH_MPIEXEC:-mpiexec.mpich}" -n 1 ./rundishtiny --RUN_SECONDS 10 --N_CELLS 100
}

function invoking_dishtiny_multithread_exits_success { #@test
  "${DISH_MPIEXEC:-mpiexec.mpich}" -n 1 ./rundishtiny --RUN_SECONDS 10 --N_THREADS 2 --N_CELLS 100
}

function invoking_dishtiny_multiproc_exits_success { #@test
  "${DISH_MPIEXEC:-mpiexec.mpich}" -n 2 ./rundishtiny --RUN_SECONDS 10 --N_CELLS 100
}

function invoking_dishtiny_multiproc_multithread_exits_success { #@test
  "${DISH_MPIEXEC:-mpiexec.mpich}" -n 2 ./rundishtiny --RUN_SECONDS 10 --N_THREADS 2 --N_CELLS 100
}

function single_thread_is_deterministic { #@test
  run mkdir first; cp rundishtiny first; cd first; "${DISH_MPIEXEC:-mpiexec.mpich}" -n 1 ./rundishtiny --RUN_UPDATES 64 --N_THREADS 1 --N_CELLS 100; cd ..
  run mkdir second; cp rundishtiny second; cd second; "${DISH_MPIEXEC:-mpiexec.mpich}" -n 1 ./rundishtiny --RUN_UPDATES 64 --N_THREADS 1 --N_CELLS 100; cd ..
  run diff first/ second/
  [ "$status" -eq 0 ]
}
