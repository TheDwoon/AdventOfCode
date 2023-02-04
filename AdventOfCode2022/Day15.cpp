#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>

#define TITLE "Day 15"

template<typename T>
struct vec2 {
    T x { 0 };
    T y { 0 };

    bool operator<(const vec2<T> &other) const {
        return y < other.y || y == other.y && x < other.x;
    }

    bool operator==(const vec2<T> &other) const {
        return x == other.x && y == other.y;
    }

    T distance(const vec2<T>& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
};

typedef vec2<int> vec2i;

struct reading {
    vec2i sensor;
    vec2i beacon;
    int range {0 };
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef std::vector<reading> day_t;

day_t parseInput(std::string &input) {
    day_t result;

    std::regex regex(R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))");
    for (auto it = std::sregex_iterator(input.begin(), input.end(), regex); it != std::sregex_iterator(); ++it) {
        const auto &match = *it;

        reading r { std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[4]) };
        r.range = r.sensor.distance(r.beacon);

        result.push_back(r);
    }

    return result;
}

std::string runPart1(day_t& input) {
    std::stringstream output;

    const int y = 2000000;
    int minX = input[0].sensor.x - input[0].range;
    int maxX = input[0].sensor.x + input[0].range;
    for (size_t i = 1; i < input.size(); ++i) {
        minX = std::min(minX, input[i].sensor.x - input[i].range);
        maxX = std::max(maxX, input[i].sensor.x + input[i].range);
    }

    int sum { 0 };
    for (vec2i position { minX, y }; position.x <= maxX; ++position.x) {
        bool insideCoverage { false };
        bool beaconOnPosition { false };
        for (const reading &r : input) {
            insideCoverage |= position.distance(r.sensor) <= r.range;
            beaconOnPosition |= position == r.beacon;
        }

        if (!beaconOnPosition && insideCoverage)
            sum += 1;
    }

    std::cout << "\n";
    output << sum;
    return output.str();
}

uint64_t tuningFrequency(const vec2i &other) {
    return other.x * 4000000L + other.y;
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    const int limit = 4000000;

    uint64_t f;
    int step;
    for (vec2i position { 0, 0 }; position.y <= limit; ++position.y) {
        for (position.x = 0; position.x <= limit; position.x += step) {
            bool inRange { false };
            step = 1;
            for (const reading &r: input) {
                const int myDistance = position.distance(r.sensor);
                if (myDistance <= r.range) {
                    inRange = true;

                    const int dy = std::abs(r.sensor.y - position.y);
                    int dx1 = 0;
                    if (position.x <= r.sensor.x)
                        dx1 = myDistance - dy + 1;
                    int dx2 = r.range - dy - std::max(0, position.x - r.sensor.x);

                    step = std::max(step, dx1 + dx2);
                }
            }

            if (!inRange) {
                f = tuningFrequency(position);
                output << f;
            }
        }
    }

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
