#include <string>
#include <chrono>
#include "parser.cpp"
#include "vec2.cpp"

#define TITLE "Day 04"

struct map {
    const char* buffer;
    const int width;
    const int height;
    const int lineLength;

    char operator()(const unsigned int x, const unsigned int y) const {
        assert (buffer[y * lineLength + x] != '\n');
        return buffer[y * lineLength + x];
    }
};

bool foundString(const char* str, unsigned int length, const vec2i &v, vec2i pos, const map &m) {
    for (int i = 0; pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height && str[i] == m(pos.x, pos.y); pos += v, i++) {
        if (i == length - 1) {
            return true;
        }
    }

    return false;
}

void runDay(const char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;


    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    const vec2i directions[] { {-1, 1}, {0, 1}, {1, 1}, {1, 0}};
    map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };

    // Part 1
    for (vec2i pos; pos.y < m.height; pos.y++) {
        for (pos.x = 0; pos.x < m.width; pos.x++) {
            for (const vec2i &direction : directions) {
                if (m(pos.x, pos.y) == 'X' && foundString("XMAS", 4, direction, pos, m))
                    part1++;
                if (m(pos.x, pos.y) == 'S' && foundString("SAMX", 4, direction, pos, m))
                    part1++;
            }
            if (pos.x < m.width - 2 && pos.y < m.height - 2
                && (m(pos.x, pos.y) == 'S' || m(pos.x, pos.y) == 'M')
                && m(pos.x + 1, pos.y + 1) == 'A'
                && (m(pos.x + 2, pos.y) == 'S' || m(pos.x + 2, pos.y) == 'M')
                && (m(pos.x, pos.y + 2) == 'S' || m(pos.x, pos.y + 2) == 'M')
                && (m(pos.x + 2, pos.y + 2) == 'S' || m(pos.x + 2, pos.y + 2) == 'M')
                && m(pos.x, pos.y) != m(pos.x + 2, pos.y + 2)
                && m(pos.x, pos.y + 2) != m(pos.x + 2, pos.y)) {
                part2++;
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
