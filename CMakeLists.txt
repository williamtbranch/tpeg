cmake_minimum_required(VERSION 3.16)

project(tpeg VERSION 0.01)

set(SOURCES
    src/tpeg.cpp
)

add_executable(tpeg ${SOURCES})

target_include_directories(tpeg
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)