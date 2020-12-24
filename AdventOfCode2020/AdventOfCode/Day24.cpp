#include "Day24.h"
#include <regex>
#include <map>
#include <set>
#include "vec2.h"

const vec2i MOVE_E(1, 0);
const vec2i MOVE_W(-1, 0);
const vec2i MOVE_SE(0, -1);
const vec2i MOVE_NW(0, 1);
const vec2i MOVE_SW(-1, -1);
const vec2i MOVE_NE(1, 1);

std::vector<path_t> Day24::parseInput(std::string& input)
{
    std::vector<path_t> parsed;
    std::regex regex("(e|se|sw|w|nw|ne)");

    std::stringstream stream(input);
    for (std::string line; std::getline(stream, line);) {
        path_t path;
        for (auto it = std::sregex_iterator(line.begin(), line.end(), regex); it != std::sregex_iterator(); ++it) {
            std::string match = (*it)[0];
            if (match == "e")
                path.push_back(Direction::EAST);
            else if (match == "se")
                path.push_back(Direction::SOUTH_EAST);
            else if (match == "sw")
                path.push_back(Direction::SOUTH_WEST);
            else if (match == "w")
                path.push_back(Direction::WEST);
            else if (match == "nw")
                path.push_back(Direction::NORTH_WEST);
            else if (match == "ne")
                path.push_back(Direction::NORTH_EAST);
            else
                throw std::exception();
        }

        parsed.push_back(path);
    }

    return parsed;
}

std::map<vec2i, Color> generateInitialPattern(std::vector<path_t> &input) {
    std::map<vec2i, Color> floor;

    for (const path_t& path : input) {
        vec2i pos(0, 0);
        for (const Direction& direction : path) {
            switch (direction)
            {
            case EAST:
                pos += MOVE_E;
                break;
            case WEST:
                pos += MOVE_W;
                break;
            case SOUTH_EAST:
                pos += MOVE_SE;
                break;
            case NORTH_WEST:
                pos += MOVE_NW;
                break;
            case SOUTH_WEST:
                pos += MOVE_SW;
                break;
            case NORTH_EAST:
                pos += MOVE_NE;
                break;
            default:
                throw std::exception();
            }
        }

        Color& color = floor[pos];
        color = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }

    return floor;
}

std::string Day24::runPart1(day_t& input)
{
    std::stringstream output;
    std::map<vec2i, Color> floor = generateInitialPattern(input);

    size_t blackTilesCount = 0;
    for (auto it = floor.begin(); it != floor.end(); ++it) {
        blackTilesCount += (it->second == Color::BLACK);
    }

    output << blackTilesCount;
    return output.str();
}

inline Color getTileColor(const vec2i& pos, const std::map<vec2i, Color>& floor) {
    auto it = floor.find(pos);
    if (it == floor.end())
        return Color::WHITE;
    else
        return it->second;
}

inline bool isBlackTile(const vec2i& pos, const std::map<vec2i, Color>& floor) {
    auto it = floor.find(pos);
    if (it == floor.end())
        return false;
    else
        return it->second == Color::BLACK;
}

size_t countBlackNeighbors(const vec2i &pos, const std::map<vec2i, Color> &floor) {
    vec2i e = pos + MOVE_E;
    vec2i w = pos + MOVE_W;
    vec2i se = pos + MOVE_SE;
    vec2i nw = pos + MOVE_NW;
    vec2i sw = pos + MOVE_SW;
    vec2i ne = pos + MOVE_NE;

    size_t neighbors = isBlackTile(e, floor);
    neighbors += isBlackTile(w, floor);
    neighbors += isBlackTile(se, floor);
    neighbors += isBlackTile(nw, floor); 
    neighbors += isBlackTile(sw, floor); 
    neighbors += isBlackTile(ne, floor);
    return neighbors;
}

Color nextTileColor(const vec2i& pos, const std::map<vec2i, Color>& floor) {
    Color currentColor = getTileColor(pos, floor);
    size_t blackNeighbors = countBlackNeighbors(pos, floor);

    if (currentColor == Color::BLACK && (blackNeighbors == 0 || blackNeighbors == 2))
        return Color::WHITE;
    else if (currentColor == Color::WHITE && blackNeighbors == 2)
        return Color::BLACK;
    else
        return currentColor;
}

void checkTile(const vec2i& pos, const std::map<vec2i, Color>& floor, std::map<vec2i, Color>& buffer) {
    Color currentColor = getTileColor(pos, floor);
    if (currentColor == Color::BLACK) {
        vec2i e = pos + MOVE_E;
        vec2i w = pos + MOVE_W;
        vec2i se = pos + MOVE_SE;
        vec2i nw = pos + MOVE_NW;
        vec2i sw = pos + MOVE_SW;
        vec2i ne = pos + MOVE_NE;
        checkTile(e, floor, buffer);
        checkTile(w, floor, buffer);
        checkTile(se, floor, buffer);
        checkTile(nw, floor, buffer);
        checkTile(sw, floor, buffer);
        checkTile(ne, floor, buffer);
    }

    buffer[pos] = nextTileColor(pos, floor);
}

std::string Day24::runPart2(day_t& input)
{
    std::map<vec2i, Color> floor = generateInitialPattern(input);
    std::map<vec2i, Color> buffer;

    for (int i = 0; i < 100; i++) {
        std::set<vec2i> positions;
        for (auto it = floor.begin(); it != floor.end(); ++it) {
            if (it->second == Color::WHITE)
                continue;

            positions.insert(it->first);  
            positions.insert(it->first + MOVE_E);
            positions.insert(it->first + MOVE_W);
            positions.insert(it->first + MOVE_SE);
            positions.insert(it->first + MOVE_NW);
            positions.insert(it->first + MOVE_SW);
            positions.insert(it->first + MOVE_NE);
        }

        for (auto it = positions.begin(); it != positions.end(); ++it) {
            const vec2i& pos = *it;
            buffer[pos] = nextTileColor(pos, floor);
        }

        std::swap(floor, buffer);
    }

    size_t blackTilesCount = 0;
    for (auto it = floor.begin(); it != floor.end(); ++it) {
        blackTilesCount += (it->second == Color::BLACK);
    }

    std::stringstream output;
    output << blackTilesCount;
    return output.str();
}
