#include "Day03.h"
#include <sstream>


std::vector<char> Day03::parseInput(std::string& input)
{
    std::vector<char> parsed;
    std::stringstream stream(input);
    for (std::string line; std::getline(stream, line);)
        for (char& c : line)
            parsed.push_back(c);

    return parsed;
}

std::string Day03::runPart1(day_t& input)
{
    std::stringstream output;
    output << ride_slope(3, 1, input);
    return output.str();
}

std::string Day03::runPart2(day_t& input)
{
    std::stringstream output;
    long long a1 = ride_slope(1, 1, input);
    long long a2 = ride_slope(3, 1, input);
    long long a3 = ride_slope(5, 1, input);
    long long a4 = ride_slope(7, 1, input);
    long long a5 = ride_slope(1, 2, input);

    output << (a1 * a2 * a3 * a4 * a5);

    return output.str();
}

const int MAP_WIDTH = 31;

int Day03::ride_slope(int dx, int dy, std::vector<char>& map)
{
    int treesHit = 0;
    int x = 0;
    int y = 0;
    char lastChar = '.';
    while (lastChar != 'X') {
        if (lastChar == '#')
            treesHit++;

        x += dx;
        y += dy;
        lastChar = access_map(x, y, map);
    }

    return treesHit;
}

char Day03::access_map(int x, int y, std::vector<char>& map)
{
    int accessX = x % MAP_WIDTH;
    int accessY = y;

    int idx = y * MAP_WIDTH + accessX;
    if (idx < map.size())
        return map[idx];
    else
        return 'X';
}
