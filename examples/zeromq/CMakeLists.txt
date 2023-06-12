if(NOT TARGET libzmq)
  set(ZMQ_BUILD_TESTS OFF)
  cpmaddpackage("gh:zeromq/libzmq@4.3.4")
endif()

if(NOT TARGET cppzmq)
  set(CPPZMQ_BUILD_TESTS OFF)
  cpmaddpackage("gh:zeromq/cppzmq@4.9.0")
endif()

add_executable(zeromq_example
  zeromq_example.cpp)

target_link_libraries(
  zeromq_example
  PRIVATE
    monitoring::monitoring
    libzmq
    cppzmq)
