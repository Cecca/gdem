add_executable ( serial
      hll_counter.c
      parser.c
      graph.c
      hyper_anf_serial.c
      serial.c
)

add_executable ( parallel
      hll_counter.c
      parser.c
      graph.c
      hyper_anf_mpi.c
      parallel.c
)
target_link_libraries(parallel ${MPI_C_LIBRARIES})

add_executable ( mpiHello
      mpi_hello.c
)
target_link_libraries(mpiHello ${MPI_C_LIBRARIES})

if(${CHECK_FOUND})
  add_executable ( testHllCounter
    hll_counter.c
    test_hll_counter.c
  )
  target_link_libraries(testHllCounter ${CHECK_LIBRARIES})

  add_test(testHllCounter ${PROJECT_BINARY_DIR}/bin/testHllCounter)

  add_executable ( testParser
    parser.c
    graph.c
    test_parser.c
  )
  target_link_libraries(testParser ${CHECK_LIBRARIES})

  add_test(testParser ${PROJECT_BINARY_DIR}/bin/testParser)
endif()
