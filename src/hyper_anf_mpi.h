#ifndef _HYPER_ANF_MPI_H_
#define _HYPER_ANF_MPI_H_

#include <mpi.h>

// ----------------------------------------------------------------------------
//     MPI derived data types
// ----------------------------------------------------------------------------

/**
 * @brief MPI datatype for the array of registers of an hll counter.
 *
 * The type of `hll_reg_t` that is making up the array of counters is
 *`uint8_t`, hence the appropriate MPI type to replicate is `MPI_UINT8_T`
 */
MPI_Datatype mpi_hll_counter;

/**
 * @brief Registers to the system the MPI datatype for hll counters.
 *
 * **Attention**: must be called at the beginning of the program, before using
 * counter.
 */
void mpi_hll_counter_type_commit(size_t num_registers);

/**
 * @brief Unregisters from the system the MPI datatype.
 */
void mpi_hll_counter_type_free();


#endif // _HYPER_ANF_MPI_H_
