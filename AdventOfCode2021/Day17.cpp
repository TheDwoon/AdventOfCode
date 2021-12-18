#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>
#include <cassert>

#define TITLE "Day 17"

struct target_area {
    int x1, x2;
    int y1, y2;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef target_area day_t;

day_t parseInput(std::string &input) {
//    std::regex regex(R"(target area: x=(\d+)..(\d+), y=(\d+)..(\d+))");
    std::regex regex("(\\d+)..(\\d+), y=-(\\d+)..-(\\d+)");
    std::smatch match;
    bool matched = std::regex_search(input, match, regex);
    assert(matched);

    day_t parsed;
    parsed.x1 = std::stoi(match[1].str());
    parsed.x2 = std::stoi(match[2].str());
    parsed.y1 = -std::stoi(match[3].str());
    parsed.y2 = -std::stoi(match[4].str());

    return parsed;
}

bool test_hit(int x, int y, const target_area& target) {
    int vel_x { x };
    int vel_y { y };
    int current_x { 0 };
    int current_y { 0 };

    while (current_y >= target.y1) {
        current_x += vel_x;
        current_y += vel_y;

//        vel_x = vel_x - ((vel_x > 0) ? 1 : ((vel_x < 0) ? - 1 : 0));
        if (vel_x > 0)
            vel_x -= 1;
        else if (vel_x < 0)
            vel_x += 1;

        vel_y -= 1;

        if (current_x >= target.x1 && current_x <= target.x2
            && current_y >= target.y1 && current_y <= target.y2)
            return true;
    }

    return false;
}

inline int flight_height(int y) {
    return (y + y * y) / 2;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    int max_possible = std::max(std::abs(input.y1), std::abs(input.y2)) + 1;

    int best_x;
    int max_y = 0;
    for (int x = 1; x < input.x1; x++) {
        for (int y = max_y; y < max_possible; y++) {
            if (test_hit(x, y, input)) {
                max_y = y;
                best_x = x;
            }
        }
    }

    output << flight_height(max_y);
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    int max_possible = std::max(std::abs(input.y1), std::abs(input.y2)) + 1;

    int options { 0 };
    for (int x = 0; x <= input.x2; x++) {
        for (int y = -max_possible; y <= max_possible; y++) {
            options += test_hit(x, y, input);
        }
    }

    output << options;
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
