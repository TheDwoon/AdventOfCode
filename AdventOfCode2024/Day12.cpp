#include <cstring>
#include "vec2.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct map {
    char* buffer;
    const int width;
    const int height;
    const int lineLength;

    [[nodiscard]]
    bool contains(const vec2i& pos) const {
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
    }

    [[nodiscard]]
    char& at(const int x, const int y) const {
        assert (x >= 0 && x < width);
        assert (y >= 0 && y < height);
        return buffer[y * lineLength + x];
    }

    [[nodiscard]]
    char& at(const vec2i& pos) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const vec2i& pos) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const int x, const int y) const {
        assert (x >= 0 && x < width);
        assert (y >= 0 && y < height);
        return buffer[y * lineLength + x];
    }
};


void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    // parse input
    const map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };

    char area[26] { 0 };
    int perimeter[26] { 0 };

    for (int y = 0; y < m.height - 1; y++) {
        for (int x = 0; x < m.width - 1; x++) {
            const char current_plot = m(x, y);
            const int current_plot_index = current_plot - 'A';

            const char next_plot = m(x + 1, y);
            const char below_plot = m(x, y + 1);

            area[current_plot_index] += 1;

            if (current_plot != next_plot) {
                const int next_plot_index = next_plot - 'A';

                perimeter[current_plot_index] += 1;
                perimeter[next_plot_index] += 1;
            }

            if (current_plot != below_plot) {
                const int below_plot_index = below_plot - 'A';

                perimeter[current_plot_index] += 1;
                perimeter[below_plot_index] += 1;
            }
        }
    }

    // top edge & bottom edge
    for (int x = 0; x < m.width; x++) {
        const char top_plot = m(x, 0);
        const char bottom_plot = m(x, m.height - 1);

        // area not accounted for in main loop
        area[bottom_plot - 'A'] += 1;

        // account for outer perimeter
        perimeter[top_plot - 'A'] += 1;
        perimeter[bottom_plot - 'A'] += 1;
    }

    // left edge & right edge
    for (int y = 0; y < m.height; y++) {
        const char left_plot = m(0, y);
        const char right_plot = m(m.width - 1, y);

        // area not accounted for in main loop
        area[right_plot - 'A'] += 1;

        // account for outer perimeter
        perimeter[left_plot - 'A'] += 1;
        perimeter[right_plot - 'A'] += 1;
    }

    // incremented by both top and right edge loop
    area[m(m.width - 1, m.height - 1) - 'A'] -= 1;

    // right most horizontal edge segments missed by main loop
    for (int y = 0; y < m.height - 1; y++) {
        const char current_plot = m(m.width - 1, y);
        const char bottom_plot = m(m.width - 1, y + 1);

        if (current_plot != bottom_plot) {
            const int current_plot_index = current_plot - 'A';
            const int bottom_plot_index = bottom_plot - 'A';

            perimeter[current_plot_index] += 1;
            perimeter[bottom_plot_index] += 1;
        }
    }

    // bottom most vertical edge segments missed by main loop
    for (int x = 0; x < m.width - 1; x++) {
        const char current_plot = m(x, m.height - 1);
        const char next_plot = m(x + 1, m.height - 1);

        if (current_plot != next_plot) {
            const int current_plot_index = current_plot - 'A';
            const int next_plot_index = next_plot - 'A';

            perimeter[current_plot_index] += 1;
            perimeter[next_plot_index] += 1;
        }
    }

#ifndef NDEBUG
    printf("Areas:\n");
    for (int i = 0; i < 26; i++) {
        if (area[i] > 0) {
            printf("%c: %3d\n", i + 'A', area[i]);
        }
    }

    printf("Perimeter:\n");
    for (int i = 0; i < 26; i++) {
        if (perimeter[i] > 0) {
            printf("%c: %3d\n", i + 'A', perimeter[i]);
        }
    }
#endif

    printf("%d\n", part1);
    printf("%d\n", part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
