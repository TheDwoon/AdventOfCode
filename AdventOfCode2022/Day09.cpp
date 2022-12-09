#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <set>

#define TITLE "Day 09"
#define SIGN(X) ((0 < (X)) - ((X) < 0))
#define NUM_SEGMENTS 10

template<typename T>
struct vec2 {
    T x { 0 };
    T y { 0 };

    bool operator<(const vec2<T> &other) const {
        return y < other.y || y == other.y && x < other.x;
    }
};

typedef vec2<int> vec2i;

struct movement {
    char direction;
    int amount;
};

inline void moveTail(const vec2i &head, vec2i &tail) {
    if (std::abs(head.x - tail.x) > 1) {
        tail.x = head.x - SIGN(head.x - tail.x);
        tail.y = head.y;
    } else if (std::abs(head.y - tail.y) > 1) {
        tail.y = head.y - SIGN(head.y - tail.y);
        tail.x = head.x;
    }
}

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<movement> day_t;

day_t parseInput(std::string &input) {
    day_t parsed;
    std::stringstream stream(input);
    movement m { 0, 0 };
    while (stream >> m.direction >> m.amount) {
        parsed.push_back(m);
    }

    return parsed;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    std::set<vec2i> tailPositions;
    vec2i head;
    vec2i tail;

    for (const movement &m : input) {
        // std::cout << m.direction << " " << m.amount << "\n";

        for (int i = 0; i < m.amount; ++i) {
            switch (m.direction) {
                case 'R':
                    head.x += 1;
                    break;
                case 'L':
                    head.x -= 1;
                    break;
                case 'U':
                    head.y += 1;
                    break;
                case 'D':
                    head.y -= 1;
                    break;
                default:
                    throw std::exception();
            }

            moveTail(head, tail);
            // std::cout << "H(" << r.head.x << "," << r.head.y << ") T(" << r.tail.x << "," << r.tail.y << ")\n";
            tailPositions.insert(tail);
        }
    }

    output << tailPositions.size();
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    std::set<vec2i> tailPositions;
    vec2i segments[NUM_SEGMENTS];
    vec2i &head = segments[0];
    vec2i &tail = segments[NUM_SEGMENTS - 1];

    for (const movement &m : input) {
        // std::cout << m.direction << " " << m.amount << "\n";

        for (int i = 0; i < m.amount; ++i) {
            switch (m.direction) {
                case 'R':
                    head.x += 1;
                    break;
                case 'L':
                    head.x -= 1;
                    break;
                case 'U':
                    head.y += 1;
                    break;
                case 'D':
                    head.y -= 1;
                    break;
                default:
                    throw std::exception();
            }

            for (int j = 0; j < NUM_SEGMENTS - 1; ++j) {
                moveTail(segments[j], segments[j + 1]);
            }
            // std::cout << "H(" << r.head.x << "," << r.head.y << ") T(" << r.tail.x << "," << r.tail.y << ")\n";
            tailPositions.insert(tail);
        }
    }

    output << tailPositions.size();
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
