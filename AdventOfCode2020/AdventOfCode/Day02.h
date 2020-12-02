#pragma once
#include "AbstractDay.h"
#include <vector>
#include <sstream>

struct password_entry {
	unsigned short min;
	unsigned short max;
	char c;
	std::string password;
};

class Day02 : public virtual AbstractDay<std::vector<password_entry>>
{
public:
	Day02() : AbstractDay("Day 02", "input/input02.txt") {}

	day_t parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;

private:
	int countMatches(std::string& password, char c) {
		int matches = 0;
		for (int i = 0; i < password.size(); ++i)
		{
			if (password[i] == c)
				++matches;
		}

		return matches;
	}
};

