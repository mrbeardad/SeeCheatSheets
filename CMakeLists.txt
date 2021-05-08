# project
###################################################################################################
cmake_minimum_required(VERSION 3.10)
project(SeeCheatSheet VERSION 3.0)

# prepare
###################################################################################################
message("[33m===>[m You must install source-highlight libraries by your package manager!")

# build type
###################################################################################################
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-g3 -ggdb3 -rdynamic)
endif()
message("[34m===>[m Build Type: " ${CMAKE_BUILD_TYPE})

# targets
###################################################################################################
add_library(mkdhilit STATIC src/see.cpp src/code.cpp src/comment.cpp src/header.cpp src/list.cpp
    src/normal.cpp src/quote.cpp src/seperator.cpp src/table.cpp)
add_executable(see src/main.cpp)
add_executable(see_test test/test.cpp)

# target: mkdhilit
###################################################################################################
target_include_directories(mkdhilit
    PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(mkdhilit
    PRIVATE source-highlight)
target_compile_features(mkdhilit
    PRIVATE cxx_std_20)

# target: see
###################################################################################################
target_link_libraries(see
    PRIVATE mkdhilit)
target_compile_features(see
    PRIVATE cxx_std_20)

# target: see_test
###################################################################################################
target_link_libraries(see_test
    PRIVATE mkdhilit gtest gtest_main)
target_compile_features(see_test
    PRIVATE cxx_std_20)

# custom_target: test
###################################################################################################
add_custom_target(test
    ./see_test)
add_dependencies(test see_test)

# custom_target: prof
###################################################################################################
add_custom_target(prof
    env LD_PRELOAD=/usr/lib/libprofiler.so CPUPROFILE=prof.out CPUPROFILE_FREQUENCY=1000 ./see -p e > /dev/null
    COMMAND pprof --web see prof.out)
add_dependencies(prof see)

# custom_target: prof_heap
###################################################################################################
add_custom_target(prof_heap
    env LD_PRELOAD=/usr/lib/libprofiler.so HEAPPROFILE=heap.out ./see -p e
    COMMAND pprof see heap.out* --web)
add_dependencies(prof_heap see)

###################################################################################################
install(TARGETS see DESTINATION $ENV{HOME}/.local/bin)

