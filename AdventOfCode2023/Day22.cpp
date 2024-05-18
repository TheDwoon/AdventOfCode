#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"
#include "util.cpp"
#include "vec3.cpp"

#define TITLE "Day 22"

struct brick {
    vec3i start;
    vec3i end;
};

typedef std::vector<brick> day_t;

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    while (!p.eof()) {
        brick b;
        p.readInt(b.start.x);
        p.consume(',');
        p.readInt(b.start.y);
        p.consume(',');
        p.readInt(b.start.z);
        p.consume('~');
        p.readInt(b.end.x);
        p.consume(',');
        p.readInt(b.end.y);
        p.consume(',');
        p.readInt(b.end.z);
        result.push_back(b);

        p.readNewLine();

    }

    return result;
}

std::vector<int> buildLayerIndex(day_t &bricks) {
    std::sort(bricks.begin(), bricks.end(), [](const brick& a, const brick& b) {
        return a.start.z < b.start.z;
    });

    std::vector<int> layers;
    int layer = 0;

    for (int i = 0; i < bricks.size(); ++i) {
        for (; layer <= bricks[i].start.z; layer++) {
            layers.push_back(i);
        }
    }

    return layers;
}

bool overlapXY(const vec3i &start1, const vec3i &end1, const vec3i &start2, const vec3i &end2) {
    return start1.x < end2.x && end1.x > start2.x && start1.y < end2.y && end1.y > start2.y;
}

bool hasSpaceBelow(const day_t &input, const std::vector<int> &layers, const brick &b) {
    if (b.start.z == 0)
        return false;

    for (int i = layers[b.start.z - 1]; i < input.size() && input[i].start.z < b.start.z; i++) {

    }

    return true;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    std::vector<int> layers = buildLayerIndex(input);


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
