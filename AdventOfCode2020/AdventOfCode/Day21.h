#pragma once
#include "AbstractDay.h"
#include <vector>

struct Food {
    std::vector<std::string> ingredients;
    std::vector<std::string> allergen;
};

class Day21 : public virtual AbstractDay<std::vector<Food>> {
public:
    Day21() : AbstractDay("Day 21" , "input/input21.txt") {}
    std::vector<Food> parseInput(std::string &input) override;
    std::string runPart1(day_t &input) override;
    std::string runPart2(day_t &input) override;
};
