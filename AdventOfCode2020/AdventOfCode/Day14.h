#pragma once
#include "AbstractDay.h"
#include <vector>

struct FuzzyInstruction {
    bool isMask;
    uint64_t mask;
    uint64_t maskedSet;

    uint64_t memoryAccess;
    uint64_t memoryValue;
};

class Day14 : public virtual AbstractDay<std::vector<FuzzyInstruction>> {
public:
    Day14() : AbstractDay("Day 14", "input/input14.txt") {}

    std::vector<FuzzyInstruction> parseInput(std::string& input) override;
    std::string runPart1(day_t& input) override;
    std::string runPart2(day_t& input) override;
};
