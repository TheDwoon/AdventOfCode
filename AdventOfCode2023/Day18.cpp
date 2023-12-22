#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"
#include "util.cpp"

#define TITLE "Day 18"

#define EXCAVATED 1
#define DIRT 0

struct instruction {
    vec2i direction;
    int dimension {0};
    char color[6] {'0', '0', '0', '0', '0', '0'};
};

typedef std::vector<instruction> day_t;

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    while (!p.eof()) {
        instruction instr;

        char direction = p.read();
        switch (direction) {
            case 'U': instr.direction = aoc::direction::UP; break;
            case 'D': instr.direction = aoc::direction::DOWN; break;
            case 'L': instr.direction = aoc::direction::LEFT; break;
            case 'R': instr.direction = aoc::direction::RIGHT; break;
            default: assert(false); break;
        }

        p.consume(' ');
        p.readNumber(instr.dimension);

        p.consume(" (#");
        instr.color[0] = p.read();
        instr.color[1] = p.read();
        instr.color[2] = p.read();
        instr.color[3] = p.read();
        instr.color[4] = p.read();
        instr.color[5] = p.read();
        p.consume(')');
        p.readNewLine();

        result.push_back(instr);
    }

    return result;
}

bool isExcavated(const aoc::map2d<int> &map, int x, int y) {
    return (map(x,y) & 1) == 1;
}

void excavate(aoc::map2d<int> &map, const vec2i &position) {
    map(position) = EXCAVATED;
}

void markInside(aoc::map2di &map, vec2i position, const vec2i& direction) {
    bool inWall = false;
    while (map.contains(position)) {
        if (map(position) == EXCAVATED) {

        }
    }
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    const int width = 10;
    const int height = 15;

    aoc::map2d<int> map(width, height);
    vec2i excavator;

    for (const instruction &instr : input) {
        for (int i = 0; i < instr.dimension; i++) {
            excavator += instr.direction;
            excavate(map, excavator);
        }
    }

    std::cout << "**************\n";
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            std::cout << (map(x, y) == 1 ? '#' : '.');
        }

        std::cout << std::endl;
    }

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
        output << t.count() / 1000 << "µs";
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
