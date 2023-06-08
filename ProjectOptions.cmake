include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(monitoring_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(monitoring_setup_options)
  option(monitoring_ENABLE_HARDENING "Enable hardening" ON)
  option(monitoring_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    monitoring_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    monitoring_ENABLE_HARDENING
    OFF)

  monitoring_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR monitoring_PACKAGING_MAINTAINER_MODE)
    option(monitoring_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(monitoring_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(monitoring_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(monitoring_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(monitoring_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(monitoring_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(monitoring_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(monitoring_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(monitoring_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(monitoring_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(monitoring_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(monitoring_ENABLE_PCH "Enable precompiled headers" OFF)
    option(monitoring_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(monitoring_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(monitoring_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(monitoring_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(monitoring_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(monitoring_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(monitoring_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(monitoring_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(monitoring_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(monitoring_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(monitoring_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(monitoring_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(monitoring_ENABLE_PCH "Enable precompiled headers" OFF)
    option(monitoring_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      monitoring_ENABLE_IPO
      monitoring_WARNINGS_AS_ERRORS
      monitoring_ENABLE_USER_LINKER
      monitoring_ENABLE_SANITIZER_ADDRESS
      monitoring_ENABLE_SANITIZER_LEAK
      monitoring_ENABLE_SANITIZER_UNDEFINED
      monitoring_ENABLE_SANITIZER_THREAD
      monitoring_ENABLE_SANITIZER_MEMORY
      monitoring_ENABLE_UNITY_BUILD
      monitoring_ENABLE_CLANG_TIDY
      monitoring_ENABLE_CPPCHECK
      monitoring_ENABLE_COVERAGE
      monitoring_ENABLE_PCH
      monitoring_ENABLE_CACHE)
  endif()

  monitoring_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (monitoring_ENABLE_SANITIZER_ADDRESS OR monitoring_ENABLE_SANITIZER_THREAD OR monitoring_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(monitoring_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(monitoring_global_options)
  if(monitoring_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    monitoring_enable_ipo()
  endif()

  monitoring_supports_sanitizers()

  if(monitoring_ENABLE_HARDENING AND monitoring_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR monitoring_ENABLE_SANITIZER_UNDEFINED
       OR monitoring_ENABLE_SANITIZER_ADDRESS
       OR monitoring_ENABLE_SANITIZER_THREAD
       OR monitoring_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${monitoring_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${monitoring_ENABLE_SANITIZER_UNDEFINED}")
    monitoring_enable_hardening(monitoring_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(monitoring_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(monitoring_warnings INTERFACE)
  add_library(monitoring_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  monitoring_set_project_warnings(
    monitoring_warnings
    ${monitoring_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(monitoring_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(monitoring_options)
  endif()

  include(cmake/Sanitizers.cmake)
  monitoring_enable_sanitizers(
    monitoring_options
    ${monitoring_ENABLE_SANITIZER_ADDRESS}
    ${monitoring_ENABLE_SANITIZER_LEAK}
    ${monitoring_ENABLE_SANITIZER_UNDEFINED}
    ${monitoring_ENABLE_SANITIZER_THREAD}
    ${monitoring_ENABLE_SANITIZER_MEMORY})

  set_target_properties(monitoring_options PROPERTIES UNITY_BUILD ${monitoring_ENABLE_UNITY_BUILD})

  if(monitoring_ENABLE_PCH)
    target_precompile_headers(
      monitoring_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(monitoring_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    monitoring_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(monitoring_ENABLE_CLANG_TIDY)
    monitoring_enable_clang_tidy(monitoring_options ${monitoring_WARNINGS_AS_ERRORS})
  endif()

  if(monitoring_ENABLE_CPPCHECK)
    monitoring_enable_cppcheck(${monitoring_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(monitoring_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    monitoring_enable_coverage(monitoring_options)
  endif()

  if(monitoring_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(monitoring_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(monitoring_ENABLE_HARDENING AND NOT monitoring_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR monitoring_ENABLE_SANITIZER_UNDEFINED
       OR monitoring_ENABLE_SANITIZER_ADDRESS
       OR monitoring_ENABLE_SANITIZER_THREAD
       OR monitoring_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    monitoring_enable_hardening(monitoring_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
