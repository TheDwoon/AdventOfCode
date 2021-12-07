#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>

#define TITLE "Day 07"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<int> day_t;

day_t parseInput(std::string &input) {
    day_t parsed;
    std::stringstream stream(input);
    int number;
    while (stream >> number) {
        stream.get();
        parsed.push_back(number);
    }

    return parsed;
}

inline int sumForP_1(const day_t& input, int p) {
    int sum { 0 };
    for (int x : input) {
        sum += std::abs(x - p);
    }

    return sum;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    int max = *std::max_element(input.begin(), input.end());

    int best_sum = sumForP_1(input, 0);
    for (int p = 1; p < max; ++p) {
        int sum = sumForP_1(input, p);
        if (sum < best_sum) {
            best_sum = sum;
        }
    }

    output << best_sum;
    return output.str();
}

inline int sumForP_2(const day_t& input, int p) {
    int sum { 0 };
    for (int x : input) {
        int d = std::abs(x - p);
        sum += ((1 + d) * d) / 2;
    }

    return sum;
}


std::string runPart2(day_t& input) {
    std::stringstream output;

    int max = *std::max_element(input.begin(), input.end());

    int best_sum = sumForP_2(input, 0);
    for (int p = 1; p < max; ++p) {
        int sum = sumForP_2(input, p);
        if (sum < best_sum) {
            best_sum = sum;
        }
    }

    output << best_sum;
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
