cmake_minimum_required(VERSION 3.10)
project(SeeCheatSheet VERSION 2.0)

add_library(libsee.a STATIC see.cpp)
add_executable(see main.cpp)

target_compile_features(libsee.a PRIVATE cxx_std_17)
target_include_directories(libsee.a
    PUBLIC ${PROJECT_SOURCE_DIR}            # <see.hpp>
    PUBLIC ${PROJECT_BINARY_DIR}/include)   # <mine.cpp> <unicode/display_width.hpp>
target_link_libraries(libsee.a PRIVATE source-highlight)

target_compile_features(see PRIVATE cxx_std_17)
target_link_libraries(see PRIVATE libsee.a)

install(TARGETS see DESTINATION $ENV{HOME}/.local/bin)

