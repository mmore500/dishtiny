#ifndef TAGS
#define TAGS ""
#endif

TEST_CASE("Ring Mesh connectivity " IMPL_NAME, TAGS) { REPEAT {

  auto [input, output] = make_ring_pd_bundle<Spec>();

  // check that everyone's connected properly
  output.TryPut(uitsl::get_rank());
  output.TryFlush();

  REQUIRE( input.GetNext() == uitsl::safe_cast<MSG_T>(
    uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
  ) );

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Ring Mesh sequential consistency " IMPL_NAME, TAGS) { {

  auto [input, output] = make_ring_pd_bundle<Spec>();

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {

    UITSL_Barrier( MPI_COMM_WORLD );
    output.Put(i);
    output.Flush();
    REQUIRE(input.GetNext() == i);

  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Producer-Consumer Mesh connectivity " IMPL_NAME, TAGS) { REPEAT {

  auto [input, output] = make_producer_consumer_pd_bundle<Spec>();

  // check that everyone's connected properly
  if (output) {
    output->Put(uitsl::get_rank());
    output->Flush();
  }

  // did we get expected rank ID as message?
  if (uitsl::get_nprocs() % 2 && uitsl::get_rank() + 1 == uitsl::get_nprocs()) {
    // is odd process loop at end
    REQUIRE( input->GetNext() == uitsl::get_rank());
  } else if (input) {
    // is consumer
    REQUIRE(
      input->GetNext() == uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
      )
    );
  } else REQUIRE( uitsl::get_rank() % 2 == 0 ); // is producer

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Producer-Consumer Mesh sequential consistency " IMPL_NAME, TAGS) { {

  auto [input, output] = make_producer_consumer_pd_bundle<Spec>();

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {

    UITSL_Barrier( MPI_COMM_WORLD );
    if (output) {
      output->Put(i);
      output->Flush();
    }
    if (input) REQUIRE( input->GetNext() == i );

  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Dyadic Mesh connectivity " IMPL_NAME, TAGS) { REPEAT {

  auto [input, output] = make_dyadic_pd_bundle<Spec>();
  UITSL_Barrier(MPI_COMM_WORLD);

  // check that everyone's connected properly
  output.Put(uitsl::get_rank());
  output.Flush();

  // did we get expected rank ID as message?
  if (uitsl::get_nprocs() % 2 && uitsl::get_rank() + 1 == uitsl::get_nprocs()) {
    // is connected to self (is odd process loop at end)
    REQUIRE( input.GetNext() == uitsl::get_rank() );
  } else {
    // is connected to neighbor
    REQUIRE( input.GetNext() == uitsl::safe_cast<int>(
      uitsl::circular_index(
        uitsl::get_rank(),
        uitsl::get_nprocs(),
        // is pointing forwards or backwards
        (uitsl::get_rank() % 2) ? -1 : 1
      )
    ) );
  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Dyadic Mesh sequential consistency " IMPL_NAME, TAGS) { {

  auto [input, output] = make_dyadic_pd_bundle<Spec>();

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {

    UITSL_Barrier( MPI_COMM_WORLD );
    output.Put(i);
    output.Flush();
    REQUIRE( input.GetNext() == i );

  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }
