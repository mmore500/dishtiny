#pragma once

#include <string>

#include "../../../third-party/Empirical/include/emp/config/config.hpp"

EMP_BUILD_CONFIG(
  Config,

  GROUP(EXECUTION, "EXECUTION"),
  VALUE(N_THREADS, size_t, 1, "How many threads should each process run with?"),
  VALUE(N_NODES_PER_CPU, size_t, 2048, "How many nodes should we simulate per CPU?"),
  VALUE(RUN_SECONDS, double, 5,
    "How many seconds should we run the experiment for?"
  ),
  VALUE(ASYNCHRONOUS, size_t, 1,
    "Should updates occur asynchronously across threads and processes?"
    "0: no, "
    "1: yes, "
    "2: yes and disable interthread, interproccess communication, "
  ),


  GROUP(EXPERIMENT, "EXPERIMENT"),
  VALUE(N_CHANNELS, size_t, 3, "Number of node colors available"),
  VALUE(B, double, 0.1, "Node channel stickiness"),

)
