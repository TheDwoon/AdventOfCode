#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 01"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef const char* day_t;

day_t parseInput(std::string &input) {
    return input.c_str();
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int sum = 0;

    int first = -1;
    int second = -1;
    for (const char* c = input; *c != '\0'; ++c) {
        if (first == - 1 && *c >= '0' && *c <= '9')
            first = *c - '0';
        else if (*c >= '0' && *c <= '9')
            second = *c - '0';
        else if (*c == '\n') {
            sum += first * 10 + (second == -1 ? first : second);
            first = -1;
            second = -1;
        }
    }

    output << sum;
    return output.str();
}

const char one[] = "one";
const char two[] = "two";
const char three[] = "three";
const char four[] = "four";
const char five[] = "five";
const char six[] = "six";
const char seven[] = "seven";
const char eight[] = "eight";
const char nine[] = "nine";

const char* const numbers[] = { one, two, three, four, five, six, seven, eight, nine };

bool match(const char* c, const char* number) {
    while (*c != '\0' && *number != '\0' && *c == *number) {
        ++c;
        ++number;
    }

    return '\0' == *number;
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    int sum = 0;

    int first = -1;
    int second = -1;
    for (const char* c = input; *c != '\0'; ++c) {
        for (int i = 0; i < 9; i++) {
            if (match(c, numbers[i])) {
                if (first == -1)
                    first = i + 1;
                else
                    second = i + 1;
            }
        }

        if (first == - 1 && *c >= '0' && *c <= '9')
            first = *c - '0';
        else if (*c >= '0' && *c <= '9')
            second = *c - '0';
        else if (*c == '\n') {
            sum += first * 10 + (second == -1 ? first : second);
            first = -1;
            second = -1;
        }
    }

    output << sum;
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
