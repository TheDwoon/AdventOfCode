#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <map>
#include "parser.cpp"

#define TITLE "Day 08"

struct map_node {
    std::array<char, 3> left {0};
    std::array<char, 3> right {0};
};

struct desert_map {
    std::vector<char> instructions;
    std::map<std::array<char, 3>, map_node> nodes;
};

typedef desert_map day_t;

day_t parseInput(std::string &input) {
    desert_map map;
    Parser p(input.c_str());
    while (p.isLetter()) {
        map.instructions.push_back(p.read());
    }

    p.readNewLine();
    p.readNewLine();

    std::array<char, 3> key {0};
    map_node node;
    while (!p.eof()) {
        p.readToBuffer(key.data(), 3);
        p.consume(" = (");
        p.readToBuffer(node.left.data(), 3);
        p.consume(", ");
        p.readToBuffer(node.right.data(), 3);
        p.consume(")");
        p.readNewLine();

        map.nodes[key] = node;
    }

    return map;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    size_t step = 0;

    std::array<char, 3> pos { 'A', 'A', 'A' };
    while (pos[0] != 'Z' || pos[1] != 'Z' || pos[2] != 'Z') {
        const map_node &node = input.nodes[pos];
        if (input.instructions[step++ % input.instructions.size()] == 'L') {
            pos = node.left;
        } else {
            pos = node.right;
        }
    }

    output << step;
    return output.str();
}

typedef std::array<char, 3> map_key;

bool reachedEnd(const std::vector<map_key> &positions) {
    for (const map_key& key : positions) {
        if (key[2] != 'Z')
            return false;
    }

    return true;
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    std::vector<map_key> positions;
    for (const auto& node : input.nodes) {
        if (node.first[2] == 'A')
            positions.push_back(node.first);
    }

    const size_t size = input.instructions.size();
    size_t step = 0;
    while (!reachedEnd(positions)) {
        for (map_key &pos : positions) {
            const map_node &node = input.nodes[pos];
            if (input.instructions[step % size] == 'L') {
                pos = node.left;
            } else {
                pos = node.right;
            }
        }

        step++;

        if (step % 10000 == 0)
            std::cout << step << '\n';
    }

    output << step;
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
