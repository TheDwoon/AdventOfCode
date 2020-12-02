#include "Day02.h"
#include <regex>

std::vector<password_entry> Day02::parseInput(std::string& input)
{
    std::vector<password_entry> parsed;
    std::regex regex("^(\\d+)-(\\d+) (.): (.+)$");

    auto begin = std::sregex_iterator(input.begin(), input.end(), regex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it)
    {
        std::smatch match = *it;
        
        password_entry entry;
        entry.min = std::stoi(match[1].str());
        entry.max = std::stoi(match[2].str());
        entry.c = match[3].str()[0];
        entry.password = match[4].str();
        parsed.push_back(entry);
    }

    return parsed;
}

std::string Day02::runPart1(day_t& input)
{
    std::stringstream output;

    int correct = 0;
    for (auto& entry : input) {
        int matches = countMatches(entry.password, entry.c);
        if (entry.min <= matches && matches <= entry.max)
            ++correct;
    }

    output << correct;
    return output.str();
}

std::string Day02::runPart2(day_t& input)
{
    std::stringstream output;

    int correct = 0;
    for (auto& entry : input) {
        bool firstMatch = entry.password[entry.min - 1] == entry.c;
        bool secondMatch = entry.password[entry.max - 1] == entry.c;
        correct += firstMatch ^ secondMatch;
    }

    output << correct;
    return output.str();
}
