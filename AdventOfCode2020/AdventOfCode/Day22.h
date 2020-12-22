#pragma once
#include "AbstractDay.h"
#include <deque>

typedef int card_t;
typedef std::deque<card_t> deck_t;

struct Game {
    deck_t player1;
    deck_t player2;

    bool operator<(const Game& other) const {
        return player1 < other.player1 || (player1 == other.player1 && player2 < other.player2);
    }
};

enum Winner {
    PLAYER_1, PLAYER_2
};

class Day22 : public virtual AbstractDay<Game> {
public:
    Day22() : AbstractDay("Day 22", "input/input22.txt") {}
    Game parseInput(std::string &input) override;
    std::string runPart1(Game &input) override;
    std::string runPart2(Game &input) override;
};
