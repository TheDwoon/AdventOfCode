#pragma once
#include "AbstractDay.h"
#include <array>

#define WIDTH 95
#define HEIGHT 90
//#define WIDTH 10
//#define HEIGHT 10

#define FLOOR 0
#define EMPTY_SEAT 2
#define FULL_SEAT 3

class Day11 : public virtual AbstractDay<std::array<char, HEIGHT * WIDTH>>{
public:
    Day11() : AbstractDay("Day 11", "input/input11.txt") {}

    std::array<char, HEIGHT * WIDTH> parseInput(std::string& input) override;
    std::string runPart1(day_t& input) override;
    std::string runPart2(day_t& input) override;

private:
    char nextState(int x, int y, const day_t map) const;
    bool isFull(int x, int y, const day_t& map) const;
    bool isFullRay(int x, int y, int dx, int dy, const day_t& map) const;
};

