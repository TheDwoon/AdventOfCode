#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>
#include "parser.cpp"
#include "util.cpp"
#include "vec3.cpp"

#define TITLE "Day 22"

struct brick {
    int num;
    vec3i start;
    vec3i end;
};

typedef std::vector<brick> day_t;

day_t parseInput(const std::string &input) {
    int i = 0;
    day_t result;
    Parser p(input.c_str());
    while (!p.eof()) {
        brick b;
        b.num = ++i;
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

        assert(b.start.x <= b.end.x);
        assert(b.start.y <= b.end.y);
        assert(b.start.z <= b.end.z);

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

bool overlapXY(const brick& a, const brick& b) {
    const vec3i& start1 { a.start };
    const vec3i& end1 { a.end };
    const vec3i& start2 { b.start };
    const vec3i& end2 { b.end };

    assert(start1.x <= end1.x);
    assert(start1.y <= end1.y);
    assert(start2.y <= end2.y);
    assert(start2.y <= end2.y);

    return aoc::hasOverlap(start1.x, end1.x, start2.x, end2.x) && aoc::hasOverlap(start1.y, end1.y, start2.y, end2.y);
}

bool overlapXYZ(const brick& a, const brick& b) {
    const vec3i& start1 { a.start };
    const vec3i& end1 { a.end };
    const vec3i& start2 { b.start };
    const vec3i& end2 { b.end };

    assert(start1.x <= end1.x);
    assert(start1.y <= end1.y);
    assert(start1.z <= end1.z);
    assert(start2.x <= end2.x);
    assert(start2.y <= end2.y);
    assert(start2.z <= end2.z);

    return aoc::hasOverlap(start1.x, end1.x, start2.x, end2.x)
        && aoc::hasOverlap(start1.y, end1.y, start2.y, end2.y)
        && aoc::hasOverlap(start1.z, end1.z, start2.z, end2.z);
}

bool hasSpaceBelow(const day_t &input, const std::vector<int> &layers, brick b) {
    if (b.start.z <= 1)
        return false;

    b.start.z -= 1;
    b.end.z -= 1;

    // for (int i = layers[b.start.z - 1]; i < layers[b.start.z]; i++) {
    //     if (overlapXY(input[i], b))
    //         return false;
    // }

    for (const brick& brick : input) {
        if (brick.num != b.num && overlapXYZ(brick, b))
            return false;
    }

    return true;
}

int countBricksBelow(const day_t &input, const std::vector<int> &layers, const brick &b) {
    if (b.start.z <= 1)
        return 1;

    int count = 0;
    for (int i = layers[b.start.z - 1]; i < layers[b.start.z]; i++) {
        if (overlapXY(input[i], b))
            count += 1;
    }

    return count;
}

std::vector<int> getBricksBelow(const day_t &input, const std::vector<int> &layers, brick b) {
    std::vector<int> supporingBricks;
    if (b.start.z <= 1) {
        supporingBricks.push_back(0);
    } else {
        b.start.z -= 1;
        b.end.z = b.start.z;

        for (const brick& brick : input) {
            if (overlapXYZ(brick, b))
                supporingBricks.push_back(brick.num);
        }

        // for (int i = layers[b.start.z - 1]; i < layers[b.start.z]; i++) {
        //     if (overlapXY(input[i], b))
        //         supporingBricks.push_back(input[i].num);
        // }
    }

    return supporingBricks;
}

void printSlice(const day_t& input, const vec3i& origin, vec3i xDirection, int width, vec3i yDirection, int height) {
    std::vector<int> blocks;
    blocks.resize(width * height, 0);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vec3i position = origin + y * yDirection + x * xDirection;

            for (const brick& b : input) {
                if (aoc::inRange(b.start, b.end, position)) {
                    assert(blocks[y * width + x] == 0);
                    blocks[y * width + x] = b.num;
                }
            }
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%2d ", blocks[y * width + x]);
        }
        printf("\n");
    }
}

std::string runPart1(day_t& input) {
    assert(!aoc::hasOverlap(0, 1, 3, 4));
    assert(aoc::hasOverlap(1, 3, 3, 4));
    assert(aoc::hasOverlap(1, 4, 2, 3));
    assert(aoc::hasOverlap(0, 2, 2, 4));
    assert(aoc::hasOverlap(0, 2, 1, 2));
    assert(aoc::hasOverlap(0, 3, 1, 2));
    assert(aoc::hasOverlap(1, 2, 0, 3));

    std::stringstream output;

    bool changed;
    do {
        changed = false;

        std::vector<int> layers = buildLayerIndex(input);
        for (brick& b : input) {
            if (hasSpaceBelow(input, layers, b)) {
                changed = true;
                b.start.z -= 1;
                b.end.z -= 1;
            }
        }
    } while (changed);

    for (const brick& a : input) {
        for (const brick& b : input) {

        }
    }

    for (vec3i origin = {0,0,1}; origin.z < 7; origin.z++) {
        std::cout << "Layer " << origin.z << ":\n";
        printSlice(input, origin, {1, 0, 0}, 3, {0, 1, 0}, 3);
    }

    aoc::directed_graph graph;
    graph.addNode(0);
    for (brick& b : input) {
        graph.addNode(b.num);
    }

    std::vector<int> layers = buildLayerIndex(input);
    for (brick& b : input) {
        auto& supportedNode = graph.nodes[b.num];
        std::vector<int> bricksBelow = getBricksBelow(input, layers, b);
        for (int brickNumber : bricksBelow) {
            auto& supporting_node = graph.nodes[brickNumber];
            graph.addEdge(supporting_node, supportedNode, 0);
        }
    }

    for (const auto& node : graph.nodes) {
        std::cout << node->value << " supports ";
        for (const auto& edge : node->edges_out) {
            std::cout << edge->target->value << " ";
        }

        std::cout << std::endl;
    }

    for (const auto& node : graph.nodes) {
        std::cout << node->value << " rests on ";
        for (const auto& edge : node->edges_in) {
            std::cout << edge->source->value << " ";
        }

        std::cout << std::endl;
    }

    int desintegrated = 0;
    for (int i = 1; i < graph.nodes.size(); i++) {
        const auto& node = graph.nodes[i];
        bool canRemove = true;
        for (const auto& supported : node->edges_out) {
            if (supported->target->edges_in.size() <= 1)
                canRemove = false;
        }

        if (canRemove)
            desintegrated += 1;
    }

    output << desintegrated;
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
