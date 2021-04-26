find_package(Catch2 REQUIRED)
find_package(PythonInterp 3 REQUIRED)

set(NETUIT_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/AdjacencyFileTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/CompleteTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/DyadicTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/LoopTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/NavigableSmallWorldTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/ProConTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/RingTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/SoftRandomGeometricTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/SmallWorldGridTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/ToroidalTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/arrange/ToroidalGridTopologyFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/AssignAvailableProcs.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/AssignAvailableThreads.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/AssignContiguously.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/AssignIntegrated.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/AssignRandomly.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/AssignRoundRobin.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/AssignSegregated.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/assign/GenerateMetisAssignments.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/mesh/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/mesh/MeshNode.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/mesh/MeshNodeInput.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/mesh/MeshNodeOutput.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/mesh/MeshTopology.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/topology/TopoEdge.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/topology/TopoNode.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/topology/TopoNodeInput.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/topology/TopoNodeOutput.cpp
    ${CMAKE_SOURCE_DIR}/tests/netuit/topology/Topology.cpp
    )
set(UIT_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/ducts/Duct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/intra/accumulating+type=any/double/a::AccumulatingDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/intra/accumulating+type=any/int/a::AccumulatingDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/intra/put=dropping+get=stepping+type=any/a::HeadTailDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/intra/put=dropping+get=stepping+type=any/a::SerialPendingDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/intra/put=growing+get=stepping+type=any/a::DequeDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/mock/EmpAssertDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/mock/NopDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/mock/ThrowDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/double/aggregated+inlet=Accumulate+outlet=WithdrawingWindow_f::AggregatedIaOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/double/aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f::AggregatedIrOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/double/inlet=Accumulate+outlet=WithdrawingWindow_f::IaOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/double/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/int/aggregated+inlet=Accumulate+outlet=WithdrawingWindow_f::AggregatedIaOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/int/aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f::AggregatedIrOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/int/inlet=Accumulate+outlet=WithdrawingWindow_f::IaOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=fundamental/int/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=span/double/buffered+inlet=BufferedIsend+outlet=Irecv_s::BufferedIbiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=span/double/inlet=Isend+outlet=Irecv_s::IiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=span/int/buffered+inlet=BufferedIsend+outlet=Irecv_s::BufferedIbiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=span/int/inlet=Isend+outlet=Irecv_s::IiOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=spanfundamental/double/inlet=Accumulate+outlet=WithdrawingWindow_sf::IaOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=spanfundamental/double/inlet=Raccumulate+outlet=WithdrawingWindow_sf::IrOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=spanfundamental/int/inlet=Accumulate+outlet=WithdrawingWindow_sf::IaOwwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=spanfundamental/int/inlet=Raccumulate+outlet=WithdrawingWindow_sf::IrOwwDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=trivial/double/aggregated+inlet=Isend+outlet=Irecv_t::AggregatedIiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=trivial/double/buffered+inlet=BufferedIsend+outlet=Irecv_t::BufferedIiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=trivial/double/inlet=Isend+outlet=Irecv_t::IiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=trivial/int/aggregated+inlet=Isend+outlet=Irecv_t::AggregatedIiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=trivial/int/buffered+inlet=BufferedIsend+outlet=Irecv_t::BufferedIiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/accumulating+type=trivial/int/inlet=Isend+outlet=Irecv_t::IiOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/backend/AccumulatingPooledBackEnd.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/backend/AggregatedBackEnd.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/backend/MockBackEnd.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/backend/PooledBackEnd.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/backend/RdmaBackEnd.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/backend/RuntimeSizeBackEnd.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/backend/RuntimeSizeRdmaBackEnd.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/impl/AggregatorSpec.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/impl/InletMemoryAccumulatingPool.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/impl/InletMemoryAggregator.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/impl/InletMemoryPool.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/impl/OutletMemoryAggregator.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/impl/OutletMemoryPool.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/backend/impl/PoolSpec.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=fundamental/f::AccumulateDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=fundamental/f::RaccumulateDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=span/s::BufferedIsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=span/s::IsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=spanfundamental/sf::AccumulateDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=spanfundamental/sf::RaccumulateDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=trivial/t::BufferedIsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/accumulating+type=trivial/t::IsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=dropping+type=cereal/c::RingIrsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=dropping+type=cereal/c::RingIsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=dropping+type=span/s::RingIrsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=dropping+type=span/s::RingIsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=dropping+type=trivial/t::RingIrsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=dropping+type=trivial/t::RingIsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=dropping+type=trivial/t::RingRputDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=cereal/c::DequeIrsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=cereal/c::DequeIsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=cereal/c::RsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=cereal/c::SendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=trivial/t::DequeIrsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=trivial/t::DequeIsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=trivial/t::PutDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=trivial/t::RsendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/put=growing+type=trivial/t::SendDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/templated/impl/BufferSpec.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/templated/templated/AggregatedInletDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/templated/templated/BufferedInletDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/inlet/templated/templated/PooledInletDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/accumulating+type=fundamental/f::WithdrawingWindowDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/accumulating+type=span/s::IrecvDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/accumulating+type=spanfundamental/sf::WithdrawingWindowDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/accumulating+type=trivial/t::IrecvDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/get=skipping+type=span/s::BlockIrecvDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/get=skipping+type=trivial/t::BlockIrecvDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/get=skipping+type=trivial/t::WindowDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/get=stepping+type=cereal/c::IprobeDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/get=stepping+type=span/s::IprobeDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/get=stepping+type=trivial/t::RingIrecvDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/templated/AggregatedOutletDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/templated/BufferedOutletDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/impl/outlet/templated/PooledOutletDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIrsend+outlet=BlockIrecv_s::IrirObiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlocIrecv_s::IriObiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingIsend+outlet=BlockIrecv_t::IriObiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingRput+outlet=Window_t::IrrOwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=skipping+type=trivial/pooled+inlet=RingIsend+outlet=BlockIrecv_t::PooledIriObiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIrsend+outlet=Iprobe_c::IrirOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=span/inlet=RingIrsend+outlet=Iprobe_s::IrirOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=span/inlet=RingIsend+outlet=Iprobe_s::IriOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=trivial/buffered+inlet=RingIsend+outlet=Iprobe_t::BufferedIriOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIrsend+outlet=RingIrecv_t::IrirOriDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIsend+outlet=RingIrecv_t::IriOriDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=trivial/pooled+inlet=RingIsend+outlet=Iprobe_t::PooledIriOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeIrsend+outlet=BlockIrecv_t::IdirObiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeIsend+outlet=BlockIrecv_t::IdiObiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeRput+outlet=Window_t:IdrOwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Put+outlet=Window_t::IpOwDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Rsend+outlet=BlockIrecv_t::IrObiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Send+outlet=BlockIrecv_t::IsObiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=DequeIrsend+outlet=Iprobe_c::IdirOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=DequeIsend+outlet=Iprobe_c::IdiOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Rsend+outlet=Iprobe_c::IrOiDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Send+outlet=Iprobe_c::IsOiDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=DequeIrsend+outlet=RingIrecv_t::IdirOriDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=DequeIsend+outlet=RingIrecv_t::IdiOriDuct.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=Rsend+outlet=RingIrecv_t::IrOri.cpp
    #${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=Send+outlet=RingIrecv_t::IsOriDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/accumulating+type=any/a::MutexAccumulatingDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/accumulating+type=fundamental/f::CompareExchangeDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/put=dropping+get=stepping+type=any/a::AtomicPendingDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/put=dropping+get=stepping+type=any/a::BoundedMoodyCamelDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/put=dropping+get=stepping+type=any/a::RigtorpDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/put=growing+get=skipping+type=any/a::AtomicSconceDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/ducts/thread/put=growing+get=stepping+type=any/a::UnboundedMoodyCamelDuct.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/fixtures/Conduit.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/fixtures/Sink.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/fixtures/Source.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/setup/ImplSpec.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/setup/InterProcAddress.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/spouts/spouts/Inlet.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/spouts/spouts/Outlet.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/spouts/wrappers/inlet/CachingInletWrapper.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/spouts/wrappers/outlet/CachingOutletWrapper.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/spouts/wrappers/wrappers/CachingSpoutWrapper.cpp
    ${CMAKE_SOURCE_DIR}/tests/uit/spouts/wrappers/wrappers/TrivialSpoutWrapper.cpp
    )
