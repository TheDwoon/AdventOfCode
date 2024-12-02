#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "parser.cpp"

#define TITLE "Day 02"

struct reactor {
    std::vector<int> levels;
    std::vector<int> boundaries;
};

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct input {
    char buffer[INPUT_BUFFER_SIZE];
    long length;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef reactor day_t;

day_t parseInput(const input &input) {
    day_t reactor;
    Parser p(input.buffer);

    int globalIndex = 0;

    reactor.boundaries.push_back(0);
    while (!p.eof()) {
        while (!p.isNewLine()) {
            int i = 0;
            p.readInt(i);
            p.consumeWhitespace();
            reactor.levels.push_back(i);

            globalIndex += 1;
        }

        p.readNewLine();
        reactor.boundaries.push_back(globalIndex);
    }

    return reactor;
}

std::string runPart1(const day_t& input) {
    std::stringstream output;
    int sum = 0;

    const std::vector<int> levels = input.levels;
    const std::vector<int> boundaries = input.boundaries;

    const int levelCount = static_cast<int>(boundaries.size());
    for (int i = 0; i < levelCount - 1; i++) {
        const int startIdx = boundaries[i];
        const int endIdx = boundaries[i + 1];

        bool valid = true;
        bool ascending = levels[startIdx] < levels[startIdx + 1];
        for (int j = startIdx; j < endIdx - 1 && valid; j++) {
            int diff = std::abs(levels[j] - levels[j + 1]);
            if (ascending != levels[j] < levels[j + 1] || diff > 3 || diff < 1) {
                valid = false;
            }
        }

        if (valid) {
            sum += 1;
        }
    }

    output << sum;
    return output.str();
}

inline int signum(int a, int b) {
    return (a > b) - (a < b);
}

bool canMove(int sourceIdx, int targetIdx, const std::vector<int> &levels, int sign) {
    const int sourceLevel = levels[sourceIdx];
    const int targetLevel = levels[targetIdx];
    const int diff = std::abs(sourceLevel - targetLevel);

    return diff <= 3 && diff >= 1 && (sign == 0 || signum(sourceLevel, targetLevel) == sign);
}

bool isValidLine(int startIdx, int endIdx, const std::vector<int> &levels, int sign = 0, bool skipped = false) {
    int cost = 0;
    for (int i = startIdx; i < endIdx - 1; i++) {
        if (canMove(i, i + 1, levels, sign)) {
            sign = signum(levels[i], levels[i + 1]);
            cost += 1;
        } else if (!skipped && i + 2 < endIdx && canMove(i, i + 2, levels, sign)) {
            sign = signum(levels[i], levels[i + 2]);
            skipped = true;
            cost += 2;
            i++;
        } else if (!skipped && i + 2 == endIdx) {
            skipped = true;
            cost += 1;
            i++;
        }
    }

    return cost == endIdx - startIdx - 1;
}

std::string runPart2(const day_t& input) {
    std::stringstream output;
    int sum = 0;

    const std::vector<int> levels = input.levels;
    const std::vector<int> boundaries = input.boundaries;

    const int levelCount = static_cast<int>(boundaries.size());
    for (int i = 0; i < levelCount - 1; i++) {
        const int startIdx = boundaries[i];
        const int endIdx = boundaries[i + 1];

        if (isValidLine(startIdx, endIdx, levels, 1, false)
            || isValidLine(startIdx, endIdx, levels, -1, false)
            || isValidLine(startIdx + 1, endIdx, levels, 1 , true)
            || isValidLine(startIdx + 1, endIdx, levels, -1 , true)) {
            sum += 1;
        }
    }

    output << sum;
    return output.str();
}

// BOILER PLATE CODE BELOW

void readInput(input &i) {
    std::cin.read(i.buffer, INPUT_BUFFER_SIZE);

    i.length = std::cin.gcount();
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
#ifdef NDEBUG
    auto gt0 = std::chrono::high_resolution_clock::now();
#endif
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input in;
	readInput(in);
	const day_t parsedInput = parseInput(in);
	const std::string output_part1 = runPart1(parsedInput);
	const std::string output_part2 = runPart2(parsedInput);

	std::cout << "Part 1: " << output_part1 << '\n';
	std::cout << "Part 2: " << output_part2 << '\n';

#ifdef NDEBUG
    auto gt2 = std::chrono::high_resolution_clock::now();
    std::cout << "Global Time: " << formatTime(gt2 - gt0) << '\n';
#endif
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
