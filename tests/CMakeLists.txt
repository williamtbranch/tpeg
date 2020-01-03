cmake_minimum_required(VERSION 3.1)

project(TpegTests VERSION 0.1
    DESCRIPTION "Test Suite for the tpeg class and utilities"
    LANGUAGES CXX)

find_package(Catch2 REQUIRED)

add_executable(tests ttest.cpp)
target_link_libraries(tests tpeg parse_machine Catch2::Catch2 ttest)
include(CTest)
include(Catch)
catch_discover_tests(tests)

include_directories(../include)

add_library(ttest main.cpp)
add_library(tpeg ../src/tpeg.cpp)
add_library(parse_machine ../src/parse_machine.cpp)
