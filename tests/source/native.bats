function setup_file {
  TO_ROOT=$(git rev-parse --show-cdup)
  cd "${TO_ROOT}"
  make clean && make ${TEST_MODE}
  cd -
}

function teardown_file {
  rm -f dishtiny
}

function setup {
  TO_ROOT=$(git rev-parse --show-cdup)
  cp "${TO_ROOT}/dishtiny" .
}

function teardown {
  rm -f *.csv
}

function invoking_dishtiny_exits_success { #@test
  ./dishtiny --RUN_SECONDS 10
}
