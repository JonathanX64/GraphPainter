cmake_minimum_required(VERSION 3.12)
project(coursach_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(coursach_2
        Graph.cpp
        Graph.hpp
        main.cpp)
