#include "Day06.h"
#include <sstream>

std::vector<std::array<uint8_t, 26>> Day06::parseInput(std::string& input)
{
    std::vector<std::array<uint8_t, 26>> parsed;
    std::stringstream stream(input);
    
    std::array<uint8_t, 26> answers;
    for (std::string line; std::getline(stream, line);)
    {
        if (line.empty()) 
        {
            parsed.push_back(answers);
            //for (auto& e : answers)
            //    std::cout << (e == 0 ? "0" : "1") << " ";
            //std::cout << std::endl;

            answers = std::array<uint8_t, 26>();
        }
        else 
        {
            for (size_t i = 0; i < line.size() / sizeof(char); i++)
            {
                std::cout << line[i];
                answers[line[i] - 'a'] = 1;
            }
        }
    }

    parsed.push_back(answers);
    return parsed;
}

std::string Day06::runPart1(day_t& input)
{
    std::stringstream output;

    int sum = 0;
    for (auto& answers : input)
    {
        for (auto& e : answers)
        {
            std::cout << (e == 0 ? "0" : "1");
            sum += e;
        }

        std::cout << std::endl;
    }

    output << sum;
    return output.str();
}
