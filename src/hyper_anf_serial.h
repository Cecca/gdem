#ifndef _HYPER_ANF_SERIAL_H_
#define _HYPER_ANF_SERIAL_H_

#include "graph.h"

/**
 * @brief Computes the effective diameter at alpha.
 *
 * @param graph
 * @param n
 * @param alpha
 * @param bits
 * @param max_iteration
 * @return
 */
int effective_diameter( node_t *graph,
                        size_t n,
                        double alpha,
                        int bits,
                        int max_iteration);

#endif // _HYPER_ANF_SERIAL_H_
