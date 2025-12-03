#include <cstdint>
#include <cstring>
#include <pstl/execution_defs.h>

#include "parser.cpp"
#include "util.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

void findHighestInInterval(const aoc::map<char> &bank, const int y, const int startX, const int endX, char &highest, int &matchedX)
{
    highest = bank(startX, y);
    matchedX = startX;
    for (int x = startX + 1; x < endX && highest < '9'; x++)
    {
        if (bank(x, y) > highest)
        {
            highest = bank(x, y);
            matchedX = x;
        }
    }
}

template<int activeBatteries>
uint64_t computeJoltage(const aoc::map<char> &banks)
{
    uint64_t score = 0;
    char bestCombination[activeBatteries];

    for (int y = 0; y < banks.height; y++)
    {
        std::memset(bestCombination, 0, activeBatteries);

        int x = -1;
        for (int battery = 0; battery < activeBatteries; battery++)
        {
            findHighestInInterval(banks, y, x + 1, banks.width - activeBatteries + battery + 1, bestCombination[battery], x);
        }

        // if (banks(banks.width - 1, y) > bestCombination[1])
        //     bestCombination[1] = banks(banks.width - 1, y);

        uint64_t joltage = 0;
        uint64_t multiplier = 1;
        for (int battery = activeBatteries - 1; battery >= 0; --battery)
        {
            joltage += multiplier * (bestCombination[battery] - '0');
            multiplier *= 10;
        }

        score += joltage;
    }

    return score;
}

void runDay(char* const buffer, const int length) {
    uint64_t part1 = 0;
    uint64_t part2 = 0;

    Parser p(buffer);
    int offset;
    p.findNext("\n", offset);

    const aoc::map banks(buffer, offset, length / (offset + 1), offset + 1);

    part1 = computeJoltage<2>(banks);
    part2 = computeJoltage<12>(banks);

    printf("%llu\n",part1);
    printf("%llu\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
