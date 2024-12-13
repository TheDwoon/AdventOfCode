#include <cstdint>
#include <cstring>
#include <vector>
#include "vec2.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct claw_machine {
    vec2i buttonA;
    vec2i buttonB;
    vec2i price;
};

void parse_input(const char* const buffer, std::vector<claw_machine>& machines) {
    Parser p(buffer);
    claw_machine machine;
    while (!p.eof()) {
        p.consume("Button A: X+");
        p.readNumber(machine.buttonA.x);
        p.consume(", Y+");
        p.readNumber(machine.buttonA.y);
        p.readNewLine();

        p.consume("Button B: X+");
        p.readNumber(machine.buttonB.x);
        p.consume(", Y+");
        p.readNumber(machine.buttonB.y);
        p.readNewLine();

        p.consume("Prize: X=");
        p.readNumber(machine.price.x);
        p.consume(", Y=");
        p.readNumber(machine.price.y);
        p.readNewLine();
        p.readNewLine();

        machines.push_back(machine);
    }
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    uint64_t part2 = 0;

    std::vector<claw_machine> machines;
    parse_input(buffer, machines);
    constexpr int64_t price_offset = 10000000000000LL;

    for (const auto&[buttonA, buttonB, price] : machines) {
        // part 1
        const int numerator = buttonA.x * price.y - buttonA.y * price.x;
        const int denominator = buttonA.x * buttonB.y - buttonB.x * buttonA.y;
        const int b = numerator / denominator;
        const int a = (price.x - b * buttonB.x) / buttonA.x;

        if (numerator % denominator == 0 && (price.x - b * buttonB.x) % buttonA.x == 0) {
            part1 += 3 * a + b;
#ifndef NDEBUG
            printf("Part 1) a: %d, b: %d\n", a, b);
        } else {
            printf("Part 1) Machine cannot be solved\n");
#endif
        }

        // part 2
        const vec2<int64_t> corrected_price { price_offset + price.x, price_offset + price.y };
        const int64_t large_numerator = buttonA.x * corrected_price.y - buttonA.y * corrected_price.x;
        const int64_t large_denominator = buttonA.x * buttonB.y - buttonB.x * buttonA.y;
        const int64_t large_b = large_numerator / large_denominator;
        const int64_t large_a = (corrected_price.x - large_b * buttonB.x) / buttonA.x;

        if (large_numerator % large_denominator == 0 && (corrected_price.x - large_b * buttonB.x) % buttonA.x == 0) {
            part2 += 3ULL * large_a + large_b;
#ifndef NDEBUG
            printf("Part 2) a: %llu, b: %llu\n", large_a, large_b);
        } else {
            printf("Part 2) Machine cannot be solved\n");
#endif
        }
    }

    printf("%d\n",part1);
    printf("%llu\n", part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
