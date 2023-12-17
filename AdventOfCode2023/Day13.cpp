#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"
#include "vec2.cpp"

#define TITLE "Day 13"

struct note {
    std::vector<char> map;
    int width {0};
    int height {0};

    char operator()(int x, int y) const {
        return map[y * width + x];
    }
};

typedef std::vector<note> day_t;

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    while (!p.eof()) {
        note n;
        while (!p.isNewLine() && !p.eof()) {
            while (!p.isNewLine()) {
                n.map.push_back(p.read());
            }

            if (n.width == 0)
                n.width = static_cast<int>(n.map.size());

            p.readNewLine();
        }

        assert(static_cast<int>(n.map.size()) % n.width == 0);
        n.height = static_cast<int>(n.map.size()) / n.width;

        p.readNewLine();
        result.push_back(n);
    }

    return result;
}

bool isHorizontalMirrorAxis(const note& n, int y, int smudges) {
    int distance = 0;
    for (int d = 0; y - d >= 0 && y + d < n.height - 1 && distance <= smudges; d++) {
        for (int x = 0; x < n.width && distance <= smudges; x++) {
            if (n(x, y - d) != n(x, y + 1 + d))
                distance += 1;
        }
    }

    return distance == smudges;
}

bool isVerticalMirrorAxis(const note& n, int x, int smudges) {
    int distance = 0;
    for (int d = 0; x - d >= 0 && x + d < n.width - 1 && distance <= smudges; d++) {
        for (int y = 0; y < n.height && distance <= smudges; y++) {
            if (n(x - d, y) != n(x + 1 + d, y))
                distance += 1;
        }
    }

    return distance == smudges;
}

int computeScore(const day_t& input, int smudges) {
    int score = 0;

    for (const note& n : input) {
        int yAxis = 0;
        int xAxis = 0;

        // vertical
        while (yAxis < n.height - 1 && !isHorizontalMirrorAxis(n, yAxis, smudges))
            yAxis += 1;

        if (yAxis >= n.height - 1) {
            while (xAxis < n.width - 1 && !isVerticalMirrorAxis(n, xAxis, smudges))
                xAxis += 1;

            if (xAxis < n.width - 1) {
                // found xAxis
                score += xAxis + 1;
            }
        } else {
            // found yAxis
            score += 100 * (yAxis + 1);
        }
    }

    return score;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int score = computeScore(input, 0);
    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    int score = computeScore(input, 1);
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
