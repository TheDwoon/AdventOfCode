#include <cstdint>
#include <cstring>
#include <vector>

#include "parser.cpp"
#include "vec2.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

void runDay(char* const buffer, const int length) {
    uint64_t maxArea = 0;
    int part2 = 0;

    Parser p(buffer);
    std::vector<vec2i> redTiles;

    while (!p.eof())
    {
        vec2i pos;
        p.readNumber(pos.x);
        p.consume(',');
        p.readNumber(pos.y);
        p.readNewLine();
        redTiles.push_back(pos);
    }

    const int redTileCount = static_cast<int>(redTiles.size());
    for (int i = 0; i < redTileCount; ++i)
    {
        for (int j = i + 1; j < redTileCount; ++j)
        {
            const vec2i min = vec2i::min(redTiles[i], redTiles[j]);
            const vec2i max = vec2i::max(redTiles[i], redTiles[j]);

            const uint64_t area = static_cast<uint64_t>(max.x - min.x + 1) * static_cast<uint64_t>(max.y - min.y + 1);
            if (maxArea < area) maxArea = area;
        }
    }

    printf("%llu\n",maxArea);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
