#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 03"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<std::string> day_t;

day_t parseInput(std::string &input) {
    std::vector<std::string> parsed;
    std::stringstream  stream(input);

    std::string bitfields;
    while (stream >> bitfields)
        parsed.push_back(bitfields);

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    size_t numberOfBits = input[0].size();
    auto *ones = new uint16_t[numberOfBits];
    std::fill(ones, ones + numberOfBits, 0);

    for (size_t i = 0; i < input.size(); ++i) {
        const std::string &line = input[i];
        for (size_t c = 0; c < numberOfBits; ++c) {
            ones[c] += (line[c] == '1');
        }
    }

    size_t half = input.size() / 2;

    uint16_t gamma { 0 };
    uint16_t epsilon { 0 };
    for (size_t i = 0; i < numberOfBits; ++i) {
        epsilon = (epsilon << 1) | (ones[i] < half);
        gamma = (gamma << 1) | (ones[i] >= half);
    }

    output << (gamma * epsilon);

    delete[] ones;
    return output.str();
}

uint16_t countOnes(const std::vector<std::string> &input, const bool* enabled, uint16_t bit) {
    uint16_t ones { 0 };
    for (size_t i = 0; i < input.size(); ++i) {
        ones += (enabled[i] && input[i][bit] == '1');
    }

    return ones;
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    bool *enabled = new bool[input.size()];
    std::fill(enabled, enabled + input.size(), true);
    size_t numbersLeft = input.size();
    const uint16_t bits = input[0].size();

    // oxygen generator rating
    for (uint16_t bit = 0; bit < bits && numbersLeft > 1; ++bit) {
        uint16_t ones = countOnes(input, enabled, bit);
        char keep = 2 * ones >= numbersLeft ? '1' : '0';

        for (size_t line = 0; line < input.size(); ++line) {
            if (!enabled[line])
                continue;

            if (input[line][bit] != keep) {
                enabled[line] = false;
                numbersLeft -= 1;
            }
        }
    }

    std::string oxygenRating;
    for (size_t i = 0; i < input.size(); ++i) {
        if (enabled[i]) {
            oxygenRating = input[i];
            break;
        }
    }

    // scrubber rating
    std::fill(enabled, enabled + input.size(), true);
    numbersLeft = input.size();
    for (uint16_t bit = 0; bit < bits && numbersLeft > 1; ++bit) {
        uint16_t ones = countOnes(input, enabled, bit);
        char keep = 2 * ones < numbersLeft ? '1' : '0';

        for (size_t line = 0; line < input.size(); ++line) {
            if (!enabled[line])
                continue;

            if (input[line][bit] != keep) {
                enabled[line] = false;
                numbersLeft -= 1;
            }
        }
    }

    std::string scrubberRating;
    for (size_t i = 0; i < input.size(); ++i) {
        if (enabled[i]) {
            scrubberRating = input[i];
            break;
        }
    }

    // convert ratings to numbers
    uint16_t oxygen { 0 };
    uint16_t scrubber { 0 };
    for (size_t i = 0; i < bits; ++i) {
        oxygen = (oxygen << 1) | (oxygenRating[i] == '1');
        scrubber = (scrubber << 1) | (scrubberRating[i] == '1');
    }

    output << oxygen * scrubber;

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
