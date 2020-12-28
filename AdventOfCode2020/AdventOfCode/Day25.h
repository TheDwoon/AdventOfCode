#pragma once
#include "AbstractDay.h"

#define SUBJECT_NUMBER 7
#define SUBJECT_DIVIDER 20201227

typedef uint64_t public_key_t;

struct Keys {
    public_key_t door;
    public_key_t card;
};

class Day25 : public AbstractDay<Keys> {
public:
    Day25() : AbstractDay("Day 25", "input/input25.txt", "input/input25_test.txt") {}
    Keys parseInput(std::string &input) override;
    bool testPart1(Keys &input) override;
    std::string runPart1(Keys &input) override;
};
