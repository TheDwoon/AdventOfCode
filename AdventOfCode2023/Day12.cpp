#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <deque>
#include "parser.cpp"

#define BROKEN_SPRING '#'
#define WORKING_SPRING '.'
#define UNKNOWN_SPRING '?'

#define TITLE "Day 12"

struct engineer_note {
    std::vector<char> springs;
    std::vector<int> groupSizes;
};

typedef std::vector<engineer_note> day_t;

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());

    while (!p.eof()) {
        engineer_note note;

        while (!p.isWhitespace()) {
            char c = p.read();
            note.springs.push_back(c);
            assert(c == WORKING_SPRING || c == BROKEN_SPRING || c == UNKNOWN_SPRING);
        }

        p.consume(' ');

        int groupSize;
        while (!p.isNewLine() && p.readNumber(groupSize)) {
            note.groupSizes.push_back(groupSize);
            p.consume(',');
        }

        p.readNewLine();
        result.push_back(note);
    }

    return result;
}

void printNote(const engineer_note& note) {
    for (char c : note.springs)
        std::cout << c;

    std::cout << " ";
    for (int i : note.groupSizes)
        std::cout << i << " ";

    std::cout << '\n';
}

bool isValid(const engineer_note &n) {
    int brokenGroupIndex = 0;
    int i = 0;
    while (i < n.springs.size()) {
        while (i < n.springs.size() && n.springs[i] != BROKEN_SPRING)
            i += 1;

        int groupSize = 0;
        while (i < n.springs.size() && n.springs[i] == BROKEN_SPRING) {
            groupSize += 1;
            i += 1;
        }

        if (groupSize > 0) {
            if (brokenGroupIndex >= n.groupSizes.size() || n.groupSizes[brokenGroupIndex] != groupSize)
                return false;
            else
                brokenGroupIndex += 1;
        }
    }

    return brokenGroupIndex == n.groupSizes.size();
}

std::string runPart1(day_t& notes) {
    std::stringstream output;
    std::deque<engineer_note> queue(notes.cbegin(), notes.cend());
    int score = 0;

    while (!queue.empty()) {
        const engineer_note &note = queue.front();

        int i = 0;
        while (i < note.springs.size() && note.springs[i] != UNKNOWN_SPRING) {
            i += 1;
        }

        if (i < note.springs.size()) {
            engineer_note copy1 = note;
            copy1.springs[i] = WORKING_SPRING;
            engineer_note copy2 = note;
            copy2.springs[i] = BROKEN_SPRING;

            queue.push_back(copy1);
            queue.push_back(copy2);
        } else {
            if (isValid(note)) {
                score += 1;
            }
        }

        queue.pop_front();
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

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
        output << t.count() / 1000 << "µs";
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
