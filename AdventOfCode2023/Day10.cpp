#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"
#include "vec2.cpp"

#define TITLE "Day 10"

enum direction {
    NONE = 0,
    NORTH = 1,
    EAST = 2,
    SOUTH = 4,
    WEST = 8
};

struct pipe_map {
    std::vector<int> map;
    int width {0};
    int height {0};
    vec2i posAnimal {0,0};

    int& at(int x, int y) {
        return map[y * width + x];
    }

    int operator()(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height)
            return map[y * width + x];
        else
            return 0;
    }
};


typedef pipe_map day_t;

bool hasConnection(const pipe_map &map, int x, int y, direction d) {
    return (map(x, y) & d) == d;
}

bool canWalk(const pipe_map &map, int x, int y, direction d) {
    switch (d) {
        case NORTH:
            return hasConnection(map, x, y, d) && hasConnection(map, x + 0, y - 1, SOUTH);
        case EAST:
            return hasConnection(map, x, y, d) && hasConnection(map, x + 1, y + 0, WEST);
        case SOUTH:
            return hasConnection(map, x, y, d) && hasConnection(map, x + 0, y + 1, NORTH);
        case WEST:
            return hasConnection(map, x, y, d) && hasConnection(map, x - 1, y + 0, EAST);
        default: return false;
    }
}

day_t parseInput(std::string &input) {
    day_t map;
    Parser p(input.c_str());
    while (!p.eof()) {
        while (!p.isNewLine()) {
            int value {0};
            switch (p.read()) {
                case '|':
                    value = NORTH | SOUTH;
                    break;
                case '-':
                    value = EAST | WEST;
                    break;
                case 'L':
                    value = NORTH | EAST;
                    break;
                case 'J':
                    value = NORTH | WEST;
                    break;
                case '7':
                    value = SOUTH | WEST;
                    break;
                case 'F':
                    value = SOUTH | EAST;
                    break;
                case 'S':
                    map.posAnimal = vec2<int>(static_cast<int>(map.map.size() % map.width), static_cast<int>(map.map.size() / map.width));
                case '.':
                    break;
            }

            map.map.push_back(value);
        }

        if (map.width == 0)
            map.width = static_cast<int>(map.map.size());

        p.readNewLine();
    }

    map.height = static_cast<int>(map.map.size()) / map.width;
    assert(static_cast<int>(map.map.size()) % map.width == 0);

    if (map.posAnimal != vec2i(0, 0)) {
        const int x = map.posAnimal.x;
        const int y = map.posAnimal.y;
        assert(map(x, y) == 0);
        int& v = map.at(x, y);
        if (hasConnection(map, x + 1, y, WEST))
            v |= EAST;
        if (hasConnection(map, x - 1, y, EAST))
            v |= WEST;
        if (hasConnection(map, x, y - 1, SOUTH))
            v |= NORTH;
        if (hasConnection(map, x, y + 1, NORTH))
            v |= SOUTH;
    }

    return map;
}

std::string runPart1(day_t& map) {
    std::stringstream output;
    vec2i startingPosition = map.posAnimal;
    vec2i pos = startingPosition;
    int steps = 0;
    direction blockedDirection = NONE;
    do {
        if (blockedDirection != EAST && canWalk(map, pos.x, pos.y, EAST)) {
            blockedDirection = WEST;
            pos.x += 1;
        } else if (blockedDirection != SOUTH && canWalk(map, pos.x, pos.y, SOUTH)) {
            blockedDirection = NORTH;
            pos.y += 1;
        } else if (blockedDirection != WEST && canWalk(map, pos.x, pos.y, WEST)) {
            blockedDirection = EAST;
            pos.x -= 1;
        } else if (blockedDirection != NORTH && canWalk(map, pos.x, pos.y, NORTH)) {
            blockedDirection = SOUTH;
            pos.y -= 1;
        }
        steps++;
    } while (pos != startingPosition);

    output << (steps / 2);
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    return output.str();
}

// BOILER PLATE CODE BELOW

std::string readInput() {
    std::cin >> std::noskipws;

    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string fileContent(it, end);

    return fileContent;
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
	std::cout << "######################################" << std::endl;
	std::cout << "############### " << TITLE << " ###############" << std::endl;
	std::cout << "######################################" << std::endl;

    auto gt0 = std::chrono::high_resolution_clock::now();
	const std::string originalInput = readInput();
    auto gt1 = std::chrono::high_resolution_clock::now();

    std::cout << "Reading Input: " << formatTime(gt1 - gt0) << '\n';
	std::cout << "**************************************\n";

    std::string input = originalInput;
	auto t0 = std::chrono::high_resolution_clock::now();
	day_t parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << '\n';
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 1 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

	input = originalInput;
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 2 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

    auto gt2 = std::chrono::high_resolution_clock::now();
    std::cout << "Global Time: " << formatTime(gt2 - gt0) << '\n';
	std::cout << "**************************************" << std::endl;
}
