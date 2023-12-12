#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include "parser.cpp"
#include "vec2.cpp"

#define TITLE "Day 11"

struct image {
    std::vector<char> data;
    int width {0};
    int height {0};
};

typedef image day_t;

day_t parseInput(std::string &input) {
    day_t image;
    Parser p(input.c_str());
    while (!p.eof()) {
        while (!p.isNewLine()) {
            image.data.push_back(p.read());
        }

        p.readNewLine();
        if (image.width == 0)
            image.width = static_cast<int>(image.data.size());
    }

    assert(static_cast<int>(image.data.size()) % image.width == 0);
    image.height =  static_cast<int>(image.data.size()) / image.width;

    return image;
}

template<typename T>
std::vector<vec2<T>> expandGalaxies(const day_t &image, T expansionFactor) {
    std::vector<vec2<T>> galaxies;
    std::vector<bool> emptyRow;
    emptyRow.reserve(image.height);
    std::vector<bool> emptyCol;
    emptyCol.reserve(image.width);

    // find empty rows
    for (int y = 0; y < image.height; y++) {
        bool empty = true;
        for (int x = 0; x < image.width; x++) {
            if (image.data[y * image.width + x] == '#') {
                empty = false;
                break;
            }
        }

        emptyRow[y] = empty;
    }

    // find empty columns
    for (int x = 0; x < image.width; x++) {
        bool empty = true;
        for (int y = 0; y < image.height; y++) {
            if (image.data[y * image.width + x] == '#') {
                empty = false;
                break;
            }
        }

        emptyCol[x] = empty;
    }

    // Expanding the universe
    vec2<T> expansion { 0, 0 };
    for (int y = 0; y < image.height; y++) {
        expansion.x = 0;
        if (emptyRow[y]) {
            expansion.y += expansionFactor;
        } else {
            for (int x = 0; x < image.width; x++) {
                if (emptyCol[x]) {
                    expansion.x += expansionFactor;
                } else {
                    if (image.data[y * image.width + x] == '#') {
                        vec2<T> pos{x, y};
                        pos += expansion;
                        galaxies.push_back(pos);
                    }
                }
            }
        }
    }

    return galaxies;
}

std::string runPart1(day_t& image) {
    std::stringstream output;

    std::vector<vec2i> galaxies = expandGalaxies<int>(image, 1);

    int score = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            score += std::abs(galaxies[i].x - galaxies[j].x) + std::abs(galaxies[i].y - galaxies[j].y);
        }
    }

    output << score;
    return output.str();
}

std::string runPart2(day_t& image) {
    std::stringstream output;

    std::vector<vec2<long long>> galaxies = expandGalaxies<long long>(image, 1000000 - 1);

    long long score = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            score += std::abs(galaxies[i].x - galaxies[j].x) + std::abs(galaxies[i].y - galaxies[j].y);
        }
    }

    output << score;
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
