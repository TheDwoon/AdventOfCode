#pragma once
#include "AbstractDay.h"
#include <vector>

class Day03 : public virtual AbstractDay<std::vector<std::string>> {
public:
    Day03() : AbstractDay("Day 03", "input/input03.txt", "input/input03_t1.txt") {}

    day_t parseInput(std::string& input) override;
    bool testPart1(std::vector<std::string> &input) override;
    std::string runPart1(day_t& input) override;
    std::string runPart2(day_t& input) override;
};
