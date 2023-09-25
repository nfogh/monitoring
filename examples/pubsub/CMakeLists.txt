
add_executable(PubSubMonitorExample
  PubSubMonitorExample.cpp)

target_link_libraries(
  PubSubMonitorExample
  PRIVATE
    monitoring::monitoring)
