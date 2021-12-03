#include "Day01.h"
#include <sstream>

std::vector<int> Day01::parseInput(std::string& input)
{
    std::vector<int> parsed;
    std::stringstream stream(input);

    int v;
    while (stream >> v) {
        parsed.push_back(v);
    }

    return parsed;
}

std::string Day01::runPart1(day_t& input)
{
    std::stringstream output;

    const int size = input.size();
    int increased = 0;
    for (size_t i = 0; i < size - 1; ++i)
    {
        increased += (input[i] < input[i + 1]);
    }

    output << increased;

    return output.str();
}

std::string Day01::runPart2(day_t& input)
{
    std::stringstream output;

    const int size = input.size();
    int increased = 0;
    for (size_t i = 0; i < size - 3; ++i) {
        increased += (input[i] < input[i + 3]);
    }

    output << increased;

    return output.str();
}

