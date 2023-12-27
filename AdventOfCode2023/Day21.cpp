#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <deque>
#include "parser.cpp"
#include "util.cpp"

#define TITLE "Day 21"
#define START_MARKER 'S'
#define ROCK '#'
#define GARDEN_PLOT '.'

typedef aoc::map2dc day_t;

day_t parseInput(std::string &input) {
    day_t map;
    Parser p(input.c_str());
    while (!p.eof()) {
        while (!p.isNewLine()) {
            map.data.push_back(p.read());
        }

        if (map.width == 0)
            map.width = static_cast<int>(map.data.size());

        p.readNewLine();
    }

    assert(static_cast<int>(map.data.size()) % map.width == 0);
    map.height = static_cast<int>(map.data.size()) / map.width;

    return map;
}

vec2i getStartingPosition(const aoc::map2dc& map) {
    vec2i pos;
    for (pos.y = 0; pos.y < map.height; pos.y++) {
        for (pos.x = 0; pos.x < map.width; pos.x++) {
            if (map(pos) == 'S')
                return pos;
        }
    }

    assert(false);
    return pos;
}

bool canVisit(const aoc::map2dc &map, const vec2i &position) {
    return map.contains(position) && map(position) != ROCK;
}

aoc::map2di findMinimalSteps(const aoc::map2dc &map, const vec2i &startingPosition) {
    aoc::map2di steps(map.width, map.height, INT_MAX);
    steps(startingPosition) = 0;

    std::deque<vec2i> queue { startingPosition };
    while (!queue.empty()) {
        vec2i position = queue.front();
        queue.pop_front();
        int currentSteps = steps(position);

        vec2i north = position + aoc::direction::NORTH;
        if (canVisit(map, north) && steps(north) > currentSteps + 1) {
            steps(north) = currentSteps + 1;
            queue.push_back(north);
        }

        vec2i south = position + aoc::direction::SOUTH;
        if (canVisit(map, south) && steps(south) > currentSteps + 1) {
            steps(south) = currentSteps + 1;
            queue.push_back(south);
        }

        vec2i west = position + aoc::direction::WEST;
        if (canVisit(map, west) && steps(west) > currentSteps + 1) {
            steps(west) = currentSteps + 1;
            queue.push_back(west);
        }

        vec2i east = position + aoc::direction::EAST;
        if (canVisit(map, east) && steps(east) > currentSteps + 1) {
            steps(east) = currentSteps + 1;
            queue.push_back(east);
        }

    }

    return steps;
}

bool canReach(const aoc::map2di &steps, const vec2i &position, int maxSteps) {
    if (steps.contains(position) && steps(position) % 2 == maxSteps % 2 && steps(position) <= maxSteps)
        return true;

    return false;
}

std::string runPart1(day_t& map) {
    std::stringstream output;
    const int maxSteps = 64;
    int score = 0;
    vec2i startingPosition = getStartingPosition(map);
    aoc::map2di steps = findMinimalSteps(map, startingPosition);

    vec2i pos;
    for (pos.y = 0; pos.y < map.height; pos.y++) {
        for (pos.x = 0; pos.x < map.width; pos.x++) {
            if (canVisit(map, pos) && canReach(steps, pos, maxSteps))
                score += 1;
        }
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
//    const int maxSteps = 26501365;
    const int maxSteps = 12;
    int score = 0;
    vec2i startingPosition = getStartingPosition(input);
    input(startingPosition) = GARDEN_PLOT;
    startingPosition += vec2i(input.width, input.height);

    aoc::map2dc map(3 * input.width, 3 * input.height);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            map.copy(input, vec2i(i * input.width, j * input.height));
        }
    }

    vec2i pos;
    for (pos.y = 0; pos.y < map.height; pos.y++) {
        for (pos.x = 0; pos.x < map.width; pos.x++) {
            std::cout << map(pos);
        }

        std::cout << std::endl;
    }
    std::cout << std::endl;

    aoc::map2di steps = findMinimalSteps(map, startingPosition);

    for (pos.y = 0; pos.y < map.height; pos.y++) {
        for (pos.x = 0; pos.x < map.width; pos.x++) {
            if (canVisit(map, pos) && canReach(steps, pos, maxSteps)) {
                std::cout << 'O';
                score += 1;
            } else {
                std::cout << map(pos);
            }
        }

        std::cout << std::endl;
    }

    output << score;
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
