#pragma once
#include "AbstractDay.h"
#include <vector>

class Day15 : public virtual AbstractDay<std::vector<int>>{
public:
    Day15() : AbstractDay("Day 15", "input/input15.txt") {}

    std::vector<int> parseInput(std::string &input) override;
    std::string runPart1(day_t &input) override;
    std::string runPart2(day_t &input) override;
};
