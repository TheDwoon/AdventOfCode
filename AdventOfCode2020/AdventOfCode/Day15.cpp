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

bool canRemember(int number, std::map<int, std::array<int, 2>> &memory) {
   return memory.find(number) != memory.end();
}

void rememberTurns(int number, int turn, std::map<int, std::array<int, 2>> &memory) {
    std::array<int, 2>& turns = memory[number];
    turns[1] = turns[0];
    turns[0] = turn;
}

std::string Day15::runPart1(std::vector<int> &input) {
    std::stringstream output;

    std::map<int, std::array<int, 2>> memory;

    int turn = 1;
    for (int number : input) {
        rememberTurns(number, turn++, memory);
    }

    int lastSpokenNumber = input[input.size() - 1];
    for (;turn <= 2020; turn++) {
        if (!canRemember(lastSpokenNumber, memory)) {
            lastSpokenNumber = 0;
        } else {
            std::array<int, 2>& rememberedTurns = memory[lastSpokenNumber];
            if (rememberedTurns[1] == 0) {
                lastSpokenNumber = turn - 1 - rememberedTurns[0];
            } else {
                lastSpokenNumber = rememberedTurns[0] - rememberedTurns[1];
            }
        }

        rememberTurns(lastSpokenNumber, turn, memory);
    }

    output << lastSpokenNumber;
    return output.str();
}

std::string Day15::runPart2(std::vector<int> &input) {
    std::stringstream output;

    std::map<int, std::array<int, 2>> memory;

    int turn = 1;
    for (int number : input) {
        rememberTurns(number, turn++, memory);
    }

    int lastSpokenNumber = input[input.size() - 1];
    for (;turn <= 30000000; turn++) {
        if (!canRemember(lastSpokenNumber, memory)) {
            lastSpokenNumber = 0;
        } else {
            std::array<int, 2>& rememberedTurns = memory[lastSpokenNumber];
            if (rememberedTurns[1] == 0) {
                lastSpokenNumber = turn - 1 - rememberedTurns[0];
            } else {
                lastSpokenNumber = rememberedTurns[0] - rememberedTurns[1];
            }
        }

        rememberTurns(lastSpokenNumber, turn, memory);
    }

    output << lastSpokenNumber;
    return output.str();
}
