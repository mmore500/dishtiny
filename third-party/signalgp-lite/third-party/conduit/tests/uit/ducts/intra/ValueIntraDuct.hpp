TEST_CASE("Test IntraDuct Validity " IMPL_NAME) {

  netuit::Mesh<Spec> mesh{ netuit::RingTopologyFactory{}(num_nodes) };

  std::unordered_map<size_t, MSG_T> last;
  for (MSG_T msg = 0; msg < 10 * std::kilo::num; ++msg) {
    for (auto & node : mesh.GetSubmesh()) {
      node.GetOutput(0).TryPut(msg);
      const MSG_T current = node.GetInput(0).JumpGet();
      REQUIRE( current >= 0 );
      REQUIRE( current < 10 * std::kilo::num );
      REQUIRE( last[node.GetNodeID()] <= current );
      last[node.GetNodeID()] = current;
    }
  }

  for (size_t i = 0; i < 10 * std::kilo::num; ++i) {
    for (auto & node : mesh.GetSubmesh()) {
      REQUIRE( node.GetInput(0).JumpGet() >= 0 );
      REQUIRE( node.GetInput(0).JumpGet() == node.GetInput(0).JumpGet() );
    }
  }

}

TEST_CASE("ValueIntraDuct unmatched puts " IMPL_NAME) { REPEAT {

  netuit::Mesh<Spec> mesh{ netuit::DyadicTopologyFactory{}(num_nodes) };

  for (auto & node : mesh.GetSubmesh()) {
    for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) node.GetOutput(0).TryPut(1);
  }

} }
