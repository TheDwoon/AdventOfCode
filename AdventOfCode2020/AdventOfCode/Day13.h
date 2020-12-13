#pragma once
#include "AbstractDay.h"
#include <vector>

class Day13 : public virtual AbstractDay<std::vector<int>> {
public:
    Day13() : AbstractDay("Day 13", "input/input13.txt") {}

    std::vector<int> parseInput(std::string& input) override;
    std::string runPart1(day_t& input) override;
    std::string runPart2(day_t& input) override;
};
