#pragma once
#include "AbstractDay.h"
#include <vector>

class Day05 : public virtual AbstractDay<std::vector<uint16_t>>
{
public:
	Day05() : AbstractDay("Day 05", "input/input05.txt") {}

	std::vector<uint16_t> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;
};

