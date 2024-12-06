#include <algorithm>
#include <set>
#include "parser.cpp"
#include "vec2.cpp"
#include "directions.cpp"

struct map {
    char* buffer;
    const int width;
    const int height;
    const int lineLength;

    [[nodiscard]]
    char& at(const int x, const int y) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[y * lineLength + x];
    }

    [[nodiscard]]
    char& at(const vec2i& pos) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const vec2i& pos) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const int x, const int y) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[y * lineLength + x];
    }
};

vec2i findStartingPosition(const map& m) {
    vec2i pos;
    for (pos.y = 0; pos.y < m.height; pos.y++) {
        for (pos.x = 0; pos.x < m.width; pos.x++) {
            if (m(pos.x, pos.y) == '^')
                return pos;
        }
    }

    return {-1, -1};
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    const map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };

    vec2i facing = aoc::direction::UP;
    vec2i pos = findStartingPosition(m);
    while (pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height) {
        m.at(pos) = 'X';

        if (m(pos + facing) == '#') {
            facing = aoc::direction::TURN_RIGHT * facing;
        } else {
            pos += facing;
        }
    }

    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            if (m(x, y) == 'X') {
                part1++;
            }
        }
    }

    printf("%d\n",part1);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
    char buffer[INPUT_BUFFER_SIZE];
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
