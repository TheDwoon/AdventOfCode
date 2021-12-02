#include "Day02.h"
#include <sstream>

std::vector<command> Day02::parseInput(std::string& input)
{
    std::vector<command> parsed;
    std::stringstream stream(input);

    command c;
    while (stream >> c.command >> c.movement) {
        parsed.push_back(c);
    }

    return parsed;
}

std::string Day02::runPart1(day_t& input)
{
    std::stringstream output;

    int x = 0;
    int depth = 0;

    for (const command &c : input) {
        switch (c.command[0]) {
            case 'f':
                x += c.movement;
                break;
            case 'u':
                depth -= c.movement;
                break;
            case 'd':
                depth += c.movement;
                break;
        }
    }

    output << (x * depth);

    return output.str();
}

std::string Day02::runPart2(day_t& input)
{
    std::stringstream output;

    int x = 0;
    int depth = 0;
    int aim = 0;

    for (const command &c : input) {
        switch (c.command[0]) {
            case 'f':
                x += c.movement;
                depth += aim * c.movement;
                break;
            case 'u':
                aim -= c.movement;
                break;
            case 'd':
                aim += c.movement;
                break;
        }
    }

    output << (x * depth);

    return output.str();
}

