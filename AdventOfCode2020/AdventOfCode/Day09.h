#pragma once
#include "AbstractDay.h"
#include <vector>

class Day09 : public virtual AbstractDay<std::vector<uint64_t>>
{
public:
	Day09() : AbstractDay("Day 09", "input/input09.txt") {}

	std::vector<uint64_t> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;

private:
	bool validateXMAS(const uint64_t* v);
};

