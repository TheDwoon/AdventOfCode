#pragma once
#include "AbstractDay.h"
#include <vector>
#include <array>

struct group_vote {
	uint16_t voters = 0;
	std::vector<std::array<uint8_t, 32>> votes;
};

class Day06 : public virtual AbstractDay<std::vector<group_vote>>
{
public:
	Day06() : AbstractDay("Day 06", "input/input06.txt") {}

	std::vector<group_vote> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;
};

