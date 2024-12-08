#include <algorithm>
#include <cstring>
#include <vector>
#include "parser.cpp"
#include "vec2.cpp"
#include "directions.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
constexpr char MASK_UP = 1;
constexpr char MASK_DOWN = 2;
constexpr char MASK_LEFT = 4;
constexpr char MASK_RIGHT = 8;

struct map {
    char* buffer;
    const int width;
    const int height;
    const int lineLength;

    [[nodiscard]]
    bool contains(const vec2i& pos) const {
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
    }

    [[nodiscard]]
    char& at(const int x, const int y) const {
        assert (x >= 0 && x < width);
        assert (y >= 0 && y < height);
        assert (buffer[y * lineLength + x] != '#');
        return buffer[y * lineLength + x];
    }

    [[nodiscard]]
    char& at(const vec2i& pos) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        assert (buffer[pos.y * lineLength + pos.x] != '#');
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const vec2i& pos) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const int x, const int y) const {
        assert (x >= 0 && x < width);
        assert (y >= 0 && y < height);
        return buffer[y * lineLength + x];
    }
};

int signum(const int x) {
    return (x > 0) - (x < 0);
}

void printMap(const map& m) {
    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            char c = m(x, y);
            if (c == 0)
                c = ' ';
            else if (c == '#')
                c = '#';
            else if (c == 'O')
                c = 'O';
            else if (c & MASK_UP)
                c = '^';
            else if (c & MASK_DOWN)
                c = 'v';
            else if (c & MASK_LEFT)
                c = '<';
            else if (c & MASK_RIGHT)
                c = '>';
            else
                c = '+';

            printf("%c", c);
        }
        printf("\n");
    }
    printf("=========================\n");
}

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

bool runsInCircle(vec2i pos, vec2i facing, const map &m) {
    while (pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height) {
        if (m.contains(pos + facing) && m(pos + facing) == '#') {
            char mask;
            if (facing == aoc::direction::UP) {
                mask = MASK_UP;
            } else if (facing == aoc::direction::DOWN) {
                mask = MASK_DOWN;
            } else if (facing == aoc::direction::LEFT) {
                mask = MASK_LEFT;
            } else {
                mask = MASK_RIGHT;
            }

            const char c = m(pos);
            if ((c & mask) == mask) {
                // m.at(pos) = 'O';
                // printMap(m);
                return true;
            }

            m.at(pos) = static_cast<char>(c | mask);

            facing = aoc::direction::TURN_RIGHT * facing;
        } else {
            pos += facing;
        }
    }

    return false;
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    char originalInput[INPUT_BUFFER_SIZE];
    memcpy(originalInput, buffer, sizeof(originalInput));
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++) {
        if (originalInput[i] != '#')
            originalInput[i] = 0;
    }

    char virtualBuffer[INPUT_BUFFER_SIZE];

    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    const map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };

    vec2i facing = aoc::direction::UP;
    const vec2i startPos = findStartingPosition(m);
    vec2i pos = startPos;

    while (pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height) {
        if (m(pos) != 'X') {
            if (pos != startPos) {
                // Pretend there is a boulder!
                memcpy(virtualBuffer, originalInput, sizeof(virtualBuffer));
                map mVirtual { virtualBuffer, m.width, m.height, m.lineLength };
                mVirtual.at(pos) = '#';
                if (runsInCircle(startPos, aoc::direction::UP, mVirtual)) {
                    part2++;
                }
            }

            m.at(pos) = 'X';
        }

        if (m.contains(pos + facing) && m(pos + facing) == '#') {
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
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
