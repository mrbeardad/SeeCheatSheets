#!/bin/bash -e

mkdir -p src/build/include/unicode
cd src/build
if [[ ! -e include/mine.hpp ]] ;then
    curl -Lo include/mine.hpp https://raw.githubusercontent.com/mrbeardad/Practices/master/include/mine.hpp
fi
if [[ ! -e include/unicode/display_width.hpp ]] ;then
    curl -Lo include/unicode/display_width.hpp https://github.com/mrbeardad/unicode_display_width/raw/master/include/unicode/display_width.hpp
fi
echo "\033[33mWarning:\033[m you must install source-highlight libraries by your package manager!"

cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install .
