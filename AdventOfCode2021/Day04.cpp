#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 04"

struct Field {
    uint16_t number { 0 };
    bool marked { false };
};

struct Board {
    Field fields[25];

    size_t sumUnmarked() {
        size_t sum { 0 };
        for (size_t y = 0; y < 5; ++y) {
            for (size_t x = 0; x < 5; ++x) {
                Field &field = getField(x, y);
                if (!field.marked)
                    sum += field.number;
            }
        }

        return sum;
    }

    void mark(uint16_t number) {
        for (size_t y = 0; y < 5; ++y) {
            for (size_t x = 0; x < 5; ++x) {
                Field &field = getField(x, y);
                if (field.number == number)
                    field.marked = true;
            }
        }
    }

    bool isWon() {
        for (size_t row = 0; row < 5; row++) {
            if (getField(0, row).marked
                && getField(1, row).marked
                && getField(2, row).marked
                && getField(3, row).marked
                && getField(4, row).marked)
                return true;
        }

        for (size_t col = 0; col < 5; col++) {
            if (getField(col, 0).marked
                && getField(col, 1).marked
                && getField(col, 2).marked
                && getField(col, 3).marked
                && getField(col, 4).marked)
                return true;
        }

        return false;
    }

    inline Field& getField(size_t x, size_t y) {
        return fields[y * 5 + x];
    }
};

struct Bingo {
    std::vector<uint16_t> drawnNumbers;
    std::vector<Board> boards;

    int isWon() {
        for (int i = 0; i < boards.size(); ++i)
            if (boards[i].isWon())
                return i;

        return -1;
    }
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef Bingo day_t;

day_t parseInput(std::string &input) {
    Bingo bingo;
    std::stringstream stream(input);

    std::vector<std::string> blocks = tokenize(input, "\n\n");

    // parse numbers
    std::stringstream numberStream(blocks[0]);
    uint16_t number;

    while (numberStream >> number) {
        bingo.drawnNumbers.push_back(number);
        numberStream.get();
    }

    for (size_t i = 1; i < blocks.size(); ++i) {
        Board board;

        std::stringstream fieldStream(blocks[i]);
        size_t j { 0 };
        while (fieldStream >> number)
            board.fields[j++].number = number;

        bingo.boards.push_back(board);
    }

    return bingo;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    uint16_t currentNumber;
    int winningBoard = -1;
    for (size_t round = 0; winningBoard < 0; ++round) {
        currentNumber = input.drawnNumbers[round];
        for (Board &board : input.boards) {
            board.mark(currentNumber);
        }

        winningBoard = input.isWon();
    }

    output << (input.boards[winningBoard].sumUnmarked() * currentNumber);

    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    uint16_t currentNumber;
    size_t playingBoards = input.boards.size();
    size_t round = 0;
    for (; playingBoards > 1 && round < input.drawnNumbers.size(); ++round) {
        currentNumber = input.drawnNumbers[round];
        for (Board &board : input.boards) {
            board.mark(currentNumber);
        }

        int winningBoard = input.isWon();
        while (winningBoard >= 0) {
            input.boards.erase(input.boards.begin() + winningBoard);
            playingBoards--;
            winningBoard = input.isWon();
        }
    }

    Board &board = input.boards[0];
    while (!board.isWon()) {
        currentNumber = input.drawnNumbers[round++];
        board.mark(currentNumber);
    }

    output << (board.sumUnmarked() * currentNumber);

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
    std::cout << std::endl;
    std::cout << "**************************************" << std::endl;
    std::cout << std::endl;

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