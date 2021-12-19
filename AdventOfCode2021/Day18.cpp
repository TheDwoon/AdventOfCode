#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <memory>
#include <cassert>

#define TITLE "Day 18"

struct snailfish_number {
    std::shared_ptr<snailfish_number> left;
    uint16_t left_number;

    std::shared_ptr<snailfish_number> right;
    uint16_t right_number;

    bool hasLeftTree() const {
        if (left)
            return true;
        else
            return false;
    }

    bool hasRightTree() const {
        if (right)
            return true;
        else
            return false;
    }
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<std::shared_ptr<snailfish_number>> day_t;

std::shared_ptr<snailfish_number> parse_number(std::stringstream &stream) {
    std::shared_ptr<snailfish_number> ptr = std::make_shared<snailfish_number>();
    char c;

    // parse left side
    stream.get(c);
    if (c >= '0' && c <= '9') {
        ptr->left_number = c - '0';
    } else if (c == '[') {
        ptr->left = parse_number(stream);
    }

    // expect comma here
    stream.get(c);
    assert(c == ',');

    // parse right side
    stream.get(c);
    if (c >= '0' && c <= '9') {
        ptr->right_number = c - '0';
    } else if (c == '[') {
        ptr->right = parse_number(stream);
    }

    // expect closing bracket
    stream.get(c);
    assert(c == ']');

    return ptr;
}

day_t parseInput(std::string &input) {
    day_t parsed;
    input = "[[[5,[9,2]],[9,0]],[[5,[5,7]],4]]\n";
    std::stringstream stream(input);

    int line {1};
    char c;
    while (stream) {
        stream.get(c);
        assert(c == '[');
        std::shared_ptr<snailfish_number> ptr = parse_number(stream);
        parsed.push_back(std::move(ptr));

        stream.get(c);
        assert(c == '\n' || c == std::stringstream::traits_type::eof());
        line++;
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
