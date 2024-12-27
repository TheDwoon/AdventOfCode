#include <cstring>
#include <vector>

#include "directions.cpp"
#include "vec2.cpp"
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

constexpr char MOVE_UP = '^';
constexpr char MOVE_DOWN = 'v';
constexpr char MOVE_LEFT = '<';
constexpr char MOVE_RIGHT = '>';

constexpr char MAP_CRATE = 'O';
constexpr char MAP_WALL = '#';
constexpr char MAP_FREE = '.';

struct map {
    char* const buffer;
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

#ifndef NDEBUG
    void printWarehouse(const map& m, const vec2i& robot_position) {
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

void parseInput(const char* const buffer, std::vector<char>& map_data, std::vector<char>& move_instructions, int &map_width, int &map_data_length) {
    Parser p(buffer);
    p.findNext("\n", map_width);
    p.findNext("\n\n", map_data_length);

    // include a single \n in the map_data and the end of the last line to match the other lines already in the map
    map_data_length += 1;
    map_data.reserve(map_data_length);
    map_data.insert(map_data.begin(), buffer, buffer + map_data_length);

    p.global_seek(map_data_length + 2);
    while (!p.eof()) {
        const char c = p.read();
        if (c != '\n')
            move_instructions.push_back(c);
    }
}

vec2i get_tile_after_crates(const map& warehouse, vec2i position, const vec2i& direction) {
    while (warehouse.contains(position) && warehouse(position) == MAP_CRATE) {
        position += direction;
    }

    return position;
}

void move_robot(map &warehouse, vec2i &robot_position, const vec2i &direction) {
    const vec2i next_robot_position = robot_position + direction;
    switch (warehouse(next_robot_position)) {
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

void runDay(const char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    std::vector<char> map_data;
    std::vector<char> move_instructions;
    int map_width;
    int map_data_length;
    parseInput(buffer, map_data, move_instructions, map_width, map_data_length);

    map warehouse(map_data.data(), map_width, map_data_length / (map_width + 1), map_width + 1);

    vec2i robot_position;
    for (int i = 0; i < map_data.size(); ++i) {
        if (map_data[i] == '@') {
            robot_position = { i % (map_width + 1), i / (map_width + 1) };
            break;
        }
    }

    // erase robot marker from map
    warehouse.at(robot_position) = MAP_FREE;

#ifndef NDEBUG
        printf("Initial:\n");
        printWarehouse(warehouse, robot_position);
#endif

    for (const char instruction : move_instructions) {
        switch (instruction) {
            case MOVE_UP:
                move_robot(warehouse, robot_position, aoc::direction::UP);
                break;
            case MOVE_DOWN:
                move_robot(warehouse, robot_position, aoc::direction::DOWN);
                break;
            case MOVE_LEFT:
                move_robot(warehouse, robot_position, aoc::direction::LEFT);
                break;
            case MOVE_RIGHT:
                move_robot(warehouse, robot_position, aoc::direction::RIGHT);
                break;
        }

#ifndef NDEBUG
        printf("Move: %c\n", instruction);
        printWarehouse(warehouse, robot_position);
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
        }
#ifndef NDEBUG
        printf("\n");
#endif
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
