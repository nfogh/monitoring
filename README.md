# Declarative Monitoring of variables with C++

[![ci](https://github.com/nfogh/monitoring/actions/workflows/ci.yml/badge.svg)](https://github.com/nfogh/monitoring/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/nfogh/monitoring/branch/main/graph/badge.svg)](https://app.codecov.io/gh/nfogh/monitoring/)
[![CodeQL](https://github.com/nfogh/monitoring/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/nfogh/monitoring/actions/workflows/codeql-analysis.yml)

## About libmonitoring

Libmonitoring provides a declarative style to create monitoring classes which can 
check values for certain conditions, and invoke various handlers when these conditions
are violated.

A relatively recent C++ compiler supporting C++17 is needed.

## Syntax

A monitor is created using the following Syntax:

    auto myMonitor = Monitoring::Monitor(condition, handler);
    int variable;
    myMonitor(variable);

A monitor is an invocable class.

When a monitor is invoked with a variable, that variable will be checked for
the conditions, and the handler is called to handle the result of the check.

The condition is a callable which must take the same parameters as the variables
that are supposed to be monitored, and returns a boolean indicating whether the
condition is satisfied. In the example above, the condition will be a callable
taking an int as a parameter.

The handler is any callable which takes a single boolean parameter. When the
handler is called, the boolean parameter will be set to true or false depending
on whether the condition was satisfied or not.

A more detailed example is shown below:

    auto myMonitor = Monitor([](int i){ return i > 0; }, [](bool valid){ std::cout << "Valid: " << valid << std::endl; }]);
    int variable = 0;
    myMonitor(variable); // Prints Valid: 0
    variable = 1;
    myMonitor(variable); // Prints Valid: 1

## Conditions
One of the advantages this library gives, is that it includes a large set of
predefined conditions. Using these conditions, the definition of the monitor
gets clearer. The monitor in the previous example can be simplified as such:

    auto myMonitor = Monitor(Minimum(1), PrintResult());

Also, the conditions can be combined using logical operators &&, || or !,
so a more advanced condition can be made easily:

    auto myMonitor = Monitor((Minimum(1) && Maximum(10)) || !Even(), PrintResult());

This monitor will (as can be read rather easily directly from the source)
check if a value is minimum 1, maximum 10 or not even, and it will print
the result of the check.

## Multiple arguments
Some of the conditions can take multiple arguments. For example the
MaxDifference condition:

    auto myMonitor = Monitor(MaxDifference(10), PrintResult());
    myMonitor(1, 2);    // Prints Valid: 1
    myMonitor(-10, 10); // Prints Valid: 0

## Extracting data from structs
Often, the data you want to check against is contained in structs.
libmonitoring has utilities to extract data from structs.
Assuming you have a struct which has a member variable "i". You
can use the "Field" getter to get the member variable:

    struct MyStruct {
        int i = 0;
    };

    auto myMonitor = Monitor(Max(Field(&MyStruct::i, 10)), PrintResult());

    myMonitor(MyStruct{1});
    myMonitor(MyStruct{11});

You can also get data from calling methods in the struct. This is
done by the "Property" getter:

    struct MyStruct {
        int GetI() const { return i; }
    private:
        int i = 0;
    }

    auto myMonitor = Monitor(Max(Property(&MyStruct::GetI, 10)), PrintResult());

    myMonitor(MyStruct{1});
    myMonitor(MyStruct{11});

## Picking out some arguments
If you want to match just some of the arguments, you can pick out
arguments by using FirstArg, SecondArg, or the Args<...> getter.

    auto myMonitor = Monitor(FirstArg(Max(10)) && SecondArg(Max(5)), PrintResult());

    myMonitor(1, 1); // Prints Valid: 1
    myMonitor(11, 5); // Prints Valid: 0
    myMonitor(10, 6);  // Prints Valid: 0

## Kudos
Kudos to Jason Turner for the CMake template that was used as scaffolding.
