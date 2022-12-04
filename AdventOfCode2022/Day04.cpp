#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>

#define TITLE "Day 04"

struct ranges {
    int s1;
    int e1;
    int s2;
    int e2;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<ranges> day_t;


day_t parseInput(std::string &input) {
    day_t result;
    std::regex regex_cords(R"((\d+)-(\d+),(\d+)-(\d+))");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex_cords); it != std::sregex_iterator(); ++it) {
        const auto &match = *it;

        ranges r { std::stoi(match[1]), std::stoi(match[2]),
                   std::stoi(match[3]), std::stoi(match[4]) };
        result.push_back(r);
    }

    return result;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    int sum { 0 };
    for (const ranges& r : input) {
        sum += (r.s1 <= r.s2 and r.e1 >= r.e2) || (r.s1 >= r.s2 and r.e1 <= r.e2);
    }

    output << sum;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    int sum { 0 };
    for (const ranges& r : input) {
        sum += std::min(r.e1, r.e2) >= std::max(r.s1, r.s2);
    }

    output << sum;
    return output.str();
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
