imgutils
========

Utilities to work with images.

They are too small to deserve their own project.

Building
--------

To compile, use

    $ mkdir build
    $ cd build
    $ cmake .. [-D CMAKE_CXX_COMPILER=/path/of/c++/compiler -D CMAKE_C_COMPILER=/path/of/c/compiler] [-D CMAKE_BUILD_TYPE=Debug]
    $ make

To rebuild, e.g. when the code is modified, use

    $ cd build
    $ make
