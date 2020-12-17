#pragma once
#include "AbstractDay.h"
#include <vector>

#define INPUT_SIZE 8

class Day17 : public virtual AbstractDay<std::vector<char>>{
public:
    Day17() : AbstractDay("Day 17", "input/input17.txt") {}

    std::vector<char> parseInput(std::string &input) override;
    std::string runPart1(day_t &input) override;
};
