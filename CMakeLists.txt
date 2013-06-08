cmake_minimum_required(VERSION 2.8)

project(graph-diameter-estimator-mpi)

find_package(MPI REQUIRED)
include_directories(${MPI_C_INCLUDE_PATH})


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lm -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Hash selection options
option(HASH_SIZE "The size of the hash to use")

if(${HASH_SIZE} STREQUAL "Small")
  message(STATUS "Using 8 bit hashes")
  add_definitions(-DHASH_SMALL)
elseif(${HASH_SIZE} STREQUAL "Medium")
  message(STATUS "Using 16 bit hashes")
  add_definitions(-DHASH_MEDIUM)
elseif(${HASH_SIZE} STREQUAL "Huge")
  message(STATUS "Using 64 bit hashes")
  add_definitions(-DHASH_HUGE)
else()
  message(STATUS "Using 32 bit hashes")
  add_definitions(-DHASH_BIG)
endif()

find_package(Check)
if(${CHECK_FOUND})
  include_directories(${CHECK_INCLUDE_DIR})
  enable_testing()
endif()

add_subdirectory(src)

# Configures Doxigen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with doxygen" VERBATIM)
endif(DOXYGEN_FOUND)
