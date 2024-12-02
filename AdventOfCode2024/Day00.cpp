#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 00"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef const char* day_t;

day_t parseInput(const std::string &input) {
    return input.c_str();
}

std::string runPart1(const day_t& input) {
    std::stringstream output;

    return output.str();
}

std::string runPart2(const day_t& input) {
    std::stringstream output;
    return output.str();
}

// BOILER PLATE CODE BELOW

std::string readInput() {
    constexpr  size_t buffer_size = 32 * 1024;
    char buffer[buffer_size] {};
    std::cin.read(buffer, buffer_size);

    auto input_length = std::cin.gcount();
    std::string fileContent(buffer, input_length);
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
#ifdef NDEBUG
    auto gt0 = std::chrono::high_resolution_clock::now();
#endif
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

	const std::string originalInput = readInput();
	day_t parsedInput = parseInput(originalInput);
	std::string output_part1 = runPart1(parsedInput);

	parsedInput = parseInput(originalInput);
	std::string output_part2 = runPart2(parsedInput);

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