# project
cmake_minimum_required(VERSION 3.10)
project(SeeCheatSheet VERSION 3.0)

# prepare
message(WARNING "You may need to execute `git submodule update --init --recursive`")
message(WARNING "You must install source-highlight libraries by yourself!")
message("Build Type: " ${CMAKE_BUILD_TYPE})

# compile_commands.json
if(NOT CMAKE_EXPORT_COMPILE_COMMANDS EQUAL 0)
    add_compile_options(
        -O2
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pedantic
        -Wno-global-constructors
        -Wno-exit-time-destructors
    )
endif()

# targets
file(GLOB MKDHILIT_FILES src/mkdhilit/*.cpp)
add_library(mkdhilit STATIC ${MKDHILIT_FILES})
add_executable(see src/see.cpp)
add_executable(see_test src/test/test.cpp)

# target: mkdhilit
target_include_directories(mkdhilit PUBLIC src)
target_include_directories(mkdhilit PRIVATE src/third_party/unicode_display_width/include)
target_link_libraries(mkdhilit PRIVATE source-highlight)
target_compile_features(mkdhilit PRIVATE cxx_std_17)

# target: see
target_include_directories(see PRIVATE src/third_party/unicode_display_width/include)
target_link_libraries(see PRIVATE mkdhilit)
target_compile_features(see PRIVATE cxx_std_17)

# target: see_test
target_link_libraries(see_test PRIVATE mkdhilit gtest gtest_main)
target_compile_features(see_test PRIVATE cxx_std_17)

# custom_target: test
add_custom_target(test see_test DEPENDS see_test)

# custom_target: prof
add_custom_target(prof
    env LD_PRELOAD=/usr/lib/libtcmalloc_and_profiler.so CPUPROFILE=prof.out CPUPROFILE_FREQUENCY=1000 see -p e > /dev/null
    COMMAND pprof --web see prof.out
    DEPENDS see)

# install
install(TARGETS see DESTINATION $ENV{HOME}/.local/bin)
