#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 06"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::string day_t;

day_t parseInput(std::string &input) {
    return input;
}

bool isDistinct(const std::string &seq, int pos, int len) {
    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (seq[pos + i] == seq[pos + j])
                return false;
        }
    }

    return true;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    /*for (int i = 3; i < input.size(); ++i) {
        if (input[i - 0] != input[i - 1] && input[i - 0] != input[i - 2] && input[i - 0] != input[i - 3]
        && input[i - 1] != input[i - 0] && input[i - 1] != input[i - 2] && input[i - 1] != input[i - 3]
        && input[i - 2] != input[i - 0] && input[i - 2] != input[i - 1] && input[i - 2] != input[i - 3]
        && input[i - 3] != input[i - 0] && input[i - 3] != input[i - 1] && input[i - 3] != input[i - 2]) {
            output << (i + 1);
            break;
        }
    }*/

    const int groupSize = 4;
    for (int i = 0; i < input.size() - groupSize; ++i) {
        if (isDistinct(input, i, groupSize)) {
            output << (i + groupSize);
            break;
        }
    }

    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    const int groupSize = 14;
    for (int i = 0; i < input.size() - groupSize; ++i) {
        if (isDistinct(input, i, groupSize)) {
            output << (i + groupSize);
            break;
        }
    }

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
