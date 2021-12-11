#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <vector>

#define TITLE "Day 11"

#define ACCESS_MAP(map, x, y) ((x >= 0 && x < map.width && y >= 0 && y < map.height) ? (map.tiles[(y) * map.width + (x)]) : (9))
#define SET_MAP(map, x, y, v) if (x >= 0 && x < map.width && y >= 0 && y < map.height) { map.tiles[(y) * map.width + (x)] = (v); }


struct Map {
    uint16_t width { 0 };
    uint16_t height { 0 };
    std::vector<uint8_t> tiles;
};

std::vector<std::string> tokenize(const std::string &input, const std::string &separator);
typedef Map day_t;

#ifdef DEBUG
void print_map(const Map &map) {
    for (uint16_t y = 0; y < map.height; y++) {
        for (uint16_t x = 0; x < map.width; x++) {
            std::cout << (int)ACCESS_MAP(map, x, y);
        }
        std::cout << '\n';
    }

    std::cout << std::endl;
}
#endif

day_t parseInput(std::string &input) {
    Map map;
    map.width = input.find('\n');

    std::stringstream stream(input);
    char c;
    while (stream.get(c)) {
        if (c >= '0' && c <= '9') {
            map.tiles.push_back(c - '0');
        }
    }

    map.height = map.tiles.size() / map.width;
    return map;
}

std::string runPart1(day_t& map) {
    std::stringstream output;

    Map fired;
    fired.width = map.width;
    fired.height = map.height;
    fired.tiles.resize(map.tiles.size());

#ifdef DEBUG
    std::cout << "Initial state \n";
    print_map(map);
#endif

    uint32_t flashes { 0 };
    for (int step = 0; step < 100; ++step) {
        std::fill(fired.tiles.begin(), fired.tiles.end(), 0);
        for (uint8_t &energy_level : map.tiles) {
            energy_level += 1;
        }

        bool flashed;
        do {
            flashed = false;
            for (uint16_t y = 0; y < map.height; y++) {
                for (uint16_t x = 0; x < map.width; x++) {
                    uint8_t score = ACCESS_MAP(map, x, y);
                    if (score > 9 && ACCESS_MAP(fired, x, y) == 0) {
                        SET_MAP(fired, x, y, 1);
                        SET_MAP(map, x + 1, y + 1, ACCESS_MAP(map, x + 1, y + 1) + 1)
                        SET_MAP(map, x + 1, y + 0, ACCESS_MAP(map, x + 1, y + 0) + 1)
                        SET_MAP(map, x + 1, y - 1, ACCESS_MAP(map, x + 1, y - 1) + 1)
                        SET_MAP(map, x + 0, y - 1, ACCESS_MAP(map, x + 0, y - 1) + 1)
                        SET_MAP(map, x - 1, y - 1, ACCESS_MAP(map, x - 1, y - 1) + 1)
                        SET_MAP(map, x - 1, y + 0, ACCESS_MAP(map, x - 1, y + 0) + 1)
                        SET_MAP(map, x - 1, y + 1, ACCESS_MAP(map, x - 1, y + 1) + 1)
                        SET_MAP(map, x + 0, y + 1, ACCESS_MAP(map, x + 0, y + 1) + 1)
                        flashed = true;
                        flashes += 1;
                    }
                }
            }
        } while (flashed);

        for (uint8_t &energy_level : map.tiles) {
            if (energy_level > 9)
                energy_level = 0;
        }

#ifdef DEBUG
        std::cout << "After Step: " << (step + 1) << '\n';
        print_map(map);
#endif
    }

    output << flashes;
    return output.str();
}

std::string runPart2(day_t& map) {
    std::stringstream output;

    Map fired;
    fired.width = map.width;
    fired.height = map.height;
    fired.tiles.resize(map.tiles.size());

#ifdef DEBUG
    std::cout << "Initial state \n";
    print_map(map);
#endif

    uint32_t flashes { 0 };
    uint32_t step { 0 };
    for (bool all_flashed = false; !all_flashed; ++step) {
        std::fill(fired.tiles.begin(), fired.tiles.end(), 0);
        for (uint8_t &energy_level : map.tiles) {
            energy_level += 1;
        }

        bool flashed;
        do {
            flashed = false;
            for (uint16_t y = 0; y < map.height; y++) {
                for (uint16_t x = 0; x < map.width; x++) {
                    uint8_t score = ACCESS_MAP(map, x, y);
                    if (score > 9 && ACCESS_MAP(fired, x, y) == 0) {
                        SET_MAP(fired, x, y, 1);
                        SET_MAP(map, x + 1, y + 1, ACCESS_MAP(map, x + 1, y + 1) + 1)
                        SET_MAP(map, x + 1, y + 0, ACCESS_MAP(map, x + 1, y + 0) + 1)
                        SET_MAP(map, x + 1, y - 1, ACCESS_MAP(map, x + 1, y - 1) + 1)
                        SET_MAP(map, x + 0, y - 1, ACCESS_MAP(map, x + 0, y - 1) + 1)
                        SET_MAP(map, x - 1, y - 1, ACCESS_MAP(map, x - 1, y - 1) + 1)
                        SET_MAP(map, x - 1, y + 0, ACCESS_MAP(map, x - 1, y + 0) + 1)
                        SET_MAP(map, x - 1, y + 1, ACCESS_MAP(map, x - 1, y + 1) + 1)
                        SET_MAP(map, x + 0, y + 1, ACCESS_MAP(map, x + 0, y + 1) + 1)
                        flashed = true;
                        flashes += 1;
                    }
                }
            }
        } while (flashed);

        for (uint8_t &energy_level : map.tiles) {
            if (energy_level > 9)
                energy_level = 0;
        }

        all_flashed = true;
        for (uint8_t flashed : fired.tiles) {
            if (!flashed) {
                all_flashed = false;
                break;
            }
        }

#ifdef DEBUG
        std::cout << "After Step: " << (step + 1) << '\n';
        print_map(map);
#endif
    }

    output << step;
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
