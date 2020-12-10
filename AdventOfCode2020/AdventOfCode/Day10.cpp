#include "Day10.h"
#include <algorithm>

std::vector<int> Day10::parseInput(std::string &input) {
    std::vector<int> parsed;
    std::stringstream stream(input);
    int v;
    while (stream && stream >> v)
        parsed.push_back(v);

    return parsed;
}

std::string Day10::runPart1(std::vector<int> &input) {
    std::stringstream output;
    std::sort(input.begin(), input.end());

    int joltJumps[4] {0, 0, 0, 1};
    int currentJolts = 0;
    for (int jolt : input) {
        joltJumps[jolt - currentJolts]++;
        currentJolts = jolt;
    }

    output << (joltJumps[1] * joltJumps[3]);
    return output.str();
}

std::string Day10::runPart2(std::vector<int> &input) {
    std::stringstream output;
    std::sort(input.begin(), input.end());

    input.insert(input.begin(), 0);
    input.push_back(input[input.size() - 1] + 3);

    uint64_t possibilities = 1;
    size_t start = 0, end = 0;
    while (start < input.size()) {
        while (input[end + 1] - input[end] == 1)
            end++;
        end++;

        possibilities *= countPossibilities(start, end, input);

        start = end;
        end++;
    }

    output << possibilities ;
    return output.str();
}

uint64_t Day10::countPossibilities(size_t idx, size_t end, const std::vector<int> &input) {
    // we found one path!
    if (idx == end - 1 || idx == end)
        return 1;

    // this is a guarantee. The next element will never be out of reach for the current element
    uint64_t possibilities = countPossibilities(idx + 1, end, input);

    if (idx + 2 < end && input[idx + 2] - input[idx] <= 3)
        possibilities += countPossibilities(idx + 2, end, input);

    if (idx + 3 < end && input[idx + 3] - input[idx] <= 3)
        possibilities += countPossibilities(idx + 3, end, input);

    return possibilities;
}
