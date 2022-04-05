# project
cmake_minimum_required(VERSION 3.10)
project(SeeCheatSheet VERSION 3.0)

# prepare
message("Build Type: " ${CMAKE_BUILD_TYPE})
message(WARNING "You must install source-highlight libraries by yourself!")

# compile_commands.json
if(DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
    add_compile_options(
        -O2
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pedantic
        -Wno-missing-prototypes
        -Wno-padded
        -Wno-old-style-cast
        -Wno-global-constructors
        -Wno-exit-time-destructors
    )
endif()

# targets
file(GLOB MKDHILIT_FILES src/blocks/*.cpp)
add_library(mkdhilit STATIC ${MKDHILIT_FILES})
add_executable(see src/see.cpp)
add_executable(see_test src/test/test.cpp)

# target: mkdhilit
###################################################################################################
target_include_directories(mkdhilit PUBLIC src/include)
target_link_libraries(mkdhilit PRIVATE source-highlight)
target_compile_features(mkdhilit PRIVATE cxx_std_20)

# target: see
###################################################################################################
target_link_libraries(see PRIVATE mkdhilit)
target_compile_features(see PRIVATE cxx_std_20)

# target: see_test
###################################################################################################
target_link_libraries(see_test PRIVATE mkdhilit gtest gtest_main)
target_compile_features(see_test PRIVATE cxx_std_20)

# custom_target: test
###################################################################################################
add_custom_target(test ./see_test)
add_dependencies(test see_test)

# custom_target: prof
###################################################################################################
add_custom_target(prof
    env LD_PRELOAD=/usr/lib/libtcmalloc_and_profiler.so CPUPROFILE=prof.out CPUPROFILE_FREQUENCY=1000 ./see -p e > /dev/null
    COMMAND pprof --web see prof.out)
add_dependencies(prof see)

###################################################################################################
install(TARGETS see DESTINATION $ENV{HOME}/.local/bin)

