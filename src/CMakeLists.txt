cmake_minimum_required(VERSION 3.10)

project(SeeCheatSheet VERSION 1.0)

add_executable(see see.cpp)

target_compile_features(see PRIVATE cxx_std_17)

#include <mine.cpp>
#include <unicode/display_width.hpp>
target_include_directories(see PRIVATE ${PROJECT_BINARY_DIR}/include)

#include <boost/locale.hpp>
find_package(Boost 1.70
    REQUIRED
    COMPONENTS locale)

if(NOT Boost_locale_FOUND)
    message(FATAL_ERROR "didn't find libboost_locale")
else()
    target_link_libraries(see PRIVATE ${Boost_locale_LIBRARY})
endif()

#include <srchilite/sourcehighlight.h>
target_link_libraries(see PRIVATE source-highlight)

install(TARGETS see DESTINATION $ENV{HOME}/.local/bin)
