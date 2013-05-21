add_executable ( serial
      hll_counter.c
      serial.c
)

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
