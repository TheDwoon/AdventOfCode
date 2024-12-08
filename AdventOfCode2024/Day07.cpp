#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdint>
#include <string.h>

#include "parser.cpp"

struct test_data {
    int64_t result;
    std::vector<int64_t> numbers;
};

template<int OPERATIONS_COUNT>
bool testConfiguration(const test_data& t, int configuration) {
    const size_t size = t.numbers.size();

    int64_t result = t.numbers[0];
    for (size_t i = 1; i < size && result <= t.result; i++) {
        int value = configuration % OPERATIONS_COUNT;
        configuration /= OPERATIONS_COUNT;
        if (value == 0) {
            result += t.numbers[i];
        } else if (value == 1) {
            result *= t.numbers[i];
        } else {
            int64_t multiplier = 10;
            while (multiplier <= t.numbers[i]) {
                multiplier *= 10;
            }

            result = result * multiplier + t.numbers[i];
        }
    }

    return t.result == result;
}

bool fuzzySolvePart1(const test_data& data) {
    const int maxConfig = static_cast<int>(std::pow(2, data.numbers.size() - 1));

    for (int configuration = 0; configuration < maxConfig; configuration++) {
        if (testConfiguration<2>(data, configuration)) {
            return true;
        }
    }

    return false;
}

bool fuzzySolvePart2(const test_data& data) {
    const int maxConfig = static_cast<int>(std::pow(3, data.numbers.size() - 1));

    for (int configuration = 0; configuration < maxConfig; configuration++) {
        if (testConfiguration<3>(data, configuration)) {
            return true;
        }
    }

    return false;
}

void runDay(const char* const buffer, const int length) {
    int64_t part1 = 0;
    int64_t part2 = 0;

    // read input
    Parser p(buffer);
    std::vector<test_data> tests;
    while (!p.eof()) {
        test_data data;
        p.readNumber(data.result);

        p.consume(": ");
        int i;
        while (!p.isNewLine()) {
            p.readNumber(i);
            p.consumeWhitespace();
            data.numbers.push_back(i);
        }

        tests.push_back(data);
        p.readNewLine();
    }

    // part 1
    for (const test_data &data: tests) {
        if (fuzzySolvePart1(data)) {
            part1 += data.result;
        }

        if (fuzzySolvePart2(data)) {
            part2 += data.result;
        }
    }

    printf("%ld\n",part1);
    printf("%ld\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
