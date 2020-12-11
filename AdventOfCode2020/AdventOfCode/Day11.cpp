#include "Day11.h"

std::array<char, HEIGHT * WIDTH> Day11::parseInput(std::string &input) {
    std::array<char, HEIGHT * WIDTH> parsed;
    char* data = parsed.data();

    std::stringstream stream(input);
    char c;
    while (stream && stream >> c) {
        if (c == '\n' || c == '\r')
            continue;

        if (c == 'L')
            c = EMPTY_SEAT;
        else
            c = FLOOR;
        *(data++) = c;
    }

    return parsed;
}

std::string Day11::runPart1(std::array<char, HEIGHT * WIDTH> &input) {
    std::array<char, HEIGHT * WIDTH> oldMap = input;
    std::array<char, HEIGHT * WIDTH> nextMap;
    int oldSeatedPeople = 0;
    int nextSeatedPeople = 0;

    do {
        nextSeatedPeople = 0;
        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++) {
                nextMap[y * WIDTH + x] = nextState(x, y, oldMap);
                nextSeatedPeople += (nextMap[y * WIDTH + x] == FULL_SEAT);
            }

        std::swap(oldMap, nextMap);
        std::swap(oldSeatedPeople, nextSeatedPeople);
    } while (oldSeatedPeople != nextSeatedPeople);

    std::stringstream output;
    output << nextSeatedPeople;
    return output.str();
}

bool Day11::isFull(int x, int y, const std::array<char, HEIGHT * WIDTH> &map) const {
    const int idx = y * WIDTH + x;
    return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) && map[idx] == FULL_SEAT;
}

char Day11::nextState(int x, int y, const std::array<char, HEIGHT * WIDTH> map) const {
    if (map[y * WIDTH + x] == FLOOR)
        return FLOOR;

    char occupied = isFull(x - 1, y - 1, map)
            + isFull(x + 0, y - 1, map)
            + isFull(x + 1, y - 1, map)
            + isFull(x - 1, y, map)
            + isFull(x + 1, y, map)
            + isFull(x - 1, y + 1, map)
            + isFull(x + 0, y + 1, map)
            + isFull(x + 1, y + 1, map);
    if (occupied == 0)
        return FULL_SEAT;
    else if (occupied >= 4)
        return EMPTY_SEAT;
    else
        return map[y * WIDTH + x];
}

std::string Day11::runPart2(std::array<char, WIDTH * HEIGHT> &input) {
    std::array<char, HEIGHT * WIDTH> oldMap = input;
    std::array<char, HEIGHT * WIDTH> nextMap;
    int oldSeatedPeople = 0;
    int nextSeatedPeople = 0;

    do {
        nextSeatedPeople = 0;
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                char tile = oldMap[y * WIDTH + x];
                if (tile == FLOOR) {
                    nextMap[y * WIDTH + x] = FLOOR;
                    continue;
                }

                char occupied = isFullRay(x - 1, y - 1, -1, -1, oldMap)
                                + isFullRay(x + 0, y - 1, 0, -1, oldMap)
                                + isFullRay(x + 1, y - 1, 1, -1, oldMap)
                                + isFullRay(x - 1, y, -1, 0, oldMap)
                                + isFullRay(x + 1, y, 1, 0, oldMap)
                                + isFullRay(x - 1, y + 1, -1, 1, oldMap)
                                + isFullRay(x + 0, y + 1, 0, 1, oldMap)
                                + isFullRay(x + 1, y + 1, 1, 1, oldMap);
                if (occupied == 0) {
                    tile = FULL_SEAT;
                } else if (occupied >= 5)
                    tile = EMPTY_SEAT;

                nextSeatedPeople += (tile == FULL_SEAT);
                nextMap[y * WIDTH + x] = tile;
            }
        }

        std::swap(oldMap, nextMap);
        std::swap(oldSeatedPeople, nextSeatedPeople);
    } while (oldSeatedPeople != nextSeatedPeople);

    std::stringstream output;
    output << nextSeatedPeople;
    return output.str();
}

bool Day11::isFullRay(int x, int y, int dx, int dy, const std::array<char, HEIGHT * WIDTH> &map) const {
    while (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        char tile = map[y * WIDTH + x];
        if (tile == FULL_SEAT)
            return true;
        else if (tile == EMPTY_SEAT)
            return false;

        x += dx;
        y += dy;
    }

    return false;
}
