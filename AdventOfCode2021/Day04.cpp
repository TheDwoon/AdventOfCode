#include "Day04.h"

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

Bingo Day04::parseInput(std::string &input) {
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

std::string Day04::runPart1(Bingo &input) {
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

std::string Day04::runPart2(Bingo &input) {
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
