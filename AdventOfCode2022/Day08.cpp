#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 08"

struct map {
    size_t width;
    size_t height;
    uint8_t* tiles;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef map day_t;

void printMap(const day_t& map, const bool* visible) {
    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            std::cout << (char) (map.tiles[y * map.width + x] + '0');
            if (visible[y * map.width + x])
                std::cout << "X";
            else
                std::cout << "_";
        }
        std::cout << "\n";
    }
}

day_t parseInput(std::string &input) {
    day_t map;
    map.width = input.find('\n');
    map.height = input.length() / (map.width + 1);
    map.tiles = new uint8_t[map.width * map.height];

    for (size_t y = 0; y < map.height; ++y) {
        for (size_t x = 0; x < map.width; ++x) {
            map.tiles[y * map.width + x] = input[y * (map.width + 1) + x] - '0';
        }
    }

    return map;
}

void castRay(size_t srcX, size_t srcY, int dx, int dy, const day_t& map, bool* visible) {
    size_t x { srcX + dx };
    size_t y { srcY + dy };
    visible[srcY * map.width + srcX] = true;

    uint8_t blockedHeight = map.tiles[srcY * map.width + srcX];

    while (x > 0 && x < map.width - 1 && y > 0 && y < map.height - 1) {
        if (blockedHeight < map.tiles[y * map.width + x]) {
            blockedHeight = map.tiles[y * map.width + x];
            visible[y * map.width + x] = true;
        }

        if (blockedHeight == 9)
            break;

        x += dx;
        y += dy;
    }
}

std::string runPart1(day_t& map) {
    std::stringstream output;
    bool* visible = new bool[map.width * map.height];
    std::fill(visible, visible + map.width * map.height, false);

    // corner pieces
    visible[0] = true;
    visible[map.width - 1] = true;
    visible[(map.height - 1) * map.width] = true;
    visible[(map.height - 1) * map.width + map.width - 1] = true;

    for (int x = 1; x < map.width - 1; ++x) {
        castRay(x, 0, 0, 1, map, visible);
        castRay(x, map.height - 1, 0, -1, map, visible);
    }

    for (int y = 1; y < map.height; ++y) {
        castRay(0, y, 1, 0, map, visible);
        castRay(map.width - 1, y, -1, 0, map, visible);
    }

    // printMap(map, visible);

    size_t count { 0 };
    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            count += visible[y * map.width + x];
        }
    }

    delete[] visible;
    delete[] map.tiles;

    output << count;
    return output.str();
}

size_t computeScore(const map& map, int srcX, int srcY) {

    size_t rightCount { 0 };
    size_t leftCount { 0 };
    size_t upCount { 0 };
    size_t downCount { 0 };

    uint8_t height = map.tiles[srcY * map.width + srcX];

    int x;
    int y;

    // x+
    x = srcX + 1;
    y = srcY;

    while (x < map.width) {
        ++rightCount;

        if (map.tiles[y * map.width + x] >= height)
            break;

        ++x;
    }

    // x-
    x = srcX - 1;
    y = srcY;

    while (x >= 0) {
        ++leftCount;

        if (map.tiles[y * map.width + x] >= height)
            break;

        --x;
    }

    // y+
    x = srcX;
    y = srcY + 1;
    while (y < map.height) {
        ++upCount;

        if (map.tiles[y * map.width + x] >= height)
            break;

        ++y;
    }

    // y-
    x = srcX;
    y = srcY - 1;
    while (y >= 0) {
        ++downCount;

        if (map.tiles[y * map.width + x] >= height)
            break;

        --y;
    }

    return leftCount * rightCount * upCount * downCount;
}

std::string runPart2(day_t& map) {
    std::stringstream output;

    size_t bestScore { 0 };
    size_t score;
    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            score = computeScore(map, x, y);
            if (score > bestScore)
                bestScore = score;
        }
    }

    output << bestScore;
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
