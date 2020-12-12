#pragma once
#include "AbstractDay.h"
#include <vector>
#include "vec2.h"

#define EAST 'E'
#define SOUTH 'S'
#define WEST 'W'
#define NORTH 'N'

#define FORWARD 'F'
#define LEFT 'L'
#define RIGHT 'R'

struct MovementInstruction {
    char operation;
    int distance;
};

class Day12 : public virtual AbstractDay<std::vector<MovementInstruction>> {
public:
    Day12() : AbstractDay("Day 12", "input/input12.txt") {}

    std::vector<MovementInstruction> parseInput(std::string& input) override;
    std::string runPart1(day_t& input) override;
    std::string runPart2(day_t& input) override;
};
