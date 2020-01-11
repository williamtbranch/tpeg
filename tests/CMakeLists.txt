cmake_minimum_required(VERSION 3.1)

project(TpegTests VERSION 0.1
    DESCRIPTION "Test Suite for the tpeg class and utilities"
    LANGUAGES CXX)

enable_testing()
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)

add_executable(tests tests.cpp)
add_library(main main.cpp)
add_library(parse_machine ../src/parse_machine.cpp)
add_library(bc_parse ../src/bc_parse.cpp)
target_link_libraries(
  tests
  main
  parse_machine
  bc_parse
  Catch2::Catch2)
 catch_discover_tests(tests)
#  include(ParseAndAddCatchTests)
#  ParseAndAddCatchTests(tests)

include_directories(../include .)

