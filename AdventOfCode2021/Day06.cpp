#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <list>

#define TITLE "Day 06"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::list<std::uint8_t> day_t;

day_t parseInput(std::string &input) {
    day_t parsed;
    std::stringstream stream(input);
    uint16_t number;

    while (stream >> number) {
        stream.get();
        parsed.push_back(number);
    }

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    uint64_t schools[9] { 0 };
    uint64_t schools_next[9] { 0 };

    for (const uint8_t& fish : input)
        schools[fish] += 1;

    for (size_t day = 0; day < 80; ++day) {
        std::fill(schools_next, schools_next + 9, 0);
        schools_next[8] = schools[0];
        schools_next[6] = schools[0];

        for (size_t i = 1; i < 9; ++i) {
            schools_next[i - 1] += schools[i];
        }

        std::swap(schools, schools_next);
    }

    uint64_t fishes { 0 };
    for (uint64_t school : schools) {
        fishes += school;
    }

    output << fishes;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    uint64_t schools[9] { 0 };
    uint64_t schools_next[9] { 0 };

    for (const uint8_t& fish : input)
        schools[fish] += 1;

    for (size_t day = 0; day < 256; ++day) {
        std::fill(schools_next, schools_next + 9, 0);
        schools_next[8] = schools[0];
        schools_next[6] = schools[0];

        for (size_t i = 1; i < 9; ++i) {
            schools_next[i - 1] += schools[i];
        }

        std::swap(schools, schools_next);
    }

    uint64_t fishes { 0 };
    for (uint64_t school : schools) {
        fishes += school;
    }

    output << fishes;
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
