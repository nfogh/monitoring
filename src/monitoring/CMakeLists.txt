#include(GenerateExportHeader)

add_library(monitoring INTERFACE)

add_library(monitoring::monitoring ALIAS monitoring)

target_link_libraries(monitoring INTERFACE monitoring_options monitoring_warnings)

target_include_directories(monitoring ${WARNING_GUARD} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(monitoring INTERFACE cxx_std_17)

set_target_properties(
  monitoring
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

#generate_export_header(monitoring EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/monitoring/monitoring_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(monitoring INTERFACE SAMPLE_LIBRARY_STATIC_DEFINE)
endif()
