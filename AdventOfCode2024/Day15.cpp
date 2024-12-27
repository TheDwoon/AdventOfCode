#include <algorithm>
#include <cstring>
#include <deque>
#include <vector>
#include <unordered_set>

#include "directions.cpp"
#include "vec2.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

constexpr char MOVE_UP = '^';
constexpr char MOVE_DOWN = 'v';
constexpr char MOVE_LEFT = '<';
constexpr char MOVE_RIGHT = '>';

constexpr char MAP_ROBOT = '@';
constexpr char MAP_CRATE = 'O';
constexpr char MAP_WALL = '#';
constexpr char MAP_FREE = '.';
constexpr char MAP_CRATE_LEFT = '[';
constexpr char MAP_CRATE_RIGHT = ']';

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

#ifndef NDEBUG
void printWarehouse(const map &m, const vec2i &robot_position) {
    for (int y = 0; y < m.height; ++y) {
        for (int x = 0; x < m.width; ++x) {
            if (robot_position.x == x && robot_position.y == y) {
                assert(m(robot_position) == MAP_FREE);
                printf("@");
            } else {
                printf("%c", m(x, y));
            }
        }

        printf("\n");
    }
}
#endif

void parseInput(const char *const buffer, std::vector<char> &map_data, std::vector<char> &move_instructions,
                int &map_width, int &map_data_length) {
    Parser p(buffer);
    p.findNext("\n", map_width);
    p.findNext("\n\n", map_data_length);

    // include a single \n in the map_data and the end of the last line to match the other lines already in the map
    map_data_length += 1;
    map_data.reserve(map_data_length);
    map_data.insert(map_data.begin(), buffer, buffer + map_data_length);

    p.global_seek(map_data_length + 1);
    while (!p.eof()) {
        const char c = p.read();
        if (c != '\n')
            move_instructions.push_back(c);
    }
}

vec2i get_tile_after_crates(const map &warehouse, vec2i position, const vec2i &direction) {
    while (warehouse.contains(position) && warehouse(position) == MAP_CRATE) {
        position += direction;
    }

    return position;
}

void move_robot(map &warehouse, vec2i &robot_position, const vec2i &direction) {
    const vec2i next_robot_position = robot_position + direction;
    const char tile = warehouse(next_robot_position);
    switch (tile) {
        case MAP_CRATE: {
            const vec2i spot_after_crates = get_tile_after_crates(warehouse, next_robot_position, direction);
            if (!warehouse.contains(spot_after_crates) || warehouse(spot_after_crates) == MAP_WALL) {
                break;
            }

            warehouse.at(next_robot_position) = MAP_FREE;
            warehouse.at(spot_after_crates) = MAP_CRATE;
        }
        case MAP_FREE:
            robot_position = next_robot_position;
    }
}

void find_connected_positions(const map &warehouse, const vec2i &position, const vec2i &direction, std::unordered_set<vec2i>& connected_crates, std::unordered_set<vec2i> &edge_set) {
    assert(warehouse(position) == MAP_CRATE_LEFT || warehouse(position) == MAP_CRATE_RIGHT);

    connected_crates.insert(position);
    std::deque<vec2i> queue { position };
    if (direction.y != 0) {
        switch (warehouse(position)) {
            case MAP_CRATE_LEFT: {
                const vec2i other_half = position + aoc::direction::RIGHT;
                queue.push_back(other_half);
                connected_crates.insert(other_half);
                break;
            }
            case MAP_CRATE_RIGHT: {
                const vec2i other_half = position + aoc::direction::LEFT;
                queue.push_back(other_half);
                connected_crates.insert(other_half);
                break;
            }
        }

        // Vertical boxes can branch out and need to be handled differently
        while (!queue.empty()) {
            const vec2i next_position = queue.front() + direction;
            queue.pop_front();
            assert(warehouse.contains(next_position));
            const char tile = warehouse(next_position);
            switch (tile) {
                case MAP_CRATE_LEFT: {
                    if (!connected_crates.contains(next_position)) {
                        queue.push_back(next_position);
                        queue.push_back(next_position + aoc::direction::RIGHT);
                        connected_crates.insert(next_position);
                        connected_crates.insert(next_position + aoc::direction::RIGHT);
                    }

                    break;
                }
                case MAP_CRATE_RIGHT: {
                    if (!connected_crates.contains(next_position)) {
                        queue.push_back(next_position);
                        queue.push_back(next_position + aoc::direction::LEFT);
                        connected_crates.insert(next_position);
                        connected_crates.insert(next_position + aoc::direction::LEFT);
                    }

                    break;
                }
                default:
                    edge_set.insert(next_position);
                    break;
            }
        }
    } else {
        while (!queue.empty()) {
            const vec2i next_position = queue.front() + direction;
            queue.pop_front();
            if (warehouse.contains(next_position)) {
                const char tile = warehouse(next_position);
                switch (tile) {
                    case MAP_CRATE_LEFT:
                    case MAP_CRATE_RIGHT:
                        queue.push_back(next_position);
                        connected_crates.insert(next_position);
                        break;
                    default:
                        edge_set.insert(next_position);
                        break;
                }
            }
        }
    }
}

