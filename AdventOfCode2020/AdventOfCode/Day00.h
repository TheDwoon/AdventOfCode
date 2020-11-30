#pragma once
#include "AbstractDay.cpp"

class Day00 : public virtual AbstractDay<std::string>
{
public:
	Day00() : AbstractDay("input/day00.txt") {};

	day_t parseInput(std::string& input) override {
		return input;
	}

	std::string runPart1(day_t& input) override {
		return input;
	}
};

