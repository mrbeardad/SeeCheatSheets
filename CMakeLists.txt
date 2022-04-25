# project
cmake_minimum_required(VERSION 3.10)
project(SeeCheatSheet VERSION 3.0)

# prepare
message("You may need to execute `git submodule update --init --recursive`")
message("You must install source-highlight library and boost library by yourself!")
message("Build Type: " ${CMAKE_BUILD_TYPE})
message("C++ Compiler: " ${CMAKE_CXX_COMPILER})
message("Export compile_commands.json" ${CMAKE_EXPORT_COMPILE_COMMANDS})

# global compile options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra")
configure_file(src/project.h.in project.hpp)
include_directories(src ${CMAKE_CURRENT_BINARY_DIR})

# targets
file(GLOB MKDHILIT_FILES src/mkdhilit/*.cpp)
add_library(mkdhilit STATIC ${MKDHILIT_FILES})

file(GLOB UTILS_FILES src/utils/*.cpp)
add_library(utils STATIC ${UTILS_FILES})

add_executable(see src/see.cpp)

add_custom_target(prof
    env LD_PRELOAD=/usr/lib/libtcmalloc_and_profiler.so CPUPROFILE=prof.out CPUPROFILE_FREQUENCY=1000 ./see -p e > /dev/null
    COMMAND pprof --web ./see prof.out
    DEPENDS see)

file(GLOB TEST_FILES src/test/*.cpp)
add_executable(test_bin ${TEST_FILES})
add_custom_target(test ./test_bin DEPENDS test_bin)

# target: mkdhilit
target_include_directories(mkdhilit PRIVATE src/third_party/unicode_display_width/include)
target_link_libraries(mkdhilit PRIVATE source-highlight)

# target: utils
target_link_libraries(utils PRIVATE boost_locale)

# target: see
target_include_directories(see PRIVATE src/third_party/unicode_display_width/include)
target_link_libraries(see PRIVATE mkdhilit)

# target: test_bin
target_link_libraries(test_bin PRIVATE utils gtest gtest_main pthread)

# install
install(TARGETS see DESTINATION $ENV{HOME}/.local/bin)
