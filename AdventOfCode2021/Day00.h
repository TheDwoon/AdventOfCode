#pragma once
#include "AbstractDay.h"

class Day00 : public virtual AbstractDay<std::string>
{
public:
	Day00() : AbstractDay("Day 00", "input/day00.txt") {};

	day_t parseInput(std::string& input) override {
		return input;
	}

	std::string runPart1(day_t& input) override {
		return input;
	}
};

