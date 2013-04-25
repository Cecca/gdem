cmake_minimum_required(VERSION 2.8)

project(graph-diameter-estimator-mpi)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm -g -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

find_package(Check)

if(${CHECK_FOUND})
  include_directories(${CHECK_INCLUDE_DIRS})
  enable_testing()
endif()

add_subdirectory(src)

