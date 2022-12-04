#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>

#define TITLE "Day 03"

int getPriority(char c) {
    int code { c };
    assert('a' <= code && 'z' >= code || 'A' <= code && 'Z' >= code);

    if (code >= 'a' && code <= 'z')
        return code - 'a' + 1;
    else
        return code - 'A' + 1 + 26;
}

uint64_t encodeContents(const std::string& content) {
    uint64_t encoded { 0 };
    for (char c : content) {
        uint64_t one { 1 };
        encoded |= one << getPriority(c);
    }

    return encoded;
}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<std::string> day_t;

day_t parseInput(std::string &input) {
    day_t result;
    std::stringstream stream(input);
    std::string line;

    while (std::getline(stream, line, '\n')) {
        result.push_back(line);
    }

    return result;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int score { 0 };
    for (const std::string& backpack : input) {
        int priority { 0 };
        std::string leftPocket = backpack.substr(0, backpack.size() / 2);
        std::string rightPocket = backpack.substr(backpack.size() / 2);
        assert(rightPocket.size() == leftPocket.size());

        uint64_t left = encodeContents(leftPocket);
        uint64_t right = encodeContents(rightPocket);
        uint64_t combined = left & right;
        assert(combined != 0);
        while (combined > 1) {
            priority += 1;
            combined >>= 1;
        }

        score += priority;
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    int score { 0 };
    for (size_t i = 0; i < input.size(); i += 3) {
        int priority { 0 };

        uint64_t backpack1 = encodeContents(input[i + 0]);
        uint64_t backpack2 = encodeContents(input[i + 1]);
        uint64_t backpack3 = encodeContents(input[i + 2]);
        uint64_t combined = backpack1 & backpack2 & backpack3;
        assert(combined != 0);
        while (combined > 1) {
            priority += 1;
            combined >>= 1;
        }

        score += priority;
    }

    output << score;
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
