cmake_minimum_required(VERSION 3.16)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(Day01 Day01.cpp)
add_executable(Day02 Day02.cpp)
add_executable(Day03 Day03.cpp)
add_executable(Day04 Day04.cpp)
add_executable(Day05 Day05.cpp)
add_executable(Day06 Day06.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -DNRELEASE -Ofast")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=x86-64")