#pragma once
#include "AbstractDay.h"
#include <vector>

enum class op_code {
HALT,
ACC,
NOP,
JUMP,
ABORT
};

struct Instruction {
	op_code code;
	int argument;
};

struct Processor {
	std::vector<Instruction> program;
	size_t ic = 0;
	int acc = 0;
};

class Day08 : public virtual AbstractDay<Processor>
{
public:
	Day08() : AbstractDay("Day 08", "input/input08.txt") {}

	Processor parseInput(std::string& input) override;
	std::string runPart1(day_t& input) override;
	std::string runPart2(day_t& input) override;

private:
	op_code runForkedProcessor(Processor& processor);
};

