# Declarative Monitoring of variables with C++

[![ci](https://github.com/nikolaj.fogh/monitoring/actions/workflows/ci.yml/badge.svg)](https://github.com/nikolaj.fogh/monitoring/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/nikolaj.fogh/monitoring/branch/main/graph/badge.svg)](https://codecov.io/gh/nikolaj.fogh/monitoring)
[![Language grade: C++](https://img.shields.io/lgtm/grade/cpp/github/nikolaj.fogh/monitoring)](https://lgtm.com/projects/g/nikolaj.fogh/monitoring/context:cpp)
[![CodeQL](https://github.com/nikolaj.fogh/monitoring/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/nikolaj.fogh/monitoring/actions/workflows/codeql-analysis.yml)

## About libmonitoring

Libmonitoring provides a declarative style to create monitoring classes which can 
check values for certain conditions, and invoke various handlers when these conditions
are violated.

In order to have the maximum performance and flexibility, libmonitoring uses templates.

A relatively recent C++ compiler supporting C++17 is needed.

## Kudos
Kudos to Jason Turner for the CMake template that was used as scaffolding.
