add_executable ( mainCounter
      fm_counter.c
      hll_counter.c
      main.c
)

add_executable ( serial
      hll_counter.c
      serial.c
)

if(${CHECK_FOUND})
  add_executable ( testFmCounter
    fm_counter.c
    test_fm_counter.c
  )
  target_link_libraries(testFmCounter ${CHECK_LIBRARIES})

  add_executable ( testHllCounter
    hll_counter.c
    test_hll_counter.c
  )
  target_link_libraries(testHllCounter ${CHECK_LIBRARIES})

#  add_test(testFmCounter ${PROJECT_BINARY_DIR}/bin/testFmCounter)
  add_test(testHllCounter ${PROJECT_BINARY_DIR}/bin/testHllCounter)
endif()
