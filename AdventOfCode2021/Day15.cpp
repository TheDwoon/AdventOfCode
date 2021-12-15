#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <set>
#include <deque>
#include <list>
#include <algorithm>
#include "vec2.h"

#define TITLE "Day 15"
#define DEBUG

struct Map {
    uint16_t width { 0 };
    uint16_t height { 0 };
    std::vector<uint8_t> tiles;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef Map day_t;

day_t parseInput(std::string &input) {
    Map map;
    map.width = input.find('\n');

    std::stringstream stream(input);
    char c;
    while (stream.get(c)) {
        if (c >= '0' && c <= '9') {
            map.tiles.push_back(c - '0');
        }
    }

    map.height = map.tiles.size() / map.width;
    return map;
}

struct node {
    const node* previous;
    vec2i position;
    uint32_t risk_movement;
    uint32_t risk_heuristic;
    uint32_t total_risk;
};

node* build_node(const day_t& map, std::list<node*> &allocated_nodes, const node* previous, int x, int y) {
    node* next_node = new node;
    next_node->previous = previous;
    next_node->position = { x, y };
    next_node->risk_movement = previous->risk_movement + map.tiles[y * map.width + x];
    next_node->risk_heuristic = (map.width - 1 - x) + (map.height - 1 - y);
    next_node->total_risk = next_node->risk_movement + next_node->risk_heuristic;

    allocated_nodes.push_back(next_node);
    return next_node;
}

const vec2i dir_up { 0, 1 };
const vec2i dir_down { 0, -1 };
const vec2i dir_left { -1, 0 };
const vec2i dir_right { 1, 0 };

const node* a_star(const day_t& map, std::list<node*> &allocated_nodes) {
    std::set<vec2i> visited;
    std::deque<node*> queue;
    queue.push_back(allocated_nodes.front());
    visited.insert(allocated_nodes.front()->position);

    const node* target_node {nullptr};

    while (!queue.empty()) {
        const node* current_node = queue.front();

        const vec2i& current_position = current_node->position;
#ifdef DEBUG
        std::cout << current_position.x << "," << current_position.y << '\n';
#endif
        if (current_position.x == map.width - 1 && current_position.y == map.height - 1) {
            target_node = current_node;
            break;
        }

        const vec2i up = current_position + dir_up;
        const vec2i down = current_position + dir_down;
        const vec2i left = current_position + dir_left;
        const vec2i right = current_position + dir_right;

        if (left.x >= 0 && visited.find(left) == visited.end()) {
            visited.insert(left);
            queue.emplace_back(build_node(map, allocated_nodes, current_node, left.x, left.y));
        }
        if (right.x < map.width && visited.find(right) == visited.end()) {
            visited.insert(right);
            queue.emplace_back(build_node(map, allocated_nodes, current_node, right.x, right.y));
        }
        if (down.y >= 0 && visited.find(down) == visited.end()) {
            visited.insert(down);
            queue.emplace_back(build_node(map, allocated_nodes, current_node, down.x, down.y));
        }
        if (up.y < map.height && visited.find(up) == visited.end()) {
            visited.insert(up);
            queue.emplace_back(build_node(map, allocated_nodes, current_node, up.x, up.y));
        }

        queue.pop_front();

        std::sort(queue.begin(), queue.end(), [](const node* lhs, const node* rhs) -> bool {
            return lhs->total_risk < rhs->total_risk;
        });
    }

#ifdef DEBUG
    const node* cp = target_node;
    while (cp != nullptr) {
        std::cout << cp->position.x << ',' << cp->position.y << " <- ";
        cp = cp->previous;
    }

    std::cout << '\n';
#endif

    return target_node;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    std::list<node*> allocated_nodes;

    node* n = new node();
    n->position = { 0, 0 };
    n->risk_movement = 0;
    n->risk_heuristic = 0;
    n->total_risk = 0;
    allocated_nodes.push_back(n);

    const node* path = a_star(input, allocated_nodes);

    output << path->risk_movement;

    for (const node* node : allocated_nodes) {
        delete node;
    }

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
