#include <cstring>
#include <memory>
#include <unordered_set>
#include <queue>

#include "directions.cpp"
#include "vec2.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

constexpr char MAP_FREE = '.';
constexpr char MAP_WALL = '#';
constexpr char MAP_START = 'S';
constexpr char MAP_END = 'E';

struct map {
    char *const buffer;
    const int width;
    const int height;
    const int lineLength;

    [[nodiscard]]
    bool contains(const vec2i &pos) const {
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
    }

    [[nodiscard]]
    char &at(const int x, const int y) const {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        return buffer[y * lineLength + x];
    }

    [[nodiscard]]
    char &at(const vec2i &pos) const {
        assert(pos.x >= 0 && pos.x < width);
        assert(pos.y >= 0 && pos.y < height);
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const vec2i &pos) const {
        assert(pos.x >= 0 && pos.x < width);
        assert(pos.y >= 0 && pos.y < height);
        return buffer[pos.y * lineLength + pos.x];
    }

    char operator()(const int x, const int y) const {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        return buffer[y * lineLength + x];
    }
};

int get_heuristic_cost(const vec2i &pos, const vec2i &target) {
    return abs(pos.x - target.x) + abs(pos.y - target.y);
}

struct pose {
    vec2i position;
    vec2i facing;

    friend bool operator==(const pose &p1, const pose &p2) noexcept {
        return p1.position == p2.position && p1.facing == p2.facing;
    }
};

template<>
struct std::hash<pose> {
    auto operator()(const pose &p) const noexcept {
        return (p.facing.x << 24) ^ (p.facing.y << 16) ^ (p.position.y << 8) ^ p.position.x;
    }
};

struct node {
    pose p;
    int actual_cost;
    int expected_cost;
    int total_cost;
    std::shared_ptr<node> previous;

    node() = default;
    node(const pose& p, const int actual_cost, const int expected_cost, const int total_cost, const std::shared_ptr<node> &previous) : p(p), actual_cost(actual_cost), expected_cost(expected_cost), total_cost(total_cost), previous(previous) {

    }
    node(const vec2i position, const vec2i facing, const int actual_cost, const vec2i &target_position) : p(position, facing), actual_cost(actual_cost) {
        expected_cost = get_heuristic_cost(position, target_position);
        total_cost = expected_cost + actual_cost;
    }

    node(const node& previous_node, const pose& current_pose, const int cost_step, const vec2i& target_position) : p(current_pose) {
        actual_cost = previous_node.actual_cost + cost_step;
        expected_cost = get_heuristic_cost(current_pose.position, target_position);
        total_cost = expected_cost + actual_cost;
        previous = std::make_shared<node>(previous_node.p, previous_node.actual_cost, previous_node.expected_cost, previous_node.total_cost, previous_node.previous);
    }

    friend bool operator<(const node &a, const node &b) noexcept {
        return a.total_cost < b.total_cost;
    }
};

struct worst_score {
    bool operator()(const node& n1, const node& n2) const noexcept {
        return n1.total_cost < n2.total_cost;
    }
};

struct best_score {
    bool operator()(const node& n1, const node& n2) const noexcept {
        return n1.total_cost > n2.total_cost;
    }
};

node find_worst_path(const map& m, const vec2i &initial_position, const vec2i &final_position) {
    std::unordered_set<pose> visited;
    std::priority_queue<node, std::vector<node>, best_score> queue;
    queue.emplace(initial_position, aoc::direction::EAST, 0, final_position);
    while (!queue.empty()) {
        const node n = queue.top();
        queue.pop();
        if (n.p.position == final_position)
            return n;

#ifndef NDEBUG
        printf("Checking: (%2d, %2d) (%2d, %2d) [%5d (%5d + %3d)]\n", n.p.position.x, n.p.position.y, n.p.facing.x, n.p.facing.y, n.total_cost, n.actual_cost, n.expected_cost);
#endif

        pose forward { n.p.position + n.p.facing, n.p.facing };
        pose turn_left { n.p.position, aoc::direction::TURN_LEFT * n.p.facing };
        pose turn_right { n.p.position, aoc::direction::TURN_RIGHT * n.p.facing };

        const char next_tile = m(forward.position);
        if (next_tile == MAP_FREE && !visited.contains(forward)) {
            visited.insert(forward);
            queue.emplace(n, forward, 1, final_position);
        }
        if (!visited.contains(turn_left)) {
            visited.insert(turn_left);
            queue.emplace(n, turn_left, 1000, final_position);
        }
        if (!visited.contains(turn_right)) {
            visited.insert(turn_right);
            queue.emplace(n, turn_right, 1000, final_position);
        }
    }

    return {};
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    int map_width {0};
    p.findNext("\n", map_width);

    vec2i start_position;
    vec2i final_position;

    map m { buffer, map_width, length / (map_width + 1), map_width + 1 };
    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            const char c = m(x, y);
            switch (c) {
                case MAP_START:
                    start_position = vec2i(x, y);
                    m.at(x, y) = MAP_FREE;
                    break;
                case MAP_END:
                    final_position = vec2i(x, y);
                    m.at(x, y) = MAP_FREE;
                    break;
            }
        }
    }

    node n = find_worst_path(m, start_position, final_position);

    printf("%d\n", n.total_cost);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
