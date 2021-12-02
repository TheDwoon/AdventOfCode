#pragma once
#include "AbstractDay.h"
#include <vector>

struct command {
    std::string command { 0 };
    int movement { 0 };
};

class Day02 : public virtual AbstractDay<std::vector<command>>
{
public:
	Day02() : AbstractDay("Day 02", "input/input02.txt") {}

	day_t parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;
};

