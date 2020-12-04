#pragma once
#include "AbstractDay.h"
#include <vector>
#include <map>

const std::string byr("byr"); // (Birth Year)
const std::string iyr("iyr"); // (Issue Year)
const std::string eyr("eyr"); // (Expiration Year)
const std::string hgt("hgt"); // (Height)
const std::string hcl("hcl"); // (Hair Color)
const std::string ecl("ecl"); // (Eye Color)
const std::string pid("pid"); // (Passport ID)
const std::string cid("cid"); // (Country ID)

class Day04 : public virtual AbstractDay<std::vector<std::map<std::string, std::string>>>
{
public:
	Day04() : AbstractDay("Day 04", "input/input04.txt") {}

	std::vector<std::map<std::string, std::string>> parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;
};

