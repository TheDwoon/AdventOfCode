#include "Day03.h"

std::vector<std::string> Day03::parseInput(std::string &input) {
    std::vector<std::string> parsed;
    std::stringstream  stream(input);

    std::string bitfields;
    while (stream >> bitfields)
        parsed.push_back(bitfields);

    return parsed;
}

std::string Day03::runPart1(std::vector<std::string> &input) {
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

std::string Day03::runPart2(std::vector<std::string> &input) {
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

bool Day03::testPart1(std::vector<std::string> &input) {
    std::string result = runPart1(input);
    return "198" == result;
}
