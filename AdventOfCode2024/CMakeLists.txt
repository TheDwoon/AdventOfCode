cmake_minimum_required(VERSION 3.16)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(Day01 Day01.cpp)
add_executable(Day02 Day02.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -DNRELEASE -Ofast")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=x86-64")