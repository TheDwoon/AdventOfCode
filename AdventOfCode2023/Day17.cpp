#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <queue>
#include "parser.cpp"
#include "vec2.cpp"
#include "mat2.cpp"

#define TITLE "Day 17"

struct map {
    std::vector<int> data;
    int width {0};
    int height {0};

    int operator()(const vec2i &pos) const {
        assert(pos.x >= 0 && pos.x < width);
        assert(pos.y >= 0 && pos.y < height);
        return data[pos.y * width + pos.x];
    }
};

typedef map day_t;

day_t parseInput(std::string &input) {
    day_t map;
    Parser p(input.c_str());
    while (!p.eof()) {
        while (!p.isNewLine()) {
            map.data.push_back(p.read() - '0');
        }

        p.readNewLine();

        if (map.width == 0)
            map.width = static_cast<int>(map.data.size());
    }

    assert(static_cast<int>(map.data.size()) % map.width == 0);
    map.height = static_cast<int>(map.data.size()) / map.width;

    return map;
}

const static mat2i TURN_RIGHT {0, -1, 1, 0};
const static mat2i TURN_LEFT {0, 1, -1, 0};

const static vec2i NORTH {0, -1};
const static vec2i SOUTH {0, 1};
const static vec2i WEST {-1, 0};
const static vec2i EAST {1, 0};

struct node {
    vec2i position {0,0};
    vec2i heading {0,0};
    int walkedStraight {0};
    int lostHeat {0};
};

struct customCompare {
    bool operator()(const node &lhs, const node &rhs) const {
        return lhs.lostHeat > rhs.lostHeat;
    }
};

bool isInMap(const map &m, const vec2i &pos) {
    return pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height;
}

bool canVisit(const map &m, std::vector<int> &costMap, const vec2i &pos, int minWalk, int maxWalked, int walked, int cost) {
    auto& pastCost = costMap[(maxWalked - minWalk + 1) * (pos.y * m.width + pos.x) + (walked - minWalk)];
    if (pastCost == -1 || cost < pastCost) {
        pastCost = cost;
        return true;
    } else {
        return false;
    }
}

void checkDirection(std::priority_queue<node, std::vector<node>, customCompare> &q, const node &n, const map &m, std::vector<int>& costMap, const vec2i &heading, int minWalk, int maxWalk) {
    int lostHeat = n.lostHeat;
    int alreadyWalked = 0;
    if (n.heading == heading)
        alreadyWalked = n.walkedStraight;

    for (int i = 1; i <= maxWalk - alreadyWalked; i++) {
        vec2i pos = n.position + i * heading;
        if (isInMap(m, pos)) {
            lostHeat += m(pos);

            if (i >= minWalk && canVisit(m, costMap, pos, minWalk, maxWalk, alreadyWalked + i, lostHeat)) {
                q.emplace(pos, heading, i, lostHeat);
            }
        }
    }
}

std::vector<int>& chooseCostMap(std::vector<int> *costs, const vec2i &heading) {
    if (heading == NORTH) {
        return costs[0];
    } else if (heading == EAST) {
        return costs[1];
    } else if (heading == SOUTH) {
        return costs[2];
    } else if (heading == WEST) {
        return costs[3];
    } else {
        assert(false);
        return costs[0];
    }
}

int minimizeHeatLoss(const map &m, int minWalked, int maxWalked) {
    std::priority_queue<node, std::vector<node>, customCompare> queue;
    node n { vec2i(0, 0), SOUTH, 0, 0 };
    const vec2i target(m.width - 1, m.height - 1);
    queue.push(n);

    std::vector<int> costs[4];

    int costSize = (maxWalked - minWalked + 1) * m.width * m.height;
    costs[0].resize(costSize, -1);
    costs[1].resize(costSize, -1);
    costs[2].resize(costSize, -1);
    costs[3].resize(costSize, -1);

    while (!queue.empty() && (n.position.x != target.x || n.position.y != target.y)) {
        n = queue.top();
        queue.pop();

        assert(n->heading.x >= -1 && n->heading.x <= 1);
        assert(n->heading.y >= -1 && n->heading.y <= 1);
        assert(n->heading.x != 0 || n->heading.y != 0);

        checkDirection(queue, n, m, chooseCostMap(costs, n.heading), n.heading, minWalked, maxWalked);

        vec2i leftHeading = TURN_LEFT * n.heading;
        checkDirection(queue, n, m, chooseCostMap(costs, leftHeading), leftHeading, minWalked, maxWalked);

        vec2i rightHeading = TURN_RIGHT * n.heading;
        checkDirection(queue, n, m, chooseCostMap(costs, rightHeading), rightHeading, minWalked, maxWalked);
    }

    assert(n->position.x == m.width - 1);
    assert(n->position.y == m.height - 1);
    return n.lostHeat;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int minLostHeat = minimizeHeatLoss(input, 1, 3);

    output << minLostHeat;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    int minLostHeat = minimizeHeatLoss(input, 4, 10);

    output << minLostHeat;
    return output.str();
}

// BOILER PLATE CODE BELOW

std::string readInput() {
    std::cin >> std::noskipws;

    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string fileContent(it, end);

    return fileContent;
}

std::string formatTime(std::chrono::duration<long long, std::nano> t) {
    std::stringstream output;
    if (t.count() < 10000) {
        output << t.count() << "ns";
    }
    else if (t.count() < 10000000) {
        output << t.count() / 1000 << "µs";
    }
    else {
        output << t.count() / 1000000 << "ms";
    }

    return output.str();
}

int main()
{
	std::cout << "######################################" << std::endl;
	std::cout << "############### " << TITLE << " ###############" << std::endl;
	std::cout << "######################################" << std::endl;

    auto gt0 = std::chrono::high_resolution_clock::now();
	const std::string originalInput = readInput();
    auto gt1 = std::chrono::high_resolution_clock::now();

    std::cout << "Reading Input: " << formatTime(gt1 - gt0) << '\n';
	std::cout << "**************************************\n";

    std::string input = originalInput;
	auto t0 = std::chrono::high_resolution_clock::now();
	day_t parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << '\n';
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 1 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

	input = originalInput;
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************\n";
	std::cout << output << '\n';
	std::cout << "*************** Task 2 ***************\n";
	std::cout << "Parsing: " << formatTime(t1 - t0) << '\n';
	std::cout << "Running: " << formatTime(t2 - t1) << '\n';
	std::cout << "Total: " << formatTime(t2 - t0) << '\n';
	std::cout << "**************************************" << std::endl;

    auto gt2 = std::chrono::high_resolution_clock::now();
    std::cout << "Global Time: " << formatTime(gt2 - gt0) << '\n';
	std::cout << "**************************************" << std::endl;
}
