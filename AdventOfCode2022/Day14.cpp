#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>

#define TITLE "Day 14"
#define MAP_WIDTH 1000
#define MAP_HEIGHT 250
#define ACCESS_MAP(MAP, X, Y) (MAP[(Y) * (MAP_WIDTH) + (X)])

#define WALL '#';
#define AIR ' '
#define SAND 'O'

struct point {
    int x;
    int y;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<std::vector<point>> day_t;

day_t parseInput(std::string &input) {
    day_t result;

    std::stringstream lineStream(input);
    std::string line;

    int x, y;
    char c;
    char ignore[4];

    while (std::getline(lineStream, line)) {
        std::stringstream stream(line);
        std::vector<point> segment;
        while (stream >> x >> c >> y) {
            segment.push_back({x, y});
            stream >> ignore;
        }

        result.push_back(std::move(segment));
    }

    return result;
}

int fillMap(const day_t& input, char* map) {
    int maxY = 0;

    for (const std::vector<point>& segment : input) {
        for (size_t i = 0; i < segment.size() - 1; ++i) {
            int x = segment[i + 0].x;
            int y = segment[i + 0].y;
            const point& end = segment[i + 1];

            maxY = std::max(maxY, std::max(y, end.y));

            int dx = std::clamp(end.x - x, -1, 1);
            int dy = std::clamp(end.y - y, -1, 1);

            ACCESS_MAP(map, x, y) = WALL;
            while (x != end.x || y != end.y) {
                assert(x < MAP_WIDTH && x >= 0);
                assert(y < MAP_HEIGHT && y >= 0);

                x += dx;
                y += dy;

                ACCESS_MAP(map, x, y) = WALL;
            }
        }
    }

    return maxY;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    char map[MAP_WIDTH * MAP_HEIGHT];
    std::fill(map, map + MAP_WIDTH * MAP_HEIGHT, AIR);
    fillMap(input, map);

    int sandPlaced = 0;
    bool fellOutOfMap = false;
    while (!fellOutOfMap) {
        int x = 500;
        int y = 0;

        bool atRest = false;
        while (y < MAP_HEIGHT - 1 && !atRest) {
            if (ACCESS_MAP(map, x, y + 1) == AIR) {
                y = y + 1;
            } else if (ACCESS_MAP(map, x - 1, y + 1) == AIR) {
                x = x - 1;
                y = y + 1;
            } else if (ACCESS_MAP(map, x + 1, y + 1) == AIR) {
                x = x + 1;
                y = y + 1;
            } else {
                atRest = true;
            }
        }

        if (y >= MAP_HEIGHT - 1)
            fellOutOfMap = true;
        else {
            ACCESS_MAP(map, x, y) = SAND;
            ++sandPlaced;
        }
    }

    output << sandPlaced;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    char map[MAP_WIDTH * MAP_HEIGHT];
    std::fill(map, map + MAP_WIDTH * MAP_HEIGHT, AIR);
    int maxY = fillMap(input, map) + 2;

    int sandPlaced = 0;
    bool fellOutOfMap = false;
    while (ACCESS_MAP(map, 500, 0) == AIR) {
        int x = 500;
        int y = 0;

        bool atRest = false;
        while (y < maxY - 1 && !atRest) {
            if (ACCESS_MAP(map, x, y + 1) == AIR) {
                y = y + 1;
            } else if (ACCESS_MAP(map, x - 1, y + 1) == AIR) {
                x = x - 1;
                y = y + 1;
            } else if (ACCESS_MAP(map, x + 1, y + 1) == AIR) {
                x = x + 1;
                y = y + 1;
            } else {
                atRest = true;
            }
        }

        ACCESS_MAP(map, x, y) = SAND;
        ++sandPlaced;
    }

    output << sandPlaced;
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
