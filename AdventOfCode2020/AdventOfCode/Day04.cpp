#include "Day04.h"
#include <sstream>
#include <regex>

std::vector<std::map<std::string, std::string>> Day04::parseInput(std::string& input)
{
    std::vector<std::map<std::string, std::string>> parsed;
    std::stringstream stream(input);

    std::regex regex("([a-z]{3}):([a-zA-Z0-9#]+)");
    std::map<std::string, std::string> current;
    for (std::string line; std::getline(stream, line);)
    {
        if (line.empty())
        {
            parsed.push_back(current);
            current = std::map<std::string, std::string>();
        }
        else 
        {
            auto begin = std::sregex_iterator(line.begin(), line.end(), regex);
            auto end = std::sregex_iterator();
            for (auto it = begin; it != end; ++it)
            {
                std::smatch match = *it;

                std::string key = match[1].str();
                std::string value = match[2].str();
                current[key] = value;
            }
        }
    }

    parsed.push_back(current);

    return parsed;
}

/*
byr (Birth Year)
iyr (Issue Year)
eyr (Expiration Year)
hgt (Height)
hcl (Hair Color)
ecl (Eye Color)
pid (Passport ID)
cid (Country ID)
*/

std::string Day04::runPart1(day_t& input)
{
    std::stringstream output;

    int valid = 0;
    for (auto& passport : input)
    {
        const auto end = passport.end();
        if (passport.find(byr) != end && passport.find(iyr) != end && passport.find(eyr) != end
            && passport.find(hgt) != end && passport.find(hcl) != end && passport.find(ecl) != end
            && passport.find(pid) != end)
            valid++;
    }

    output << valid;
    return output.str();
}

std::regex valid_hcl("#[0-9a-f]{6}");
std::regex valid_ecl("(amb|blu|brn|gry|grn|hzl|oth)");
std::regex valid_pid("[0-9]{9}");

std::string Day04::runPart2(day_t& input)
{
    std::stringstream output;

    int valid = 0;
    for (auto& passport : input)
    {
        const auto end = passport.end();
        if (passport.find(byr) != end && passport.find(iyr) != end && passport.find(eyr) != end
            && passport.find(hgt) != end && passport.find(hcl) != end && passport.find(ecl) != end
            && passport.find(pid) != end)
        {

            int bYear = std::stoi(passport[byr]);
            if (bYear < 1920 || bYear > 2002)
                continue;

            int iYear = std::stoi(passport[iyr]);
            if (iYear < 2010 || iYear > 2020)
                continue;

            int expYear = std::stoi(passport[eyr]);
            if (expYear < 2020 || expYear > 2030)
                continue;

            std::string height = passport[hgt];
            size_t inPos = height.find("in");
            size_t cmPos = height.find("cm");
            if (cmPos != std::string::npos) {
                int h = std::stoi(height.substr(0, inPos));
                if (h < 150 || h > 193)
                    continue;
            } else if (inPos != std::string::npos) {
                int h = std::stoi(height.substr(0, inPos));
                if (h < 59 || h > 76)
                    continue;
            }
            else {
                continue;
            }

            std::smatch match;
            if (!std::regex_match(passport[hcl], match, valid_hcl))
                continue;

            if (!std::regex_match(passport[ecl], match, valid_ecl))
                continue;

            if (!std::regex_match(passport[pid], match, valid_pid))
                continue;

            valid++;
        }
    }

    output << valid;
    return output.str();
}
