#pragma once
#include "AbstractDay.h"
#include <vector>

enum Direction {
	EAST, SOUTH_EAST, SOUTH_WEST, WEST, NORTH_WEST, NORTH_EAST
};

enum Color {
	WHITE = 0, BLACK = 1
};

typedef std::vector<Direction> path_t;

class Day24 : public virtual AbstractDay<std::vector<path_t>>
{
public:
	Day24() : AbstractDay("Day 24", "input/input24.txt", "input/input24_part1.txt", "input/input24_part1.txt") {}
	std::vector<path_t> parseInput(std::string& input) override;
	bool testPart1(day_t& input) override;
	std::string runPart1(day_t& input) override;
	bool testPart2(day_t& input) override;
	std::string runPart2(day_t& input) override;
};

