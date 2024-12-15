#include <cstring>
#include <vector>
#include <numeric>
#include <algorithm>

#include "vec2.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
constexpr int MAP_WIDTH = 101;
constexpr int MAP_HEIGHT = 103;
constexpr int SIGNIFICANT_EASTER_EGG_CLUSTER = 80;
constexpr int MAX_SECONDS_CHECKED = 10000;

struct robot {
    vec2i position;
    vec2i velocity;
};

void parseInput(const char* const buffer, std::vector<robot>& robots) {
    Parser p(buffer);
    int p_x, p_y, v_x, v_y;
    while (!p.eof()) {
        p.consume("p=");
        p.readNumber(p_x);
        p.consume(',');
        p.readNumber(p_y);
        p.consume(" v=");
        p.readNumber(v_x);
        p.consume(',');
        p.readNumber(v_y);
        p.readNewLine();

        robots.emplace_back(vec2i(p_x, p_y), vec2i(v_x, v_y));
    }
}

void simulate(std::vector<robot>& robots, const int seconds) {
    for (auto &[position, velocity] : robots) {
        position += seconds * velocity;
        position.x = position.x % MAP_WIDTH;
        position.y = position.y % MAP_HEIGHT;

        if (position.x < 0) position.x = MAP_WIDTH + position.x;
        if (position.y < 0) position.y = MAP_HEIGHT + position.y;
    }
}

#ifndef NDEBUG
void printMap(const std::vector<robot>& robots) {
    int *map = new int[MAP_WIDTH * MAP_HEIGHT];
    memset(map, 0, MAP_WIDTH * MAP_HEIGHT * sizeof(int));
    for (const auto &[position, velocity] : robots) {
        map[position.y * MAP_WIDTH + position.x] += 1;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            const int robot_count = map[y * MAP_WIDTH + x];
            if (robot_count == 0) {
                printf(".");
            } else {
                printf("%d", robot_count);
            }
        }

        printf("\n");
    }
    printf("\n");

    delete[] map;
}
#endif

void runDay(const char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    std::vector<robot> robots;
    parseInput(buffer, robots);
    std::vector<robot> easter_robots = robots;

    simulate(robots, 100);

    constexpr int x_half = MAP_WIDTH / 2;
    constexpr int y_half = MAP_HEIGHT / 2;

    int q[4] { 0, 0, 0, 0 };
    for (const robot &r : robots) {
        if (r.position.x == x_half || r.position.y == y_half)
            continue;

        const int idx = (r.position.y > y_half ? 2 : 0) + (r.position.x > x_half ? 1 : 0);
        q[idx] += 1;
    }

    part1 = q[0] * q[1] * q[2] * q[3];

    int seconds = 0;
    constexpr int CLUSTER_WIDTH = MAP_WIDTH / 10 + 1;
    constexpr int CLUSTER_HEIGHT = MAP_HEIGHT / 10 + 1;
    constexpr int CLUSTER_SIZE = CLUSTER_WIDTH * CLUSTER_HEIGHT;
    int clusters[CLUSTER_SIZE];
    int max_robots_in_cluster = 0;
    int current_robots_in_cluster = 0;
    do {
        seconds += 1;
        for (auto&[position, velocity] : easter_robots) {
            position += velocity;
            position.x = position.x % MAP_WIDTH;
            position.y = position.y % MAP_HEIGHT;
            if (position.x < 0) position.x = MAP_WIDTH + position.x;
            if (position.y < 0) position.y = MAP_HEIGHT + position.y;
        }

        memset(clusters, 0, CLUSTER_SIZE * sizeof(int));
        for (const robot &r : easter_robots) {
            const int c_x = r.position.x / CLUSTER_WIDTH;
            const int c_y = r.position.y / CLUSTER_HEIGHT;
            clusters[c_y * CLUSTER_WIDTH + c_x] += 1;
        }

        current_robots_in_cluster = *std::max_element(clusters, clusters + CLUSTER_SIZE);
        max_robots_in_cluster = std::max(current_robots_in_cluster, max_robots_in_cluster);

#ifndef NDEBUG
        printf("max_robot_cluster: %d\n", max_robots_in_cluster);
#endif
    } while (current_robots_in_cluster < SIGNIFICANT_EASTER_EGG_CLUSTER && seconds < MAX_SECONDS_CHECKED);

    part2 = seconds;

#ifndef NDEBUG
    printMap(easter_robots);
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
