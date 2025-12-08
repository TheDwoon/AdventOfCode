#include <cstdint>
#include <cstring>
#include <numeric>

#include "util.cpp"
#include "parser.cpp"


constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    uint64_t part2 = 0;

    Parser p(buffer);
    int offset;
    p.findNext("\n", offset);

    const aoc::map map(buffer, offset, length / (offset + 1), offset + 1);

    uint64_t* input = new uint64_t[map.width]{};
    uint64_t* output = new uint64_t[map.width]{};

    for (int x = 0; x < map.width; x++)
        if (map(x, 0) == 'S') input[x] = 1;

    for (int y = 1; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            if (input[x] > 0)
            {
                if (map(x, y) == '^')
                {
                    ++part1;

                    output[x - 1] += input[x];
                    output[x + 1] += input[x];
                } else
                {
                    output[x] += input[x];
                }
            }
        }

        std::memset(input, 0, map.width * sizeof(uint64_t));
        std::swap(input, output);
    }

    part2 = std::accumulate(input, input + map.width, 0ULL);

    delete[] input;
    delete[] output;

    printf("%d\n",part1);
    printf("%llu\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
