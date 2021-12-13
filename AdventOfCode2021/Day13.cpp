#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>
#include "vec2.h"

#define TITLE "Day 13"
//#define DEBUG

enum fold_axis {
    X, Y
};

struct fold {
    int position;
    fold_axis axis;
};

struct FoldedMessage {
    std::vector<vec2i> points;
    std::vector<fold> folds;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef FoldedMessage day_t;

day_t parseInput(std::string &input) {
    day_t parsed;

    std::regex regex_cords("(\\d+),(\\d+)");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex_cords); it != std::sregex_iterator(); ++it) {
        const auto &match = *it;

        parsed.points.emplace_back( std::stoi(match[1]), std::stoi(match[2]));
    }

    std::regex regex_folds("fold along (x|y)=(\\d+)");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex_folds); it != std::sregex_iterator(); ++it) {
        const auto &match = *it;

        fold f;
        f.axis = match[1] == "x" ? fold_axis::X : fold_axis::Y;
        f.position = std::stoi(match[2]);
        parsed.folds.push_back(f);
    }

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    int max_x { 0 }, max_y { 0 };
    for (const vec2i &vec : input.points) {
        max_x = std::max(max_x, vec.x);
        max_y = std::max(max_y, vec.y);
    }

    const int field_width = max_x + 1;
    const int field_height = max_y + 1;

    bool* field = new bool[field_width * field_height];
    std::fill(field, field + field_width * field_height, false);
    for (const vec2i &vec : input.points) {
        field[vec.y * field_width + vec.x] = true;
    }

    int folded_height = field_height;
    int folded_width = field_width;

#ifdef DEBUG
    for (int y = 0; y < folded_height; y++) {
        for (int x = 0; x < folded_width; x++) {
            std::cout << (field[y * field_width + x] ? '#' : '.');
        }
        std::cout << '\n';
    }
    std::cout << '\n';
#endif

    const fold &f = input.folds[0];
    switch (f.axis) {
        case X:
            folded_width = f.position;
            break;
        case Y:
            folded_height = f.position;
            break;
    }

    for (int y = 0; y < folded_height; y++) {
        for (int x = 0; x < folded_width; x++) {
            int axis_distance;
            switch (f.axis) {
                case X:
                    axis_distance = folded_width - x;
                    field[y * field_width + x] =
                            field[y * field_width + x] || field[y * field_width + folded_width + axis_distance];
                    break;
                case Y:
                    axis_distance = folded_height - y;
                    field[y * field_width + x] =
                            field[y * field_width + x] || field[(folded_height + axis_distance) * field_width + x];
                    break;
            }
#ifdef DEBUG
            std::cout << (field[y * field_width + x] ? '#' : '.');
#endif
        }
#ifdef DEBUG
        std::cout << '\n';
#endif
    }

    int dots { 0 };
    for (int y = 0; y < folded_height; y++) {
        for (int x = 0; x < folded_width; x++) {
            dots += field[y * field_width + x];
        }
    }

    output << dots;
    delete[] field;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    int max_x { 0 }, max_y { 0 };
    for (const vec2i &vec : input.points) {
        max_x = std::max(max_x, vec.x);
        max_y = std::max(max_y, vec.y);
    }

    const int field_width = max_x + 1;
    const int field_height = max_y + 1;

    bool* field = new bool[field_width * field_height];
    std::fill(field, field + field_width * field_height, false);
    for (const vec2i &vec : input.points) {
        field[vec.y * field_width + vec.x] = true;
    }

    int folded_height = field_height;
    int folded_width = field_width;

#ifdef DEBUG
    for (int y = 0; y < folded_height; y++) {
        for (int x = 0; x < folded_width; x++) {
            std::cout << (field[y * field_width + x] ? '#' : '.');
        }
        std::cout << '\n';
    }
    std::cout << '\n';
#endif

    for (const fold &f : input.folds) {
        switch (f.axis) {
            case X:
                folded_width = f.position;
                break;
            case Y:
                folded_height = f.position;
                break;
        }

        for (int y = 0; y < folded_height; y++) {
            for (int x = 0; x < folded_width; x++) {
                int axis_distance;
                switch (f.axis) {
                    case X:
                        axis_distance = folded_width - x;
                        field[y * field_width + x] =
                                field[y * field_width + x] || field[y * field_width + folded_width + axis_distance];
                        break;
                    case Y:
                        axis_distance = folded_height - y;
                        field[y * field_width + x] =
                                field[y * field_width + x] || field[(folded_height + axis_distance) * field_width + x];
                        break;
                }
#ifdef DEBUG
                std::cout << (field[y * field_width + x] ? '#' : '.');
#endif
            }
#ifdef DEBUG
            std::cout << '\n';
#endif
        }
    }

    for (int y = 0; y < folded_height; y++) {
        for (int x = 0; x < folded_width; x++) {
            output << (field[y * field_width + x] ? '#' : ' ');
        }
        output << '\n';
    }

    delete[] field;
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
