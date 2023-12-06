#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <cmath>
#include "parser.cpp"

#define TITLE "Day 06"

struct races {
    std::vector<uint64_t> times;
    std::vector<uint64_t> distances;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef races day_t;

day_t parseInput(std::string &input) {
    day_t result;
    uint64_t i;
    Parser p(input.c_str());

    p.consume("Time:");
    while (p.consumeWhitespace() && p.readNumber(i)) {
        result.times.push_back(i);
    }
    p.readNewLine();
    p.consume("Distance:");
    while (p.consumeWhitespace() && p.readNumber(i)) {
        result.distances.push_back(i);
    }

    return result;
}

bool solve(uint64_t time, uint64_t distance, int &min, int &max) {
    const double a = -1;
    const double b = static_cast<double>(time);
    const double c = -1 * static_cast<double>(distance);
    double r = b * b - 4 * a * c;
    // d = (T - x) * x = Tx - x²
    // (-b +- sqrt(b² - 4ac)) / 2a

    if (r <= 0)
        return false;

    r = std::sqrt(r);
    double x1 = (-b + r) / (2 * a);
    double x2 = (-b - r) / (2 * a);
    min = std::floor(x1 + 1);
    max = std::ceil(x2 - 1);
    return true;
}


std::string runPart1(day_t& input) {
    std::stringstream output;

    uint64_t score = 1;
    for (unsigned int i = 0; i < input.times.size(); i++) {
        int min = 0, max = 0;
        uint64_t time = input.times[i];
        uint64_t distance = input.distances[i];
        if (solve(time, distance, min, max)) {
            score *= (max - min + 1);
        }
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    uint64_t score { 0 };

    uint64_t time = input.times[0];
    uint64_t distance = input.distances[0];
    for (unsigned int i = 1; i < input.times.size(); i++) {
        time = time * std::pow(10, static_cast<uint64_t>(std::ceil(std::log10(input.times[i])))) + input.times[i];
        distance = distance * std::pow(10, static_cast<uint64_t>(std::ceil(std::log10(input.distances[i])))) + input.distances[i];

    }

    int min = 0, max = 0;
    if (solve(time, distance, min, max)) {
        score = (max - min + 1);
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
