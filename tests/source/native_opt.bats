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
  ./rundishtiny --RUN_SECONDS 10
}

function single_thread_is_deterministic { #@test
  run mkdir first; cp rundishtiny first; cd first; ./rundishtiny --RUN_UPDATES 1024 --N_THREADS 1; cd ..
  run mkdir second;  cp rundishtiny second; cd second; ./rundishtiny --RUN_UPDATES 1024 --N_THREADS 1; cd ..
  run diff first/ second/
  [ "$status" -eq 0 ]
}
