cmake_minimum_required(VERSION 3.16)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(Day01 Day01.cpp)
add_executable(Day01Y Day01Y.cpp)
add_executable(Day02 Day02.cpp parser.cpp)
add_executable(Day03 Day03.cpp parser.cpp)
add_executable(Day04 Day04.cpp parser.cpp)
add_executable(Day05 Day05.cpp parser.cpp)
add_executable(Day06 Day06.cpp parser.cpp)
add_executable(Day07 Day07.cpp parser.cpp)
add_executable(Day08 Day08.cpp parser.cpp)
add_executable(Day09 Day09.cpp parser.cpp)
add_executable(Day10 Day10.cpp parser.cpp vec2.cpp)
add_executable(Day11 Day11.cpp parser.cpp vec2.cpp)


SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=native")