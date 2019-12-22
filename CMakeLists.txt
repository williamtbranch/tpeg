cmake_minimum_required(VERSION 3.16)

project(tpeg VERSION 0.01)

include_directories(include)
set(SOURCES
    src/runner.cpp
    src/tpeg.cpp
    src/pegtest.cpp
)

add_executable(runner ${SOURCES})