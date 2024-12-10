#ifndef NDEBUG
#include <chrono>
#endif

#include <cstring>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "parser.cpp"
#include "buffer.cpp"
#include "directions.cpp"
#include "vec2.cpp"

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

struct trail_segment {
    int trail_id;
    vec2i pos;
};

void runDay(char* const buffer, const int length) {
    // parse input
    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    const map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };

    // part 1
    aoc::ring_buffer<trail_segment, 2048> ring_buffer;

    for (int y = 0; y < m.height; ++y) {
        for (int x = 0; x < m.width; ++x) {
            if (m(x, y) == '0')
                ring_buffer.emplace_back({ y * 100 + x, {x, y} });
        }
    }

#ifndef NDEBUG
    unsigned int max_usage = 0;
#endif
    std::unordered_map<int, int> trail_ratings;
    std::unordered_map<int, std::unordered_set<vec2i>> trails;
    while (!ring_buffer.is_empty()) {
#ifndef NDEBUG
        max_usage = std::max(max_usage, ring_buffer.available());
#endif
        const trail_segment segment = ring_buffer.pop_front();

        const char c = m(segment.pos);
        if (c < '9') {
            const vec2i up = segment.pos + aoc::direction::UP;
            const vec2i down = segment.pos + aoc::direction::DOWN;
            const vec2i left = segment.pos + aoc::direction::LEFT;
            const vec2i right = segment.pos + aoc::direction::RIGHT;

            if (m.contains(up) && m(up) == c + 1) {
                ring_buffer.emplace_back({ segment.trail_id, {up.x, up.y} });
            }
            if (m.contains(down) && m(down) == c + 1) {
                ring_buffer.emplace_back({ segment.trail_id, {down.x, down.y} });
            }
            if (m.contains(left) && m(left) == c + 1) {
                ring_buffer.emplace_back({ segment.trail_id, {left.x, left.y} });
            }
            if (m.contains(right) && m(right) == c + 1) {
                ring_buffer.emplace_back({ segment.trail_id, {right.x, right.y} });
            }
        } else {
            trail_ratings[segment.trail_id] += 1;
            trails[segment.trail_id].insert(segment.pos);
        }
    }

#ifndef NDEBUG
    printf("%d / %d\n", max_usage, ring_buffer.capacity());
#endif

    int part1 = 0;
    int part2 = 0;

    for (const auto& trail : trails) {
#ifndef NDEBUG
        const int trailY = trail.first / 100;
        const int trailX = trail.first % 100;
        for (const vec2i& endingPos : trail.second) {
            printf("trail (%d, %d) has ending at (%d, %d)\n", trailX, trailY, endingPos.x, endingPos.y);
        }
#endif
        part1 += trail.second.size();
    }

    for (const auto& trail : trail_ratings) {
        part2 += trail.second;
    }

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