void move_robot_wide(map &warehouse, vec2i &robot_position, const vec2i &direction) {
    const vec2i next_robot_position = robot_position + direction;
    const char tile = warehouse(next_robot_position);
    switch (tile) {
        case MAP_CRATE_LEFT:
        case MAP_CRATE_RIGHT: {
            std::unordered_set<vec2i> connected_crates;
            std::unordered_set<vec2i> edge_set;

            find_connected_positions(warehouse, next_robot_position, direction, connected_crates, edge_set);
            if (std::ranges::any_of(edge_set, [warehouse](const vec2i &p) { return warehouse(p) != MAP_FREE; })) {
                break;
            }

            std::vector<vec2i> crates(connected_crates.begin(), connected_crates.end());
            std::ranges::sort(crates, [direction](const vec2i &p1, const vec2i &p2) {
                return p1.x * direction.x > p2.x * direction.x || p1.y * direction.y > p2.y * direction.y;
            });

            for (const vec2i &crate : crates) {
                warehouse.at(crate + direction) = warehouse(crate);
                warehouse.at(crate) = MAP_FREE;
            }
        }
        case MAP_FREE:
            robot_position = next_robot_position;
    }
}

void runDay(const char *const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    // part 1
    std::vector<char> map_data;
    std::vector<char> move_instructions;
    int map_width;
    int map_data_length;
    parseInput(buffer, map_data, move_instructions, map_width, map_data_length);

    std::vector<char> wide_warehouse_data(2 * map_data.size());
    map warehouse(map_data.data(), map_width, map_data_length / (map_width + 1), map_width + 1);
    map wide_warehouse(wide_warehouse_data.data(), 2 * warehouse.width, warehouse.height, 2 * warehouse.width);

    vec2i robot_position_wide;
    vec2i robot_position;
    for (int y = 0; y < warehouse.height; ++y) {
        for (int x = 0; x < warehouse.width; ++x) {
            const char c = warehouse(x, y);
            switch (c) {
                case MAP_ROBOT:
                    robot_position = {x, y};
                    robot_position_wide = {2 * x, y};

                    warehouse.at(robot_position) = MAP_FREE;
                case MAP_FREE:
                    wide_warehouse.at(2 * x, y) = MAP_FREE;
                    wide_warehouse.at(2 * x + 1, y) = MAP_FREE;
                    break;
                case MAP_CRATE:
                    wide_warehouse.at(2 * x, y) = MAP_CRATE_LEFT;
                    wide_warehouse.at(2 * x + 1, y) = MAP_CRATE_RIGHT;
                    break;
                case MAP_WALL:
                    wide_warehouse.at(2 * x, y) = MAP_WALL;
                    wide_warehouse.at(2 * x + 1, y) = MAP_WALL;
                    break;
            }
        }
    }

#ifndef NDEBUG
    printf("Initial:\n");
    // printWarehouse(warehouse, robot_position);
    printWarehouse(wide_warehouse, robot_position_wide);
#endif

    for (const char instruction: move_instructions) {
        switch (instruction) {
            case MOVE_UP:
                move_robot(warehouse, robot_position, aoc::direction::UP);
                move_robot_wide(wide_warehouse, robot_position_wide, aoc::direction::UP);
                break;
            case MOVE_DOWN:
                move_robot(warehouse, robot_position, aoc::direction::DOWN);
                move_robot_wide(wide_warehouse, robot_position_wide, aoc::direction::DOWN);
                break;
            case MOVE_LEFT:
                move_robot(warehouse, robot_position, aoc::direction::LEFT);
                move_robot_wide(wide_warehouse, robot_position_wide, aoc::direction::LEFT);
                break;
            case MOVE_RIGHT:
                move_robot(warehouse, robot_position, aoc::direction::RIGHT);
                move_robot_wide(wide_warehouse, robot_position_wide, aoc::direction::RIGHT);
                break;
        }

#ifndef NDEBUG
        printf("Move: %c\n", instruction);
        // printWarehouse(warehouse, robot_position);
        printWarehouse(wide_warehouse, robot_position_wide);
#endif
    }

    for (int y = 0; y < warehouse.height; ++y) {
        for (int x = 0; x < warehouse.width; ++x) {
#ifndef NDEBUG
            printf("%c", warehouse(x, y));
#endif
            if (warehouse(x, y) == MAP_CRATE) {
                part1 += y * 100 + x;
            }
            if (wide_warehouse(2 * x, y) == MAP_CRATE_LEFT) {
                part2 += y * 100 + 2 * x;
            } else if (wide_warehouse(2 * x + 1, y) == MAP_CRATE_LEFT) {
                part2 += y * 100 + 2 * x + 1;
            }
        }
#ifndef NDEBUG
        printf("\n");
#endif
    }

    printf("%d\n", part1);
    printf("%d\n", part2);
}

// BOILER PLATE CODE BELOW

int main() {
    char buffer[INPUT_BUFFER_SIZE]{};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
    runDay(buffer, length);
}
