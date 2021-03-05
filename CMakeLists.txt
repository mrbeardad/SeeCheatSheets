cmake_minimum_required(VERSION 3.10)
project(SeeCheatSheet VERSION 2.0)

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

message(WARNING "you must install source-highlight libraries by your package manager!")

###################################################################################################

add_library(libsee.a STATIC src/see.cpp)
add_executable(see src/main.cpp)

###################################################################################################

target_include_directories(libsee.a
    PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(libsee.a
    PRIVATE source-highlight)
target_compile_features(libsee.a
    PRIVATE cxx_std_17)

###################################################################################################

target_link_libraries(see
    PRIVATE libsee.a)
target_compile_features(see
    PRIVATE cxx_std_17)

###################################################################################################

install(TARGETS see DESTINATION $ENV{HOME}/.local/bin)

