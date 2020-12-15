#include "Day15.h"
#include <map>
#include <set>

std::vector<int> Day15::parseInput(std::string &input) {
    std::vector<int> parsed;
    std::stringstream stream(input);
    int i = 0;
    char c;
    while (stream && stream >> i) {
        parsed.push_back(i);
        stream >> c;
    }

    return parsed;
}

int playGame(std::vector<int>& initial, int rounds) {
    std::map<int, int> memory;

    int turn = 0;
    for (int i = 0; i < initial.size() - 1; i++) {
        memory[initial[i]] = ++turn;
    }

    int lastNumber = initial[initial.size() - 1];
    turn = initial.size() + 1;
    for (;turn <= rounds; ++turn) {
        int nextNumber = memory[lastNumber];
        if (nextNumber != 0) {
            nextNumber = turn - 1 - nextNumber;
        }
        memory[lastNumber] = turn - 1;
        lastNumber = nextNumber;
    }

    return lastNumber;
}

std::string Day15::runPart1(std::vector<int> &input) {
    std::stringstream output;

    output << playGame(input, 2020);
    return output.str();
}

std::string Day15::runPart2(std::vector<int> &input) {
    std::stringstream output;
    output << playGame(input, 30000000);

    return output.str();
}
