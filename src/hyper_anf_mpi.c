#include "hyper_anf_mpi.h"
#include <mpi.h>

void mpi_hll_counter_type_commit(size_t num_registers) {
  MPI_Type_contiguous(num_registers, MPI_INT, &mpi_hll_counter);
}
