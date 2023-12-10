#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"

#define TITLE "Day 09"

typedef std::vector<int> sequence;
typedef std::vector<sequence> day_t;

day_t parseInput(std::string &input) {
    day_t result;
    int i;
    Parser p(input.c_str());
    while (!p.eof()) {
        sequence s;
        while (p.readNumber(i)) {
            p.consume(' ');
            s.push_back(i);
        }

        p.readNewLine();
        result.push_back(s);
    }

    return result;
}

sequence generateDeltaSequence(const sequence &s) {
    assert (s.size() > 1);
    sequence r(s.size() - 1);
    for (unsigned int i = 0; i < r.size(); i++) {
        r[i] = s[i + 1] - s[i];
    }
    
    return r;
}

bool isAllZero(const sequence &s) {
    for (int i : s) {
        if (i != 0)
            return false;
    }

    return true;
}

int extrapolateValue(const sequence &s) {
    std::vector<sequence> sequences { s };
    while (!isAllZero(sequences.back())) {
        sequences.push_back(generateDeltaSequence(sequences.back()));
    }

    int value = 0;
    for (int i = static_cast<int>(sequences.size()) - 2; i >= 0; i--) {
        value = value + sequences[i].back();
    }

    return value;
}

int extrapolatePreviousValue(const sequence &s) {
    std::vector<sequence> sequences { s };
    while (!isAllZero(sequences.back())) {
        sequences.push_back(generateDeltaSequence(sequences.back()));
    }

    int value = 0;
    for (int i = static_cast<int>(sequences.size()) - 2; i >= 0; i--) {
        value = sequences[i].front() - value;
    }

    return value;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int score = 0;
    for (const sequence &s : input) {
        score += extrapolateValue(s);
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    int score = 0;
    for (const sequence &s : input) {
        score += extrapolatePreviousValue(s);
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

    auto gt0 = std::chrono::high_resolution_clock::now();
	const std::string originalInput = readInput();
    auto gt1 = std::chrono::high_resolution_clock::now();

    std::cout << "Reading Input: " << formatTime(gt1 - gt0) << '\n';
	std::cout << "**************************************\n";

    std::string input = originalInput;
	auto t0 = std::chrono::high_resolution_clock::now();
	day_t parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << '\n';
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 1 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

	input = originalInput;
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 2 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

    auto gt2 = std::chrono::high_resolution_clock::now();
    std::cout << "Global Time: " << formatTime(gt2 - gt0) << '\n';
	std::cout << "**************************************" << std::endl;
}
