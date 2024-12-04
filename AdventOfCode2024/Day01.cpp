#include <vector>
#include <algorithm>
#include "parser.cpp"

#define TITLE "Day 01"

struct notes {
    std::vector<int> leftSide;
    std::vector<int> rightSide;
};

typedef notes day_t;

day_t parseInput(const char* const buffer, const int length) {
    day_t note;
    Parser p(buffer);
    while (!p.eof()) {
        int a { 0 }, b { 0 };
        p.readInt(a);
        p.consumeWhitespace();
        p.readInt(b);
        p.readNewLine();

        note.leftSide.push_back(a);
        note.rightSide.push_back(b);
    }

    std::sort(note.leftSide.begin(), note.leftSide.end());
    std::sort(note.rightSide.begin(), note.rightSide.end());

    return note;
}

void runPart1(const day_t& input) {
    int sum = 0;
    for (size_t i = 0; i < input.leftSide.size(); i++) {
        sum += std::abs(input.leftSide[i] - input.rightSide[i]);
    }

    printf("%d\n", sum);
}

void runPart2(const day_t& input) {
    int sum = 0;

    int j = 0;
    const int size = static_cast<int>(input.leftSide.size());
    for (int i = 0; i < size; i++) {
        while (j < size && input.leftSide[i] > input.rightSide[j])
            j++;

        int countRight = 0;
        while (j < size && input.leftSide[i] == input.rightSide[j]) {
            j++;
            countRight++;
        }

        int countLeft = 1;
        while (i + 1 < size && input.leftSide[i] == input.leftSide[i + 1]) {
            i++;
            countLeft++;
        }

        sum += countLeft * input.leftSide[i] * countRight;
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