set(UITSL_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/uitsl/algorithm/get_plurality.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/algorithm/normalize.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/algorithm/upper_uniquify.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/chrono/CoarseMonoClock.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/chrono/CoarseRealClock.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/chrono/CycleClock.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/chrono/RawCycleClock.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/chrono/SplitWatch.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/chrono/cycle_freq.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/concurrent/ConcurrentTimeoutBarrier.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/concurrent/Gatherer.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/concurrent/ThreadSafeIbarrierRequest.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/containers/safe/deque.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/containers/safe/list.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/containers/safe/unordered_map.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/containers/safe/vector.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/countdown/ProgressBar.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/datastructs/MirroredRingBuffer.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/datastructs/PodInternalNode.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/datastructs/PodLeafNode.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/datastructs/RingBuffer.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/datastructs/SiftingArray.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/datastructs/VectorMap.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/debug/IsFirstExecutionChecker.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/debug/OncePerThreadChecker.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/debug/WarnOnce.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/debug/audit_compare.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/debug/compare_files.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/debug/compare_streams.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/debug/safe_compare.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/distributed/CachePacket.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/distributed/DistributedTimeoutBarrier.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/distributed/MsgAccumulatorBundle.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/distributed/RdmaAccumulatorBundle.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/distributed/do_successively.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/fetch/autoinstall.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/fetch/detect_gz.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/fetch/detect_tar.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/fetch/fetch.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/fetch/untar.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/fetch/inflate.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/initialization/Uninitialized.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/initialization/ValueInitialized.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/math/mapping_utils.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/math/math_utils.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/math/ratio_to_double.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/math/shift_mod.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/Curry.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/IdentityWrapper.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/a::static_test.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/c::static_test.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/f::static_test.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/s::static_test.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/t::static_test.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/tuple_has_type.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/meta/tuple_index.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/MpiGuard.cpp
    #${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/MpiMultithreadGuard.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/Request.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/comm_utils.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/group_utils.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/mpi_types.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/mpi_utils.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/request_utils.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/routine_functors.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/nonce/CircularIndex.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/nonce/ScopeGuard.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/nonce/spector.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/AlignedImplicit.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/AlignedInherit.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/ParallelBarrier.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/ParallelTimeoutBarrier.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/RecursiveExclusiveLock.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/RecursiveMutex.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/RelaxedAtomic.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/ThreadIbarrier.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/ThreadIbarrierFactory.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/ThreadLocalChecker.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/parallel/ThreadMap.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/polyfill/filesystem_emscripten.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/polyfill/filesystem_native.cpp
    ${CMAKE_SOURCE_DIR}/tests/uitsl/utility/NamedArrayElement.cpp
    )

