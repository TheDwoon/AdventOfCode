cmake_minimum_required(VERSION 3.17)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(AdventOfCode
        AbstractDay.h
        AdventOfCode.cpp
        Day00.cpp
        Day00.h
        Day01.cpp
        Day01.h
        Day02.cpp
        Day02.h
        Day03.cpp
        Day03.h
        Day04.cpp
        Day04.h
        Day05.cpp
        Day05.h
        Day06.cpp
        Day06.h
        Day07.cpp
        Day07.h
        Day08.cpp
        Day08.h
        Day09.cpp
        Day09.h Day10.cpp Day10.h Day11.cpp Day11.h)

add_custom_command(TARGET AdventOfCode PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/input/ $<TARGET_FILE_DIR:AdventOfCode>/input/)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=native")