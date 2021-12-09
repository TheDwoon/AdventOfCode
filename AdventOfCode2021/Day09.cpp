#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <deque>
#include <set>
#include <algorithm>
#include "vec2.h"

#define TITLE "Day 09"

#define ACCESS_MAP(map, x, y) ((x >= 0 && x < map.width && y >= 0 && y < map.height) ? (map.tiles[(y) * map.width + (x)]) : (9))

struct Map {
    uint16_t width { 0 };
    uint16_t height { 0 };
    std::vector<uint8_t> tiles;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef Map day_t;

day_t parseInput(std::string &input) {
    Map map;
    map.width = input.find('\n');

    std::stringstream stream(input);
    char c;
    while (stream.get(c)) {
        if (c >= '0' && c <= '9') {
            map.tiles.push_back(c - '0');
        }
    }

    map.height = map.tiles.size() / map.width;
    return map;
}

std::string runPart1(day_t& map) {
    std::stringstream output;

    uint32_t riskLevel {0 };
    for (uint16_t y = 0; y < map.height; ++y) {
        for (uint16_t x = 0; x < map.width; ++x) {
            uint8_t v = ACCESS_MAP(map, x, y);
            riskLevel += (v < ACCESS_MAP(map, x - 1, y) && v < ACCESS_MAP(map, x + 1, y)
                          && v < ACCESS_MAP(map, x, y - 1) && v < ACCESS_MAP(map, x, y + 1)) * (v + 1);
        }
    }

    output << riskLevel;
    return output.str();
}

const vec2i UP { 0, 1 };
const vec2i DOWN { 0, -1 };
const vec2i LEFT { -1, 0 };
const vec2i RIGHT { 1, 0 };

struct Basin {
    vec2i center;
    uint32_t size { 0 };
};

bool compare_basins(const Basin &l, const Basin &r) {
    return l.size < r.size;
}

std::string runPart2(day_t& map) {
    std::stringstream output;

    // find basins
    std::vector<Basin> basins;
    for (uint16_t y = 0; y < map.height; ++y) {
        for (uint16_t x = 0; x < map.width; ++x) {
            uint8_t v = ACCESS_MAP(map, x, y);
            if ((v < ACCESS_MAP(map, x - 1, y) && v < ACCESS_MAP(map, x + 1, y)
                 && v < ACCESS_MAP(map, x, y - 1) && v < ACCESS_MAP(map, x, y + 1))) {
                Basin basin;
                basin.center = { x, y };
                basins.push_back(basin);
            }
        }
    }

    // grow basins
    for (Basin &basin : basins) {
        std::deque<vec2i> queue;
        std::set<vec2i> visited;
        queue.push_back(basin.center);
        while (!queue.empty()) {
            const vec2i &pos = queue.front();
            vec2i u = pos + UP;
            vec2i d = pos + DOWN;
            vec2i l = pos + LEFT;
            vec2i r = pos + RIGHT;
            visited.emplace(pos);
            basin.size += 1;
            queue.pop_front();

            if (ACCESS_MAP(map, u.x, u.y) < 9 && visited.find(u) == visited.end()) {
                queue.push_back(u);
                visited.emplace(u);
            }
            if (ACCESS_MAP(map, d.x, d.y) < 9 && visited.find(d) == visited.end()) {
                queue.push_back(d);
                visited.emplace(d);
            }
            if (ACCESS_MAP(map, l.x, l.y) < 9 && visited.find(l) == visited.end()) {
                queue.push_back(l);
                visited.emplace(l);
            }
            if (ACCESS_MAP(map, r.x, r.y) < 9 && visited.find(r) == visited.end()) {
                queue.push_back(r);
                visited.emplace(r);
            }
        }
    }

    // profit
    std::sort(basins.begin(), basins.end(), compare_basins);

    size_t num_basins = basins.size();
    output << basins[num_basins - 3].size * basins[num_basins - 2].size * basins[num_basins - 1].size;
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

	const std::string originalInput = readInput();

    std::string input = originalInput;
	auto t0 = std::chrono::high_resolution_clock::now();
	day_t parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 1 ***************" << std::endl;
	std::cout << "Parsing: " << formatTime(t1 - t0) << std::endl;
	std::cout << "Running: " << formatTime(t2 - t1) << std::endl;
	std::cout << "Total: " << formatTime(t2 - t0) << std::endl;
	std::cout << "**************************************" << std::endl;

	input = originalInput;
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 2 ***************" << std::endl;
	std::cout << "Parsing: " << formatTime(t1 - t0) << std::endl;
	std::cout << "Running: " << formatTime(t2 - t1) << std::endl;
	std::cout << "Total: " << formatTime(t2 - t0) << std::endl;
	std::cout << "**************************************" << std::endl;
}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator) {
    std::vector<std::string> tokenized;

    size_t pos = 0;
    size_t tokenEnd;
    do {
        tokenEnd = input.find(separator, pos);
        std::string token = input.substr(pos, (tokenEnd - pos));
        tokenized.push_back(token);
        pos = tokenEnd + separator.size();
    } while(tokenEnd != std::string::npos);

    return tokenized;
}
