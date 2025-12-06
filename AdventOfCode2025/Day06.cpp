#include <array>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <vector>

#include "util.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct exercise
{
    uint8_t length { 0 };
    std::array<uint64_t, 4> numbers {};
    char op {'?'};
};

uint64_t parseRowNumber(const aoc::map<const char>& exercises, int x, const int y)
{
    uint64_t number { 0 };
    while (x < exercises.width && exercises(x, y) == ' ')
        x++;

    while (x < exercises.width && exercises(x, y) != ' ')
    {
        number *= 10;
        number += exercises(x, y) - '0';
        x++;
    }

    return number;
}

uint64_t cephalopodsMath(const aoc::map<const char>& exercises, const int x, char op)
{
    uint64_t sum {};

    switch (op)
    {
    case '+':
        sum = 0;
        for (int y = 0; y < exercises.height - 1; y++)
            sum += parseRowNumber(exercises, x, y);

        break;
    case '*':
        sum = 1;
        for (int y = 0; y < exercises.height - 1; y++)
            sum *= parseRowNumber(exercises, x, y);

        break;
    }

    return sum;
}

uint64_t parseColumnNumber(const aoc::map<const char>& exercises, const int x)
{
    uint64_t number { 0 };
    for (int y = 0; y < exercises.height - 1; y++)
    {
        if (exercises(x, y) != ' ')
        {
            number *= 10;
            number += exercises(x, y) - '0';
        }
    }

    return number;
}

uint64_t actualCephalopodsMath(const aoc::map<const char>& exercises, int x, char op)
{
    uint64_t sum {};

    int maxX = x + 1;
    while (maxX < exercises.width && exercises(maxX, exercises.height - 1) == ' ')
        maxX++;

    if (maxX < exercises.width)
        maxX--;

    switch (op)
    {
    case '+':
        sum = 0;
        for (; x < maxX; x++)
            sum += parseColumnNumber(exercises, x);

        break;
    case '*':
        sum = 1;
        for (; x < maxX; x++)
            sum *= parseColumnNumber(exercises, x);

        break;
    }

    return sum;
}

void runDay(const char* const buffer, const int length) {
    uint64_t part1 = 0;
    uint64_t part2 = 0;

    const char* const end = buffer + length;

    Parser p(buffer);
    int offset;
    p.findNext("\n", offset);

    const aoc::map exercise(buffer, offset, length / (offset + 1), offset + 1);

    // Part 1
    const int operationLine = exercise.height - 1;
    int x = 0;
    while (x < exercise.width)
    {
        const char op = exercise(x, operationLine);
        part1 += cephalopodsMath(exercise, x, op);
        part2 += actualCephalopodsMath(exercise, x, op);
        x++;

        while (x < exercise.width && exercise(x, operationLine) == ' ')
            x++;
    }

    printf("%llu\n",part1);
    printf("%llu\n",part2);
}

// 6724298077865 --> too low

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
