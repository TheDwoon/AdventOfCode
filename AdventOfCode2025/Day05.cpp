#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>
#include <set>

#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct range
{
    uint64_t start;
    uint64_t end;
};

bool operator<(const range& lhs, const range& rhs) { return lhs.start < rhs.start; }

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    uint64_t part2 = 0;

    std::vector<range> validRanges;
    std::vector<uint64_t> numbers;

    Parser p(buffer);
    while (!p.isNewLine())
    {
        range r;
        p.readNumber(r.start);
        p.consume('-');
        p.readNumber(r.end);
        p.readNewLine();
        validRanges.push_back(r);
    }

    p.readNewLine();
    while (!p.eof())
    {
        uint64_t n;
        p.readNumber(n);
        p.readNewLine();
        numbers.push_back(n);
    }

    std::sort(validRanges.begin(), validRanges.end());
    std::ranges::sort(numbers);
    std::vector<range> distinctValidRanges;

    range currentRange = validRanges[0];
    for (size_t i = 1; i < validRanges.size(); ++i)
    {
        if (validRanges[i].start <= currentRange.end)
        {
            currentRange.start = std::min(currentRange.start, validRanges[i].start);
            currentRange.end = std::max(currentRange.end, validRanges[i].end);
        } else
        {
            distinctValidRanges.push_back(currentRange);
            currentRange = validRanges[i];
        }
    }

    distinctValidRanges.push_back(currentRange);

    auto numberIt = numbers.begin();
    auto rangeIt = distinctValidRanges.begin();

    while (numberIt != numbers.end() && rangeIt != distinctValidRanges.end())
    {
        if (*numberIt < rangeIt->start)
            ++numberIt;
        else if (*numberIt > rangeIt->end)
            ++rangeIt;
        else if (rangeIt->start <= *numberIt && *numberIt <= rangeIt->end)
        {
            ++part1;
            ++numberIt;
        }
    }

    for (const auto& r : distinctValidRanges)
    {
        part2 += r.end - r.start + 1;
    }

    printf("%d\n",part1);
    printf("%llu\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    printf("ready!\n");
    fflush(stdout);

    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
