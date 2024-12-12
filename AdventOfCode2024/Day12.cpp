#include <cstring>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include "directions.cpp"
#include "util.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;


void findCluster(const int x, const int y, const int cluster_number, const aoc::map<char> &plot_map, aoc::map<int> &cluster_map) {
    std::unordered_set<vec2i> visited;
    std::deque<vec2i> queue { {x, y} };
    while (!queue.empty()) {
        // put cluster number in cluster map
        const vec2i &v = queue.front();
        cluster_map.at(v) = cluster_number;

        // expand in all directions if possible
        const char current_plot = plot_map(v);

        const vec2i up = v + aoc::direction::UP;
        const vec2i down = v + aoc::direction::DOWN;
        const vec2i right = v + aoc::direction::RIGHT;
        const vec2i left = v + aoc::direction::LEFT;

        if (plot_map.contains(up) && plot_map(up) == current_plot && !visited.contains(up)) {
            visited.insert(up);
            queue.push_back(up);
        }
        if (plot_map.contains(down) && plot_map(down) == current_plot && !visited.contains(down)) {
            visited.insert(down);
            queue.push_back(down);
        }
        if (plot_map.contains(left) && plot_map(left) == current_plot && !visited.contains(left)) {
            visited.insert(left);
            queue.push_back(left);
        }
        if (plot_map.contains(right) && plot_map(right) == current_plot && !visited.contains(right)) {
            visited.insert(right);
            queue.push_back(right);
        }

        queue.pop_front();
    }
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    unsigned int lineLength;
    p.findNext("\n", lineLength);

    // parse input
    const aoc::map m { buffer, static_cast<int>(lineLength), static_cast<int>(length / (lineLength + 1)), static_cast<int>(lineLength + 1) };

    int* const cluster_buffer = new int[m.width * m.height];
    memset(cluster_buffer, 0, m.width * m.height * sizeof(int));

    aoc::map cluster_map { cluster_buffer, m.width, m.height, m.width  };

    int cluster_number = 1;
    for (int y = 0; y < m.height; ++y) {
        for (int x = 0; x < m.width; ++x) {
            if (cluster_map(x, y) == 0) {
                findCluster(x, y, cluster_number++, m, cluster_map);
            }
        }
    }
#ifndef NDEBUG
    for (int y = 0; y < m.height; ++y) {
        for (int x = 0; x < m.width; ++x) {
            printf("%2d ", cluster_map(x, y));
        }
        printf("\n");
    }
    printf("\n");
#endif

    std::unordered_map<int, int> cluster_area;
    std::unordered_map<int, int> cluster_perimeter;

    for (int y = 0; y < cluster_map.height - 1; y++) {
        for (int x = 0; x < cluster_map.width - 1; x++) {
            const int current_cluster = cluster_map(x, y);

            const int next_cluster = cluster_map(x + 1, y);
            const int below_cluster = cluster_map(x, y + 1);

            cluster_area[current_cluster] += 1;

            if (current_cluster != next_cluster) {
                cluster_perimeter[current_cluster] += 1;
                cluster_perimeter[next_cluster] += 1;
            }

            if (current_cluster != below_cluster) {
                cluster_perimeter[current_cluster] += 1;
                cluster_perimeter[below_cluster] += 1;
            }
        }
    }

    // top edge & bottom edge
    for (int x = 0; x < cluster_map.width; x++) {
        const int top_cluster = cluster_map(x, 0);
        const int bottom_cluster = cluster_map(x, cluster_map.height - 1);

        // area not accounted for in main loop
        cluster_area[bottom_cluster] += 1;

        // account for outer perimeter
        cluster_perimeter[top_cluster] += 1;
        cluster_perimeter[bottom_cluster] += 1;
    }

    // left edge & right edge
    for (int y = 0; y < cluster_map.height; y++) {
        const int left_cluster = cluster_map(0, y);
        const int right_cluster = cluster_map(cluster_map.width - 1, y);

        // area not accounted for in main loop
        cluster_area[right_cluster] += 1;

        // account for outer perimeter
        cluster_perimeter[left_cluster] += 1;
        cluster_perimeter[right_cluster] += 1;
    }

    // incremented by both top and right edge loop
    cluster_area[cluster_map(m.width - 1, m.height - 1)] -= 1;

    // right most horizontal edge segments missed by main loop
    for (int y = 0; y < cluster_map.height - 1; y++) {
        const int current_cluster = cluster_map(cluster_map.width - 1, y);
        const int bottom_cluster = cluster_map(cluster_map.width - 1, y + 1);

        if (current_cluster != bottom_cluster) {
            cluster_perimeter[current_cluster] += 1;
            cluster_perimeter[bottom_cluster] += 1;
        }
    }

    // bottom most vertical edge segments missed by main loop
    for (int x = 0; x < cluster_map.width - 1; x++) {
        const int current_cluster = cluster_map(x, cluster_map.height - 1);
        const int next_cluster = cluster_map(x + 1, cluster_map.height - 1);

        if (current_cluster != next_cluster) {
            cluster_perimeter[current_cluster] += 1;
            cluster_perimeter[next_cluster] += 1;
        }
    }

#ifndef NDEBUG
    printf("Areas:\n");
    for (auto const& area : cluster_area) {
        printf("%3d: %3d\n", area.first, area.second);
    }

    printf("Perimeter:\n");
    for (auto const& perimeter : cluster_perimeter) {
        printf("%3d: %3d\n", perimeter.first, perimeter.second);
    }
#endif

    for (auto [cluster_number, area] : cluster_area) {
        const int perimeter = cluster_perimeter[cluster_number];

        part1 += area * perimeter;
    }

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
