#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <list>
#include "parser.cpp"

#define TITLE "Day 15"

typedef std::vector<std::string> day_t;

struct node {
    std::string label;
    char lense {0};
};

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    while (!p.isNewLine()) {
        p.mark();
        while (p.peek() != ',' && !p.isNewLine()) {
            p.read();
        }

        result.emplace_back(p.getMarker(), p.getCurrent());
        p.consume(',');
    }

    return result;
}

unsigned char hash(const char* c) {
    unsigned char hash = 0;
    while (*c != '\0') {
        hash = static_cast<char>((hash + *c) * 17);
        c++;
    }

    return hash;
}

std::string runPart1(day_t& input) {
    std::stringstream output;
    int score = 0;
    for (const std::string &s : input) {
        unsigned char h = hash(s.c_str());
        score += h;
    }

    output << score;
    return output.str();
}

std::list<node>& get_box(std::list<node>* map, const std::string &label) {
    unsigned char h = hash(label.c_str());
    return map[h];
}

void insert(std::list<node>* map, const node &n) {
    std::list<node>& box = get_box(map, n.label);

    for (node& en : box) {
        if (en.label == n.label) {
            en.lense = n.lense;
            return;
        }
    }

    box.insert(box.end(), n);
}

void remove(std::list<node>* map, const std::string &label) {
    std::list<node>& box = get_box(map, label);
    for (auto it = box.begin(); it != box.end(); ++it) {
        if (it->label == label) {
            box.erase(it);
            return;
        }
    }
}

std::string runPart2(day_t& input) {
    std::stringstream output;
    std::list<node> boxes[256];
    for (const std::string& s : input) {
        size_t idx = s.find('=');
        if (idx != std::string::npos) {
            // assign operation
            node n;
            n.label = std::string(s.cbegin(), s.cbegin() + idx);
            n.lense = std::atoi(s.c_str() + idx  + 1);
            insert(boxes, n);
        } else {
            // delete operation
            std::string label(s.cbegin(), s.cend() - 1);
            remove(boxes, label);
        }
    }

    int focalPower = 0;
    for (int i = 0; i < 256; i++) {
        const std::list<node>& box = boxes[i];
        int multiplier = 1;

        for (const node &n : box) {
            focalPower += (i + 1) * (multiplier++) * static_cast<int>(n.lense);
        }
    }

    output << focalPower;
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
