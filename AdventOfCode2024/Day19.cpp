#include <cstring>
#include <deque>
#include <vector>

#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

constexpr char WHITE = 'w';
constexpr char BLUE = 'u';
constexpr char BLACK = 'b';
constexpr char RED = 'r';
constexpr char GREEN = 'g';

struct node {
    char color;
    node* next[5];
};

int getTowelIndex(const char stripe) {
    switch (stripe) {
        case WHITE:
            return 0;
        case BLUE:
            return 1;
        case BLACK:
            return 2;
        case RED:
            return 3;
        case GREEN:
            return 4;
        default:
            return -1;
    }
}

bool checkPattern(const std::vector<char>& pattern, const std::vector<char>& stripe, const int index) {
    for (int i = 0; i < stripe.size(); i++) {
        if (index + i >= pattern.size() || pattern[index + i] != stripe[i]) {
            return false;
        }
    }

    return true;
}

int canProducePattern(const std::vector<std::vector<char>> *pattern_tree, const std::vector<char> &pattern) {
    int matches = 0;

    std::deque<int> queue { 0 };
    while (!queue.empty()) {
        const int index = queue.front();
        queue.pop_front();

        const int towelIndex = getTowelIndex(pattern[index]);
        const std::vector<std::vector<char>> &available_patterns = pattern_tree[towelIndex];
        for (const std::vector<char>& partial_pattern : available_patterns) {
            if (checkPattern(pattern, partial_pattern, index)) {
                const int next_index = static_cast<int>(partial_pattern.size()) + index;
                if (next_index == pattern.size()) {
                    matches++;
                } else {
                    assert(next_index < pattern.size());
                    queue.push_back(next_index);
                }
            }
        }
    }

    return matches;
}

void runDay(const char* buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    std::vector<std::vector<char>> towels[5];

    std::vector<char> pattern;
    pattern.reserve(8);

    while (*buffer != '\n') {
        const char c = *buffer++;
        switch (c) {
            case WHITE:
            case BLUE:
            case BLACK:
            case RED:
            case GREEN:
                pattern.push_back(c);
                break;
            case ',':
                const int towelIndex = getTowelIndex(pattern[0]);
                assert(towelIndex != -1);
                towels[towelIndex].push_back(pattern);
                pattern.clear();
                break;
        }
    }

    const int towelIndex = getTowelIndex(pattern[0]);
    assert(towelIndex != -1);
    towels[towelIndex].push_back(pattern);
    pattern.clear();

    buffer += 2;

    std::vector<std::vector<char>> suggested_patterns;
    while (*buffer != '\0') {
        const char c = *buffer++;
        switch (c) {
            case WHITE:
            case BLUE:
            case BLACK:
            case RED:
            case GREEN:
                pattern.push_back(c);
                break;
            case '\n':
                suggested_patterns.push_back(pattern);
                pattern.clear();
        }
    }

    for (const std::vector<char>& suggested_pattern : suggested_patterns) {
        printf("A line!\n");
        if (canProducePattern(towels, suggested_pattern) > 0) {
            part1++;
        }
    }

    printf("%d\n",part1);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
