#include <vector>
#include "parser.cpp"

#define TITLE "Day 02"

struct reactor {
    std::vector<int> levels;
    std::vector<int> boundaries;
};

typedef reactor day_t;

day_t parseInput(const char* const buffer, const int length) {
    day_t reactor;
    Parser p(buffer);

    int globalIndex = 0;

    reactor.boundaries.push_back(0);
    while (!p.eof()) {
        while (!p.isNewLine()) {
            int i = 0;
            p.readInt(i);
            p.consumeWhitespace();
            reactor.levels.push_back(i);

            globalIndex += 1;
        }

        p.readNewLine();
        reactor.boundaries.push_back(globalIndex);
    }

    return reactor;
}

void runPart1(const day_t& input) {
    int sum = 0;

    const std::vector<int> levels = input.levels;
    const std::vector<int> boundaries = input.boundaries;

    const int levelCount = static_cast<int>(boundaries.size());
    for (int i = 0; i < levelCount - 1; i++) {
        const int startIdx = boundaries[i];
        const int endIdx = boundaries[i + 1];

        bool valid = true;
        bool ascending = levels[startIdx] < levels[startIdx + 1];
        for (int j = startIdx; j < endIdx - 1 && valid; j++) {
            int diff = std::abs(levels[j] - levels[j + 1]);
            if (ascending != levels[j] < levels[j + 1] || diff > 3 || diff < 1) {
                valid = false;
            }
        }

        if (valid) {
            sum += 1;
        }
    }

    printf("%d\n", sum);
}

inline int signum(int a, int b) {
    return (a > b) - (a < b);
}

bool canMove(int sourceIdx, int targetIdx, const std::vector<int> &levels, int sign) {
    const int sourceLevel = levels[sourceIdx];
    const int targetLevel = levels[targetIdx];
    const int diff = std::abs(sourceLevel - targetLevel);

    return diff <= 3 && diff >= 1 && (sign == 0 || signum(sourceLevel, targetLevel) == sign);
}

bool isValidLine(int startIdx, int endIdx, const std::vector<int> &levels, int sign = 0, bool skipped = false) {
    int cost = 0;
    for (int i = startIdx; i < endIdx - 1; i++) {
        if (canMove(i, i + 1, levels, sign)) {
            sign = signum(levels[i], levels[i + 1]);
            cost += 1;
        } else if (!skipped && i + 2 < endIdx && canMove(i, i + 2, levels, sign)) {
            sign = signum(levels[i], levels[i + 2]);
            skipped = true;
            cost += 2;
            i++;
        } else if (!skipped && i + 2 == endIdx) {
            skipped = true;
            cost += 1;
            i++;
        }
    }

    return cost == endIdx - startIdx - 1;
}

void runPart2(const day_t& input) {
    int sum = 0;

    const std::vector<int> levels = input.levels;
    const std::vector<int> boundaries = input.boundaries;

    const int levelCount = static_cast<int>(boundaries.size());
    for (int i = 0; i < levelCount - 1; i++) {
        const int startIdx = boundaries[i];
        const int endIdx = boundaries[i + 1];

        if (isValidLine(startIdx, endIdx, levels, signum(levels[startIdx], levels[endIdx - 1]), false)
            || isValidLine(startIdx + 1, endIdx, levels, signum(levels[startIdx + 1], levels[endIdx - 1]) , true)) {
            sum += 1;
        }
    }

    printf("%d\n", sum);
}

// BOILER PLATE CODE BELOW

int main()
{
    constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
    char buffer[INPUT_BUFFER_SIZE];
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));

	const day_t parsedInput = parseInput(buffer, length);
	runPart1(parsedInput);
	runPart2(parsedInput);
}