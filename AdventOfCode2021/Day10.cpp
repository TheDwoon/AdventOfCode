#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <deque>
#include <algorithm>

#define TITLE "Day 10"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<std::string> day_t;

day_t parseInput(std::string &input) {
    day_t parsed;

    std::stringstream stream(input);
    for (std::string line; std::getline(stream, line); ) {
        parsed.push_back(line);
    }

    return parsed;
}

inline uint64_t error_points_p1(char c) {
    switch (c) {
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
        default: return 0;
    }
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    uint64_t error_score { 0 };
    for (const std::string &line : input) {
        std::deque<char> stack;
        for (const char c : line) {
            bool valid = true;
            switch (c) {
                case '{':
                case '(':
                case '[':
                case '<':
                    stack.push_front(c);
                    break;
                case '}':
                    if (stack.front() == '{')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
                case ']':
                    if (stack.front() == '[')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
                case ')':
                    if (stack.front() == '(')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
                case '>':
                    if (stack.front() == '<')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
            }

            if (!valid) {
                error_score += error_points_p1(c);
                break;
            }
        }
    }

    output << error_score;
    return output.str();
}

inline uint64_t error_points_p2(char c) {
    switch (c) {
        case '(': return 1;
        case '[': return 2;
        case '{': return 3;
        case '<': return 4;
        default: return 0;
    }
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    std::vector<uint64_t> error_scores;
    for (const std::string &line : input) {
        std::deque<char> stack;
        for (const char c : line) {
            bool valid = true;
            switch (c) {
                case '{':
                case '(':
                case '[':
                case '<':
                    stack.push_front(c);
                    break;
                case '}':
                    if (stack.front() == '{')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
                case ']':
                    if (stack.front() == '[')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
                case ')':
                    if (stack.front() == '(')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
                case '>':
                    if (stack.front() == '<')
                        stack.pop_front();
                    else
                        valid = false;
                    break;
            }

            if (!valid) {
                stack.clear();
                break;
            }
        }

        if (!stack.empty()) {
            uint64_t error { 0 };
            for (char c : stack) {
                error = (error * 5) + error_points_p2(c);
            }

            error_scores.push_back(error);
        }
    }

    std::sort(error_scores.begin(), error_scores.end());
    output << error_scores[error_scores.size() / 2];
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
