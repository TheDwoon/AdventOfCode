#include <string>
#include <cstring>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>

#define TITLE "Day 14"

struct Task {
    std::string starting_sequence;
    std::map<std::string, std::string> mappings;
};

template<unsigned int N>
struct linked_node {
    linked_node<N>* next { nullptr };
    const unsigned int size { N };
    unsigned char used { 0 };
    char buffer[N + 1] { 0 };
};

template<unsigned int N>
struct linked_string {
    unsigned int size { 0 };
    unsigned int nodes { 0 };
    linked_node<N> *start { nullptr };
};

template<unsigned int N>
linked_node<N>* create_exploded(const char* c, linked_node<N>* attach = nullptr) {
    linked_node<N>* node = attach;
    while (*c != 0) {
        auto* n = new linked_node<N>;
        if (node != nullptr)
            node->next = n;

        node->buffer[0] = *c;

        node = n;
        ++c;
    }

    return node;
}

template<unsigned int N>
void insert_single_after(linked_node<N>* node, unsigned int index, const char* c) {
    // index has to exist for this method to work!

    // move to index through existing nodes
    while (node != nullptr && node->used < index) {
        node = node->next;
        index -= node->used;
    }

    // find spot within node
    unsigned int lower_bound { 0 };
    while (index > 0) {
        if (node->buffer[lower_bound] != 0)
            index -= 1;

        lower_bound += 1;
    }


}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef Task day_t;

day_t parseInput(std::string &input) {
    day_t parsed;
    std::stringstream stream(input);
    stream >> parsed.starting_sequence;

    std::regex mapping_pattern("([A-Z]{2}) -> ([A-Z])");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), mapping_pattern); it != std::sregex_iterator(); ++it) {
        const auto& match = *it;
        parsed.mappings[match[1]] = match[2];
    }

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

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
