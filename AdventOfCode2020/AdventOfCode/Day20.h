#pragma once
#include "AbstractDay.h"
#include <vector>

#define EMPTY_SPOT '.'
#define FULL_SPOT '#'
#define TILE_SIZE 10
#define ACCESS_TILE(map, x, y) (map[(y) * TILE_SIZE + (x)])

struct Tile {
    int number;
    char map[TILE_SIZE * TILE_SIZE];
    unsigned short leftSignature;
    unsigned short leftFlippedSignature;
    unsigned short rightSignature;
    unsigned short rightFlippedSignature;
    unsigned short topSignature;
    unsigned short topFlippedSignature;
    unsigned short bottomSignature;
    unsigned short bottomFlippedSignature;

    bool contains(unsigned short i) const {
        return i == leftSignature || i == leftFlippedSignature || i == rightSignature || i == rightFlippedSignature
            || i == topSignature || i == topFlippedSignature || i ==  bottomSignature || i == bottomFlippedSignature;
    }
};

class Day20 : public virtual AbstractDay<std::vector<Tile>> {
public:
    Day20() : AbstractDay("Day 20", "input/input20.txt") {}
    std::vector<Tile> parseInput(std::string &input) override;
    std::string runPart1(std::vector<Tile> &input) override;
};
