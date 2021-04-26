#define STD_IMPL_NAME IMPL_NAME " SteppingThreadDuct"

TEMPLATE_TEST_CASE("Ring Mesh connectivity " STD_IMPL_NAME, "[nproc:1]", two_thread, three_thread) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::RingTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // check that everyone's connected properly
    output.TryPut(uitsl::get_rank());

    REQUIRE( input.GetNext() == uitsl::safe_cast<MSG_T>(
      uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
    ) );

  } THREADED_END

} }

TEMPLATE_TEST_CASE("Ring Mesh sequential consistency " STD_IMPL_NAME, "[nproc:1]", two_thread, three_thread) {
  std::barrier<> barrier(uitsl::safe_cast<std::ptrdiff_t>(TestType::value));

  REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::RingTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {

      barrier.arrive_and_wait();

      output.Put(i);
      REQUIRE(input.GetNext() == i);

    }

  } THREADED_END

} }

TEMPLATE_TEST_CASE("Producer-Consumer Mesh connectivity " STD_IMPL_NAME, "[nproc:1]", two_thread, three_thread) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::RingTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

    // check that everyone's connected properly
    if (output) output->Put(uitsl::get_rank());

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

  } THREADED_END

} }

TEMPLATE_TEST_CASE("Producer-Consumer Mesh sequential consistency " STD_IMPL_NAME, "[nproc:1]", two_thread, three_thread) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::RingTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {

      if (output) output->Put(i);
      if (input) REQUIRE( input->GetNext() == i );

    }

  } THREADED_END

} }

TEMPLATE_TEST_CASE("Dyadic Mesh connectivity " STD_IMPL_NAME, "[nproc:1]", two_thread, three_thread) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::DyadicTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // check that everyone's connected properly
    output.Put(uitsl::get_rank());

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

  } THREADED_END

} }

TEMPLATE_TEST_CASE("Dyadic Mesh sequential consistency " STD_IMPL_NAME, "[nproc:1]", two_thread, three_thread) {
  std::barrier<> barrier(uitsl::safe_cast<std::ptrdiff_t>(TestType::value));

  REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::DyadicTopologyFactory{}(TestType::value),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {

      barrier.arrive_and_wait();

      output.Put(i);
      REQUIRE( input.GetNext() == i );

    }

  } THREADED_END

} }
