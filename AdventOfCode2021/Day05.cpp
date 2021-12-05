#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>
#include <map>

#define TITLE "Day 05"
#define SIGN(X) ((X) > 0 ? 1 : ((X) < 0 ? -1 : 0))

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);

struct Line {
    int32_t x1 { 0 };
    int32_t y1 { 0 };
    int32_t x2 { 0 };
    int32_t y2 { 0 };
};

typedef std::vector<Line> day_t;

day_t parseInput(std::string &input) {
    day_t parsed;

    std::stringstream stream(input);
    std::regex regex(R"((\d+),(\d+) -> (\d+),(\d+))");

    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex); it != std::sregex_iterator(); ++it) {
        const auto &match = *it;

        Line line;
        line.x1 = std::stoi(match[1]);
        line.y1 = std::stoi(match[2]);
        line.x2 = std::stoi(match[3]);
        line.y2 = std::stoi(match[4]);

        parsed.push_back(line);
    }

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    std::map<std::tuple<int32_t, int32_t>, uint32_t> markedSpots;
    for (const Line &line : input) {
        if (line.x1 != line.x2 && line.y1 != line.y2)
            continue;

        int32_t endX = std::max(line.x1, line.x2);
        int32_t endY = std::max(line.y1, line.y2);

        for (uint16_t x = std::min(line.x1, line.x2); x <= endX; x++) {
            for (uint16_t y = std::min(line.y1, line.y2); y <= endY; y++) {
                markedSpots[std::make_tuple(x, y)] += 1;
            }
        }
    }

    uint32_t dangerousSpots { 0 };
    for (const auto & markedSpot : markedSpots) {
        dangerousSpots += markedSpot.second > 1;
    }

    output << dangerousSpots;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    std::map<std::tuple<uint16_t, uint16_t>, uint16_t> markedSpots;
    for (const Line &line : input) {
        int32_t deltaX = line.x2 - line.x1;
        int32_t deltaY = line.y2 - line.y1;

        int32_t length = std::max(std::abs(deltaX), std::abs(deltaY));
        int32_t stepX = SIGN(deltaX);
        int32_t stepY = SIGN(deltaY);

        for (int32_t i = 0; i <= length; i++) {
            int32_t currentX = line.x1 + stepX * i;
            int32_t currentY = line.y1 + stepY * i;
            markedSpots[std::make_tuple(currentX, currentY)] += 1;
        }
    }

    uint32_t dangerousSpots { 0 };
    for (const auto & markedSpot : markedSpots) {
        dangerousSpots += markedSpot.second > 1;
    }

    output << dangerousSpots;
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
	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << std::endl;

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
