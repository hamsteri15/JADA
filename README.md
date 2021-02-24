# JADA - A library for generic stencil operations


## Overview


Although, the library is intended to be as dependency-free as possible, the following libraries are still required:

- C++20 (for co-routines)
- [CMake](https://cmake.org/) build script for cross-platform configuration (see 
  [here](#cmakeliststxt) for more info)
- [Hpx](https://stellar-group.org/libraries/hpx/) A runtime scheduler to handle all kinds of things parallel
- [Boost](https://www.boost.org/) Extension to standard library

In addition the following libraries are used but they ship alongside the library

- [Catch](https://github.com/catchorg/Catch2) unit testing framework (see 
  [here](#mytestscpp))

The following tools are also used but are not mandatory

- [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) for automated 
  source code formatting (see [here](#clang-format))
- [Clang-Tidy](http://clang.llvm.org/extra/clang-tidy/) for static analysis to 
  catch coding errors (see [here](#clang-tidy))




### Linux

```bash
$ mkdir build
$ cd build
$ cmake ..
```
