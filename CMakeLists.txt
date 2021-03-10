# project
###################################################################################################
cmake_minimum_required(VERSION 3.10)
project(SeeCheatSheet VERSION 3.0)

# prepare
###################################################################################################
function(download_file url filename)
    if(NOT EXISTS ${filename})
        message("Downloading ... " ${filename})
        file(DOWNLOAD ${url} ${filename}
            INACTIVITY_TIMEOUT 10
            SHOW_PROGRESS
            STATUS statu)
        list(GET ${statu} 0 exitCode)
        message(FATAL_ERROR ${exitCode})
        if(${exitCode} NOT EQUAL 0)
            list(GET ${statu} 1 exitInfo)
            message(FATAL_ERROR ${exitInfo})
        endif()
        message("Downloaded")
    endif()
endfunction(download_file)

download_file(http://github.com/mrbeardad/Practices/raw/master/include/mine.hpp
    ${PROJECT_SOURCE_DIR}/include/mine.hpp)

download_file(https://github.com/mrbeardad/unicode_display_width/raw/master/include/unicode/display_width.hpp
    ${PROJECT_SOURCE_DIR}/include/unicode/display_width.hpp)

message("[33m===>[m You must install source-highlight libraries by your package manager!")

# build type
###################################################################################################
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-g3 -ggdb3 -rdynamic)
endif()
message("[34m===>[m Build Type: " ${CMAKE_BUILD_TYPE})

# targets
###################################################################################################
add_library(mkdhilit STATIC src/see.cpp src/normal.cpp)
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

