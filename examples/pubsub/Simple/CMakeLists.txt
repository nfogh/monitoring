
add_executable(PubSubMonitorSimpleExample
  PubSubMonitorExample.cpp)

  target_include_directories(PubSubMonitorSimpleExample
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/..)

target_link_libraries(
  PubSubMonitorSimpleExample
  PRIVATE
    monitoring::monitoring)
