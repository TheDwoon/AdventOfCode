#pragma once
#include "AbstractDay.h"
#include <vector>

struct Field {
    uint16_t number { 0 };
    bool marked { false };
};

struct Board {
    Field fields[25];

    size_t sumUnmarked() {
        size_t sum { 0 };
        for (size_t y = 0; y < 5; ++y) {
            for (size_t x = 0; x < 5; ++x) {
                Field &field = getField(x, y);
                if (!field.marked)
                    sum += field.number;
            }
        }

        return sum;
    }

    void mark(uint16_t number) {
        for (size_t y = 0; y < 5; ++y) {
            for (size_t x = 0; x < 5; ++x) {
                Field &field = getField(x, y);
                if (field.number == number)
                    field.marked = true;
            }
        }
    }

    bool isWon() {
        for (size_t row = 0; row < 5; row++) {
            if (getField(0, row).marked
                && getField(1, row).marked
                && getField(2, row).marked
                && getField(3, row).marked
                && getField(4, row).marked)
                return true;
        }

        for (size_t col = 0; col < 5; col++) {
            if (getField(col, 0).marked
                && getField(col, 1).marked
                && getField(col, 2).marked
                && getField(col, 3).marked
                && getField(col, 4).marked)
                return true;
        }

        return false;
    }

    inline Field& getField(size_t x, size_t y) {
        return fields[y * 5 + x];
    }
};

struct Bingo {
    std::vector<uint16_t> drawnNumbers;
    std::vector<Board> boards;

    int isWon() {
        for (int i = 0; i < boards.size(); ++i)
            if (boards[i].isWon())
                return i;

       return -1;
    }
};

class Day04 : public virtual AbstractDay<Bingo> {
public:
    Day04() : AbstractDay("Day 04", "input/input04.txt") {}

    day_t parseInput(std::string& input) override;
    std::string runPart1(day_t& input) override;
    std::string runPart2(day_t& input) override;
};
