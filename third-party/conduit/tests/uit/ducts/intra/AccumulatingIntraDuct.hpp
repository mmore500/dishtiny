TEST_CASE("Validity " IMPL_NAME) { REPEAT {

  netuit::Mesh<Spec> mesh{ netuit::RingTopologyFactory{}(num_nodes) };

  std::deque<int> sums;

  // 1/2 n * (n + 1)
  const int expected_sum = (std::kilo::num - 1) * std::kilo::num / 2;

  for (auto & node : mesh.GetSubmesh()) {

    sums.emplace_back();

    for (MSG_T msg = 0; msg < std::kilo::num; ++msg) {

      node.GetOutput(0).TryPut(msg);

      const MSG_T received = node.GetInput(0).JumpGet();
      REQUIRE( received <= expected_sum );
      REQUIRE( received >= 0 );
      sums.back() += received;

    }

  }

  for (auto & node : mesh.GetSubmesh()) {

    while (sums.front() != expected_sum) {
      const MSG_T received = node.GetInput(0).JumpGet();
      REQUIRE( received <= expected_sum );
      REQUIRE( received >= 0);
      sums.front() += received;
    }

    REQUIRE( sums.front() == expected_sum );

    for (size_t i = 0; i < 10 * std::kilo::num; ++i) {
      REQUIRE( node.GetInput(0).JumpGet() == 0 );
    }

    sums.pop_front();

  }

} }
