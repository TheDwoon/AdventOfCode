#include "Day05.h"
#include <sstream>
#include <algorithm>

std::vector<uint16_t> Day05::parseInput(std::string& input)
{
    std::vector<uint16_t> parsed;
    std::stringstream stream(input);
    for (std::string line; std::getline(stream, line);)
    {
        uint16_t code{ 0 };
        for (size_t i = 0; i < 7; i++)
            code = (code << 1) + (line[i] == 'B');

        for (size_t i = 0; i < 3; i++)
            code = (code << 1) + (line[7 + i] == 'R');

        parsed.push_back(code);
    }

    return parsed;
}

std::string Day05::runPart1(day_t& input)
{
    std::stringstream output;

    uint16_t maxSeatId = 0;
    for (auto& seatId : input)
    {
        if (seatId > maxSeatId)
            maxSeatId = seatId;
    }

    output << maxSeatId;
    return output.str();
}

std::string Day05::runPart2(day_t& input)
{
    std::stringstream output;

    std::sort(input.begin(), input.end());
    for (size_t i = 0; i < input.size() - 1; ++i)
        if (input[i] + 2 == input[i + 1])
        {
            output << (input[i] + 1);
            break;
        }

    return output.str();
}
