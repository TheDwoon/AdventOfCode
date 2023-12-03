#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <set>
#include "parser.cpp"

#define TITLE "Day 03"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);

struct board {
    unsigned int width { 0 };
    unsigned int height { 0 };
    const char* map { nullptr };

    char operator()(unsigned int x, unsigned int y) const {
        return map[y * width + x];
    }
};

typedef board day_t;

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isDot(char c) {
    return c == '.';
}

bool isSymbol(char c) {
    return !isNumber(c) && !isDot(c);
}

bool hasAdjacentSymbol(const board &board, unsigned int x, unsigned int y) {
    return (x > 0 && y > 0 && isSymbol(board(x - 1, y - 1)))
           || (x > 0 && isSymbol(board(x - 1, y + 0)))
           || (x > 0 && y + 1 < board.height && isSymbol(board(x - 1, y + 1)))
           || (y + 1 < board.height && isSymbol(board(x + 0, y + 1)))
           || (y > 0 && isSymbol(board(x + 0, y - 1)))
           || (x + 1 < board.width && y > 0 && isSymbol(board(x + 1, y - 1)))
           || (x + 1 < board.width && isSymbol(board(x + 1, y + 0)))
           || (x + 1 < board.width && y + 1 < board.height && isSymbol(board(x + 1, y + 1)));
}

day_t parseInput(std::string &input) {
    day_t board;

    const size_t length = input.length();
    const char* source = input.c_str();
    Parser p(source);

    unsigned int padding {0};
    if (p.findNext("\r\n", board.width)) {
        padding = 2;
    } else if (p.findNext("\n", board.width)) {
        padding = 1;
    }

    assert(board.width > 0);
    board.height = length / (board.width + padding);
    assert(board.height > 0);

    char* map = new char[board.height * board.width];
    for (unsigned int y = 0; y < board.height; y++) {
        for (unsigned int x = 0; x < board.width; x++) {
            assert(y * (board.width + padding) + x < length);
            map[y * board.width + x] = source[y * (board.width + padding) + x];
        }
    }

    board.map = map;
    return board;
}

std::string runPart1(day_t& board) {
    std::stringstream output;

    int score = 0;

    for (unsigned int y = 0; y < board.height; y++) {
        bool adjacentSymbol = false;

        int parsedNumber = 0;
        for (unsigned int x = 0; x < board.width; x++) {
            char c = board(x, y);

            if (isNumber(c)) {
                parsedNumber = parsedNumber * 10 + (c - '0');
                adjacentSymbol |= hasAdjacentSymbol(board, x, y);
            } else if (parsedNumber > 0 && adjacentSymbol) {
                score += parsedNumber;
                parsedNumber = 0;
                adjacentSymbol = false;
            } else if (parsedNumber > 0) {
                parsedNumber = 0;
            }
        }

        if (parsedNumber > 0 && adjacentSymbol) {
            score += parsedNumber;
        }
    }

    output << score;
    return output.str();
}

int getNumberStartIndex(const board &board, int x, int y) {
    while (x > 0 && isNumber(board(x - 1 , y)))
        x--;

    return static_cast<int>(y * board.width + x);
}

unsigned int getNumber(const board &board, int x, int y) {
    while (x > 0 && isNumber(board(x - 1 , y)))
        x--;

    unsigned int number = 0;
    for (; x < board.width && isNumber(board(x, y)); x++) {
        number = number * 10 + (board(x, y) - '0');
    }

    return number;
}

unsigned int getNumber(const board &board, int index) {
    int x = static_cast<int>(index % board.width);
    int y = static_cast<int>(index / board.width);

    return getNumber(board, x, y);
}


std::string runPart2(day_t& board) {
    std::stringstream output;

    unsigned long long score = 0;
    for (int y = 0; y < board.height; y++) {
        for (int x = 0; x < board.height; x++) {
            if (board(x, y) != '*')
                continue;

            int numbers[3] {-1,-1,-1};
            for (int sy = std::max(0, y - 1); sy <= y + 1 && sy < board.height; sy++) {
                for (int sx = std::max(0, x - 1); sx <= x + 1 && sx < board.width; sx++) {
                    if (isNumber(board(sx, sy))) {
                        int number = getNumberStartIndex(board, sx, sy);

                        for (int & num : numbers) {
                            if (number == num)
                                break;
                            else if (num == -1) {
                                num = number;
                                break;
                            }
                        }
                    }
                }
            }

            if (numbers[0] != -1 && numbers[1] != -1 && numbers[2] == -1)
                score += getNumber(board, numbers[0]) * getNumber(board, numbers[1]);
        }
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
