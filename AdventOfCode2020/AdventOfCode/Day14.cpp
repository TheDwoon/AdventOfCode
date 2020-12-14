#include "Day14.h"
#include <map>
#include <math.h>

std::vector<FuzzyInstruction> Day14::parseInput(std::string &input) {
    std::vector<FuzzyInstruction> parsed;
    std::stringstream stream(input);
    for (std::string line; std::getline(stream, line);) {
        FuzzyInstruction instruction;
        if (line[1] == 'a') {
            // mask
            instruction.isMask = true;
            instruction.maskedSet = 0;
            instruction.mask = 0;

            size_t equal = line.find_first_of('=');
            for (char c : line.substr(equal + 2)) {
                instruction.mask <<= 1;
                instruction.maskedSet <<= 1;

                switch (c) {
                    case 'X':
                        instruction.mask |= 1;
                        break;
                    case '1':
                        instruction.maskedSet |= 1;
                        break;
                }
            }
        } else {
            // mem
            instruction.isMask = false;

            size_t bOpen = line.find_first_of('[');
            size_t bClose = line.find_first_of(']');
            instruction.memoryAccess = std::stoi(line.substr(bOpen + 1, bClose));

            size_t equal = line.find_first_of('=');
            instruction.memoryValue = std::stoi(line.substr(equal + 2));
        }

        parsed.push_back(instruction);
    }

    return parsed;
}

std::string Day14::runPart1(std::vector<FuzzyInstruction> &input) {
    std::stringstream output;

    uint64_t mask { 0 };
    uint64_t maskSet { 0 };
    std::map<uint64_t, uint64_t> memory;

    for (FuzzyInstruction& instruction : input) {
        if (instruction.isMask) {
            mask = instruction.mask;
            maskSet = instruction.maskedSet;
        } else {
            memory[instruction.memoryAccess] = (instruction.memoryValue & mask) | maskSet;
        }
    }

    uint64_t sum = 0;
    for (auto it = memory.begin(); it != memory.end(); ++it)
        sum += it->second;

    output << sum;
    return output.str();
}

std::string Day14::runPart2(std::vector<FuzzyInstruction> &input) {
    std::stringstream output;
    std::vector<size_t> floatingBits;
    uint64_t possibilities = 0;
    uint64_t mask { 0 };
    uint64_t maskSet { 0 };
    std::map<uint64_t, uint64_t> memory;

    for (FuzzyInstruction& instruction : input) {
        if (instruction.isMask) {
            mask = instruction.mask;
            maskSet = instruction.maskedSet;

            floatingBits.clear();
            for (int i = 0; i < 36 && (1 << i) - 1 <= mask; i++) {
                if (((mask >> i) & 1) == 1)
                    floatingBits.push_back(i);
            }
            possibilities = pow(2, floatingBits.size());
        } else {
            const uint64_t fuzzyAddress = (instruction.memoryAccess | maskSet) & ~mask;
            for (uint64_t bits = 0; bits < possibilities; bits++) {
                uint64_t address = fuzzyAddress;
                for (size_t i = 0; i < floatingBits.size() && (1 << i) <= bits; i++) {
                    size_t floating = floatingBits[i];
                    address |= ((bits >> i) & 1) << floating;
                }

                memory[address] = instruction.memoryValue;
            }
        }
    }

    uint64_t sum = 0;
    for (auto it = memory.begin(); it != memory.end(); ++it)
        sum += it->second;

    output << sum;
    return output.str();
}
