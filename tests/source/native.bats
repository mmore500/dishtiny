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
  rm -f *.csv
}

function invoking_dishtiny_exits_success { #@test
  ./rundishtiny --RUN_SECONDS 10
}
