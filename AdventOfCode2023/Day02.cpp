#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"

#define TITLE "Day 02"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);

enum cube_color { red = 0, green = 1, blue = 2, undefined = 4 };

struct cube {
    cube_color color { undefined };
    int count { 0 };
};

typedef std::vector<cube> cube_set;

struct game {
    int id { 0 };
    std::vector<cube_set> sets;
};

typedef std::vector<game> day_t;

const char color_blue[] = "blue";
const char color_red[] = "red";
const char color_green[] = "green";

void readCube(Parser &p, cube &cube) {
    p.readInt(cube.count);
    p.consume(' ');
    if (p.consume(color_red))
        cube.color = red;
    else if (p.consume(color_green))
        cube.color = green;
    else if (p.consume(color_blue))
        cube.color = blue;
    else
        throw std::exception();
}

day_t parseInput(std::string &input) {
    day_t result;

    Parser p = input.c_str();

    while (!p.eof()) {
        game game;
        p.consume("Game ");
        p.readInt(game.id);
        p.consume(": ");

        do {
            cube_set set;
            do {
                cube cube;
                readCube(p, cube);
                set.push_back(cube);
            } while (p.consume(", "));

            game.sets.push_back(set);
            set = {};
        } while (p.consume("; "));
        result.push_back(game);
        game = {};

        p.readNewLine();
    }

    return result;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    const int limits[] = { 12, 13, 14 };

    int score = 0;
    for (const game &game : input) {
        bool valid = true;
        for (const cube_set &cubeSet : game.sets) {
            for (const cube& cube : cubeSet) {
                if (cube.count > limits[cube.color]) {
                    valid = false;
                    goto endgame; // >:]
                }
            }
        }
        endgame:
        score += valid * game.id;
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    int score = 0;
    for (const game &game : input) {
        int limits[] = { 0, 0, 0 };
        for (const cube_set &cubeSet : game.sets) {
            for (const cube& cube : cubeSet) {
                limits[cube.color] = std::max(limits[cube.color], cube.count);
            }
        }

        score += limits[0] * limits[1] * limits[2];
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
