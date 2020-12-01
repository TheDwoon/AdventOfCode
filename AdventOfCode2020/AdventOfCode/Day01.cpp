#include "Day01.h"
#include <sstream>

std::vector<int> Day01::parseInput(std::string& input)
{
    std::vector<int> parsed;
    std::stringstream stream(input);

    int v;
    while (stream.rdbuf()->in_avail() > 0) {
        stream >> v;
        parsed.push_back(v);
    }

    return parsed;
}

std::string Day01::runPart1(day_t& input)
{
    std::stringstream output;

    const int size = input.size();
    for (size_t i = 0; i < size; ++i)
    {
        const int x = input[i];
        for (size_t j = i + 1; j < size; j++)
        {
            const int y = input[j];
            if (x + y == 2020) {
                output << (x * y);
            }
        }
    }

    return output.str();
}

std::string Day01::runPart2(day_t& input)
{
    std::stringstream output;

    const int size = input.size();
    for (size_t i = 0; i < size; ++i)
    {
        const int x = input[i];
        for (size_t j = i + 1; j < size; ++j)
        {
            const int y = input[j];
            for (size_t k = j + 1; k < size; ++k)
            {
                const int z = input[k];
                if (x + y + z == 2020) {
                    output << (x * y * z);
                }
            }
        }
    }

    return output.str();
}

