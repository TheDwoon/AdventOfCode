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
    int heuristic {0};
    std::shared_ptr<node> previous;
};

struct customCompare {
    bool operator()(std::shared_ptr<node> &lhs, std::shared_ptr<node> &rhs) const {
        return lhs->lostHeat > rhs->lostHeat;
    }
};

bool isInMap(const map &m, const vec2i &pos) {
    return pos.x >= 0 && pos.x < m.width && pos.y >= 0 && pos.y < m.height;
}

int getDistanceCost(const vec2i& position, const vec2i &target) {
    return 100 * (std::abs(position.x - target.x) + std::abs(position.y - target.y));
}

bool visit(const map &m, int* costMap, const vec2i &pos, const vec2i &heading, int cost) {
    int h = 0;
    if (heading == EAST)
        h = 1;
    else if (heading == SOUTH)
        h = 2;
    else if (heading == WEST)
        h = 3;

    int pastCost = costMap[4 * (pos.y * m.width + pos.x) + h];
    if (pastCost == -1 || cost <= pastCost) {
        costMap[4 * (pos.y * m.width + pos.x) + h] = cost;
        return true;
    } else {
        return false;
    }
}

void printPath(map m, std::shared_ptr<node> n) {
    int lostHeat = n->lostHeat;
    while (n) {
        std::cout << n->position << " ";
        m.data[n->position.y * m.width + n->position.x] |= 1024;
        n = n->previous;
    }
    std::cout << "[" << lostHeat << "]" << "\n";

    for (int y = 0; y < m.height; y++) {
        for (int x = 0; x < m.width; x++) {
            int i = m.data[y * m.width + x];
            if ((i & 1024) == 1024)
                std::cout << (char) (i - 1024 + '0');
            else
                std::cout << '.';
        }

        std::cout << "\n";
    }
}

int minimizeHeatLoss(const map &m, vec2i startPosition, vec2i heading, vec2i target) {
    std::priority_queue<std::shared_ptr<node>, std::vector<std::shared_ptr<node>>, customCompare> queue;
    std::shared_ptr<node> n = std::make_shared<node>(startPosition, heading, 0, 0, 0);
    queue.push(n);

    assert(EAST == TURN_RIGHT * NORTH);
    assert(SOUTH == TURN_RIGHT * EAST);
    assert(WEST == TURN_RIGHT * SOUTH);
    assert(NORTH == TURN_RIGHT * WEST);

    assert(WEST == TURN_LEFT * NORTH);
    assert(NORTH == TURN_LEFT * EAST);
    assert(EAST == TURN_LEFT * SOUTH);
    assert(SOUTH == TURN_LEFT * WEST);

    int* costMap = new int[4 * m.width * m.height];
    std::fill(costMap, costMap + 4 * m.width * m.height, -1);

    while (!queue.empty() && (n->position.x != target.x || n->position.y != target.y)) {
        n = queue.top();
        queue.pop();

        std::cout << n->lostHeat << "\n";
//        printPath(m, n);
//        std::cout << "******************" << std::endl;

        assert(n->heading.x >= -1 && n->heading.x <= 1);
        assert(n->heading.y >= -1 && n->heading.y <= 1);
        assert(n->heading.x != 0 || n->heading.y != 0);

        vec2i straightPos = n->position + n->heading;
        if (isInMap(m, straightPos) && n->walkedStraight < 2) {
            int tileHeat = m(straightPos);
            assert(tileHeat > 0);
            int heuristic = getDistanceCost(straightPos, target);
            if (visit(m, costMap, straightPos, heading, n->lostHeat + tileHeat)) {
                queue.push(std::make_shared<node>(straightPos, n->heading, n->walkedStraight + 1, n->lostHeat + tileHeat, heuristic, n));
            }
        }

        vec2i leftHeading = TURN_LEFT * n->heading;
        vec2i leftPos = n->position + leftHeading;
        if (isInMap(m, leftPos)) {
            int tileHeat = m(leftPos);
            assert(tileHeat > 0);
            int heuristic = getDistanceCost(leftPos, target);
            if (visit(m, costMap, leftPos, heading, n->lostHeat + tileHeat)) {
                queue.push(std::make_shared<node>(leftPos, leftHeading, 0, n->lostHeat + tileHeat, heuristic, n));
            }
        }

        vec2i rightHeading = TURN_RIGHT * n->heading;
        vec2i rightPos = n->position + rightHeading;
        if (isInMap(m, rightPos)) {
            int tileHeat = m(rightPos);
            assert(tileHeat > 0);
            int heuristic = getDistanceCost(rightPos, target);
            if (visit(m, costMap, rightPos, heading, n->lostHeat + tileHeat)) {
                queue.push(std::make_shared<node>(rightPos, rightHeading, 0, n->lostHeat + tileHeat, heuristic, n));
            }
        }
    }

    printPath(m, n);

    delete[] costMap;
    assert(n->position.x == m.width - 1);
    assert(n->position.y == m.height - 1);
    return n->lostHeat;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int minLostHeat = minimizeHeatLoss(input, {0, 0}, EAST, {input.width - 1, input.height - 1});

    output << minLostHeat;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

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