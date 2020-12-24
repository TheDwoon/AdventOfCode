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

    bool operator==(const Game& other) const {
        return player1 == other.player1 && player2 == other.player2;
    }
};

// define hash objects for deck_t and Game
namespace std {
    template <> struct hash<deck_t> {
        std::size_t operator()(const deck_t& deck) const noexcept {
            size_t hash = 0;
            for (const card_t &card : deck) {
                hash ^= card;
            }

            return hash;
        }
    };

    template<> struct hash<Game> {
        std::size_t operator()(const Game& game) const noexcept {
            return std::hash<deck_t>{}(game.player1) ^ std::hash<deck_t>{}(game.player2);            
        }
    };
}

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
