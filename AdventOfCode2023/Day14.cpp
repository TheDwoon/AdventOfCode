#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"

#define TITLE "Day 14"

#define ROUND_ROCK 'O'
#define CUBE_ROCK '#'
#define EMPTY_SPACE '.'

struct plateau {
    std::vector<char> map;
    int width {0};
    int height {0};

    char& operator()(int x, int y) {
        return map[y * width + x];
    }

    char operator()(int x, int y) const {
        return map[y * width + x];
    }
};

typedef plateau day_t;

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    while (!p.eof()) {
        while (!p.isNewLine()) {
            char c = p.read();
            assert(c == ROUND_ROCK || c == CUBE_ROCK || c == EMPTY_SPACE);
            result.map.push_back(c);
        }

        if (result.width == 0)
            result.width = static_cast<int>(result.map.size());

        p.readNewLine();
    }

    assert(static_cast<int>(result.map.size()) % result.width == 0);
    result.height = static_cast<int>(result.map.size()) / result.width;

    return result;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int score = 0;

    for (int y = 0; y < input.height; y++) {
        for (int x = 0; x < input.width; x++) {
            if (input(x, y) == ROUND_ROCK) {
                int ny = y;
                while (ny > 0 && input(x, ny - 1) == EMPTY_SPACE)
                    ny--;

                char& oldPosition = input(x, y);
                char& newPosition = input(x, ny);
                oldPosition = EMPTY_SPACE;
                newPosition = ROUND_ROCK;

                score += input.height - ny;
            }
        }
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
