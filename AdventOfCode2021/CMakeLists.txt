cmake_minimum_required(VERSION 3.17)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(AdventOfCode
        AbstractDay.h
        AdventOfCode.cpp
        Day00.cpp
        Day00.h
        Day01.cpp Day01.h
        Day02.cpp Day02.h
        Day03.cpp Day03.h)

add_custom_command(TARGET AdventOfCode PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/input/ $<TARGET_FILE_DIR:AdventOfCode>/input/)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=native")