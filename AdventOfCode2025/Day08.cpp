#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "parser.cpp"
#include "vec3.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

struct measurement
{
    int i;
    int j;
    int distance;

    friend bool operator<(const measurement& lhs, const measurement& rhs) { return lhs.distance < rhs.distance; }
};

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    std::map<int, std::set<vec3i>> circuits;
    std::vector<vec3i> points;
    Parser p(buffer);

    while (!p.eof())
    {
        vec3i pos;
        p.readNumber(pos.x);
        p.consume(',');
        p.readNumber(pos.y);
        p.consume(',');
        p.readNumber(pos.z);
        p.readNewLine();
        points.push_back(pos);
    }

    int circuitId = 1;
    std::vector<int> circuit;
    circuit.resize(points.size());

    std::vector<measurement> measurements;
    for (int i = 0; i < points.size(); ++i)
    {
        for (int j = i + 1; j < points.size(); ++j)
        {
            measurements.push_back({ i, j, (points[j] - points[i]).length2()});
        }
    }

    std::sort(measurements.begin(), measurements.end());

    auto it = measurements.begin();
    for (int i = 0; i < 1000; i++)
    {
        if (circuit[it->i] == 0 && circuit[it->j] == 0)
        {
            const int id = circuitId++;
            circuit[it->i] = id;
            circuit[it->j] = id;

            circuits[id].insert(points[it->i]);
            circuits[id].insert(points[it->j]);
        } else if (circuit[it->i] == 0)
        {
            const int existingNetworkId = circuit[it->j];
            circuit[it->i] = existingNetworkId;
            circuits[existingNetworkId].insert(points[it->i]);
        } else if (circuit[it->j] == 0)
        {
            const int existingNetworkId = circuit[it->i];
            circuit[it->j] = existingNetworkId;
            circuits[existingNetworkId].insert(points[it->j]);
        } else if (circuit[it->i] != circuit[it->j])
        {
            const int existingNetworkId = circuit[it->i];
            const int replacedNetworkId = circuit[it->j];
            std::ranges::replace(circuit, replacedNetworkId, existingNetworkId);
            circuits[existingNetworkId].insert(circuits[replacedNetworkId].begin(), circuits[replacedNetworkId].end());
            circuits.erase(replacedNetworkId);
        } else
        {
            printf("Nothing todo!!\n");
        }

        ++it;
    }

    std::vector<size_t> counts;
    std::ranges::transform(circuits, std::back_inserter(counts), [](const auto& pair) { return pair.second.size(); });

    std::sort(counts.begin(), counts.end(), std::greater<>());
    part1 = static_cast<int>(counts[0] * counts[1] * counts[2]);

    // 20125 --> too low

    printf("%d\n",part1);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
