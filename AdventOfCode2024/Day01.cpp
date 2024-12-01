#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"

#define TITLE "Day 01"

struct notes {
    std::vector<int> leftSide;
    std::vector<int> rightSide;
};

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct input {
    char buffer[INPUT_BUFFER_SIZE];
    long length;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef notes day_t;

day_t parseInput(const input &input) {
    day_t note;
    Parser p(input.buffer);
    while (!p.eof()) {
        int a { 0 }, b { 0 };
        p.readInt(a);
        p.consumeWhitespace();
        p.readInt(b);
        p.readNewLine();

        note.leftSide.push_back(a);
        note.rightSide.push_back(b);
    }

    std::sort(note.leftSide.begin(), note.leftSide.end());
    std::sort(note.rightSide.begin(), note.rightSide.end());

    return note;
}

std::string runPart1(const day_t& input) {
    std::stringstream output;

    int sum = 0;
    for (size_t i = 0; i < input.leftSide.size(); i++) {
        sum += std::abs(input.leftSide[i] - input.rightSide[i]);
    }

    output << sum;
    return output.str();
}

std::string runPart2(const day_t& input) {
    std::stringstream output;
    int sum = 0;

    int j = 0;
    const int size = static_cast<int>(input.leftSide.size());
    for (int i = 0; i < size; i++) {
        while (j < size && input.leftSide[i] > input.rightSide[j])
            j++;

        int countRight = 0;
        while (j < size && input.leftSide[i] == input.rightSide[j]) {
            j++;
            countRight++;
        }

        int countLeft = 1;
        while (i + 1 < size && input.leftSide[i] == input.leftSide[i + 1]) {
            i++;
            countLeft++;
        }

        sum += countLeft * input.leftSide[i] * countRight;
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
