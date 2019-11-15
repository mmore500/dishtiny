// This is the main function for the NATIVE version of this project.

#include "mpi.h"

#include "tools/Random.h"
#include "config/command_line.h"
#include "config/ArgManager.h"

#include "../Config.h"
#include "../DishWorld.h"
#ifndef NDATA
#include "../DataHelper.h"
#endif

#include "../Config.cpp"
#include "../DishWorld.cpp"
#include "../FrameHardware.cpp"
#include "../FrameCell.cpp"
#include "../Manager.cpp"
#include "../LibraryInstruction.cpp"


int main(int argc, char* argv[])
{

  MPI_Init(&argc, &argv);

  // only root process prints source & config info
  if (int procno; MPI_Comm_rank(MPI_COMM_WORLD, &procno), !procno) {

    std::cout << "SOURCE HASH " << STRINGIFY(DISHTINY_HASH_) << std::endl;
    std::cout << "EMP HASH " << STRINGIFY(EMPIRICAL_HASH_) << std::endl;
    emp::Random temp(1); // need this to prevent a memory leak
    std::cout << "MATCHBIN "
      << Config::hardware_t(nullptr, nullptr, &temp).GetMatchBin().name()
      << std::endl;

    Config cfg;
    cfg.LoadFromFile();
    const auto specs = emp::ArgManager::make_builtin_specs(&cfg);
    emp::ArgManager am(argc, argv, specs);
    am.UseCallbacks();
    if (am.HasUnused()) std::exit(EXIT_FAILURE);

    std::cout << "==============================" << std::endl;
    std::cout << "|    How am I configured?    |" << std::endl;
    std::cout << "==============================" << std::endl;
    cfg.WriteMe(std::cout);
    std::cout << "==============================\n" << std::endl;

  }

  // sync all processes up
  MPI_Barrier(MPI_COMM_WORLD);

  // make a square grid
  MPI_Comm comm_grid;
  const int ndim = 2;
  const int nprocs = [&](){
    int res;
    MPI_Comm_size(
      MPI_COMM_WORLD,
      &res
    );
    return res;
  }();
  const emp::array<int, ndim> dimension{{
    static_cast<int>(std::sqrt(nprocs)),
    static_cast<int>(std::sqrt(nprocs))
  }};
  const emp::array<int, ndim> periodic{{1, 1}}; // toroidal wraparound

  MPI_Cart_create(
    MPI_COMM_WORLD,
    ndim,
    dimension.data(),
    periodic.data(),
    1, // allow reordering of world ranks
    &comm_grid
  );

  // comm world info
  const int w_rank = [&](){
    int res;
    MPI_Comm_rank(
      MPI_COMM_WORLD,
      &res
    );
    return res;
  }();

  // comm grid info
  const emp::array<int, ndim> coords = [&](){
    emp::array<int, ndim> res;
    MPI_Cart_coords(
      comm_grid,
      w_rank,
      ndim,
      res.data()
    );
    return res;
  }();
  const int g_rank = [&](){
    int res;
    MPI_Cart_rank(
      comm_grid,
      coords.data(),
      &res
    );
    return res;
  }();

  // calculate neighboring ranks
  const emp::array<int, Cardi::Dir::NumDirs> neighs = [&](){
    emp::array<int, Cardi::Dir::NumDirs> res;
    for (size_t i = 0; i < Cardi::Dir::NumDirs; ++i) {
      MPI_Cart_shift(
        comm_grid,
        Cardi::Dim[i],
        Cardi::Dd[i],
        const_cast<int *>(&g_rank),
        &res[i]
      );

    }
    return res;
  }();

  // GOAL:
  // everyone broadcasts a unique character to each cartesian neighbor
  // send 1 byte North, 2 bytes South, 3 bytes East, and 4 bytes West
  const unsigned char id = 65 + g_rank;

  // prepare for sends
  const emp::vector<unsigned char> send_data(Cardi::Dir::NumDirs, id);
  emp::array<MPI_Request, Cardi::Dir::NumDirs> send_requests;

  // post non-blocking send requests to each neighbor
  for (size_t i = 0; i < Cardi::Dir::NumDirs; ++i) {
    MPI_Isend(
      send_data.data(),
      i + 1, // message length
      MPI_BYTE,
      neighs[i],
      i, // tag represents outgoing direction
      comm_grid,
      &send_requests[i]
    );
  }

  // whee!
  // DO COMPUTE WORK HERE
  // while messages are en route

  // incoming direction -> result
  std::unordered_map<int, emp::vector<unsigned char>> received;

  // receive in any order
  for (size_t i = 0; i < Cardi::Dir::NumDirs; ++i) {

    MPI_Status status;

    // block for a message
    MPI_Probe(
      MPI_ANY_SOURCE,
      MPI_ANY_TAG,
      comm_grid,
      &status
    );

    // how long is the message and who sent it from what direction
    const int msg_len = [&](){
      int res;
      MPI_Get_count(
        &status,
        MPI_BYTE,
        &res
      );
      return res;
    }();
    const int msg_source = status.MPI_SOURCE;
    const int msg_tag = status.MPI_TAG;
    const int incoming_direction = Cardi::Opp[msg_tag];

    // make space for received data
    received.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(incoming_direction),
      std::forward_as_tuple(msg_len)
    );

    // copy down data from message
    MPI_Recv(
      received.at(incoming_direction).data(),
      msg_len,
      MPI_BYTE,
      msg_source,
      MPI_ANY_TAG,
      comm_grid,
      MPI_STATUS_IGNORE
    );

  }

  // wait for all sends to complete
  MPI_Waitall(
    send_requests.size(),
    send_requests.data(),
    MPI_STATUSES_IGNORE
  );

  // who am I and what did I get?
  std::ostringstream oss;
  oss << "rank " << g_rank << ", id " << id
    << " @ (" << coords[0] << ", " << coords[1] << ")";

  for (const auto & [k, v] : received) {
    oss << " [" << k << ": ";
    std::for_each(
      std::begin(v),
      std::end(v),
      [&oss](auto c){ oss << c; }
    );
    oss << "]";
  }

  std::cout << oss.str() << std::endl;

  MPI_Finalize();

  return 0;
}
