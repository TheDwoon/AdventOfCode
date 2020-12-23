#pragma once
#include "AbstractDay.h"
#include <list>

typedef int cup_t;
typedef std::list<cup_t> stack_t;

class Day23 : public virtual AbstractDay<stack_t> {
public:
    Day23() : AbstractDay("Day 23", "input/input23.txt") {}
    stack_t parseInput(std::string &input) override;
    std::string runPart1(stack_t &input) override;
    std::string runPart2(stack_t &input) override;
};
