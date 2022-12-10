#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 10"

#define NOOP 1
#define NOOP_CYCLES 1
#define ADDX 2
#define ADDX_CYCLES 2

struct operation {
    uint8_t code;
    int arg;
    int duration;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<operation> day_t;

day_t parseInput(std::string &input) {
    day_t parsed;
    std::string opWord;
    int arg;

    std::stringstream stream(input);
    while (stream >> opWord) {
        if (opWord == "noop") {
            parsed.push_back({ NOOP, 0, NOOP_CYCLES });
        } else {
            stream >> arg;
            parsed.push_back({ ADDX, arg, ADDX_CYCLES });
        }
    }

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    int score { 0 };
    size_t clock { 0 };
    int x { 1 };
    for (operation& op : input) {
        while (op.duration--) {
            ++clock;

            if ((clock + 20) % 40 == 0) {
                score += clock * x;
            }

            if (op.code == ADDX && !op.duration) {
                x += op.arg;
            }
        }
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    size_t clock { 0 };
    int x { 1 };
    for (operation& op : input) {
        while (op.duration--) {
            ++clock;

            // draw CRT
            int crtPos = (clock - 1) % 40;
            if (clock > 1 && crtPos % 40 == 0)
                output << "\n";

            if (crtPos >= x - 1 && crtPos <= x + 1)
                output << "#";
            else
                output << ".";

            if (op.code == ADDX && !op.duration) {
                x += op.arg;
            }
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
