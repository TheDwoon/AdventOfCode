#pragma once
#include "AbstractDay.h"
#include <vector>
#include <map>

struct ContainedBag {
	unsigned short count = 0;
	std::string color;
};

struct Bag {
	std::string color;
	std::vector<ContainedBag> bags;
};

class Day07 : public virtual AbstractDay<std::map<std::string, Bag>>
{
public:
	Day07() : AbstractDay("Day 07", "input/input07.txt") {};

	std::map<std::string, Bag> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;

private:
	bool canContainBagWithColor(Bag& current, const std::string& searched_color, day_t& input);
	size_t countBagsInside(Bag& current, day_t& input);
};

