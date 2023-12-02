cmake_minimum_required(VERSION 3.16)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(Day01 Day01.cpp)
add_executable(Day01Y Day01Y.cpp)
add_executable(Day02 Day02.cpp parser.cpp)


SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=native")