add_executable ( serial
      hll_counter.c
      serial.c
)

if(${CHECK_FOUND})
  add_executable ( testHllCounter
    hll_counter.c
    test_hll_counter.c
  )
  target_link_libraries(testHllCounter ${CHECK_LIBRARIES})

  add_test(testHllCounter ${PROJECT_BINARY_DIR}/bin/testHllCounter)
endif()
