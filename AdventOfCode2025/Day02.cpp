#include <cstring>
#include <vector>
#include <cmath>
#include <cstdint>
#include <pstl/execution_defs.h>

#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct range
{
    uint64_t start;
    uint64_t end;
};

bool isFakeId(const uint64_t number)
{
    const int magnitude = static_cast<int>(std::log10(number)) + 1;
    if (magnitude % 2 == 1)
        return false;

    const int sword = static_cast<int>(std::pow(10, magnitude / 2));
    return number / sword == number % sword;
}

bool sliceNumber(uint64_t number, const int slice)
{
    const uint64_t pattern = number % slice;
    number /= slice;
    while (number > 0)
    {
        if (number % slice != pattern)
            return false;

        number /= slice;
    }

    return true;
}

bool isAdvancedFake(const uint64_t number)
{
    const int magnitude = static_cast<int>(std::log10(number)) + 1;
    int swordMagnitude = magnitude / 2;
    int sword = static_cast<int>(std::pow(10, swordMagnitude));
    while (sword > 1)
    {
        if (magnitude % swordMagnitude == 0 && sliceNumber(number, sword))
            return true;

        swordMagnitude--;
        sword /= 10;
    }

    return false;
}

void runDay(char* const buffer, const int length) {
    uint64_t part1 = 0;
    uint64_t part2 = 0;

    std::vector<range> ranges;
    Parser p(buffer);
    while (!p.isNewLine())
    {
        range r;
        p.readNumber(r.start);
        p.consume('-');
        p.readNumber(r.end);
        p.consume(',');
        ranges.push_back(r);
    }

    for (const auto& r : ranges)
    {
        for (uint64_t i = r.start; i <= r.end; ++i)
        {
            if (isFakeId(i))
            {
                part1 += i;
                part2 += i;
            }
            else if (isAdvancedFake(i))
            {
                part2 += i;
            }
        }
    }

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
