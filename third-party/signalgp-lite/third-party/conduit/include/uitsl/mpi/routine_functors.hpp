#pragma once
#ifndef UITSL_MPI_ROUTINE_FUNCTORS_HPP_INCLUDE
#define UITSL_MPI_ROUTINE_FUNCTORS_HPP_INCLUDE

#include <utility>

#include "audited_routines.hpp"

namespace uitsl {

  /// @private
/// Wraps corresponding MPI* call in a functor.
struct AbortFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Abort(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AccumulateFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Accumulate(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Add_error_classFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Add_error_class(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Add_error_codeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Add_error_code(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Add_error_stringFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Add_error_string(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AddressFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Address(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AllgatherFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Allgather(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AllgathervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Allgatherv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Alloc_memFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Alloc_mem(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AllreduceFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Allreduce(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AlltoallFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Alltoall(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AlltoallvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Alltoallv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct AlltoallwFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Alltoallw(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Attr_deleteFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Attr_delete(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Attr_getFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Attr_get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Attr_putFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Attr_put(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct BarrierFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Barrier(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct BcastFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Bcast(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct BsendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Bsend(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Bsend_initFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Bsend_init(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Buffer_attachFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Buffer_attach(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Buffer_detachFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Buffer_detach(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct CancelFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cancel(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cart_coordsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cart_coords(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cart_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cart_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cart_getFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cart_get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cart_mapFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cart_map(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cart_rankFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cart_rank(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cart_shiftFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cart_shift(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cart_subFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cart_sub(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Cartdim_getFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Cartdim_get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Close_portFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Close_port(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_acceptFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_accept(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_call_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_call_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_compareFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_compare(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_connectFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_connect(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_create_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_create_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_create_groupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_create_group(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_create_keyvalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_create_keyval(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_delete_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_delete_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_disconnectFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_disconnect(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_dupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_dup(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_dup_with_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_dup_with_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_free_keyvalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_free_keyval(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_get_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_get_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_get_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_get_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_get_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_get_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_get_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_get_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_get_parentFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_get_parent(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_groupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_group(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_idupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_idup(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_joinFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_join(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_rankFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_rank(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_remote_groupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_remote_group(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_remote_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_remote_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_set_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_set_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_set_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_set_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_set_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_set_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_set_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_set_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_spawnFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_spawn(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_spawn_multipleFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_spawn_multiple(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_splitFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_split(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_split_typeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_split_type(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Comm_test_interFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Comm_test_inter(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Compare_and_swapFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Compare_and_swap(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Dims_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Dims_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Dist_graph_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Dist_graph_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Dist_graph_create_adjacentFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Dist_graph_create_adjacent(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Dist_graph_neighborsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Dist_graph_neighbors(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Dist_graph_neighbors_countFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Dist_graph_neighbors_count(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Errhandler_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Errhandler_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Errhandler_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Errhandler_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Errhandler_getFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Errhandler_get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Errhandler_setFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Errhandler_set(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Error_classFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Error_class(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Error_stringFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Error_string(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ExscanFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Exscan(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Fetch_and_opFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Fetch_and_op(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_c2fFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_c2f(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_call_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_call_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_closeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_close(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_create_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_create_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_deleteFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_delete(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_f2cFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_f2c(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_amodeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_amode(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_atomicityFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_atomicity(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_byte_offsetFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_byte_offset(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_groupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_group(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_positionFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_position(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_position_sharedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_position_shared(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_type_extentFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_type_extent(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_get_viewFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_get_view(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_ireadFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iread(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iread_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iread_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iread_atFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iread_at(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iread_at_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iread_at_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iread_sharedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iread_shared(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iwriteFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iwrite(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iwrite_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iwrite_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iwrite_atFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iwrite_at(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iwrite_at_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iwrite_at_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_iwrite_sharedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_iwrite_shared(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_openFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_open(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_preallocateFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_preallocate(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_readFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_all_beginFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_all_begin(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_all_endFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_all_end(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_atFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_at(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_at_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_at_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_at_all_beginFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_at_all_begin(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_at_all_endFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_at_all_end(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_orderedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_ordered(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_ordered_beginFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_ordered_begin(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_ordered_endFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_ordered_end(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_read_sharedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_read_shared(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_seekFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_seek(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_seek_sharedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_seek_shared(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_set_atomicityFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_set_atomicity(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_set_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_set_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_set_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_set_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_set_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_set_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_set_viewFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_set_view(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_syncFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_sync(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_writeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_all_beginFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_all_begin(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_all_endFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_all_end(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_atFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_at(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_at_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_at_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_at_all_beginFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_at_all_begin(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_at_all_endFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_at_all_end(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_orderedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_ordered(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_ordered_beginFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_ordered_begin(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_ordered_endFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_ordered_end(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct File_write_sharedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_File_write_shared(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct FinalizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Finalize(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct FinalizedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Finalized(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Free_memFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Free_mem(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct GatherFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Gather(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct GathervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Gatherv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct GetFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_accumulateFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_accumulate(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_addressFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_address(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_countFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_count(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_elementsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_elements(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_elements_xFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_elements_x(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_library_versionFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_library_version(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_processor_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_processor_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Get_versionFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Get_version(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Graph_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Graph_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Graph_getFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Graph_get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Graph_mapFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Graph_map(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Graph_neighborsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Graph_neighbors(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Graph_neighbors_countFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Graph_neighbors_count(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Graphdims_getFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Graphdims_get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Grequest_completeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Grequest_complete(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Grequest_startFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Grequest_start(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_compareFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_compare(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_differenceFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_difference(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_exclFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_excl(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_inclFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_incl(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_intersectionFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_intersection(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_range_exclFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_range_excl(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_range_inclFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_range_incl(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_rankFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_rank(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_translate_ranksFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_translate_ranks(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Group_unionFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Group_union(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IallgatherFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iallgather(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IallgathervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iallgatherv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IallreduceFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iallreduce(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IalltoallFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ialltoall(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IalltoallvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ialltoallv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IalltoallwFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ialltoallw(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IbarrierFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ibarrier(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IbcastFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ibcast(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IbsendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ibsend(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IexscanFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iexscan(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IgatherFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Igather(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IgathervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Igatherv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ImprobeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Improbe(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ImrecvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Imrecv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ineighbor_allgatherFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ineighbor_allgather(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ineighbor_allgathervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ineighbor_allgatherv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ineighbor_alltoallFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ineighbor_alltoall(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ineighbor_alltoallvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ineighbor_alltoallv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ineighbor_alltoallwFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ineighbor_alltoallw(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_deleteFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_delete(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_dupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_dup(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_getFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_get(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_get_nkeysFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_get_nkeys(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_get_nthkeyFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_get_nthkey(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_get_valuelenFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_get_valuelen(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Info_setFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Info_set(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct InitFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Init(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Init_threadFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Init_thread(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct InitializedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Initialized(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Intercomm_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Intercomm_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Intercomm_mergeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Intercomm_merge(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IprobeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iprobe(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IrecvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Irecv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IreduceFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ireduce(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ireduce_scatterFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ireduce_scatter(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ireduce_scatter_blockFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ireduce_scatter_block(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IrsendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Irsend(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Is_thread_mainFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Is_thread_main(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IscanFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iscan(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IscatterFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iscatter(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IscattervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Iscatterv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IsendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Isend(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct IssendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Issend(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Keyval_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Keyval_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Keyval_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Keyval_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Lookup_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Lookup_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct MprobeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Mprobe(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct MrecvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Mrecv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Neighbor_allgatherFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Neighbor_allgather(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Neighbor_allgathervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Neighbor_allgatherv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Neighbor_alltoallFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Neighbor_alltoall(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Neighbor_alltoallvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Neighbor_alltoallv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Neighbor_alltoallwFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Neighbor_alltoallw(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Op_commuteFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Op_commute(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Op_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Op_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Op_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Op_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Open_portFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Open_port(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct PackFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Pack(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Pack_externalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Pack_external(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Pack_external_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Pack_external_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Pack_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Pack_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct PcontrolFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Pcontrol(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ProbeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Probe(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Publish_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Publish_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct PutFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Put(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Query_threadFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Query_thread(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct RaccumulateFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Raccumulate(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct RecvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Recv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Recv_initFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Recv_init(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ReduceFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Reduce(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Reduce_localFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Reduce_local(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Reduce_scatterFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Reduce_scatter(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Reduce_scatter_blockFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Reduce_scatter_block(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Register_datarepFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Register_datarep(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Request_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Request_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Request_get_statusFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Request_get_status(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct RgetFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Rget(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Rget_accumulateFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Rget_accumulate(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct RputFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Rput(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct RsendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Rsend(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Rsend_initFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Rsend_init(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ScanFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Scan(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ScatterFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Scatter(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct ScattervFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Scatterv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct SendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Send(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Send_initFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Send_init(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct SendrecvFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Sendrecv(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Sendrecv_replaceFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Sendrecv_replace(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct SsendFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ssend(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Ssend_initFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Ssend_init(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct StartFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Start(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct StartallFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Startall(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Status_set_cancelledFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Status_set_cancelled(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Status_set_elementsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Status_set_elements(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Status_set_elements_xFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Status_set_elements_x(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_category_changedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_category_changed(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_category_get_categoriesFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_category_get_categories(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_category_get_cvarsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_category_get_cvars(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_category_get_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_category_get_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_category_get_numFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_category_get_num(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_category_get_pvarsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_category_get_pvars(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_cvar_get_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_cvar_get_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_cvar_get_numFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_cvar_get_num(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_cvar_handle_allocFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_cvar_handle_alloc(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_cvar_handle_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_cvar_handle_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_cvar_readFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_cvar_read(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_cvar_writeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_cvar_write(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_enum_get_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_enum_get_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_enum_get_itemFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_enum_get_item(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_finalizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_finalize(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_init_threadFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_init_thread(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_get_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_get_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_get_numFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_get_num(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_handle_allocFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_handle_alloc(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_handle_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_handle_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_readFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_read(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_readresetFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_readreset(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_resetFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_reset(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_session_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_session_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_session_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_session_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_startFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_start(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_stopFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_stop(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct T_pvar_writeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_T_pvar_write(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct TestFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Test(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Test_cancelledFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Test_cancelled(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct TestallFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Testall(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct TestanyFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Testany(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct TestsomeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Testsome(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Topo_testFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Topo_test(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_commitFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_commit(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_contiguousFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_contiguous(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_darrayFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_darray(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_hindexedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_hindexed(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_hindexed_blockFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_hindexed_block(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_hvectorFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_hvector(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_indexed_blockFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_indexed_block(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_keyvalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_keyval(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_resizedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_resized(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_structFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_struct(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_create_subarrayFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_create_subarray(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_delete_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_delete_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_dupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_dup(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_extentFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_extent(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_free_keyvalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_free_keyval(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_contentsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_contents(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_envelopeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_envelope(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_extentFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_extent(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_extent_xFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_extent_x(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_true_extentFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_true_extent(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_get_true_extent_xFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_get_true_extent_x(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_hindexedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_hindexed(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_hvectorFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_hvector(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_indexedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_indexed(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_lbFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_lb(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_match_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_match_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_set_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_set_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_set_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_set_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_sizeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_size(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_size_xFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_size_x(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_structFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_struct(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_ubFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_ub(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Type_vectorFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Type_vector(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct UnpackFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Unpack(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Unpack_externalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Unpack_external(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Unpublish_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Unpublish_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct WaitFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Wait(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct WaitallFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Waitall(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct WaitanyFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Waitany(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct WaitsomeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Waitsome(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_allocateFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_allocate(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_allocate_sharedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_allocate_shared(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_attachFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_attach(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_call_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_call_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_completeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_complete(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_createFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_create(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_create_dynamicFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_create_dynamic(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_create_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_create_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_create_keyvalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_create_keyval(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_delete_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_delete_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_detachFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_detach(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_fenceFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_fence(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_flushFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_flush(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_flush_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_flush_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_flush_localFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_flush_local(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_flush_local_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_flush_local_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_freeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_free(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_free_keyvalFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_free_keyval(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_get_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_get_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_get_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_get_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_get_groupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_get_group(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_get_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_get_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_get_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_get_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_lockFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_lock(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_lock_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_lock_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_postFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_post(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_set_attrFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_set_attr(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_set_errhandlerFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_set_errhandler(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_set_infoFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_set_info(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_set_nameFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_set_name(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_shared_queryFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_shared_query(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_startFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_start(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_syncFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_sync(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_testFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_test(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_unlockFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_unlock(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_unlock_allFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_unlock_all(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct Win_waitFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITSL_Win_wait(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct R_Type_commitFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITR_Type_commit(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct R_Type_contiguousFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITR_Type_contiguous(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct R_Type_dupFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITR_Type_dup(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct R_Type_get_contentsFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITR_Type_get_contents(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct R_Type_indexedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITR_Type_indexed(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct R_Type_structFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITR_Type_struct(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct _Type_vectorFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITR_Type_vector(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct X_Comm_agreeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITX_Comm_agree(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct X_Comm_failure_ackFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITX_Comm_failure_ack(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct X_Comm_failure_get_ackedFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITX_Comm_failure_get_acked(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct X_Comm_revokeFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITX_Comm_revoke(std::forward<Args>(args)...);
  }
};

/// @private
/// Wraps corresponding MPI* call in a functor.
struct X_Comm_shrinkFunctor {
  template<typename... Args>
  void operator()(Args&&... args) {
    UITX_Comm_shrink(std::forward<Args>(args)...);
  }
};

} //namespace uit

#endif // #ifndef UITSL_MPI_ROUTINE_FUNCTORS_HPP_INCLUDE
