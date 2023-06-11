find_package(ZeroMQ)

add_executable(zeromq_example
  zeromq_example.cpp)

target_link_libraries(
  zeromq_example
  PRIVATE
    monitoring::monitoring
    libzmq)
