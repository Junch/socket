# GoogleMock + gcov + Cmake example

This repository contains an example of use of Cmake and [GoogleMock](https://code.google.com/p/googletest/downloads/detail?name=gmock-1.7.0.zip) for a C++ file.
It is mostly for personal use.

## Use

```bash
mkdir build
cd build
cmake ..
make
make test
ctest
make Test_coverage
```

## Inspiration and thanks
I copied upon many, more documented examples found on StackOverflow and:
* This [simple example](http://lembra.wordpress.com/2011/10/17/building-google-test-framework-with-cmake/), but with less hard-coded things
* This [very full and elaborate example](https://github.com/snikulov/google-test-examples/), but [working on OSX](https://github.com/snikulov/google-test-examples/issues/2)

