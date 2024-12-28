#include <cstring>
#include <memory>
#include <unordered_set>
#include <queue>
#include <unordered_map>

#include "util.cpp"
#include "directions.cpp"
#include "vec2.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

constexpr char MAP_FREE = '.';
constexpr char MAP_WALL = '#';
constexpr char MAP_START = 'S';
constexpr char MAP_END = 'E';

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
    pose previous_pose;
    pose current_pose;
    int cost;

    node(const vec2i position, const vec2i facing, const int cost) : previous_pose(position, facing), current_pose(position, facing), cost(cost) {}
    node(const node& previous_node, const vec2i current_position, const vec2i current_facing, const int cost) : previous_pose(previous_node.current_pose.position, previous_node.current_pose.facing), current_pose(current_position, current_facing), cost(cost) {}

    friend bool operator<(const node &p1, const node &p2) {
        return p1.cost > p2.cost;
    }
};

struct path_node {
    int cost { 1000000 };
    std::unordered_set<pose> previous;

    void update(const pose& previous_position, const int updated_cost) {
        if (updated_cost < cost) {
            cost = updated_cost;
            previous.clear();
            previous.insert(previous_position);
        } else if (updated_cost == cost) {
            previous.insert(previous_position);
        }
    }
};

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    int map_width {0};
    p.findNext("\n", map_width);

    vec2i start_position;
    vec2i end_position;

    aoc::map m { buffer, map_width, length / (map_width + 1), map_width + 1 };
    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            const char c = m(x, y);
            switch (c) {
                case MAP_START:
                    start_position = vec2i(x, y);
                    m.at(x, y) = MAP_FREE;
                    break;
                case MAP_END:
                    end_position = vec2i(x, y);
                    m.at(x, y) = MAP_FREE;
                    break;
            }
        }
    }

    std::unordered_map<pose, path_node> cost_map;

    std::priority_queue<node> queue;
    queue.emplace(start_position, aoc::direction::EAST, 0);
    while (!queue.empty()) {
        const node n = queue.top();
        queue.pop();

        cost_map[n.current_pose].update(n.previous_pose, n.cost);

        if (const node forward(n, n.current_pose.position + n.current_pose.facing, n.current_pose.facing, n.cost + 1); m(forward.current_pose.position) == MAP_FREE && forward.cost <= cost_map[forward.current_pose].cost) {
            queue.emplace(forward);
        }
        if (const node turn_left(n, n.current_pose.position, aoc::direction::TURN_LEFT * n.current_pose.facing, n.cost + 1000); m(turn_left.current_pose.position + turn_left.current_pose.facing) == MAP_FREE && turn_left.cost <= cost_map[turn_left.current_pose].cost) {
            queue.emplace(turn_left);
        }
        if (const node turn_right(n, n.current_pose.position, aoc::direction::TURN_RIGHT * n.current_pose.facing, n.cost + 1000); m(turn_right.current_pose.position + turn_right.current_pose.facing) == MAP_FREE && turn_right.cost <= cost_map[turn_right.current_pose].cost) {
            queue.emplace(turn_right);
        }
    }

    const pose finish_up { end_position, aoc::direction::UP };
    const path_node cost_map_up = cost_map[finish_up];
    int best_path_cost = cost_map_up.cost;

    const pose finish_down { end_position, aoc::direction::DOWN };
    const path_node cost_map_down = cost_map[finish_down];
    best_path_cost = std::min(best_path_cost, cost_map_down.cost);

    const pose finish_left { end_position, aoc::direction::LEFT };
    const path_node cost_map_left = cost_map[finish_left];
    best_path_cost = std::min(best_path_cost, cost_map_left.cost);

    const pose finish_right { end_position, aoc::direction::RIGHT };
    const path_node cost_map_right = cost_map[finish_right];
    best_path_cost = std::min(best_path_cost, cost_map_right.cost);

    std::unordered_set<vec2i> best_path_segments;
    std::deque<pose> path_queue { finish_up, finish_down, finish_left , finish_right };
    while (!path_queue.empty()) {
        const pose p = path_queue.front();
        path_queue.pop_front();

        auto it = cost_map.find(p);
        if (it == cost_map.end() || it->second.cost > best_path_cost) {
            continue;
        }

        best_path_segments.insert(p.position);
        for (const pose &pose : it->second.previous) {
            if (p != pose) {
                path_queue.push_back(pose);
            }
        }
    }

    // 455 -> too low
    printf("%d\n", best_path_cost);
    printf("%llu\n", best_path_segments.size());
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
