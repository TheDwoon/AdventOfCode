#include <cstring>
#include <algorithm>
#include <vector>
#include "parser.cpp"
#include "vec2.cpp"

#ifndef NDEBUG
#include <chrono>
#endif

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

struct radio {
    char frequency;
    vec2i position;
};

void printMap(const map& m) {
    printf("=========================\n");
    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            printf("%c", m(x, y));
        }
        printf("\n");
    }
    printf("=========================\n");
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    // parse input
    std::vector<radio> stations;
    const map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };
    for (int y = 0; y < m.height; ++y) {
        for (int x = 0; x < m.width; ++x) {
            if (m(x, y) != '.') {
                stations.emplace_back(m(x, y), vec2i {x, y});
            }
        }
    }

    memset(buffer, '.', length);

    char bufferCopy[INPUT_BUFFER_SIZE];
    memset(bufferCopy, '.', length);
    const map mCopy { bufferCopy, m.width, m.height, m.lineLength };

    // part 1
    std::ranges::sort(stations, [] (const radio& a, const radio& b) {
        return a.frequency < b.frequency;
    });

    const int size = static_cast<int>(stations.size());
    for (int i = 0; i < size; i++) {
        const radio& a = stations[i];
        for (int j = i + 1; j < size && stations[i].frequency == stations[j].frequency; j++) {
            const radio& b = stations[j];
            const vec2i d1 = a.position - b.position;
            const vec2i d2 = b.position - a.position;

            vec2i n1 = a.position + d1;
            vec2i n2 = b.position + d2;

            // anti node according to part 1
            if (m.contains(n1) && m(n1) == '.') {
                m.at(n1) = a.frequency;
                part1++;
            }
            if (m.contains(n2) && m(n2) == '.') {
                m.at(n2) = a.frequency;
                part1++;
            }

            // anti node according to part 2
            for (n1 = a.position; m.contains(n1); n1 += d1) {
                if (mCopy(n1) == '.') {
                    mCopy.at(n1) = a.frequency;
                    part2++;
                }
            }
            for (n2 = b.position; m.contains(n2); n2 += d2) {
                if (mCopy(n2) == '.') {
                    mCopy.at(n2) = a.frequency;
                    part2++;
                }
            }
        }
    }

    // printMap(m);

    printf("%d\n",part1);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
#ifndef NDEBUG
    auto gt0 = std::chrono::high_resolution_clock::now();
#endif
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
    runDay(buffer, length);
#ifndef NDEBUG
    auto gt1 = std::chrono::high_resolution_clock::now();
    printf("%.3f ms\n", (gt1 - gt0).count() / 1000000.0);
#endif
}
