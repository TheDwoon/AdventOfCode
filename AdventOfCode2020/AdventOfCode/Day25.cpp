#include "Day25.h"

Keys Day25::parseInput(std::string &input) {
    Keys keys;
    std::stringstream stream(input);
    stream >> keys.card >> keys.door;

    return keys;
}

uint64_t computeLoopSize(public_key_t key) {
    uint64_t loopSize = 0;
    uint64_t resultingKey = 1;
    do {
        resultingKey = (resultingKey * SUBJECT_NUMBER) % SUBJECT_DIVIDER;
        loopSize++;
    } while (resultingKey != key);

    return loopSize;
}

uint64_t applyLoop(public_key_t key, uint64_t loopSize) {
    uint64_t result = 1;
    for (int i = 0; i < loopSize; i++)
        result = (result * key) % SUBJECT_DIVIDER;

    return result;
}

bool Day25::testPart1(Keys &input) {
    std::string output = runPart1(input);
    return output == "14897079";
}

std::string Day25::runPart1(Keys &input) {
    std::stringstream output;

    uint64_t doorLoop = computeLoopSize(input.door);

    output << applyLoop(input.card, doorLoop);
    return output.str();
}
