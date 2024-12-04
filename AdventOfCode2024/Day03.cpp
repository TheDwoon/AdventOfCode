#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "parser.cpp"

#define TITLE "Day 03"

enum instruction_type {
    DO = 0,
    DONT = 1,
    MUL = 3
};

struct instruction {
    instruction_type type;
    int a;
    int b;
};

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct input {
    char buffer[INPUT_BUFFER_SIZE];
    long length;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<instruction> day_t;

day_t parseInput(const input &input) {
    day_t instructions;
    Parser p(input.buffer);
    int a, b;
    while (!p.eof()) {
        if (p.consume("mul(") && p.readNumber(a) && p.consume(",") && p.readNumber(b) && p.consume(")")) {
            instructions.emplace_back(MUL, a, b);
        } else if (p.consume("do()")) {
            instructions.emplace_back(DO, 0, 0);
        } else if (p.consume("don't()")) {
            instructions.emplace_back(DONT, 0, 0);
        } else {
          p.read();
        }
    }

    return instructions;
}

void runDay(const day_t& input) {
    int part1 = 0;
    int part2 = 0;

    bool enabled = true;
    for (const auto&[type, a, b] : input) {
        part1 += a * b;
        if (type == MUL && enabled) {
            part2 += a * b;
        } else {
            enabled = type == DO;
        }
    }

    std::cout << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
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
	runDay(parsedInput);
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
