# Declarative Monitoring of variables with C++

[![ci](https://github.com/nfogh/monitoring/actions/workflows/ci.yml/badge.svg)](https://github.com/nfogh/monitoring/actions/workflows/ci.yml)
[![codecov](https://app.codecov.io/gh/nfogh/monitoring/branch/main/graph/badge.svg)](https://app.codecov.io/gh/nfogh/monitoring/)
[![CodeQL](https://github.com/nfogh/monitoring/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/nfogh/monitoring/actions/workflows/codeql-analysis.yml)

## About libmonitoring

Libmonitoring provides a declarative style to create monitoring classes which can 
check values for certain conditions, and invoke various handlers when these conditions
are violated.

In order to have the maximum performance and flexibility, libmonitoring uses templates.

A relatively recent C++ compiler supporting C++17 is needed.

## Kudos
Kudos to Jason Turner for the CMake template that was used as scaffolding.