set(CATCH_BUILD_TESTING ON)
set(BUILD_TESTING ON)

add_executable(test-uit
    main.cpp
    ${UIT_SOURCES}
    )
add_executable(test-netuit
    main.cpp
    ${NETUIT_SOURCES}
    )
add_executable(test-uitsl
    main.cpp
    ${UITSL_SOURCES}
    )

if(DEFINED CUSTOM_SOURCES)
    add_executable(test-custom
    main.cpp
    ${CUSTOM_SOURCES}
    )
endif()

add_custom_command(TARGET test-uitsl
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying uitsl test data..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_HOME_DIRECTORY}/tests/uitsl/fetch/assets ${CMAKE_BINARY_DIR}/tests/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_HOME_DIRECTORY}/tests/uitsl/debug/assets ${CMAKE_BINARY_DIR}/tests/assets
    )

add_custom_command(TARGET test-netuit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying netuit test data..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_HOME_DIRECTORY}/tests/netuit/arrange/scripts ${CMAKE_BINARY_DIR}/tests/scripts

    COMMAND ${CMAKE_COMMAND} -E echo "Executing python scripts..."
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/tests/assets
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/tests/staging/assets

    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_complete.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_dyadic.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_loop.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_navigable_small_world.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_procon.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_ring.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_soft_random_geometric.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_small_world_grid.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_toroidal.py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/tests/scripts/make_toroidal_grid.py


    )

add_custom_target(
    test-web
    COMMAND make web
    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/tests/uitsl/fetch
    )

if(DEFINED CODE_COVERAGE)
    set(UIT_NPROC_MIN 2)
    set(UIT_NPROC_MAX 3)
endif()

include("../cmake/NProcMultiplexer.cmake")
include("../cmake/CodeCoverage2.cmake")

nproc_multiplex_tests(test-uitsl)
nproc_multiplex_tests(test-uit)
nproc_multiplex_tests(test-netuit)

if(DEFINED CUSTOM_SOURCES)
    nproc_multiplex_tests(test-custom)
endif()

target_link_libraries(test-uitsl Catch2::Catch2 conduit)
target_link_libraries(test-netuit Catch2::Catch2 conduit)
target_link_libraries(test-uit Catch2::Catch2 conduit)

if(DEFINED CUSTOM_SOURCES)
    target_link_libraries(test-custom Catch2::Catch2 conduit)
endif()

if(DEFINED CODE_COVERAGE)
    add_code_coverage_all_targets(EXCLUDE tests/* third-party/*)

    target_code_coverage(test-uitsl AUTO ALL EXCLUDE tests/* third-party/*)
    target_code_coverage(test-uit AUTO ALL EXCLUDE tests/* third-party/*)
    target_code_coverage(test-netuit AUTO ALL EXCLUDE tests/* third-party/*)

    if(DEFINED CUSTOM_SOURCES)
        target_code_coverage(test-custom)
    endif()
endif()

add_custom_target(
     run-tests
     COMMENT "Run tests"
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     COMMAND ${CMAKE_CTEST_COMMAND} -E "NOT_BUILT" --output-on-failures
)
