#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 02"
#define ME_ROCK 'X'
#define ME_PAPER 'Y'
#define ME_SCISSOR 'Z'
#define ME_LOSE 'X'
#define ME_DRAW 'Y'
#define ME_WIN 'Z'
#define ELF_ROCK 'A'
#define ELF_PAPER 'B'
#define ELF_SCISSOR 'C'
#define POINTS_ROCK 1
#define POINTS_PAPER 2
#define POINTS_SCISSOR 3
#define POINTS_LOSE 0
#define POINTS_DRAW 3
#define POINTS_WIN 6

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);

struct move {
    char elf { '0' };
    char me { '0' };
};

typedef std::vector<move> day_t;

day_t parseInput(const std::string &input) {
    std::stringstream stream(input);
    day_t result;
    move m;
    while (stream >> m.elf >> m.me) {
        result.push_back(m);
    }

    return result;
}

int get_points(const move& m) {
    int points { 0 };

    switch (m.me) {
        case ME_ROCK:
            points = POINTS_ROCK;
            switch (m.elf) {
                case ELF_ROCK:
                    points += POINTS_DRAW;
                    break;
                case ELF_PAPER:
                    points += POINTS_LOSE;
                    break;
                case ELF_SCISSOR:
                    points += POINTS_WIN;
                    break;
            }
            break;
        case ME_PAPER:
            points = POINTS_PAPER;
            switch (m.elf) {
                case ELF_ROCK:
                    points += POINTS_WIN;
                    break;
                case ELF_PAPER:
                    points += POINTS_DRAW;
                    break;
                case ELF_SCISSOR:
                    points += POINTS_LOSE;
                    break;
            }
            break;
        case ME_SCISSOR:
            points = POINTS_SCISSOR;
            switch (m.elf) {
                case ELF_ROCK:
                    points += POINTS_LOSE;
                    break;
                case ELF_PAPER:
                    points += POINTS_WIN;
                    break;
                case ELF_SCISSOR:
                    points += POINTS_DRAW;
                    break;
            }
            break;
    }

    return points;
}

void compute_action(move& m) {
    switch (m.elf) {
        case ELF_ROCK:
            switch (m.me) {
                case ME_WIN:
                    m.me = ME_PAPER;
                    break;
                case ME_DRAW:
                    m.me = ME_ROCK;
                    break;
                case ME_LOSE:
                    m.me = ME_SCISSOR;
                    break;
            }
            break;
        case ELF_PAPER:
            switch (m.me) {
                case ME_WIN:
                    m.me = ME_SCISSOR;
                    break;
                case ME_DRAW:
                    m.me = ME_PAPER;
                    break;
                case ME_LOSE:
                    m.me = ME_ROCK;
                    break;
            }
            break;
        case ELF_SCISSOR:
            switch (m.me) {
                case ME_WIN:
                    m.me = ME_ROCK;
                    break;
                case ME_DRAW:
                    m.me = ME_SCISSOR;
                    break;
                case ME_LOSE:
                    m.me = ME_PAPER;
                    break;
            }
            break;
    }
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int points { 0 };
    for (const move& m : input) {
        points += get_points(m);
    }

    output << points;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    int points { 0 };
    for (move& m : input) {
        compute_action(m);
        points += get_points(m);
    }

    output << points;
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
