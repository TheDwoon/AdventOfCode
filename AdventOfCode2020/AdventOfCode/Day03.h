#pragma once
#include "AbstractDay.h"
#include <vector>

class Day03 : public virtual AbstractDay<std::vector<char>>
{
public:
	Day03() : AbstractDay("Day03", "input/input03.txt") {}

	std::vector<char> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;

private:
	int ride_slope(int dx, int dy, std::vector<char>& map);
	char access_map(int x, int y, std::vector<char>& map);
};

