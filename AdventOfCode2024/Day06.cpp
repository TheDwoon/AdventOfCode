#include <algorithm>
#include <vector>
#include "parser.cpp"
#include "vec2.cpp"
#include "directions.cpp"

struct map {
    char* buffer;
    const int width;
    const int height;
    const int lineLength;

    [[nodiscard]]
    char& at(const int x, const int y) const {
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
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
        assert (pos.x >= 0 && pos.x < width);
        assert (pos.y >= 0 && pos.y < height);
        return buffer[y * lineLength + x];
    }
};

struct segment {
    vec2i pos;
    vec2i direction;
};

int signum(const int x) {
    return (x > 0) - (x < 0);
}

vec2i findStartingPosition(const map& m) {
    vec2i pos;
    for (pos.y = 0; pos.y < m.height; pos.y++) {
        for (pos.x = 0; pos.x < m.width; pos.x++) {
            if (m(pos.x, pos.y) == '^')
                return pos;
        }
    }

    return {-1, -1};
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    const map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };


    vec2i facing = aoc::direction::UP;
    vec2i start = findStartingPosition(m);
    vec2i pos = start;

    std::vector<segment> segments;
    while (pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height) {
        m.at(pos) = 'X';

        if (m(pos + facing) == '#') {
            facing = aoc::direction::TURN_RIGHT * facing;
            segments.emplace_back(start, pos - start);
            start = pos;
        } else {
            pos += facing;
        }
    }
    segments.emplace_back(start, pos - start);


    for (auto& segment : segments) {
        printf("(%d, %d) + (%d, %d)\n", segment.pos.x, segment.pos.y, segment.direction.x, segment.direction.y);
    }

    const int size = static_cast<int>(segments.size());
    int leftMostFound = 0;
    for (int i = 1; i < size; i++) {
        const segment &currentEdge = segments[i];
        for (int j = leftMostFound; j < i; j++) {
            const segment &previousEdge = segments[j];

            if (currentEdge.direction * previousEdge.direction == 0
                && (currentEdge.direction.x != 0 && signum(currentEdge.direction.x) == signum(previousEdge.direction.y)
                    || currentEdge.direction.y != 0 && signum(currentEdge.direction.y) != signum(previousEdge.direction.x)) ) {
                // compute easy intersect
                vec2i intersection = currentEdge.pos;
                vec2i rockPosition = intersection;
                if (currentEdge.direction.x == 0) {
                    int u = (previousEdge.pos.y - currentEdge.pos.y) / signum(currentEdge.direction.y);
                    intersection.y += u * signum(currentEdge.direction.y);
                    rockPosition.y += (u + 1) * signum(currentEdge.direction.y);
                    printf("%d (%d, %d)\n", u, intersection.x, intersection.y);
                } else {
                    int u = (previousEdge.pos.x - currentEdge.pos.x) / signum(currentEdge.direction.x);
                    intersection.x += u * signum(currentEdge.direction.x);
                    rockPosition.x += (u + 1) * signum(currentEdge.direction.x);
                    printf("%d (%d, %d)\n", u, intersection.x, intersection.y);
                }

                if (m(rockPosition) != '#')
                    printf("Loopy bit at (%d, %d)\n", intersection.x, intersection.y);
            }
        }
    }

    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            if (m(x, y) == 'X') {
                part1++;
            }
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
