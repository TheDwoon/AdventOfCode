#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>
#include <memory>
#include <map>
#include <queue>
#include <set>

#define TITLE "Day 12"

template<typename T>
struct vec2 {
    T x;
    T y;

    bool operator<(const vec2<T> &other) const {
        return y < other.y || (y == other.y && x < other.x);
    }
};

typedef vec2<int> vec2i;

struct node {
    node(const vec2i& pos) : pos(pos) {}

    vec2i pos;
    int hCost { 0 };
    int sCost { 0 };
    int totalCost { 0 };
    std::shared_ptr<node> prev;

    bool operator==(const node& other) const {
        return pos.x == other.pos.x && pos.y == other.pos.y && totalCost == other.totalCost;
    }

    bool operator!=(const node& other) const {
        return pos.x != other.pos.x || pos.y != other.pos.y || totalCost != other.totalCost;
    }
};
typedef std::shared_ptr<node> node_ptr;

struct node_cost_less {
    bool operator()(const node_ptr& left, const node_ptr& right) const {
        return left->totalCost > right->totalCost;
    }
};

struct map {
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> tiles;

    uint8_t operator()(int x, int y) const {
        return tiles[y * width + x];
    }

    vec2i start;
    vec2i end;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef map day_t;

day_t parseInput(std::string &input) {
    map map;
    map.width = input.find('\n');

    for (char c : input) {
        if (c == '\r' || c == '\n')
            continue;

        if (c == 'S') {
            c = 'a';
            int pos = map.tiles.size();
            map.start = { static_cast<int>(pos % map.width), static_cast<int>(pos / map.width) };
        } else if (c == 'E') {
            c = 'z';
            int pos = map.tiles.size();
            map.end = { static_cast<int>(pos % map.width), static_cast<int>(pos / map.width) };
        }

        map.tiles.push_back(c - 'a');
    }

    map.height = map.tiles.size() / map.width;
    assert(map.tiles.size() % map.width == 0);

    return map;
}

int distance(const vec2i& a, const vec2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<vec2i> aStar(const vec2i& start, const vec2i& end, const map& map) {
    const vec2i dir[4] { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
    std::vector<vec2i> path;
    std::map<vec2i, node_ptr> nodes;
    std::priority_queue<node_ptr, std::vector<node_ptr>, node_cost_less> pq;
    std::set<vec2i> closed;

    nodes[start] = std::make_shared<node>(start);
    pq.emplace(nodes[start]);

    while (!pq.empty() && !closed.contains(end)) {
        const node_ptr currentNode = pq.top();
        pq.pop();
        closed.emplace(currentNode->pos);
        if (*currentNode != *nodes[currentNode->pos])
            continue;

        uint8_t currentHeight = map(currentNode->pos.x, currentNode->pos.y);
        for (const vec2i& d : dir) {
            vec2i p = {currentNode->pos.x + d.x, currentNode->pos.y + d.y };
            if (closed.contains(p))
                continue;

            if (p.x >= 0 && p.x < map.width && p.y >= 0 && p.y < map.height) {
                uint8_t height = map(p.x, p.y);
                if (height <= currentHeight + 1) {
                    node_ptr nextNode = nodes[p];
                    int sCost = currentNode->sCost + 1;
                    int hCost = distance(p, end);
                    int totalCost = sCost + hCost;

                    if (!nextNode || totalCost < nextNode->totalCost) {
                        nextNode = std::make_shared<struct node>(p);
                        nextNode->prev = currentNode;
                        nextNode->sCost = sCost;
                        nextNode->hCost = hCost;
                        nextNode->totalCost = totalCost;

                        nodes[p] = nextNode;
                        pq.emplace(nextNode);
                    }
                }
            }
        }
    }

    node_ptr n = nodes[end];
    while (n) {
        path.push_back(n->pos);
        n = n->prev;
    }

    return path;
}

void drawMap(map& map, std::vector<vec2i> path) {
    std::set<vec2i> ps(path.begin(), path.end());

    vec2i v { 0, 0 };
    for (; v.y < map.height; ++v.y) {
        for (v.x = 0; v.x < map.width; ++v.x) {
            if (ps.contains(v))
                std::cout << "X";
            else
                std::cout << ".";
        }
        std::cout << std::endl;
    }
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    std::vector<vec2i> path = aStar(input.start, input.end, input);
    std::cout << std::endl;
    output << path.size() - 1;

    return output.str();
}

std::string runPart2(day_t& map) {
    std::stringstream output;

    int shortestPath = map.tiles.size();

    vec2i v { 0, 0 };
    for (v.y = 0; v.y < map.height; ++v.y) {
        for (v.x = 0; v.x < map.width; ++v.x) {
            if (map(v.x, v.y) == 0) {
                std::vector<vec2i> path = aStar(v, map.end, map);
                if (!path.empty())
                    shortestPath = std::min(static_cast<int>(path.size() - 1), shortestPath);
            }
        }
    }

    output << shortestPath;
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
        output << t.count() / 1000 << "us";
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

	const std::string originalInput = readInput();

    std::string input = originalInput;
	auto t0 = std::chrono::high_resolution_clock::now();
	day_t parsedInput = parseInput(input);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::string output = runPart1(parsedInput);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 1 ***************" << std::endl;
	std::cout << "Parsing: " << formatTime(t1 - t0) << std::endl;
	std::cout << "Running: " << formatTime(t2 - t1) << std::endl;
	std::cout << "Total: " << formatTime(t2 - t0) << std::endl;
	std::cout << "**************************************" << std::endl;

	input = originalInput;
	t0 = std::chrono::high_resolution_clock::now();
	parsedInput = parseInput(input);
	t1 = std::chrono::high_resolution_clock::now();
	output = runPart2(parsedInput);
	t2 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "**************************************" << std::endl;
	std::cout << output << std::endl;
	std::cout << "*************** Task 2 ***************" << std::endl;
	std::cout << "Parsing: " << formatTime(t1 - t0) << std::endl;
	std::cout << "Running: " << formatTime(t2 - t1) << std::endl;
	std::cout << "Total: " << formatTime(t2 - t0) << std::endl;
	std::cout << "**************************************" << std::endl;
}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator) {
    std::vector<std::string> tokenized;

    size_t pos = 0;
    size_t tokenEnd;
    do {
        tokenEnd = input.find(separator, pos);
        std::string token = input.substr(pos, (tokenEnd - pos));
        tokenized.push_back(token);
        pos = tokenEnd + separator.size();
    } while(tokenEnd != std::string::npos);

    return tokenized;
}
