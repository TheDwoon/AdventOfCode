#include "Day13.h"
#include <regex>

std::vector<int> Day13::parseInput(std::string &input) {
    std::vector<int> parsed;
    std::regex regex("(\\d+|x)");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex); it != std::sregex_iterator(); ++it) {
        std::smatch match = *it;
        if (match[1].str() == "x") {
            parsed.push_back(0);
        } else {
            parsed.push_back(std::stoi(match[1].str()));
        }
    }

    return parsed;
}

std::string Day13::runPart1(std::vector<int> &input) {
    std::stringstream output;

    int targetTime = input[0];

    int smallestId = -1, minWait = INT32_MAX;
    for (int i = 1; i < input.size(); i++) {
        if (input[i] == 0)
            continue;

        int waitTime = ((-targetTime % input[i]) + input[i]) % input[i];
        if (waitTime < minWait) {
            minWait = waitTime;
            smallestId = input[i];
        }
    }

    output << smallestId * minWait;
    return output.str();
}

std::string Day13::runPart2(std::vector<int> &input) {
    std::stringstream output;

    std::vector<int> busIds({input[1]});
    std::vector<int> delays;
    for (int i = 2; i < input.size(); i++) {
        if (input[i] != 0) {
            busIds.push_back(input[i]);
            delays.push_back(i - 1);
        }

    }
    // last bus has no delay
    delays.push_back(0);

    long result = 0;
    long step = busIds[0];
    for (int i = 1; i < busIds.size(); i++) {
        while ((result + delays[i - 1]) % busIds[i] != 0)
            result += step;

        step *= busIds[i];
    }

    output << result;
    return output.str();
}
