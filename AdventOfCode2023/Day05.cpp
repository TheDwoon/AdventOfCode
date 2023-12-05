#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>
#include <deque>
#include "parser.cpp"

#define TITLE "Day 05"

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);

struct range {
    uint64_t start;
    uint64_t length;
};

struct range_mapping {
    uint64_t destination_range_start;
    uint64_t source_range_start;
    uint64_t length;
};

struct almanac {
    std::vector<uint64_t> seeds;

    std::vector<range_mapping> seed_to_soil;
    std::vector<range_mapping> soil_to_fertilizer;
    std::vector<range_mapping> fertilizer_to_water;
    std::vector<range_mapping> water_to_light;
    std::vector<range_mapping> light_to_temperature;
    std::vector<range_mapping> temperature_to_humidity;
    std::vector<range_mapping> humidity_to_location;
};

typedef almanac day_t;

void parseConversionMap(Parser &p, const char* mapName, std::vector<range_mapping> &map) {
    p.consume(mapName);
    p.readNewLine();

    range_mapping r {0, 0, 0 };
    while (p.readNumber(r.destination_range_start) && p.consumeWhitespace()
            && p.readNumber(r.source_range_start) && p.consumeWhitespace()
            && p.readNumber(r.length) && p.readNewLine()) {
        map.push_back(r);
    }

    p.readNewLine();
}

day_t parseInput(std::string &input) {
    day_t result;
    Parser p(input.c_str());
    p.consume("seeds:");

    // read seeds
    uint64_t s {0};
    while (p.consumeWhitespace() && p.readNumber(s)) {
        result.seeds.push_back(s);
    }
    p.readNewLine();
    p.readNewLine();

    parseConversionMap(p, "seed-to-soil map:", result.seed_to_soil);
    parseConversionMap(p, "soil-to-fertilizer map:", result.soil_to_fertilizer);
    parseConversionMap(p, "fertilizer-to-water map:", result.fertilizer_to_water);
    parseConversionMap(p, "water-to-light map:", result.water_to_light);
    parseConversionMap(p, "light-to-temperature map:", result.light_to_temperature);
    parseConversionMap(p, "temperature-to-humidity map:", result.temperature_to_humidity);
    parseConversionMap(p, "humidity-to-location map:", result.humidity_to_location);

    return result;
}

uint64_t applyRangeConversion(const std::vector<range_mapping> &map, uint64_t x) {
    uint64_t result = x;
    for (const range_mapping &r : map) {
        if (x >= r.source_range_start && x < r.source_range_start + r.length) {
            result = x - r.source_range_start + r.destination_range_start;
            break;
        }
    }

    return result;
}

std::vector<range> applyRangeConversion(const std::vector<range_mapping> &map, const std::vector<range> &xs) {
    std::vector<range> result;
    std::deque<range> queue(xs.begin(), xs.end());

    while (!queue.empty()) {
        const range r = queue.front();
        queue.pop_front();

        bool mapped = false;
        for (const range_mapping &m: map) {
            uint64_t min_intersect = std::max(r.start, m.source_range_start);
            uint64_t max_intersect = std::min(r.start + r.length, m.source_range_start + m.length);
            if (max_intersect <= min_intersect)
                continue;

            mapped = true;
            result.push_back({ min_intersect - m.source_range_start + m.destination_range_start, max_intersect - min_intersect });

            if (min_intersect > r.start) {
                queue.push_back({r.start, min_intersect - r.start});
            }
            if (max_intersect < r.start + r.length) {
                queue.push_back({max_intersect, r.start + r.length - max_intersect});
            }
            break;
        }

        if (!mapped) {
            result.push_back(r);
        }
    }

    return result;
}

std::string runPart1(day_t& input) {
    // simpleTest();

    std::stringstream output;
    uint64_t location_number = -1;
    for (uint64_t x : input.seeds) {
        x = applyRangeConversion(input.seed_to_soil, x);
        x = applyRangeConversion(input.soil_to_fertilizer, x);
        x = applyRangeConversion(input.fertilizer_to_water, x);
        x = applyRangeConversion(input.water_to_light, x);
        x = applyRangeConversion(input.light_to_temperature, x);
        x = applyRangeConversion(input.temperature_to_humidity, x);
        x = applyRangeConversion(input.humidity_to_location, x);

        location_number = std::min(x, location_number);
    }

    output << location_number;
    return output.str();
}

std::string runPart2(day_t& input) {
    std::stringstream output;

    uint64_t location_number = -1;
    for (unsigned int i = 0; i < input.seeds.size(); i += 2) {
        std::vector<range> ranges;
        ranges.push_back({ input.seeds[i], input.seeds[i + 1] });
        ranges = applyRangeConversion(input.seed_to_soil, ranges);
        ranges = applyRangeConversion(input.soil_to_fertilizer, ranges);
        ranges = applyRangeConversion(input.fertilizer_to_water, ranges);
        ranges = applyRangeConversion(input.water_to_light, ranges);
        ranges = applyRangeConversion(input.light_to_temperature, ranges);
        ranges = applyRangeConversion(input.temperature_to_humidity, ranges);
        ranges = applyRangeConversion(input.humidity_to_location, ranges);

        for (const range &r : ranges)
            location_number = std::min(location_number, r.start);
    }

    output << location_number;
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
