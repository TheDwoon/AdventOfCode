#pragma once
#include "AbstractDay.h"
#include <vector>

class Day10 : public virtual AbstractDay<std::vector<int>> {
public:
    Day10() : AbstractDay("Day 10", "input/input10.txt") {}

    std::vector<int> parseInput(std::string& input) override;
    std::string runPart1(day_t& input) override;
    std::string runPart2(day_t& input) override;

private:
    uint64_t countPossibilities(size_t idx, size_t end, const day_t& input);
};
