TEST_CASE("Test Sequential Completeness " IMPL_NAME) {

  netuit::Mesh<Spec> mesh{ netuit::RingTopologyFactory{}(num_nodes) };

  emp::vector<int> sizes;

  for (auto & node : mesh.GetSubmesh()) {
    int i{};
    while ( i < std::kilo::num && node.GetOutput(0).TryPut( ++i ));
    sizes.push_back(i);
  }

  if ( uitsl::is_root() ) {
    REQUIRE( std::set<int>(std::begin(sizes), std::end(sizes)).size() == 1 );
  }

  for (auto & node : mesh.GetSubmesh()) {
    for (int j = 1; j < sizes.front(); ++j) {
      REQUIRE( node.GetInput(0).GetNext() == j );
    }
  }

}
