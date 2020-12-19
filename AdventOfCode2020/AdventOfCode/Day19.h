#pragma once
#include "AbstractDay.h"
#include <vector>
#include <map>

struct Rule {
	int id;
	std::vector<int> first;
	std::vector<int> second;
	char c;
};

struct MessageCollection {
	std::map<int, Rule> rules;
	std::vector<std::string> messages;
};

class Day19 : public virtual AbstractDay<MessageCollection>
{
public:
	Day19() : AbstractDay("Day 19", "input/input19.txt") {}
	MessageCollection parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
};

