#include <algorithm>
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

int findFakeRange(const std::vector<uint64_t>& cache, const uint64_t number)
{
    const auto it = std::lower_bound(cache.begin(), cache.end(), number);
    return static_cast<int>(it - cache.begin());
}

void runDay(char* const buffer, const int length, const std::vector<uint64_t>& cachePart1, const std::vector<uint64_t>& cachePart2) {
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
        int index = findFakeRange(cachePart1, r.start);
        while (index < cachePart1.size() && cachePart1[index] <= r.end)
        {
            part1 += cachePart1[index];
            index++;
        }
        // for (uint64_t i = r.start; i <= r.end; ++i)
        // {
        //     if (isFakeId(i))
        //     {
        //         part1 += i;
        //         part2 += i;
        //     }
        //     else if (isAdvancedFake(i))
        //     {
        //         part2 += i;
        //     }
        // }
    }

    printf("%llu\n",part1);
    printf("%llu\n",part2);
}

constexpr uint64_t pow10(int x)
{
    uint64_t result {1};
    while (x-- > 0)
        result *= 10;

    return result;
}

uint64_t constructFakeId(const uint64_t segment, const uint64_t factor, const int repeat)
{
    uint64_t fakeId = segment;
    for (int i = 1; i < repeat; ++i)
    {
        fakeId = fakeId * factor + segment;
    }

    return fakeId;
}

void generateFakeIds(std::vector<uint64_t>& fakeIds, const int magnitude, const int repeat)
{
    const uint64_t start = pow10(magnitude);
    const uint64_t end = pow10(magnitude + 1) - 1;
    const uint64_t factor = pow10(magnitude + 1);

    for (uint64_t segment = start; segment <= end; ++segment)
    {
        const uint64_t fakeId = constructFakeId(segment, factor, repeat);
        fakeIds.push_back(fakeId);
    }
}

void buildAccelerationStructure(std::vector<uint64_t>& cachePart1, std::vector<uint64_t>& cachePart2)
{
    // Generate only symmetric cases
    for (int magnitude = 0; magnitude <= 6; ++magnitude)
    {
        generateFakeIds(cachePart1, magnitude, 2);
    }

    cachePart2 = cachePart1;

    // Generate other cases
    for (int magnitude = 1; magnitude <= 6; ++magnitude)
    {
        for (int repeat = 3; repeat <= 10 && repeat * magnitude <= 10; ++repeat)
        {
            generateFakeIds(cachePart2, magnitude, repeat);
        }
    }
}

// BOILER PLATE CODE BELOW

int main()
{
    std::vector<uint64_t> cachePart1;
    std::vector<uint64_t> cachePart2;
    buildAccelerationStructure(cachePart1, cachePart2);

    std::ranges::sort(cachePart1);
    // std::ranges::sort(cachePart2);

    printf("ready!\n");
    fflush(stdout);

    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length, cachePart1, cachePart2);
}
