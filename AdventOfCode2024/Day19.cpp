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
    char color { '_' };
    bool leaf { false };
    node* next[5] { nullptr };
};

int getColorIndex(const char color) {
    switch (color) {
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

void buildTree(node* root, const std::vector<char> &pattern) {
    node* current = root;
    for (int i = 0; i < pattern.size(); i++) {
        const int color_index = getColorIndex(pattern[i]);
        if (current->next[color_index] == nullptr) {
            node* next_node = new node();
            next_node->color = pattern[i];
            current->next[color_index] = next_node;
        }

        current = current->next[color_index];
    }

    current->leaf = true;
}

bool checkTree(node* root, const std::vector<char> &pattern) {
    node* current = root;
    for (int i = 0; i < pattern.size(); i++) {
        const int color_index = getColorIndex(pattern[i]);
        if (current->next[color_index] == nullptr) {
            if (current->leaf && root->next[color_index] != nullptr) {
                current = root->next[color_index];
            } else {
                return false;
            }
        } else {
            current = current->next[color_index];
        }
    }

    return true;
}

void runDay(const char* buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    node* const root = new node();
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
                buildTree(root, pattern);
                pattern.clear();
                break;
        }
    }

    buildTree(root, pattern);
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
        if (checkTree(root, suggested_pattern)) {
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
