#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "parser.cpp"
#include "vec2.cpp"

#define TITLE "Day 04"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct input {
    char buffer[INPUT_BUFFER_SIZE];
    int length;
};

typedef input day_t;

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

void runDay(const day_t& input) {
    int part1 = 0;
    int part2 = 0;


    Parser p(input.buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    const vec2i directions[] { {-1, 1}, {0, 1}, {1, 1}, {1, 0}};
    map m { input.buffer, static_cast<int>(lineLength), static_cast<int>(input.length / (lineLength + 1)), static_cast<int>(lineLength + 1) };

    // Part 1
    for (vec2i pos; pos.y < m.height; pos.y++) {
        for (pos.x = 0; pos.x < m.width; pos.x++) {
            for (const vec2i &direction : directions) {
                if (m(pos.x, pos.y) == 'X' && foundString("XMAS", 4, direction, pos, m))
                    part1++;
                if (m(pos.x, pos.y) == 'S' && foundString("SAMX", 4, direction, pos, m))
                    part1++;
            }

            if (pos.x >= 1 && pos.x < m.width - 1 && pos.y >= 1 && pos.y < m.height - 1
                && m(pos.x, pos.y) == 'A'
                && (m(pos.x - 1, pos.y + 1) == 'S' || m(pos.x - 1, pos.y + 1) == 'M')
                && (m(pos.x - 1, pos.y - 1) == 'S' || m(pos.x -1, pos.y - 1) == 'M')
                && (m(pos.x + 1, pos.y + 1) == 'S' || m(pos.x + 1, pos.y + 1) == 'M')
                && (m(pos.x + 1, pos.y - 1) == 'S' || m(pos.x + 1, pos.y - 1) == 'M')
                && m(pos.x - 1, pos.y + 1) != m(pos.x + 1, pos.y - 1)
                && m(pos.x + 1, pos.y + 1) != m(pos.x - 1, pos.y - 1)
                ) {
                part2++;
            }
        }
    }

    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}

// BOILER PLATE CODE BELOW

void readInput(input &i) {
    std::cin.read(i.buffer, INPUT_BUFFER_SIZE);

    i.length = static_cast<int>(std::cin.gcount());
}

std::string formatTime(std::chrono::duration<long long, std::nano> t) {
    std::stringstream output;
    if (t.count() < 10000) {
        output << t.count() << "ns";
    }
    else if (t.count() < 10000000) {
        output << t.count() / 1000 << "us";
    }
    else {
        output << t.count() / 1000000 << "ms";
    }

    return output.str();
}

int main()
{
#ifdef NDEBUG
    auto gt0 = std::chrono::high_resolution_clock::now();
#endif
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input in;
	readInput(in);
	runDay(in);
#ifdef NDEBUG
    auto gt2 = std::chrono::high_resolution_clock::now();
    std::cout << "Global Time: " << formatTime(gt2 - gt0) << '\n';
#endif
}
