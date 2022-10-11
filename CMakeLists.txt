cmake_minimum_required(VERSION 3.22)
project(stippling)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(stippling
        circle_packing.cpp
        circle_packing.h
        )
