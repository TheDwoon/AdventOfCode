#include <vector>
#include "parser.cpp"

#define TITLE "Day 03"

enum instruction_type {
    DO = 0,
    DONT = 1,
    MUL = 3
};

struct instruction {
    instruction_type type;
    int a;
    int b;
};

typedef std::vector<instruction> day_t;

day_t parseInput(const char* const buffer, const int length) {
    day_t instructions;
    Parser p(buffer);
    int a, b;
    while (!p.eof()) {
        if (p.consume("mul(") && p.readNumber(a) && p.consume(",") && p.readNumber(b) && p.consume(")")) {
            instructions.emplace_back(MUL, a, b);
        } else if (p.consume("do()")) {
            instructions.emplace_back(DO, 0, 0);
        } else if (p.consume("don't()")) {
            instructions.emplace_back(DONT, 0, 0);
        } else {
          p.read();
        }
    }

    return instructions;
}

void runDay(const day_t& input) {
    int part1 = 0;
    int part2 = 0;

    bool enabled = true;
    for (const auto&[type, a, b] : input) {
        part1 += a * b;
        if (type == MUL && enabled) {
            part2 += a * b;
        } else {
            enabled = type == DO;
        }
    }

    printf("%d\n", part1);
    printf("%d\n", part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
    char buffer[INPUT_BUFFER_SIZE];
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));

	const day_t parsedInput = parseInput(buffer, length);
	runDay(parsedInput);
}
