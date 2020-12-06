#pragma once
#include "AbstractDay.h"
#include <vector>
#include <array>

class Day06 : public virtual AbstractDay<std::vector<std::array<uint8_t, 26>>>
{
public:
	Day06() : AbstractDay("Day 06", "input/input06.txt") {}

	std::vector<std::array<uint8_t, 26>> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
};

