#pragma once
#include "AbstractDay.h"
#include <vector>

class Day01 : public virtual AbstractDay<std::vector<int>>
{
public:
	Day01() : AbstractDay("Day 01", "input/input01.txt") {}

	day_t parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;
};

