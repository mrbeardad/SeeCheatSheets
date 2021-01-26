#!/bin/sh -e

mkdir -p src/build/include/unicode
cd src/build
curl -Lo include/mine.hpp https://raw.githubusercontent.com/mrbeardad/Practices/master/include/mine.hpp
curl -Lo include/unicode/display_width.hpp https://github.com/p-ranav/unicode_display_width/raw/master/include/unicode/display_width.hpp
echo "\033[33mWarning:\033[m you must install boost and source-highlight libraries by youself!"

cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install .
