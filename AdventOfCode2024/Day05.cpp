#include <algorithm>
#include <vector>
#include <set>
#include <array>
#include "parser.cpp"

struct instructions {
    std::array<std::set<int>, 100> rules;
    std::vector<std::vector<int>> updates;
};

instructions parse_instructions(const char* const buffer, const int length) {
    Parser p(buffer);
    instructions i;

    int a, b;
    while (!p.isNewLine()) {
        p.readNumber(a);
        p.consume('|');
        p.readNumber(b);
        p.readNewLine();

        i.rules[a].insert(b);
    }
    p.readNewLine();

    while (!p.eof()) {
        std::vector<int> update;
        while (!p.isNewLine()) {
            p.readNumber(a);
            p.consume(',');

            update.push_back(a);
        }

        p.readNewLine();
        i.updates.push_back(update);
    }

    return i;
}

bool checkValid(const std::vector<int>& update, const instructions& instructions) {
    int size = static_cast<int>(update.size());
    for (int i = size - 1; i > 0; i--) {
        const std::set<int>& rules = instructions.rules[update[i]];
        for (int j = 0; j < i; j++) {
            if (rules.contains(update[j])) {
                return false;
            }
        }
    }

    return true;
}

int fixSorting(std::vector<int>& update, const instructions& instructions) {
    const int size = static_cast<int>(update.size());
    for (int i = size - 1; i > 0; i--) {
        const std::set<int>& rules = instructions.rules[update[i]];
        for (int j = 0; j < i; j++) {
            if (rules.contains(update[j])) {
                std::swap(update[i], update[j]);
                i = i + 1; // one extra because of -- running at end of loop
                break;
            }
        }
    }

    return update[size / 2];
}

void runDay(const char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    instructions printInstructions = parse_instructions(buffer, length);

    for (auto& update : printInstructions.updates) {
        if (checkValid(update, printInstructions)) {
            part1 += update[update.size() / 2];
        } else {
            part2 += fixSorting(update, printInstructions);
        }
    }

    printf("%d\n",part1);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
    char buffer[INPUT_BUFFER_SIZE];
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
